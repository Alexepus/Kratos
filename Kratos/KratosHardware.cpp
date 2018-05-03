#include "stdafx.h"
#include "KratosHardware.h"
#include "ProgNew.h"
#include "hardware.h"
#include "LogToFile.h"

extern CProgNewApp theApp;

KratosHardware::KratosHardware() 
	: _counterUnit(&theApp.m_AdamCom),
	_lastCounterTime(theApp.Ini.CounterCountTime.Value)
{
	_registerN = theApp.Ini.RegisterPosition.Value;
}

void KratosHardware::Initialize()
{
	CamacHardware::Initialize();
	_counterUnit.SetUnitConfig(&theApp.Ini);
}

void KratosHardware::SetKeRetard(long retard)
{
	//Writing KE; younger KE bit = 25 mV 
	Camac_WriteLong(_crateN, _registerN, REGISTER_KE_A, REGISTER_POT_WRITE_F, (long)(retard / (*_retardCalibration) / 25));
}

void KratosHardware::SetHv(long voltage)
{
	//Writing HV; younger HV bit = 1 V 
	Camac_WriteLong(_crateN, _registerN, REGISTER_HV_A, REGISTER_POT_WRITE_F, voltage);
}

long KratosHardware::ReadCounter()
{
	int ExtraWaitTime = 10; //Extra wait time ~ 100 ms
	CounterState counterState;
	try
	{
		do {
			counterState = _counterUnit.ReadLastCounter();
			if (ExtraWaitTime<10)
			{
				LogFileFormat("ExtraWaitTime: %i ", ExtraWaitTime);
				Sleep(10);
			}
			if (--ExtraWaitTime <= 0)
				throw EXCEPTION("Ошибка в блоке USB-счетчика: счетчик во время не закончил счет.");
				
		} while (counterState.StartState == StartStates::Start);
	}
	catch (DetailedException e)
	{
		auto message = CString(" Ошибка: ") + e.what() + "\nв " + e.Place.c_str() + "\n Продолжить измерение?";
		LogFileFormat(message.GetString());
		if (IDNO == MessageBox(NULL, message.GetString(), "Error", MB_ICONSTOP | MB_YESNO))
			throw;
	}
	// ReSharper disable CppDeclaratorMightNotBeInitialized
	return counterState.Count; 
	// ReSharper restore CppDeclaratorMightNotBeInitialized
}

void KratosHardware::SetAndStartTimer(int time_ms)
{
	if (time_ms != _lastCounterTime)
	{
		_counterUnit.SetCountTime(time_ms / 10);
		_lastCounterTime = time_ms;
	}
	_counterUnit.StartTimedCount();
	if (_counterUnit.ReadState().CoolingState == CoolingStates::Interlock)
		throw EXCEPTION("Ошибка: сработала блокировка охлаждения с запретом высокого.");
}

void KratosHardware::CheckSettings()
{
	//if ini file don't has required settings or settings are not valid for CAMAC/PPI2 standard 
	if (_crateN<0 || _crateN>1 || (_registerN<1 || _registerN>23))
		throw std::exception("The crate number or block positions are set not properly. \nGo to \"Settings -> Camac Setup...\" to reconfigure. ");
}


