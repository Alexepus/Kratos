// ThermoReg.cpp: implementation of the CThermoReg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Spec.h"
#include "ThermoReg.h"
#include "Threads.h"
#include "SpecView.h"
#include "MainFrame.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CThermoReg::CThermoReg()
{
	RemoteDxpsStateCount=1000;
	m_StatusComPort=true;
	m_StatusController=true;
	wm_CurrentTempMessage = RegisterWindowMessage("RemoteCurrentTemp");
}

CThermoReg::~CThermoReg()
{

}

void CThermoReg::Init(CSpecView* pSpecView)
{
	m_pSpecView=pSpecView;
	m_Hist.m_pView=pSpecView;
	//DWORD ThreadId;
	//theApp.m_hThermoRegThread=CreateThread(NULL,NULL, HeatingThread, pSpecView, NULL, &ThreadId);
	theApp.m_SerialLink.m_BaudRate=57600;
	theApp.m_SerialLink.ReconnectCom((LPCSTR)theApp.Ini.ComPortName);
	if(!LoadThermoPairFile(theApp.Ini.ThermoCoupleFile))
		Msg("Unable to load ThermoFunction file \"%s\".  ", theApp.Ini.ThermoCoupleFile);
	theApp.m_SerialLink.m_Pool.RegisterMsgReception(pSpecView->m_hWnd);
	if(m_StatusComPort)
	{
		UpLoadThermoPairToController();
		UploadPidCoefs();
		UploadPidParams();
		UploadFfTable();
		LogFile("CThermoReg::Init Upload Params");

	}
	else
	{
		LogFile("CThermoReg::Init ComPort Status is bad");
	}
	m_pSpecView->SetTimer(TIMER_THERMOREG_1SEC,1000,NULL);
	m_Commands.m_pView=pSpecView;
	m_Commands.m_pThermoReg=this;
	m_Commands.Init();
	m_RegDxpsRemoteStartID=RegisterWindowMessage("RemoteDxpsStart");
}

bool CThermoReg::LoadThermoPairFile(CString FilePath)
{
	m_StatusThermoPairLoad=false;
	if(FilePath.IsEmpty())
		return m_StatusThermoPairLoad;
	FILE* pFile=fopen((LPCSTR)FilePath,"rt");
	if(!pFile)
		return m_StatusThermoPairLoad;
	int Unused, Points;
	double mV;
	if(fscanf(pFile,"%d\n", &Unused)<1)
		return m_StatusThermoPairLoad;
	if(fscanf(pFile,"%lf\n%lf\n%d\n",&m_ThermoPairStartT,&m_ThermoPairDeltaT, &Points)<3)
		return m_StatusThermoPairLoad;
	m_ThermoPair_mV.reserve(Points);
	m_ThermoPair_mV.clear();
	while(fscanf(pFile,"%lf\n", &mV)==1)
		m_ThermoPair_mV.push_back(mV);
	if(m_ThermoPair_mV.size()>=2)
		m_StatusThermoPairLoad=true;
	return m_StatusThermoPairLoad;
}

double CThermoReg::GetTByThermoPairmV(double mV)
{
//Линейная интерполяция
//Массив m_ThermoPair_mV - упорядочен. Ищем индекс, элемент которого 
//не меньше mV

	if(m_ThermoPair_mV.size()<2)
		return 0.;
	int i;
	for (i=1; i<m_ThermoPair_mV.size()-1;i++)
		if(m_ThermoPair_mV[i]>=mV)
			break;
	i--;
	//i - индекс элемента слева от искомой величины
	double di=(mV-m_ThermoPair_mV[i])/(m_ThermoPair_mV[i+1]-m_ThermoPair_mV[i]);
	return m_ThermoPairStartT+(i+di)*m_ThermoPairDeltaT;
}

