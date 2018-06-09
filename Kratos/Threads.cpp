#include "stdafx.h"
#include "main.h"
#include "hardware.h"
#include "SerialCounter\SerialCounterUnit.h"
#include "IHardware.h"
#include "HardwareSimulation.h"
#include "HpHardware.h"
#include "KratosHardware.h"

extern CMutex MutexThread;
extern CProgNewApp theApp;

void SetDataForGraph(THREAD_COMMON* ThComm, CRegion* pReg, DATA_OUT* NewData)
{
	if (ThComm->pMainFrame->m_Doc.m_Graph.m_pDataShort)
		delete[] ThComm->pMainFrame->m_Doc.m_Graph.m_pDataShort;
	ThComm->pMainFrame->m_Doc.m_Graph.m_pDataShort = NewData;
	ThComm->pMainFrame->m_Doc.m_Graph.m_NDataShort = pReg->m_NDataOut;
	ThComm->pMainFrame->m_Doc.m_Graph.m_NDataShortCurr = pReg->m_NDataOutCurr;

	if (pReg->m_DataIn.KE_BE == DATA_IN::EnergyType::KE)
		sprintf(ThComm->pMainFrame->m_Doc.m_Graph.m_strCaption,
		        "Region %i ( %s, Anode: %s )", pReg->ID + 1, (pReg->m_DataIn.KE_BE == DATA_IN::EnergyType::KE) ? "KE" : "BE", pReg->str.Name_h_nu);

	if (pReg->m_DataIn.Curr_N == 0)
	{
		ThComm->pMainFrame->m_Doc.m_Graph.m_pDataAll = NULL;
		ThComm->pMainFrame->m_Doc.m_Graph.m_NDataAll = 0;
	}
	else
	{
		ThComm->pMainFrame->m_Doc.m_Graph.m_pDataAll = pReg->m_pDataOut;
		ThComm->pMainFrame->m_Doc.m_Graph.m_NDataAll = pReg->m_NDataOut;
	}
}

