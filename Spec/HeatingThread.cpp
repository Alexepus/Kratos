#include "stdafx.h"
#include "Spec.h"
#include "LogToFile.h"
#include "HSChart/ChartCtrl.h"
#include "MainFrame.h"
#include "Threads.h"
#include "Functions.h"
#include <limits>


//Запускается в СMainFrame
/*
DWORD WINAPI HeatingThread(LPVOID pParam)
{
	CSpecView *pSpecView=(CSpecView *)pParam;
	CThreadSafeQueue<eCmd> * pQueue=&pSpecView->m_ThermoReg.m_Commands.Queue;
	HANDLE Events[2]={theApp.m_EventThreadExit, pQueue->m_EventHasData};
	DWORD WaitResult;
	const DWORD DefTimeout=1000;
	DWORD TimeOut=DefTimeout;
	double TheVeryFirstTime=GetTimeDouble(),CurrentTime, StartTime, WaitBeginTime, TotalWaitTime=0;
	double RefTemp, EndTemp;//, MeasTemp;
	double CurrentSlope,TimeToInflection;
	bool Wait=false;
	eCmd Cmd;
	CTempMessage TempMsg;
	while((WaitResult=WaitForMultipleObjects(2, Events, FALSE, TimeOut))!=WAIT_OBJECT_0)
	{
		CurrentTime=GetTimeDouble();
		if(WaitResult==WAIT_OBJECT_0+1)
		{ //Чтение команд пользователя
			eCmd NewCmd=pQueue->front_pop();
			if(NewCmd==CmdStart)
			{
				StartTime=CurrentTime;
				TotalWaitTime=0;
				Cmd=NewCmd;
				Wait=false;
			}
			else if(NewCmd==CmdWait)
			{
				Wait=true;
				WaitBeginTime=CurrentTime;
			}
			else if(NewCmd==CmdContinue)
			{
				if(Wait)
					TotalWaitTime+=CurrentTime-WaitBeginTime;
				Wait=false;
			}
			else
			{
				Cmd=NewCmd;
			}
		}
		//WAIT_OBJECT_0+1 или WAIT_TIMEOUT
		if((Cmd==CmdStart) && !Wait)
		{
			RefTemp=CalcRefTemp(&theApp.Ini.TRFunc, CurrentTime-StartTime-TotalWaitTime, CurrentSlope, TimeToInflection, EndTemp);
			ASSERT(TimeToInflection>0);
			TimeOut=min(TimeToInflection*1000,DefTimeout);
			TempMsg.PointType=CTempMessage::eRefTemp;
			TempMsg.Pt.Time=CurrentTime-TheVeryFirstTime;
			TempMsg.Pt.Temp=RefTemp;

			pSpecView->m_ThermoReg.m_Hist.ThermoPlotQueue.Push(TempMsg);

		}
		else if((Cmd==CmdStart) && Wait)
		{
			TempMsg.PointType=CTempMessage::eRefTemp;
			TempMsg.Pt.Time=CurrentTime-TheVeryFirstTime;
			TempMsg.Pt.Temp=RefTemp;

			pSpecView->m_ThermoReg.m_Hist.ThermoPlotQueue.Push(TempMsg);
			TimeOut=DefTimeout;
		}
		else
			TimeOut=DefTimeout;
		/*MeasTemp=EmulateMeasTemp(RefTemp, Cmd!=CmdMainOff);
		TempMsg.PointType=CTempMessage::eMeasTemp;
		TempMsg.Pt.Time=CurrentTime-TheVeryFirstTime;
		TempMsg.Pt.Temp=MeasTemp;

		pSpecView->m_ThermoReg.m_Hist.ThermoPlotQueue.Push(TempMsg);

		PostMessage(pSpecView->m_hWnd, WM_UPDATE_THERMO_DATA, NULL,NULL);


	}
	return 0;
}
*/
//Вычисляет температурные параметры, заданные термофункцией, исключая выход на PrimeTemp и остывание
//после выключения
//Входные параметры: заданная пользователем термофункция, время от запуска (сек) (исключая время Wait)
//Выходные величины: текущая температура (С), наклон прямой (С/сек), время до перегиба (сек)
double CalcRefTemp(CThermoFunc *pThermoFunc, double TimeFromStart, double &CurrentSlope, double &TimeToInflection, double &TEnd)
{
	double RefTemp;
	double ReducedTime;
	double Period=fabs((pThermoFunc->Temp1-pThermoFunc->Temp2)/pThermoFunc->BetaPlus)+
		pThermoFunc->Time1+fabs((pThermoFunc->Temp1-pThermoFunc->Temp2)/pThermoFunc->BetaMinus)+pThermoFunc->Time2;
	double FirstCycle=fabs((pThermoFunc->PrimeTemp-pThermoFunc->Temp1)/pThermoFunc->BetaPlus)+
		pThermoFunc->Time1+fabs((pThermoFunc->Temp2-pThermoFunc->Temp1)/pThermoFunc->BetaMinus)+pThermoFunc->Time2;

	if(TimeFromStart>=FirstCycle)
	{
		if(pThermoFunc->Repeat)
		{
			ReducedTime=Mod(TimeFromStart-FirstCycle,Period);
			double T1=fabs((pThermoFunc->Temp1-pThermoFunc->Temp2)/pThermoFunc->BetaPlus);
			double T2=T1+pThermoFunc->Time1;
			double T3=T2+fabs((pThermoFunc->Temp1-pThermoFunc->Temp2)/pThermoFunc->BetaMinus);
			if(ReducedTime<T1)
			{
				RefTemp=pThermoFunc->Temp2+ReducedTime*pThermoFunc->BetaPlus;
				CurrentSlope=pThermoFunc->BetaPlus;
				TimeToInflection=T1-ReducedTime;
				TEnd=pThermoFunc->Temp1;
			}
			else if(ReducedTime<T2)
			{
				RefTemp=pThermoFunc->Temp1;
				CurrentSlope=0;
				TimeToInflection=T2-ReducedTime;
				TEnd=pThermoFunc->Temp1;
			}
			else if(ReducedTime<T3)
			{
				RefTemp=pThermoFunc->Temp1-(ReducedTime-T2)*pThermoFunc->BetaMinus;
				CurrentSlope=-pThermoFunc->BetaMinus;
				TimeToInflection=T3-ReducedTime;
				TEnd=pThermoFunc->Temp2;
			}
			else
			{
				RefTemp=pThermoFunc->Temp2;
				CurrentSlope=0;
				TimeToInflection=Period-ReducedTime;
				TEnd=pThermoFunc->Temp2;
			}

		}
		else
		{
			RefTemp=pThermoFunc->Temp2;
			CurrentSlope=0;
			TimeToInflection=DBL_MAX;
			TEnd=pThermoFunc->Temp2;
		}
	}
	else
	{
		double T1=fabs((pThermoFunc->Temp1-pThermoFunc->PrimeTemp)/pThermoFunc->BetaPlus);
		double T2=T1+pThermoFunc->Time1;
		double T3=T2+fabs((pThermoFunc->Temp1-pThermoFunc->Temp2)/pThermoFunc->BetaMinus);
		if(TimeFromStart<T1)
		{
			RefTemp=pThermoFunc->PrimeTemp+TimeFromStart*pThermoFunc->BetaPlus;
			CurrentSlope=pThermoFunc->BetaPlus;
			TimeToInflection=T1-TimeFromStart;
			TEnd=pThermoFunc->Temp1;
		}
		else if(TimeFromStart<T2)
		{
			RefTemp=pThermoFunc->Temp1;
			CurrentSlope=0;
			TimeToInflection=T2-TimeFromStart;
			TEnd=pThermoFunc->Temp1;
		}
		else if(TimeFromStart<T3)
		{
			RefTemp=pThermoFunc->Temp1-(TimeFromStart-T2)*pThermoFunc->BetaMinus;
			CurrentSlope=-pThermoFunc->BetaMinus;
			TimeToInflection=T3-TimeFromStart;
			TEnd=pThermoFunc->Temp2;
		}
		else
		{
			RefTemp=pThermoFunc->Temp2;
			CurrentSlope=0;
			TimeToInflection=FirstCycle-TimeFromStart;
			TEnd=pThermoFunc->Temp2;
		}
	}
	return RefTemp;
}