double CThermoReg::GetThermoPairmVByT(double T)
{
//Линейная интерполяция
//Расчитывам индекс, который должен быть при известной начальной 
//температуре и шаге. Ничего страшного, если он нецелый, <0 
//или >=m_ThermoPair_mV.size()
	if(m_ThermoPair_mV.size()<2)
		return 0.;
	double index=(T-m_ThermoPairStartT)/m_ThermoPairDeltaT;
	int i=max(0,min(m_ThermoPair_mV.size()-2,floor(index)));
	return m_ThermoPair_mV[i]+(index-i)*(m_ThermoPair_mV[i+1]-m_ThermoPair_mV[i]);
}

void CThermoReg::UpLoadThermoPairToController()
{
//Необходимо загрузить 32 значения напряжения термопары, значение температуры 
//для первого индекса и deltaT, причем deltaT должно быть целым
	if(m_ThermoPair_mV.size()<2)
		return;

	double _deltaT=m_ThermoPair_mV.size()*m_ThermoPairDeltaT/32.;
	int deltaT=(int)floor(m_ThermoPair_mV.size()*m_ThermoPairDeltaT/32.);
	if(deltaT<1)
		deltaT=1;
	if(deltaT>255)
		deltaT=255;
	int T0=(int)floor(m_ThermoPairStartT+0.5);
	//Посылаем данные контроллеру частями по 8 индексов
	msg.Buf[0]=CMD_LOAD_TCOUPLE_TABLE;
	msg.MsgSize=18;
	for(int i=0; i<4; i++)
	{
		msg.Buf[1]=(BYTE)i;
		for(int j=0; j<8; j++)
		{
			WORD Int16Q8=(WORD)floor(GetThermoPairmVByT((i*8+j)*deltaT+T0)*256.+0.5);
			msg.Buf[2*j+2]=(BYTE)Int16Q8; //Low Byte
			msg.Buf[2*j+3]=(BYTE)(Int16Q8>>8); //High Byte
		}
		theApp.m_SerialLink.SendComMessage(msg);
	}
	msg.Buf[0]=CMD_LOAD_T0_DT;
	int Int16Q4=T0*16;
	msg.Buf[1]=(BYTE)Int16Q4;//Low Byte
	msg.Buf[2]=(BYTE)(Int16Q4>>8);//Low Byte
	msg.Buf[3]=(BYTE) deltaT;
	msg.MsgSize=4;
	theApp.m_SerialLink.SendComMessage(msg);

}

