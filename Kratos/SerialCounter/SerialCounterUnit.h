#pragma once
#include "..\StdAfx.h"
#include "AdamMessage.h"
#include "..\Exceptions.h"


enum class CoolingStates : byte {CoolingOk = 0, Interlock};
enum class StartStates : byte {Stop = 0, Start = 1};

template<class T>
bool UintTryParseHex(CString str, T& i, int minLength = 1);

class CoolingParams
{
public:
	USHORT SetThreshold;
	USHORT CountedValue;
	CoolingStates CoolingState;
	static CoolingParams Parse( CString commandData )
	{
		CoolingParams param;
		if(commandData.GetLength() != 9 || 
			(commandData[8] != '0' && commandData[8] != '1'))
			throw EXCEPTION((CString("Не удалось разобрать строку данных параметров охлаждения: ") + commandData).GetString());
		if(!UintTryParseHex<USHORT>(commandData.Mid(0,4),param.SetThreshold, 4))
			throw EXCEPTION((CString("Не удалось разобрать строку данных параметров охлаждения: ") + commandData).GetString());
		if(!UintTryParseHex<USHORT>(commandData.Mid(4,4),param.CountedValue, 4))
			throw EXCEPTION((CString("Не удалось разобрать строку данных параметров охлаждения: ") + commandData).GetString());
		param.CoolingState = (commandData[8] == '1')? CoolingStates::Interlock : CoolingStates::CoolingOk;
		return param;
	}
};

class CounterUnitState
{
public:
	
	StartStates StartState;
	CoolingStates CoolingState;
	bool ConfigLoaded;
	static CounterUnitState Parse( CString commandData )
	{
		CounterUnitState state;
		if(commandData.GetLength() != 3 || 
				(commandData[0] != '0' && commandData[0] != '1') ||
				(commandData[1] != '0' && commandData[1] != '1') ||
				(commandData[2] != '0' && commandData[2] != '1'))
			throw EXCEPTION((CString("Не удалось разобрать строку данных состояния: ") + commandData).GetString());
		state.StartState = (commandData[0] == '1')? StartStates::Start : StartStates::Stop;
		state.CoolingState = (commandData[1] == '1')? CoolingStates::Interlock : CoolingStates::CoolingOk;
		state.ConfigLoaded = (commandData[2] == '1');
		return state;
	}
};

class CounterState
{
public:
	UINT Count;
	StartStates StartState;

	static CounterState Parse( CString commandData )
	{
		CounterState state;
		if((commandData.GetLength() != 6 && commandData.GetLength() != 7) || 
			(commandData[0] != '0' && commandData[0] != '1'))
			throw EXCEPTION((CString("Не удалось разобрать строку данных состояния счетчика: ") + commandData).GetString());
		state.StartState = (commandData[0] == '1')? StartStates::Start : StartStates::Stop;
		if(!UintTryParseHex<UINT>(commandData.Mid(1),state.Count, 5))
			throw EXCEPTION((CString("Не удалось разобрать строку данных состояния счетчика: ") + commandData).GetString());
		return state;
	}
};

class IniValues;
class AdamComTransport;

class SerialCounterUnit
{
	AdamComTransport *_adamCom;
	bool _initStatus;
	static const byte UnitAddress = 0x40;

	AdamMessage CreateCommand(byte cmdNumber);
public:	

	SerialCounterUnit(AdamComTransport* adamCom)
	{
		_adamCom = adamCom;
		_initStatus = false;
	}

	virtual ~SerialCounterUnit();

	CString ReadModuleName();
	CounterUnitState ReadState();
	void StartNotTimedCount();
	void StopNotTimedCount();
	void StartTimedCount();
	void StopTimedCount();
	void SetCountTime(USHORT time_10ms);
	USHORT GetCountTime();
	void SetDetectionThreshold(byte threshold);
	byte ReadDetectionThreshold();
	CoolingParams SetCoolingThreshold(USHORT threshold);
	CoolingParams ReadCoolingThreshold();
	CounterState ReadLastCounter();
	CounterState ReadCurrentCounter();

	void SetUnitConfig(IniValues* ini);
	bool GetInitStatus() { return _initStatus; }
};
