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
	bool firstTry = true;
	int ExtraWaitTime = 300; //Extra wait time ~ 3000 ms
	CounterState counterState;
	do {
		counterState = _counterUnit.ReadLastCounter();
		if (!firstTry)
		{
			LogFileFormat("USB counter ExtraWaitTime: %i ", ExtraWaitTime);
			Sleep(10);
		}
		firstTry = false;
		if (--ExtraWaitTime <= 0)
			throw EXCEPTION("������ � ����� USB-��������: ������� �� ����� �� �������� ����.");
				
	} while (counterState.StartState == StartStates::Start);
	
	return counterState.Count; 
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
		throw EXCEPTION("������: ��������� ���������� ���������� � �������� ��������.");
}

void KratosHardware::CheckSettings()
{
	//if ini file don't has required settings or settings are not valid for CAMAC/PPI2 standard 
	if (_crateN<0 || _crateN>1 || (_registerN<1 || _registerN>23))
		throw std::exception("The crate number or block positions are set not properly. \nGo to \"Settings -> Camac Setup...\" to reconfigure. ");
}