LRESULT CThermoReg::OnSerialEvent(WPARAM WParam, LPARAM LParam)
{
	static bool wasMainOff = false;
	static bool wasWaitState = false;
	CMainFrame *pMainFrame=(CMainFrame*)theApp.m_pMainWnd;
	int MsgType=HIWORD(LParam);
	BYTE Cmd=(BYTE)LOWORD(LParam);
	if(MsgType==SERIAL_SYNC_MSG)
	{
		m_StatusComPort=true;
		m_StatusController=true;
		ComMsg msg=theApp.m_SerialLink.m_Pool.GetMsg(WParam);
		if(Cmd==CMD_GET_T_BY_V)
		{
			fprintf(TestTC_pFile,"%g\n", BytesToShort(msg.Buf[1],msg.Buf[2])/16.);
			if(++TestTC_i<340)
				TestTC(TestTC_i);
			else
				fclose(TestTC_pFile);
		}
		else if(Cmd==CMD_GET_ADC_TC_I_U_TREF)
		{   // Получение напряжения термопары, напряжения и тока нагревателя, текущего значения 
			// опорной температуры, расчитываемой в контроллере
			double VoltageTC, I, U, TRef, TCur;
			VoltageTC=BytesToShort(msg.Buf[1],msg.Buf[2])*(4.096/16384.)*(100./2.5); //В мВ термопары
			I=BytesToShort(msg.Buf[3],msg.Buf[4])*(4.096/16384.)*(25./2.5); //В Амперах
			U=BytesToShort(msg.Buf[5],msg.Buf[6])*(4.096/16384.)*(25./2.5); //В Вольтах
			TRef=BytesToShort(msg.Buf[7],msg.Buf[8])/16.;
			pMainFrame->SetStatusPower(I*U);
			pMainFrame->SetStatusTCur(GetTByThermoPairmV(VoltageTC)+theApp.Ini.ColdEndsTemp);
			if(m_Commands.State!=CmdMainOff)
			{
				m_Hist.AddRefTempPoint(TRef+theApp.Ini.ColdEndsTemp);
				pMainFrame->SetStatusTRef(TRef+theApp.Ini.ColdEndsTemp);
			}
			else
				pMainFrame->SetStatusTRef(0,false);
			TCur = GetTByThermoPairmV(VoltageTC)+theApp.Ini.ColdEndsTemp;
			m_Hist.AddMeasTempPoint(TCur);
			m_Hist.AddPowerPoint(I*U);
			// Отравка сообщения в программу Kratos
			HWND hKratosWnd = ::FindWindow(_T("KratosWindowClass"),NULL);
			if(hKratosWnd)
				SendMessage(hKratosWnd, wm_CurrentTempMessage, (LPARAM)(int)(TRef*256.), (LPARAM)(int)(TCur*256.));
		}
		else if(Cmd==CMD_GET_STATUS)
		{
			bool tablesLoaded = (bool)msg.Buf[1];
			bool mainOff = (bool)(msg.Buf[2] & 0x2);
			bool waitState = (bool)(msg.Buf[2] & 0x1);
			bool heaterIOverload = (bool)(msg.Buf[3] & 0x1);
			bool heaterUOverload = (bool)(msg.Buf[3] & 0x2);
			bool heaterPOverload = (bool)(msg.Buf[3] & 0x4);
			bool heaterILimit = (bool)(msg.Buf[3] & 0x8);

			if(!tablesLoaded)
			{
				LogFile("CThermoReg::OnSerialEvent Получен статус 'CMD_GET_STATUS.tablesNotLoaded'. Повторная загрузка конфигурации в контроллер");
				UpLoadThermoPairToController();
				UploadPidCoefs();
				UploadPidParams();
				UploadFfTable();
				LoadProtectionVals();
				ResetProtection();
			}
			if(mainOff && wasMainOff)
				m_Commands.State = CmdMainOff;
			wasMainOff = mainOff;

			if(waitState == wasWaitState)
				m_Commands.WaitState = waitState;
			wasWaitState = waitState;

			int status = 0;
			if(heaterIOverload)
				status |= CMainFrame::ProtectionStatus::OverloadI;
			if(heaterUOverload)
				status |= CMainFrame::ProtectionStatus::OverloadU;
			if(heaterPOverload)
				status |= CMainFrame::ProtectionStatus::OverloadP;
			if(heaterILimit)
				status |= CMainFrame::ProtectionStatus::LimitationI;
			if(status == 0)
				status |= CMainFrame::ProtectionStatus::OK;
			pMainFrame->SetStatusProtection(status);
		}
	}
	else if(MsgType==SERIAL_ASYNC_MSG)
	{
		if(Cmd==MESSAGE_RESET)
		{
			LogFile("CThermoReg::OnSerialEvent Получено сообщение 'MESSAGE_RESET'. Повторная загрузка конфигурации в контроллер");
			UpLoadThermoPairToController();
			UploadPidCoefs();
			UploadPidParams();
			UploadFfTable();
			LoadProtectionVals();
			ResetProtection();
			m_Commands.CommandMainOff();
		}
	}
	else if(MsgType==SERIAL_ERROR_COM_FAILED || MsgType==SERIAL_ERROR_TIMEOUT)
	{
		if (MsgType==SERIAL_ERROR_COM_FAILED)
		{
			m_StatusComPort=false;
			theApp.m_SerialLink.ReconnectCom(theApp.Ini.ComPortName);
		}
		if(MsgType==SERIAL_ERROR_TIMEOUT)
			m_StatusController=false;

		pMainFrame->SetStatusPower(-888);
		pMainFrame->SetStatusTCur(0,false);
		pMainFrame->SetStatusProtection(CMainFrame::ProtectionStatus::Unknown);
	}
	return (LRESULT)0;
}

