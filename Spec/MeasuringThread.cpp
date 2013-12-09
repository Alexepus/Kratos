#include "stdafx.h"
#include "Spec.h"
#include "SpecView.h"
#include "CamacUnits.h"
#include "VCMCDM.h"
#include "LogToFile.h"
#include "HSChart/ChartCtrl.h"
#include "MainFrame.h"
#include "Threads.h"
#include "math.h"
#include "ComThread.h"

extern CSpecApp theApp;

UINT MeasuringThread(LPVOID pParam)
{
	CSpecView *pSpecView=(CSpecView *)pParam;
	HWND ViewHwnd=pSpecView->GetSafeHwnd();
	CMassSpecDoc *pDoc=pSpecView->GetDocument();

	int RegIndex=0;
	enum {StoppedByUser, MeasurementCompleted, HardwareError, Unknown} eStopReason=Unknown;
	WORD AdcCode;
	CChartSerie* SerieByReg[100];
	memset(SerieByReg,0,sizeof(SerieByReg));
	double SeriesGage[100];
	memset(SeriesGage,0,sizeof(SeriesGage));
	DWORD StartTime=GetTickCount();
	double Mass;
	
	do
	{
		PostMessage(ViewHwnd, WM_MEAS_THREAD_DATA, MAKEWPARAM(0, MTHR_REGIONLIST_BEGIN),(LPARAM)(GetTickCount()-StartTime));
		for(RegIndex=0; RegIndex<pDoc->m_MassTable.size(); RegIndex++)
		{
			if(!pDoc->m_MassTable[RegIndex].Checked || pDoc->m_MassTable[RegIndex].Mass<=0)
				continue;
			Mass=pDoc->m_MassTable[RegIndex].Mass;

			if(!LoadMassDac(theApp.m_pMassCalibDoc->VoltageByMass(Mass)))
			{
				LogFile("MeasuringThread: LoadMassDac() failed");
				if(!LoadMassDac(theApp.m_pMassCalibDoc->VoltageByMass(Mass)))
				{
					eStopReason=HardwareError;
					goto FinishMeasure;
				}
			}
			Sleep(theApp.Ini.MassSetupDelay*1000);
			if(!GetMassIntensityAdcRaw(AdcCode))
			{
				LogFile("MeasuringThread: GetMassIntensityAdc() failed");
				if(!GetMassIntensityAdcRaw(AdcCode))
				{
					eStopReason=HardwareError;
					goto FinishMeasure;
				}
			}
			PostMessage(ViewHwnd, WM_MEAS_THREAD_DATA, MAKEWPARAM(RegIndex, MTHR_REGION_DATA),(LPARAM)AdcCode);
		}
		PostMessage(ViewHwnd, WM_MEAS_THREAD_DATA, MAKEWPARAM(0, MTHR_REGIONLIST_END),(LPARAM)(GetTickCount()-StartTime));
	}while(pSpecView->m_bMeasureStart && (GetTickCount()-StartTime)<=pDoc->m_ScanTime);
FinishMeasure:
	PostMessage(ViewHwnd, WM_MEAS_THREAD_DATA, MAKEWPARAM(0, MTHR_THREAD_END),(LPARAM)(GetTickCount()-StartTime));
	if(!pSpecView->m_bMeasureStart)
		eStopReason=StoppedByUser;
	pSpecView->m_bMeasureStart=FALSE;

	switch(eStopReason)
	{
	case StoppedByUser:
		theApp.m_pMainWnd->MessageBox("Measurement has been stopped.     ", "Spec", MB_OK|MB_ICONINFORMATION);
		LogFile("MeasuringThread: Measurement has been stopped");
		break;
	case HardwareError:
		theApp.m_pMainWnd->MessageBox("Error: controller does not respond.\nMeasurement has been stopped!       ", "Spec", MB_OK|MB_ICONINFORMATION);
		LogFile("MeasuringThread: Error: controller does not respond.Measurement has been stopped!");
		break;
	default:
		theApp.m_pMainWnd->MessageBox("Measurement has been completed.      ", "Spec", MB_OK|MB_ICONINFORMATION);
		LogFile("MeasuringThread: Measurement has been completed");
	}
	pDoc->m_bMeasurementInProgress=false;
	return 0;
}

bool LoadMassDac(double Voltage)
{
	int Val=Voltage/5.*0x3FFF;
	Val=MinMax(0,0x3FFF,Val);
	ComMsg msg;
	msg.Buf[0]=CMD_LOAD_DAC;
	msg.Buf[1]=1;
	msg.Buf[2]=(BYTE)Val;
	msg.Buf[3]=(BYTE)(Val>>8);
	msg.MsgSize=4;
	return theApp.m_SerialLink.SendWaitForReply(&msg);
}

bool GetMassIntensityAdc(double &Voltage)
{
	bool Ret;
	ComMsg msg;
	msg.Buf[0]=CMD_GET_MASS_ADC;
	msg.MsgSize=1;
	Ret=theApp.m_SerialLink.SendWaitForReply(&msg);
	Voltage=((int)msg.Buf[1]+((int)msg.Buf[2]<<8))*4.096/0x3FFF;
	return Ret;
}

bool GetMassIntensityAdcRaw(WORD &Code)
{
	bool Ret;
	ComMsg msg;
	msg.Buf[0]=CMD_GET_MASS_ADC;
	msg.MsgSize=1;
	Ret=theApp.m_SerialLink.SendWaitForReply(&msg);
	Code=(WORD)msg.Buf[1]+((WORD)msg.Buf[2]<<8);
	return Ret;
}