UINT MeasuringThread(LPVOID pParam)
{
LogFileFormat("Запуск измерения XPS");
SetThreadPriority(GetCurrentThread(),THREAD_PRIORITY_HIGHEST);
#define THR_LOCK() {ThreadLock.Lock(1000);if(ThreadLock.IsLocked())	/*LogFile("Lock", __FILE__, __LINE__)*/;else{LogFile("Failed to Lock, break thread", __FILE__, __LINE__);LeaveCrSecAndEndThread(ThComm->pMainFrame, pReg, 0, ThreadLock);}}
#define THR_UNLOCK() {ThreadLock.Unlock(); /*LogFile("UnLock", __FILE__, __LINE__);*/}
THREAD_COMMON* ThComm = (THREAD_COMMON*) pParam;
	// ReSharper restore CppCStyleCast
CSingleLock ThreadLock(&MutexThread);
CRegion* pReg = nullptr;
DATA_OUT DataOut;
int Retard; //Тормозящее напряжение (в мВ)
int CurrAttempt=1;
int RealTime;
CString strMessage;
CString fatalErrorMessage;
int errorTotalCount = 0; // Количество ошибок за все время с момента запуска изменения
const int maxErrorPassageCount = 10; // Максимальное количество ошибок за проход, когда измерение будето становлено
std::unique_ptr<IHardware> hardware;

if(theApp.Ini.CamacSimulation.Value || theApp.Ini.UsbCounterSimulation.Value)
	hardware = std::make_unique<HardwareSimulation>();
else if(theApp.Ini.HighPressureMode.Value)
	hardware = std::make_unique<HpHardware>();
else
	hardware = std::make_unique<KratosHardware>();
try
{
	hardware->Initialize();

	THR_LOCK();

	pReg = CRegion::GetNextByPriority(-1, ThComm->pRegEdit);
	if (!pReg)
	{
		THR_UNLOCK();
		goto Met_EndBigCircle;
	}

	do
	{
		int errorPassageCount = 0; // Количество ошибок за проход
		time_t regionStartMeasureTime = pReg->m_BeginTime;
		THR_UNLOCK();
		ThComm->pMainFrame->m_pRegionWnd->m_pListRegionWnd->SetIconForReg(pReg, 1);
		THR_LOCK();
		ThComm->pRegNow = pReg;
		int pointIndex = pReg->m_NDataOutCurr;  //current measuring in the region
		DATA_IN DataIn = pReg->m_DataIn;

		if (!theApp.Ini.HighPressureMode.Value) //KRATOS
		{
			if (pReg->m_DataIn.KE_BE == DATA_IN::EnergyType::KE)
			{
				pReg->m_DataIn.DeltaVolts = D2I((double)ThComm->FiTable.GetFiByHV((int)I2D(pReg->m_DataIn.HV)))
					+ D2I(100.0);
				Retard = pReg->m_pDataOut[pointIndex].x + pReg->m_DataIn.DeltaVolts;
			}
			else	//BE
			{
				pReg->m_DataIn.DeltaVolts = D2I((double)ThComm->FiTable.GetFiByHV((int)I2D(pReg->m_DataIn.HV)))
					+ D2I(CRegion::h_nu_Info.Value_h_nu[pReg->m_DataIn.N_h_nu])
					+ D2I(100.0);
				Retard = pReg->m_DataIn.DeltaVolts - pReg->m_pDataOut[pointIndex].x;
			}
		}
		else //High Pressure
		{
			if (pReg->m_DataIn.KE_BE == DATA_IN::EnergyType::KE)
			{
				pReg->m_DataIn.DeltaVolts = D2I((double)ThComm->FiTable.GetFiByHV((int)I2D(pReg->m_DataIn.HV))) - pReg->m_DataIn.HV;
				Retard = pReg->m_pDataOut[pointIndex].x + pReg->m_DataIn.DeltaVolts;
				if (Retard < 0) Retard = 0;
			}
			else	//BE
			{
				pReg->m_DataIn.DeltaVolts = D2I((double)ThComm->FiTable.GetFiByHV((int)I2D(pReg->m_DataIn.HV)))
					+ D2I(CRegion::h_nu_Info.Value_h_nu[pReg->m_DataIn.N_h_nu]) - pReg->m_DataIn.HV;
				Retard = pReg->m_DataIn.DeltaVolts - pReg->m_pDataOut[pointIndex].x;
				if (Retard < 0) Retard = 0;
			}
		}

		hardware->SetKeRetard(Retard);
		hardware->SetHv((long)I2D(DataIn.HV));

		THR_UNLOCK();
		GetXpsTimeRemainedToEnd(&ThComm->TIME);
		ThComm->pMainFrame->SetStatusTime(ThComm->TIME);
		for (int i = 0; i < 3; i++) // Ждем, пока на железе установятся KE и HV
		{
			Sleep(1000);
			ThComm->TIME -= 1000;
			ThComm->pMainFrame->SetStatusTime(ThComm->TIME);
		}
		THR_LOCK();

		DATA_OUT* NewData = new DATA_OUT[pReg->m_NDataOut];

		memmove((void*)NewData, (void*)pReg->m_pDataOut, pReg->m_NDataOut * sizeof(DATA_OUT));
		for (int i = pReg->m_NDataOutCurr; i < pReg->m_NDataOut; ++i)
			NewData[i].y = 0;

		SetDataForGraph(ThComm, pReg, NewData);

		THR_UNLOCK();
		ThComm->pMainFrame->m_Doc.m_Graph.ReDrawAll();
		THR_LOCK();

		for (; pointIndex < pReg->m_NDataOut; ++pointIndex)
		{
			int RealSubmeasuring = ThComm->SubMeasurings;
			int CurrentSubmeasuring = 1;
			int SumN = 0;
		Met_NextSubmeasuring:
			if (ThComm->StopContinue == ThComm->Stop)
			{
				AskAndSaveMeasuringData(ThComm->pMainFrame, NewData, pointIndex - 1, regionStartMeasureTime);
				LeaveCrSecAndEndThread(ThComm->pMainFrame, pReg, 0, ThreadLock, USER_STOP);
			}
			if ((pReg == ThComm->pRegEdit) || (pReg->m_DataIn.Off == TRUE))
			{
				goto Met_EndRegion;
			}
			DataOut.x = pReg->m_pDataOut[pointIndex].x;
			if (pReg->m_DataIn.KE_BE == DATA_IN::EnergyType::KE)
				Retard = pReg->m_pDataOut[pointIndex].x + pReg->m_DataIn.DeltaVolts;
			else
				Retard = pReg->m_DataIn.DeltaVolts - pReg->m_pDataOut[pointIndex].x;
			if (Retard < 0) Retard = 0;

			hardware->SetKeRetard(Retard);
			hardware->SetHv((long)I2D(DataIn.HV));

			if (pReg->m_DataIn.Curr_N <= 1) // 1st or 2nd scan
			{
				if (CurrentSubmeasuring < RealSubmeasuring) //realizing submeasuring technology
					RealTime = DataIn.Time / RealSubmeasuring;
				else //final submeasuring
					RealTime = DataIn.Time - (DataIn.Time / RealSubmeasuring)*(RealSubmeasuring - 1);
			}
			else
				RealTime = DataIn.Time;

			THR_UNLOCK();

			int tickStart = GetTickCount();

			hardware->SetAndStartTimer(RealTime);

			int startOperationLength = GetTickCount() - tickStart;
			int sleepTime = (int)(RealTime - startOperationLength * 0.4);
			if (sleepTime > 0)
				::Sleep(sleepTime);			//Wait for time interval
			int NewN;
			bool isSucsessful = false;
			do{
				try
				{
					NewN = hardware->ReadCounter();
					isSucsessful = true;
				}
				catch (std::exception& e)
				{
					errorPassageCount++;
					errorTotalCount++;
					LogFileFormat("Ошибка №%i из %i: %s", errorPassageCount++, maxErrorPassageCount, DetailedException::TryGetDetailedWhat(e).c_str());
					if (errorPassageCount > maxErrorPassageCount)
						throw EXCEPTION_WITH_INNER("Превышено допустимое количество ошибок чтения счетчика. Измерение будет остановлено.", e);
				}
			} while (!isSucsessful);
			THR_LOCK();

			if (DataIn.Time == 0)
				throw EXCEPTION("Division by 0. Breaking measuring. ");

			float NewY;
			if (pReg->m_DataIn.Curr_N <= 1) // 1st or 2nd scan
			{
				if (CurrentSubmeasuring < RealSubmeasuring) //realizing submeasuring technology
				{
					CurrentSubmeasuring++;
					SumN += NewN;
					goto Met_NextSubmeasuring;
				}
				else
				{
					SumN += NewN;
					NewY = (float)((double)SumN / (double)(DataIn.Time*0.001));
				}
			}
			else
				NewY = (float)((double)NewN / (double)(DataIn.Time*0.001));

			if (ThComm->EnableRemeasure)
			{
				switch (pReg->m_DataIn.Curr_N)
				{
				case 0:	//First scan, do nothing
					DataOut.y = NewY;
					break;
				case 1:	//Second scan, need write the least value of two last scans
					if (NewY <= pReg->m_pDataOut[pointIndex].y)
						DataOut.y = NewY;
					else
						DataOut.y = pReg->m_pDataOut[pointIndex].y;
					break;
				default: //Scans greater then second. If delta Y < NSigma*sigma, calculating new average
					if (fabs(NewY - pReg->m_pDataOut[pointIndex].y) < sqrt(fabs(pReg->m_pDataOut[pointIndex].y))*ThComm->NSigma)
						DataOut.y = (float)(((double)(pReg->m_DataIn.Curr_N - 1.)*(double)(pReg->m_pDataOut[pointIndex].y) + (double)NewY) / ((double)pReg->m_DataIn.Curr_N));
					else //If delta Y > NSigma*sigma
					{
						if (CurrAttempt < ThComm->Attempts)
						{
							CurrAttempt++;
							goto Met_NextSubmeasuring;
						}
						else
							DataOut.y = (float)(((double)(pReg->m_DataIn.Curr_N - 1.)*(double)(pReg->m_pDataOut[pointIndex].y) + (double)NewY) / ((double)pReg->m_DataIn.Curr_N));
					}
				}
			}
			else //Do not use remeasure technology, always averaging
			{
				switch (pReg->m_DataIn.Curr_N)
				{
				case 0:	//First scan, do nothing
					DataOut.y = NewY;
					break;
				default://Other scans, averaging
					DataOut.y = (float)(((double)(pReg->m_DataIn.Curr_N)*(double)pReg->m_pDataOut[pointIndex].y + (double)NewY) / (double)(pReg->m_DataIn.Curr_N + 1));
				}
			}
			CurrAttempt = 1;

			THR_UNLOCK();
			strMessage.Format(" Volts = %.3lf  F = %i  dF = %i", I2D(pReg->m_pDataOut[pointIndex].x), int(floor(NewY + 0.5)), int(floor(NewY - DataOut.y + 0.5)));
			::SendMessage(ThComm->pMainFrame->m_hStatusBar, SB_SETTEXT, CMainFrame::StatusBarPartXPSParams, (LPARAM)(LPCSTR)strMessage);
			THR_LOCK();

			NewData[pointIndex].y = DataOut.y;

			pReg->m_NDataOutCurr = ThComm->pMainFrame->m_Doc.m_Graph.m_NDataShortCurr = pointIndex + 1;
			ThComm->pMainFrame->m_Doc.m_Graph.DrawLineDataShort(pointIndex - 1, pointIndex);

			GetXpsTimeRemainedToEnd(&ThComm->TIME);
			ThComm->pMainFrame->SetStatusTime(ThComm->TIME);

			if (regionStartMeasureTime == 0)
				regionStartMeasureTime = time(nullptr);
			ThComm->MeasureSpeedStat.RegisterPointTime(pReg->ID, pReg->m_DataIn.Curr_N, pointIndex, pReg->m_DataIn.Time);
			if (pointIndex % 50 == 0)
				LogFileFormat("Статистика доп. времени измерения: [Min,Max]: [%i,%i], Average: %.1f, Sigma: %.1f",
					ThComm->MeasureSpeedStat.GetMin(),
					ThComm->MeasureSpeedStat.GetMax(),
					ThComm->MeasureSpeedStat.GetAverage(),
					ThComm->MeasureSpeedStat.GetStdDeviation());
		}

		//Закончен очередной проход по данному региону
		++pReg->m_DataIn.Curr_N;
		pReg->m_BeginTime = regionStartMeasureTime;
		pReg->UpdateStrValues();

		THR_UNLOCK();
		ThComm->pMainFrame->m_pRegionWnd->m_pListRegionWnd->UpdateItem(pReg);
		THR_LOCK();

		pReg->m_NDataOutCurr = 0;
		pReg->m_EndTime = time(nullptr);

		memmove((void*)pReg->m_pDataOut, (void*)NewData, pReg->m_NDataOut * sizeof(DATA_OUT));
		
		// Сохраняем в файл все данные по региону: массив DataOut, количество проходов, время и т.п.
		if (!SaveXpsFullRegionDataToFile(ThComm->fp, pReg))
			LeaveCrSecAndEndThread(ThComm->pMainFrame, pReg, 0, ThreadLock);

		if (ThComm->pMainFrame->m_Doc.m_Graph.m_pDataShort)
		{
			delete[] ThComm->pMainFrame->m_Doc.m_Graph.m_pDataShort;
			ThComm->pMainFrame->m_Doc.m_Graph.m_pDataShort = NULL;
		}

	Met_EndRegion:

		THR_UNLOCK()
			ThComm->pMainFrame->m_pRegionWnd->m_pListRegionWnd->SetOnOffIcon(pReg);
		if (!pReg->m_DataIn.Off)
		{
			if (ThComm->pMainFrame->m_Doc.m_Graph.m_pDataAll == NULL)
			{
				ThComm->pMainFrame->m_Doc.m_Graph.m_pDataAll = pReg->m_pDataOut;
				ThComm->pMainFrame->m_Doc.m_Graph.m_NDataAll = pReg->m_NDataOut;
			}
		}
		THR_LOCK();
		pReg = CRegion::GetNextByPriority(pReg->ID, ThComm->pRegEdit);
	} while(pReg);

	THR_UNLOCK();
}
catch(std::exception& ex)
{
	fatalErrorMessage += DetailedException::TryGetDetailedWhat(ex).c_str();
}
catch (...)
{
	fatalErrorMessage += "Произошло неизвестное исключение";
}
Met_EndBigCircle: 
// Завершающие этапы перед окончанием изменения и выходом из потока

ThComm->pRegNow = nullptr;

if (ThreadLock.IsLocked())
	ThreadLock.Unlock();

try
{
	hardware->SetKeRetard(0);
	if (theApp.Ini.HighPressureMode.Value)
		hardware->SetHv(2); //Минимально возможное значение
}
catch (std::exception& ex)
{
	fatalErrorMessage += DetailedException::TryGetDetailedWhat(ex).c_str();
}

::SendMessage(ThComm->pMainFrame->m_hToolBar, TB_CHECKBUTTON  , 
							(WPARAM) ID_PROGRAMM_START, (LPARAM) MAKELONG(FALSE, 0));
::SendMessage(ThComm->pMainFrame->m_hToolBar, TB_ENABLEBUTTON  , 
							(WPARAM) ID_FILE_OPEN_PROJECT, (LPARAM) MAKELONG(TRUE, 0));
::SendMessage(ThComm->pMainFrame->m_hToolBar, TB_ENABLEBUTTON  , 
							(WPARAM) ID_FILE_NEW_PROJECT, (LPARAM) MAKELONG(TRUE, 0));
ThComm->TIME = 0;
ThComm->pMainFrame->SetStatusTime(ThComm->TIME, false);

HMENU	SysMenu = ::GetSystemMenu(ThComm->pMainFrame->m_hWnd, FALSE);
::EnableMenuItem(SysMenu, SC_CLOSE, MF_ENABLED);

ThComm->pMainFrame->m_Doc.m_Graph.ReDrawAll();

ThComm->pMainFrame->m_StartStop = ThComm->pMainFrame->Start;

CString endMessage = fatalErrorMessage.GetLength()> 0
		? fatalErrorMessage
		: (ThComm->StopContinue == ThComm->Stop
			? "Измерение было остановлено пользователем. "
			: (errorTotalCount == 0)
				? CString("Завершено измерение всех регионов. ") 
				: Format("Завершено измерение всех регионов. Произошло %i ошибок измерения.", errorTotalCount));
LogFileFormat(endMessage);
AfxMessageBox(endMessage, fatalErrorMessage.GetLength()> 0? (MB_ICONERROR | MB_OK) :(MB_ICONINFORMATION|MB_OK));
return 0;
}