void CThermoReg::TestTC(int i)
{
	if(i==0)
	{
		TestTC_pFile=fopen("c:\\TestTC.txt","wt");
		fprintf(TestTC_pFile,";ThermoPair mV, T approx comp, T approx controller\n");

	}
	double dVi=(m_ThermoPair_mV[1]-m_ThermoPair_mV[0])*0.5;
	double mV=m_ThermoPair_mV[0]+dVi*(i-10);
	fprintf(TestTC_pFile,"%g, %g, ", mV, GetTByThermoPairmV(mV));
	msg.Buf[0]=CMD_GET_T_BY_V;
	int Int16Q8=mV*256.;
	msg.Buf[1]=(BYTE)Int16Q8;//Low Byte
	msg.Buf[2]=(BYTE)(Int16Q8>>8);//Low Byte
	msg.MsgSize=3;
	theApp.m_SerialLink.SendComMessage(msg);

}

void CThermoReg::OnTimer(UINT nIDEvent)
{
	switch(nIDEvent)
	{
		case TIMER_THERMOREG_1SEC:
			msg.Buf[0]=CMD_GET_ADC_TC_I_U_TREF;
			msg.MsgSize=1;
			theApp.m_SerialLink.SendComMessage(msg);

			msg.Buf[0]=CMD_GET_STATUS;
			msg.MsgSize=1;
			theApp.m_SerialLink.SendComMessage(msg);

			if(RemoteDxpsStateCount<2)
				RemoteDxpsStateCount++;
			else
				((CMainFrame*)theApp.m_pMainWnd)->m_wndToolBarTR.GetToolBarCtrl().MarkButton(
					ID_TR_SYNC_DXPS,TRUE);
			break;
	}
	m_Commands.OnTimer(nIDEvent);
}

void CThermoReg::UploadPidCoefs()
{
	ComMsg msg;
	msg.Buf[0]=CMD_LOAD_PIDCOEFS;
	msg.Buf[1]=Int16Q8Low(theApp.Ini.PidKp);
	msg.Buf[2]=Int16Q8High(theApp.Ini.PidKp);
	msg.Buf[3]=Int16Q8Low(theApp.Ini.PidKi);
	msg.Buf[4]=Int16Q8High(theApp.Ini.PidKi);
	msg.Buf[5]=Int16Q8Low(theApp.Ini.PidKd);
	msg.Buf[6]=Int16Q8High(theApp.Ini.PidKd);
	msg.Buf[7]=Int16Q8Low(theApp.Ini.TRegKff);
	msg.Buf[8]=Int16Q8High(theApp.Ini.TRegKff);
	msg.MsgSize=9;
	msg.WaitForReply=true;
	theApp.m_SerialLink.SendComMessage(msg);	
}

void CThermoReg::UploadPidParams()
{
	ComMsg msg;
	msg.Buf[0]=CMD_LOAD_PIDPARAMS;
	msg.Buf[1]=theApp.Ini.PidDifShift;
	msg.Buf[2]=Int16Q8Low(theApp.Ini.PidViDecay);
	msg.Buf[3]=Int16Q4Low(theApp.Ini.PidViMinMax);
	msg.Buf[4]=Int16Q4High(theApp.Ini.PidViMinMax);
	msg.Buf[5]=Int16Q4Low(theApp.Ini.PidZeroVi);
	msg.Buf[6]=Int16Q4High(theApp.Ini.PidZeroVi);
	msg.MsgSize=7;
	msg.WaitForReply=true;
	theApp.m_SerialLink.SendComMessage(msg);	
}

