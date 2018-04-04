#include "stdafx.h"
#include "HpHardware.h"
#include "ProgNew.h"
#include "function.h"
#include "hardware.h"
#include "LogToFile.h"
#include "Exceptions.h"

extern CProgNewApp theApp;

HpHardware::HpHardware()
{
	_counterN = theApp.Ini.CounterPosition.Value;
	_timerN = theApp.Ini.TimerPosition.Value;
	_registerRetardN = theApp.Ini.RegisterRetardPosition.Value;
	_registerHVN = theApp.Ini.RegisterHVPosition.Value;
}

void HpHardware::SetKeRetard(long retard)
{
	//Writing KE; younger Retard bit = 50 mV (49.8 mV - точнее)
	Camac_WriteLong(_crateN, _registerRetardN, REGISTER_5P850_A, REGISTER_5P850_WRITE_F, (long)(retard / _retardCalibration / HP_RETARD_GUAGE));
}

void HpHardware::SetHv(long voltage)
{
	//Writing HV; HV code 0...6 (20,200,10,100,5,50,20 V) 
	Camac_WriteLong(_crateN, _registerHVN, REGISTER_5P850_A, REGISTER_5P850_WRITE_F, GetRegisterHVCodeFromHV(voltage));
}

long HpHardware::ReadCounter()
{
	int ExtraWaitTime = 10; //~100 ms
	do {
		if (ExtraWaitTime<10)
			Sleep(10);
		if (--ExtraWaitTime <= 0)
			throw EXCEPTION("Ошибка ожидания окончания счета таймера КАМАК: не получен сигнал Q в течение ожидаемого времени.");

		Camac_ReadWord(_crateN, _timerN, 1, 8);

	} while (!Camac_q());

	Camac_ReadWord(_crateN, _counterN, 0, COUNTER_OVERFLOW_CHECK_F);
	if (Camac_q() && !Camac_ErrorCode())					//Counter overflow
	{
		LogFileFormat("Counter overflow. Continue measuring?");
		if (IDNO == ::MessageBox(NULL, "    Counter overflow.    \n    Continue measuring?    ", "Error", MB_ICONSTOP | MB_YESNO))
			throw EXCEPTION("Counter overflow.");
		Camac_ReadWord(_crateN, _counterN, 0, COUNTER_RESET_F);
		//Reset counter and overflow
		Camac_ReadWord(_crateN, _counterN, 0, COUNTER_OVERFLOW_LAM_RESET_F);
		//Reset LAM and overflow
	}

	auto newN = Camac_ReadLong(_crateN, _counterN, 0, COUNTER_READ_RESET_F);
	if (Camac_ErrorCode())
	{
		LogFileFormat("Counter read failure. Continue measuring?");
		if (IDNO == ::MessageBox(NULL, "    Counter read failure.       \n    Continue measuring?    ", "Camac error", MB_ICONSTOP | MB_YESNO))
			throw EXCEPTION("Counter read failure.");
	}
	return newN;
}

void HpHardware::SetAndStartTimer(int time_ms)
{
	//Reset counter and its overflow register 			
	Camac_ReadWord(_crateN, _counterN, 0, COUNTER_RESET_F);
	//Writing preset to counter; one timer unit = 0.1 ms
	Camac_WriteWord(_crateN, _counterN, 0, TIMER_WRITE_PRESET_F, (unsigned short)(time_ms * 10));
	Camac_WriteWord(_crateN, _counterN, 1, TIMER_WRITE_START_F, (unsigned short)1);
}

void HpHardware::CheckSettings()
{
	//if ini file don't has required settings or settings are not valid for CAMAC/PPI2 standard 
	if (_crateN<0 || _crateN>1 || _registerRetardN<1 || _registerRetardN>23 || _registerHVN<1 || _registerHVN>23 || _counterN<1 || _counterN>23 || _timerN<1 || _timerN>23)
		throw std::exception("The crate number or block positions are set not properly. \nGo to \"Settings -> Camac Setup...\" to reconfigure. ");
}

