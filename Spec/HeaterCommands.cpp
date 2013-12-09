#include "StdAfx.h"
#include "SpecView.h"
#include "ComThread.h"
#include "Functions.h"
#include "Threads.h"
#include "ThermoReg.h"

CHeaterCommands::CHeaterCommands()
{
	TotalWaitTime=0;
}
CHeaterCommands::~CHeaterCommands(){}

void CHeaterCommands::Init()
{
	CommandMainOff();
}

void CHeaterCommands::OnTimer(UINT nIDEvent)
{
	if(nIDEvent == TIMER_THERMOREG_THERMOFUNC)
	{
		m_pView->KillTimer(TIMER_THERMOREG_THERMOFUNC);
		CurrentTime=GetTimeDouble();
		if(StartTime>CurrentTime)
			StartTime=CurrentTime;
		if((State==CmdStart) && !WaitState)
		{
			CalcRefTemp(&theApp.Ini.TRFunc, CurrentTime-StartTime-TotalWaitTime, CurrentSlope, TimeToInflection, EndTemp);
			m_pView->SetTimer(TIMER_THERMOREG_THERMOFUNC,min(TimeToInflection*1000,5000),NULL);
			if(CurrentSlope!=0)
				SendMsgTempChange(EndTemp-theApp.Ini.ColdEndsTemp, CurrentSlope);
		}
		else if((State==CmdStart) && WaitState)
		{
		}
	}
	else if(nIDEvent == TIMER_THERMOREG_STARTDELAY)
	{
		CommandContinue();
		WaitState=false;
		m_pView->KillTimer(TIMER_THERMOREG_STARTDELAY);
		//if(State!=CmdStart)
			//return;	
		if(m_pThermoReg->m_Hist.LastRefTemp>theApp.Ini.TRFunc.PrimeTemp)
		{
			SendMsgTempChange(theApp.Ini.TRFunc.PrimeTemp-theApp.Ini.ColdEndsTemp, theApp.Ini.TRFunc.CoolingRate);
			TimeToInflection=(m_pThermoReg->m_Hist.LastRefTemp-theApp.Ini.TRFunc.PrimeTemp)/theApp.Ini.TRFunc.CoolingRate;
			CurrentTime=StartTime=GetTimeDouble()+TimeToInflection+0.1;
		}
		else if(m_pThermoReg->m_Hist.LastRefTemp<theApp.Ini.TRFunc.PrimeTemp)
		{
			SendMsgTempChange(theApp.Ini.TRFunc.PrimeTemp-theApp.Ini.ColdEndsTemp, theApp.Ini.TRFunc.HeatingRate);
			TimeToInflection=(theApp.Ini.TRFunc.PrimeTemp-m_pThermoReg->m_Hist.LastRefTemp)/theApp.Ini.TRFunc.HeatingRate;
			CurrentTime=StartTime=GetTimeDouble()+TimeToInflection+0.1;
		}
		else
		{
		CurrentTime=StartTime=GetTimeDouble();
		CalcRefTemp(&theApp.Ini.TRFunc, CurrentTime-StartTime-TotalWaitTime, CurrentSlope, TimeToInflection, EndTemp);
		SendMsgTempChange(EndTemp-theApp.Ini.ColdEndsTemp, CurrentSlope);
		}
		m_pView->SetTimer(TIMER_THERMOREG_THERMOFUNC,TimeToInflection*1000,NULL);
	}
}

void CHeaterCommands::CommandStart()
{
	int Result=VerifyThermoFunction(&theApp.Ini.TRFunc);
	ReportThermoFunctionError(Result, theApp.m_pMainWnd);

	if(Result!=TF_OK)
		return;

	if(State!=CmdStart)
	{
		State=CmdStart;
		TotalWaitTime=0;
		m_pView->KillTimer(TIMER_THERMOREG_THERMOFUNC);
		m_pView->KillTimer(TIMER_THERMOREG_STARTDELAY);
		if(theApp.Ini.SyncStartMsp)
			m_pView->StartMsp();
		if(theApp.Ini.SyncStartDxps)
		{
			HWND hWnd=::FindWindow(_T("KratosWindowClass"),NULL);
			if (hWnd)
				::SendMessage(hWnd,m_pThermoReg->m_RegDxpsRemoteStartID,0,0);
		}
		m_pView->SetTimer(TIMER_THERMOREG_STARTDELAY,theApp.Ini.TRFunc.StartDelay*1000,NULL);
	}
}

void CHeaterCommands::CommandPrime()
{
	State=CmdPrime;
	if(m_pThermoReg->m_Hist.LastRefTemp>theApp.Ini.TRFunc.PrimeTemp)
		SendMsgTempChange(theApp.Ini.TRFunc.PrimeTemp-theApp.Ini.ColdEndsTemp, theApp.Ini.TRFunc.CoolingRate);
	else
		SendMsgTempChange(theApp.Ini.TRFunc.PrimeTemp-theApp.Ini.ColdEndsTemp, theApp.Ini.TRFunc.HeatingRate);
	m_pView->KillTimer(TIMER_THERMOREG_THERMOFUNC);
	CommandContinue();
	WaitState=false;
}

void CHeaterCommands::CommandWait()
{
	if(State!=CmdMainOff && !WaitState)
	{
		WaitBeginTime=GetTimeDouble();
		WaitState=true;
		msg.Buf[0]=CMD_WAIT_CONTINUE_OFF;
		msg.Buf[1]=VAL_WAIT;
		msg.MsgSize=2;
		theApp.m_SerialLink.SendComMessage(msg);
		m_pView->KillTimer(TIMER_THERMOREG_THERMOFUNC);
	}
}

void CHeaterCommands::CommandContinue()
{
	if(WaitState)
	{
		TotalWaitTime+=GetTimeDouble()-WaitBeginTime;
		WaitState=false;
		msg.Buf[0]=CMD_WAIT_CONTINUE_OFF;
		msg.Buf[1]=VAL_CONTINUE;
		msg.MsgSize=2;
		theApp.m_SerialLink.SendComMessage(msg);
		m_pView->KillTimer(TIMER_THERMOREG_THERMOFUNC);
		m_pView->SetTimer(TIMER_THERMOREG_THERMOFUNC,TimeToInflection*1000,NULL);
	}
}

void CHeaterCommands::CommandWaitContinue()
{
	if(State!=CmdMainOff)
	{
		if(!WaitState)
			CommandWait();
		else
			CommandContinue();
	}
	else
		WaitState=false;
}

void CHeaterCommands::CommandMainOff()
{
	State=CmdMainOff;
	WaitState=false;
	msg.Buf[0]=CMD_WAIT_CONTINUE_OFF;
	msg.Buf[1]=VAL_MAIN_OFF;
	msg.MsgSize=2;
	theApp.m_SerialLink.SendComMessage(msg);
}

void CHeaterCommands::SendMsgTempChange(double TempEnd, double Beta)
{
	double AbsBeta_10ms=fabs(Beta)*0.01;
	msg.Buf[0]=CMD_START_TEMP_CHANGE;
	msg.Buf[1]=Int16Q4Low(TempEnd);
	msg.Buf[2]=Int16Q4High(TempEnd);
	msg.Buf[3]=Int16Q4Low(AbsBeta_10ms*256.*256.);
	msg.Buf[4]=Int16Q4Low(AbsBeta_10ms*256.);
	msg.Buf[5]=Int16Q4Low(AbsBeta_10ms);
	msg.Buf[6]=Int16Q4High(AbsBeta_10ms);
	msg.MsgSize=7;
	theApp.m_SerialLink.SendComMessage(msg);
}