void CThermoReg::UploadFfTable()
{
	ComMsg msg;
	msg.Buf[0]=CMD_LOAD_FF_TABLE;
	msg.Buf[1]=Int16Q4Low(theApp.Ini.FeedForwTemp[0]-theApp.Ini.ColdEndsTemp);
	msg.Buf[2]=Int16Q4High(theApp.Ini.FeedForwTemp[0]-theApp.Ini.ColdEndsTemp);
	msg.Buf[3]=Int16Q4Low(theApp.Ini.FeedForwTemp[1]-theApp.Ini.ColdEndsTemp);
	msg.Buf[4]=Int16Q4High(theApp.Ini.FeedForwTemp[1]-theApp.Ini.ColdEndsTemp);
	msg.Buf[5]=Int16Q4Low(theApp.Ini.FeedForwTemp[2]-theApp.Ini.ColdEndsTemp);
	msg.Buf[6]=Int16Q4High(theApp.Ini.FeedForwTemp[2]-theApp.Ini.ColdEndsTemp);
	msg.Buf[7]=Int16Q4Low(theApp.Ini.FeedForwTemp[3]-theApp.Ini.ColdEndsTemp);
	msg.Buf[8]=Int16Q4High(theApp.Ini.FeedForwTemp[3]-theApp.Ini.ColdEndsTemp);
	msg.Buf[9]=Int16Q4Low(theApp.Ini.FeedForwOut[0]);
	msg.Buf[10]=Int16Q4High(theApp.Ini.FeedForwOut[0]);
	msg.Buf[11]=Int16Q4Low(theApp.Ini.FeedForwOut[1]);
	msg.Buf[12]=Int16Q4High(theApp.Ini.FeedForwOut[1]);
	msg.Buf[13]=Int16Q4Low(theApp.Ini.FeedForwOut[2]);
	msg.Buf[14]=Int16Q4High(theApp.Ini.FeedForwOut[2]);
	msg.Buf[15]=Int16Q4Low(theApp.Ini.FeedForwOut[3]);
	msg.Buf[16]=Int16Q4High(theApp.Ini.FeedForwOut[3]);
	msg.MsgSize=17;
	theApp.m_SerialLink.SendComMessage(msg);	
}

void CThermoReg::SendManualHeaterMode(bool Manual)
{
	ComMsg msg;
	msg.Buf[0]=CMD_SET_MANUAL_HEATER_MODE;
	msg.Buf[1]=Manual? 1:0;
	msg.MsgSize=2;
	theApp.m_SerialLink.SendComMessage(msg);	
}

void CThermoReg::LoadDacValue(int Channel, int Value)
{
	ComMsg msg;
	msg.Buf[0]=CMD_LOAD_DAC;
	msg.Buf[1]=(BYTE) Channel;
	msg.Buf[2]=(BYTE) Value;
	msg.Buf[3]=(BYTE) (Value>>8);
	msg.MsgSize=4;
	theApp.m_SerialLink.SendComMessage(msg);	
}

LRESULT CThermoReg::OnRemoteDxpsState(WPARAM WParam, LPARAM LParam)
{
	BOOL StateReady = (BOOL)LParam;
	((CMainFrame*)theApp.m_pMainWnd)->m_wndToolBarTR.GetToolBarCtrl().MarkButton(ID_TR_SYNC_DXPS,!StateReady);
	RemoteDxpsStateCount=0;
	return TRUE;
}

void CThermoReg::LoadProtectionVals()
{
	WORD I = (WORD)(theApp.Ini.ProtectionMaxI/((4.096/16384.)*(25./2.5)));
	WORD U = (WORD)(theApp.Ini.ProtectionMaxU/((4.096/16384.)*(25./2.5)));
	DWORD P =(DWORD)(theApp.Ini.ProtectionMaxP/((4.096/16384.)*(25./2.5))/((4.096/16384.)*(25./2.5)));
	WORD limitI = (WORD)(theApp.Ini.LimitationMaxI/((4.096/16384.)*(25./2.5)));
	
	ComMsg msg;
	msg.Buf[0]=CMD_LOAD_PROTECTION_VALS;
	msg.Buf[1]=(BYTE)I;
	msg.Buf[2]=(BYTE)(I>>8);
	msg.Buf[3]=(BYTE)U;
	msg.Buf[4]=(BYTE)(U>>8);
	msg.Buf[5]=(BYTE)(P>>16);
	msg.Buf[6]=(BYTE)(P>>24);
	msg.Buf[7]=(BYTE)limitI;
	msg.Buf[8]=(BYTE)(limitI>>8);
	msg.MsgSize=9;
	theApp.m_SerialLink.SendComMessage(msg);
}

void CThermoReg::ResetProtection()
{
	ComMsg msg;
	msg.Buf[0]=CMD_RESET_PROTECTION;
	msg.MsgSize=1;
	theApp.m_SerialLink.SendComMessage(msg);
}