//Вычисляет температурные параметры, заданные термофункцией, исключая выход на PrimeTemp и остывание
//после выключения
//Входные параметры: заданная пользователем термофункция, время от запуска (сек) (исключая время Wait)
//Выходные величины: текущая температура (С), наклон прямой (С/сек), время до перегиба после полочки(сек)
/*void CalcRefTemp(CThermoFunc *pThermoFunc, double MeasTemp, double TimeFromStart, double &CurrentSlope, double &TimeToInflection, double &TEnd)
{
	double ReducedTime;
	double Period=fabs((pThermoFunc->Temp1-pThermoFunc->Temp2)/pThermoFunc->BetaPlus)+
		pThermoFunc->Time1+fabs((pThermoFunc->Temp1-pThermoFunc->Temp2)/pThermoFunc->BetaMinus)+pThermoFunc->Time2;
	double FirstCycle=fabs((pThermoFunc->PrimeTemp-pThermoFunc->Temp1)/pThermoFunc->BetaPlus)+
		pThermoFunc->Time1+fabs((pThermoFunc->Temp2-pThermoFunc->Temp1)/pThermoFunc->BetaMinus)+pThermoFunc->Time2;

}*/


//Вычисляет температурные параметры при выходе на PrimeTemp
double CalcRefTempPrimeHeat(CThermoFunc *pThermoFunc, double TimeFromPrimeCmd, double &CurrentSlope, double &TimeToInflection)
{
	return 0;
}

double EmulateMeasTemp(double RefTemp, bool On)
{
	static double OldTemp=theApp.Ini.ColdEndsTemp;
	OldTemp=On?((RefTemp>OldTemp)?OldTemp*0.5+RefTemp*0.5: OldTemp*0.9+theApp.Ini.ColdEndsTemp*0.1): OldTemp*0.9+theApp.Ini.ColdEndsTemp*0.1;
	return OldTemp;
}