//====================================================================================


UINT DxpsThread(LPVOID pParam)
{
LogFileFormat("Запуск измерения DXPS");
THREAD_COMMON* ThComm = (THREAD_COMMON*) pParam;
#define THRD_LOCK() {ThreadLock.Lock(2000);if(ThreadLock.IsLocked())	/*LogFile("Lock", __FILE__, __LINE__)*/;else{LogFile("Failed to Lock, break thread", __FILE__, __LINE__);LeaveCrSecAndEndDxpsThread(ThComm->pMainFrame, pReg, ThreadLock);}}
#define THRD_UNLOCK() {ThreadLock.Unlock(); /*LogFile("UnLock", __FILE__, __LINE__)*/;}

DxpsOutData LastRegData[100];
BOOL LastRegDataWritten[100];
memset(LastRegDataWritten,0, sizeof(LastRegDataWritten));

BOOL IsAnyOnRegion=FALSE;
CSingleLock ThreadLock(&MutexThread);
C_h_nu_Info h_nu_info;
int Retard; //Тормозящее напряжение (в мВ)
int ExtraWaitTime; //Счетчик ожидания таймера
//Оставшееся время. Инициализируется значением "Scan Time" окна DXPS, затем 
//отнимается Dwell измеренных регионов

double StepTime=0; //разница между реальным временем (в квантах DwellTime) и PassageCommonTime для каждого региона
int NewN=0; //Число считанных импульсов с пересчетки
CDxpsRegion *pReg=NULL;
int k=0;
int NewSimN[50]={120,120,120,120,120,120,120};

DxpsOutList::iterator iter=CDxpsRegion::OutData.end();

bool useUsbCounter = theApp.Ini.UseUsbCounter.Value;
int& LastRealTime = theApp.Ini.CounterCountTime.Value;
SerialCounterUnit counterUnit(&theApp.m_AdamCom);
if(!theApp.Ini.UsbCounterSimulation.Value && useUsbCounter)
{
	try
	{
		counterUnit.SetUnitConfig(&theApp.Ini);
	}
	catch(std::exception& e)
	{
		LogFileFormat(CString("Ошибка USB-счетчика: ") + e.what());
		MessageBox(NULL, e.what(), "Kratos" , MB_ICONSTOP|MB_OK);
		goto Met_End;
	}	
}

//Reading the INI file in the current folder:
int CrateN=theApp.Ini.CrateNumber.Value;
int RegisterN=theApp.Ini.RegisterPosition.Value;
int CounterN=theApp.Ini.CounterPosition.Value;
int TimerN=theApp.Ini.TimerPosition.Value;
int RegisterRetardN=theApp.Ini.RegisterRetardPosition.Value;
int RegisterHVN=theApp.Ini.RegisterHVPosition.Value;

if(!theApp.Ini.CamacSimulation.Value)
{
	//if ini file don't has required settings or settings are not valid for CAMAC/PPI2 standard 
	if (CrateN<0||CrateN>1||((RegisterN<1||RegisterN>23)&&!theApp.Ini.HighPressureMode.Value)||
		CounterN<1||CounterN>23||TimerN<1||TimerN>23||((RegisterRetardN<1||RegisterRetardN>23||RegisterHVN<1||RegisterHVN>23)&&theApp.Ini.HighPressureMode.Value))
	{
		MessageBox(NULL, "The crate number or block positions are set not properly. \nGo to \"Settings -> Camac Setup...\" to reconfigure. ", "Error" , MB_ICONSTOP|MB_OK);
		goto Met_End;
	}
	#ifdef IPS6
	Camac_Open(CrateN);
	#endif
	Camac_EnableMessage(FALSE); //camac driver won't display MessageBoxes
	Camac_Reset(CrateN);
}

pReg=CDxpsRegion::GetFirst();
if(pReg==NULL) 
{
	IsAnyOnRegion=FALSE;
	goto Met_End;
}
for (pReg=CDxpsRegion::GetFirst(); ; pReg=CDxpsRegion::GetNext(pReg))
{
	if((!pReg->Parameters.Off) && (pReg->Parameters.Type==TYPE_BE))
		IsAnyOnRegion=TRUE;
	if(pReg==CDxpsRegion::GetLast())
		break;
}
if(!IsAnyOnRegion) goto Met_End; //Нет включенных регионов

ThComm->pMainFrame->SetStatusTime((int)CDxpsRegion::ScanTime*1000);

if(CDxpsRegion::ScanTime-CDxpsRegion::PassedCommonTime<=0)
{
	MsgLog("All measurements has beed completed yet.      \n\nTo remeasure do something of listed below:\n"
		"* Clear all measured data;\n* Increase Scan Time.");
	LeaveCrSecAndEndDxpsThread(ThComm->pMainFrame, NULL, ThreadLock, USER_STOP);
}
ThComm->pMainFrame->m_Graph.PlotData();
CDxpsRegion::ScanStartDateTime = COleDateTime::GetCurrentTime();
while((CDxpsRegion::ScanTime-CDxpsRegion::PassedCommonTime>0 && ThComm->StopContinue==ThComm->Continue) || pReg!=CDxpsRegion::GetLast()) //Главный цикл
{
	//Выход из главного цикла только при достижении последнего региона
	if(pReg==CDxpsRegion::GetLast())
		pReg=CDxpsRegion::GetFirst();
	else
		pReg=CDxpsRegion::GetNext(pReg);
	if(pReg->Parameters.Off || pReg->Parameters.Type==TYPE_DIV)
		goto SkipSwitchedOff;
	theApp.m_pMainFrame->m_pDxpsDlg->SetIcon(pReg->ID,pReg->Parameters.ColorIndex,1);

	ShowWindow(ThComm->pMainFrame->m_pHideWnd->m_hWnd,SW_HIDE);
	ShowWindow(ThComm->pMainFrame->m_Graph.m_hWnd,SW_SHOW);
	
	if(!theApp.Ini.CamacSimulation.Value)
	{
		if(!theApp.Ini.HighPressureMode.Value)  //KRATOS
		{
			Retard = D2I(ThComm->FiTable.GetFiByHV((int)pReg->Parameters.HV)+ 
									h_nu_info.Value_h_nu[pReg->Parameters.Anode]+ 100.0 - pReg->Parameters.BE);
			Camac_WriteLong(CrateN, RegisterN, REGISTER_KE_A, REGISTER_POT_WRITE_F, (long)(Retard/ThComm->RetardCalibration/25));
									//Writing KE; younger KE bit = 25 mV 
			Camac_WriteLong(CrateN, RegisterN, REGISTER_HV_A, REGISTER_POT_WRITE_F, (long)(floor(pReg->Parameters.HV+0.5)));
									//Writing HV; younger HV bit = 1 V 
		}
		else	//HP
		{	Retard = D2I(ThComm->FiTable.GetFiByHV((int)pReg->Parameters.HV)+ 
									h_nu_info.Value_h_nu[pReg->Parameters.Anode] - pReg->Parameters.BE - pReg->Parameters.HV);
			if(Retard <0) Retard=0;
			Camac_WriteLong(CrateN, RegisterRetardN, REGISTER_5P850_A, REGISTER_5P850_WRITE_F, (long)(Retard/ThComm->RetardCalibration/HP_RETARD_GUAGE));
									//Writing KE; younger Retard bit = 50 mV (49.8 mV - точнее)
			Camac_WriteLong(CrateN, RegisterHVN, REGISTER_5P850_A, REGISTER_5P850_WRITE_F, GetRegisterHVCodeFromHV(pReg->Parameters.HV));
									//Writing HV; HV code 0...6 (20,200,10,100,5,50,20 V) 
		}
	}

	Sleep((int)(pReg->Parameters.Delay*1000));			//Delay before measure
	StepTime+=pReg->Parameters.Delay;
	ThComm->pMainFrame->SetStatusTime((int)(CDxpsRegion::ScanTime-CDxpsRegion::PassedCommonTime-StepTime)*1000);

	int tickStart = GetTickCount();

	if(!theApp.Ini.UsbCounterSimulation.Value && useUsbCounter)
	{
		try
		{
			if(pReg->Parameters.Dwell*1000 != LastRealTime)
			{
				counterUnit.SetCountTime((USHORT)(pReg->Parameters.Dwell*1000/10));
				LastRealTime = (int)(pReg->Parameters.Dwell*1000);
			}
			counterUnit.StartTimedCount();
			if(counterUnit.ReadState().CoolingState == CoolingStates::Interlock)
			{
				CString errorMessage = "Ошибка: сработала блокировка охлаждения с запретом высокого. Измерение будет прервано.";
				LogFileFormat(errorMessage);
				MessageBox(NULL, errorMessage, "Error" , MB_ICONSTOP|MB_OK);
				LeaveCrSecAndEndDxpsThread(ThComm->pMainFrame, pReg, ThreadLock);
			}
		}
		catch(std::exception& e)
		{
			auto msg = "Ошибка: " + DetailedException::TryGetDetailedWhat(e) + "\nПродолжить измерение?";
			LogFile(msg);
			if(IDNO == MessageBox(NULL, msg.c_str(), "Error" , MB_ICONSTOP|MB_YESNO))
				LeaveCrSecAndEndDxpsThread(ThComm->pMainFrame, pReg, ThreadLock);
		}	
	}
	
	if(!theApp.Ini.CamacSimulation.Value && !useUsbCounter)
	{
		Camac_ReadWord(CrateN, CounterN, 0, COUNTER_RESET_F);
		//Reset counter and its overflow register 			
		Camac_WriteWord(CrateN, TimerN, 0, TIMER_WRITE_PRESET_F, (unsigned short)(pReg->Parameters.Dwell*1000*10));
		Camac_WriteWord(CrateN, TimerN, 1, TIMER_WRITE_START_F, (unsigned short)1);
		//Writing preset to counter; one timer unit = 0.1 ms
	}

	int startOperationLength = GetTickCount() - tickStart;
	int sleepTime = (int)(pReg->Parameters.Dwell*1000) - startOperationLength/2;
	if(sleepTime > 0)
		::Sleep(sleepTime);			//Wait for time interval

	ExtraWaitTime=20; //Extra wait time ~ 200 ms

	if(!theApp.Ini.UsbCounterSimulation.Value && useUsbCounter)
	{
		CounterState counterState;
		try
		{

			do{
				if(ExtraWaitTime<10)
					Sleep(10);
				if(--ExtraWaitTime<=0)
				{
					LogFileFormat("Ошибка в блоке счетчика: счетчик во время не закончил счет. Продолжить измерение?");
					if(IDNO==::MessageBox(NULL, " Ошибка в блоке счетчика: счетчик во время не закончил счет.    \n Продолжить измерение?    ","Kratos", MB_ICONSTOP|MB_YESNO))
						LeaveCrSecAndEndDxpsThread(ThComm->pMainFrame, pReg, ThreadLock);
					break;                          //Abort waiting
				}
				counterState = counterUnit.ReadLastCounter();

			}while(counterState.StartState == StartStates::Start);
		}
		catch(std::exception& e)
		{
			auto msg = "Ошибка: " + DetailedException::TryGetDetailedWhat(e) + "\nПродолжить измерение?";
			LogFile(msg);
			if(IDNO == MessageBox(NULL, msg.c_str(), "Error" , MB_ICONSTOP|MB_YESNO))
				LeaveCrSecAndEndDxpsThread(ThComm->pMainFrame, pReg, ThreadLock);
		}
		NewN = counterState.Count;
	}
	
	if(!theApp.Ini.CamacSimulation.Value && !useUsbCounter)
	{
		do{
			if(ExtraWaitTime<10) 
				Sleep(10);
			if(--ExtraWaitTime<=0)
			{
				LogFileFormat("Timer read failure. Continue measuring?");
				if(IDNO==::MessageBox(NULL, "    Timer read failure.    \n    Continue measuring?    ","Camac error", MB_ICONSTOP|MB_YESNO))
					LeaveCrSecAndEndDxpsThread(ThComm->pMainFrame, pReg, ThreadLock);
				break;                          //Abort waiting
			}
			Camac_ReadLong(CrateN, TimerN, 1, 8);
		}while(!Camac_q());

		Camac_ReadWord(CrateN, CounterN, 0, COUNTER_OVERFLOW_CHECK_F);
		if (Camac_q()&&!Camac_ErrorCode())					//Counter overflow
		{
			LogFileFormat("Counter overflow. Continue measuring?");
			if(IDNO==::MessageBox(NULL, "    Counter overflow.    \n    Continue measuring?    ","Error", MB_ICONSTOP|MB_YESNO))
					LeaveCrSecAndEndDxpsThread(ThComm->pMainFrame, pReg, ThreadLock);
			Camac_ReadWord(CrateN, CounterN, 0, COUNTER_RESET_F);
							//Reset counter and overflow
			Camac_ReadWord(CrateN, CounterN, 0, COUNTER_OVERFLOW_LAM_RESET_F);
							//Reset LAM and overflow
		}
		NewN = Camac_ReadLong(CrateN, CounterN, 0, COUNTER_READ_RESET_F);
		if(Camac_ErrorCode())
		{
			LogFileFormat("Counter read failure. Continue measuring?");
			if(IDNO==::MessageBox(NULL, "    Counter read failure.       \n    Continue measuring?    ","Camac error", MB_ICONSTOP|MB_YESNO))
					LeaveCrSecAndEndDxpsThread(ThComm->pMainFrame, pReg, ThreadLock);
		}
	}
	if((theApp.Ini.UsbCounterSimulation.Value && useUsbCounter) || (theApp.Ini.CamacSimulation.Value && !useUsbCounter)) //Simulation
	{	
		NewN=NewSimN[pReg->ID]=(int)abs(NewSimN[pReg->ID]+rand()*50./RAND_MAX-25);
		NewSimN[pReg->ID]+=1;
	}

	LastRegData[pReg->ID].RegionN=pReg->ID;
	LastRegData[pReg->ID].Time=CDxpsRegion::PassedCommonTime;
	LastRegData[pReg->ID].Freq=NewN/pReg->Parameters.Dwell;
			
	bool validTemperature = true;
	if( fabs( COleDateTimeSpan( COleDateTime::GetCurrentTime() - 
			COleDateTime(ThComm->LastTemperatureTime) ).GetTotalSeconds() ) > 1.5 ) 
		validTemperature = false;

	LastRegData[pReg->ID].Tcur = validTemperature? ThComm->LastCurTemperature : std::numeric_limits<double>::quiet_NaN();
	LastRegData[pReg->ID].Tref = validTemperature? ThComm->LastRefTemperature : std::numeric_limits<double>::quiet_NaN();
	CDxpsRegion::OutData.push_back(LastRegData[pReg->ID]);
	LastRegDataWritten[pReg->ID]=TRUE;
	CDxpsRegion::PassedNumberOfPoints++;

		//ThComm->pMainFrame->m_Doc.m_Graph.m_pDataShort=CDxpsRegion::GetFirst()->m_pDataOut;
		//ThComm->pMainFrame->m_Doc.m_Graph.m_NDataShort=CDxpsRegion::GetFirst()->m_NDataOut;
		//ThComm->pMainFrame->m_Doc.m_Graph.m_NDataShortCurr=pReg->m_NDataOutCurr;
	StepTime+=pReg->Parameters.Dwell;
	ThComm->pMainFrame->SetStatusTime((int)(CDxpsRegion::ScanTime-CDxpsRegion::PassedCommonTime-StepTime)*1000);
	theApp.m_pMainFrame->m_pDxpsDlg->SetIcon(pReg->ID,pReg->Parameters.ColorIndex,0);

	ThComm->pMainFrame->m_Graph.PlotNewSegment(LastRegData[pReg->ID]);
	THRD_LOCK();
SkipSwitchedOff:
	if(pReg==CDxpsRegion::GetLast())
	{
		BOOL NoDataWritten=TRUE;
		CDxpsRegion *pReg2;
		for (pReg2=CDxpsRegion::GetFirst(); pReg2!=NULL; pReg2=CDxpsRegion::GetNext(pReg2))
		{
			if(pReg2->Parameters.Type==TYPE_DIV)
				if((pReg2->Parameters.Divident>0) && (pReg2->Parameters.Divident<=CDxpsRegion::GetRegNumber()) &&
					(pReg2->Parameters.Divisor>0) && (pReg2->Parameters.Divisor<=CDxpsRegion::GetRegNumber()) &&
					(pReg2->Parameters.Divident!=pReg2->Parameters.Divisor) &&
					(pReg2->Parameters.Divident-1!=pReg2->ID) && (pReg2->Parameters.Divisor-1!=pReg2->ID))
						if(LastRegDataWritten[pReg2->Parameters.Divident-1] && LastRegDataWritten[pReg2->Parameters.Divisor-1])
						{
							LastRegData[pReg2->ID].RegionN=pReg2->ID;
							LastRegData[pReg2->ID].Time=CDxpsRegion::PassedCommonTime;
							if(LastRegData[pReg2->Parameters.Divident-1].Freq!=0)
								LastRegData[pReg2->ID].Freq=LastRegData[pReg2->Parameters.Divident-1].Freq/LastRegData[pReg2->Parameters.Divisor-1].Freq;
							else
								LastRegData[pReg2->ID].Freq=1e100;
							CDxpsRegion::OutData.push_back(LastRegData[pReg2->ID]);
							LastRegDataWritten[pReg2->ID]=TRUE;
							CDxpsRegion::PassedNumberOfPoints++;
							ThComm->pMainFrame->m_Graph.PlotNewSegment(LastRegData[pReg2->ID]);
						}
			if(LastRegDataWritten[pReg2->ID])
				NoDataWritten=FALSE;
		}
		if(NoDataWritten)
		{
			THRD_UNLOCK();
			MsgLog("Error in region parameters: no data to measure.\nFinishing measuring.");
			LeaveCrSecAndEndDxpsThread(ThComm->pMainFrame, pReg, ThreadLock);
		}
		CDxpsRegion::PassedCommonTime+=StepTime;
		StepTime=0;
		WriteDxpsPoints(ThComm->fp);
		iter=CDxpsRegion::OutData.end();
		iter--;
		memset(LastRegDataWritten,0, sizeof(LastRegDataWritten));

	}
	THRD_UNLOCK();

} //главный цикл 



Met_End:

if(!theApp.Ini.CamacSimulation.Value) 	//Hardware
{
	//Writing HV (younger HV bit = 1 V) - зануление HV перед выходом
	if(!theApp.Ini.HighPressureMode.Value) //KRATOS
		Camac_WriteLong(CrateN, RegisterN, REGISTER_HV_A, REGISTER_POT_WRITE_F, 0L);
							//Writing HV; (younger HV bit = 1 V)
	else		//HP
	{
		if(!pReg)
			pReg=CDxpsRegion::GetFirst();
		C_h_nu_Info h_nu_info;
		int Retard = D2I(ThComm->FiTable.GetFiByHV((int)pReg->Parameters.HV)+ 
									h_nu_info.Value_h_nu[pReg->Parameters.Anode]);
		if(Retard <0) Retard=0;
		Camac_WriteLong(CrateN, RegisterRetardN, REGISTER_5P850_A, REGISTER_5P850_WRITE_F, (long)(Retard/ThComm->RetardCalibration/HP_RETARD_GUAGE));
								//Writing BE=0; younger Retard bit = 50 mV (49.8 mV - точнее)

		Camac_WriteLong(CrateN, RegisterHVN, REGISTER_5P850_A, REGISTER_5P850_WRITE_F, GetRegisterHVCodeFromHV(2.));
							//Writing HV 
	}
	#ifdef IPS6
	Camac_Close(CrateN);
	#endif
}
ThComm->pMainFrame->SetStatusTime(0);
::SendMessage(ThComm->pMainFrame->m_hToolBar, TB_CHECKBUTTON  , 
							(WPARAM) ID_PROGRAMM_START, (LPARAM) MAKELONG(FALSE, 0));
::SendMessage(ThComm->pMainFrame->m_hToolBar, TB_ENABLEBUTTON  , 
							(WPARAM) ID_FILE_OPEN_PROJECT, (LPARAM) MAKELONG(TRUE, 0));
::SendMessage(ThComm->pMainFrame->m_hToolBar, TB_ENABLEBUTTON  , 
							(WPARAM) ID_FILE_NEW_PROJECT, (LPARAM) MAKELONG(TRUE, 0));

HMENU	SysMenu = ::GetSystemMenu(ThComm->pMainFrame->m_hWnd, FALSE);
::EnableMenuItem(SysMenu, SC_CLOSE, MF_ENABLED);

if(ThComm->StopContinue==ThComm->Stop)
	MsgLog("DXPS scan has been stopped.   ");
else if(!IsAnyOnRegion)
	MsgLog("There are no valid DXPS regions to measure.   ");
else
{
	LogFileFormat("All measurements has been completed.     ");
	AfxMessageBox("All measurements has been completed.     ", MB_ICONINFORMATION|MB_OK);
}

ThComm->pMainFrame->SetStatusTime(0, false);

ThComm->pMainFrame->m_StartStop = ThComm->pMainFrame->Start;
return 0;
}

void CheckIfUsbCounterConfiguredAndConfigure(SerialCounterUnit counterUnit)
{
	try
	{
		if(!counterUnit.ReadState().ConfigLoaded)
		{
			LogFileFormat("Модуль \"Счетчик USB\" сообщает, что он не сконфигурирован. Конфигурация будет вновь загружена.");
			counterUnit.SetUnitConfig(&theApp.Ini);
		}
	}
	catch (std::exception& e)
	{
		LogFile("Ошибка: " + DetailedException::TryGetDetailedWhat(e));
	}	
}
