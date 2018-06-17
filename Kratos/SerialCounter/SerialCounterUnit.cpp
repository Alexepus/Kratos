#include "stdafx.h"
#include "SerialCounterUnit.h"
#include "..\Exceptions.h"
#include "..\IniValues.h"
#include "..\LogToFile.h"
#include "AdamComTransport.h"

const char* const InvalidReply = "Получен некорректный ответ от устройства.";
const char* const ErrorReply = "USB-cчетчик сообщил об ошибке.";

SerialCounterUnit::~SerialCounterUnit()
{
}

CString SerialCounterUnit::ReadModuleName()
{
	AdamMessage cmd = CreateCommand(0);
	AdamMessage reply = _adamCom->SendAndReceiveReply(cmd);
	if(reply.StartMark != StartMarks::Reply)
		throw EXCEPTION(ErrorReply);
	return reply.CommandData;
}

CounterUnitState SerialCounterUnit::ReadState()
{
	AdamMessage cmd = CreateCommand(1);
	AdamMessage reply = _adamCom->SendAndReceiveReply(cmd);
	if (reply.StartMark != StartMarks::Reply)
		throw EXCEPTION(ErrorReply);
	return CounterUnitState::Parse(reply.CommandData);
}

void SerialCounterUnit::StartNotTimedCount()
{
	AdamMessage cmd = CreateCommand(2);
	cmd.CommandData = "1";
	AdamMessage reply = _adamCom->SendAndReceiveReply(cmd);
	if (reply.StartMark != StartMarks::Reply)
		throw EXCEPTION(ErrorReply);
}

void SerialCounterUnit::StopNotTimedCount()
{
	AdamMessage cmd = CreateCommand(2);
	cmd.CommandData = "0";
	AdamMessage reply = _adamCom->SendAndReceiveReply(cmd);
	if (reply.StartMark != StartMarks::Reply)
		throw EXCEPTION(ErrorReply);
}

void SerialCounterUnit::StartTimedCount()
{
	AdamMessage cmd = CreateCommand(3);
	cmd.CommandData = "1";
	AdamMessage reply = _adamCom->SendAndReceiveReply(cmd);
	if (reply.StartMark != StartMarks::Reply)
		throw EXCEPTION(ErrorReply);
}

void SerialCounterUnit::StopTimedCount()
{
	AdamMessage cmd = CreateCommand(3);
	cmd.CommandData = "0";
	AdamMessage reply = _adamCom->SendAndReceiveReply(cmd);
	if (reply.StartMark != StartMarks::Reply)
		throw EXCEPTION(ErrorReply);
}

void SerialCounterUnit::SetCountTime(USHORT time_10ms)
{
	AdamMessage cmd = CreateCommand(4);
	cmd.CommandData.Format("%.4hX", time_10ms);
	AdamMessage reply = _adamCom->SendAndReceiveReply(cmd);
	if (reply.StartMark != StartMarks::Reply)
		throw EXCEPTION(ErrorReply);
}

USHORT SerialCounterUnit::GetCountTime()
{
	AdamMessage cmd = CreateCommand(4);
	AdamMessage reply = _adamCom->SendAndReceiveReply(cmd);
	if (reply.StartMark != StartMarks::Reply)
		throw EXCEPTION(ErrorReply);
	USHORT time;
	if(!UintTryParseHex<USHORT>(reply.CommandData, time, 4))
		throw EXCEPTION(InvalidReply);
	return time;
}

void SerialCounterUnit::SetDetectionThreshold(byte threshold)
{
	AdamMessage cmd = CreateCommand(5);
	cmd.CommandData.Format("%.2hX", threshold);
	AdamMessage reply = _adamCom->SendAndReceiveReply(cmd);
	if (reply.StartMark != StartMarks::Reply)
		throw EXCEPTION(ErrorReply);
}

byte SerialCounterUnit::ReadDetectionThreshold()
{
	AdamMessage cmd = CreateCommand(5);
	AdamMessage reply = _adamCom->SendAndReceiveReply(cmd);
	if (reply.StartMark != StartMarks::Reply)
		throw EXCEPTION(ErrorReply);
	byte threshold;
	if(!UintTryParseHex<byte>(reply.CommandData, threshold, 2))
		throw EXCEPTION(InvalidReply);
	return threshold;
}

CoolingParams SerialCounterUnit::SetCoolingThreshold(USHORT threshold)
{
	AdamMessage cmd = CreateCommand(6);
	cmd.CommandData.Format("%.4hX", threshold);
	AdamMessage reply = _adamCom->SendAndReceiveReply(cmd);
	if (reply.StartMark != StartMarks::Reply)
		throw EXCEPTION(ErrorReply);
	return CoolingParams::Parse(reply.CommandData);
}

CoolingParams SerialCounterUnit::ReadCoolingThreshold()
{
	AdamMessage cmd = CreateCommand(6);
	AdamMessage reply = _adamCom->SendAndReceiveReply(cmd);
	if (reply.StartMark != StartMarks::Reply)
		throw EXCEPTION(ErrorReply);
	return CoolingParams::Parse(reply.CommandData);
}

CounterState SerialCounterUnit::ReadLastCounter()
{
	AdamMessage cmd = CreateCommand(7);
	AdamMessage reply = _adamCom->SendAndReceiveReply(cmd);
	if (reply.StartMark != StartMarks::Reply)
		throw EXCEPTION(ErrorReply);
	return CounterState::Parse(reply.CommandData);
}

CounterState SerialCounterUnit::ReadCurrentCounter()
{
	AdamMessage cmd = CreateCommand(8);
	AdamMessage reply = _adamCom->SendAndReceiveReply(cmd);
	if(reply.StartMark != StartMarks::Reply)
		throw EXCEPTION(ErrorReply);
	return CounterState::Parse(reply.CommandData);
}

void SerialCounterUnit::SetUnitConfig(IniValues* ini)
{
	const int _10ms = 10;
	SetCountTime((USHORT)ini->CounterCountTime.Value/_10ms);
	SetDetectionThreshold((byte)ini->CounterDetectionThreshold.Value);
	SetCoolingThreshold((USHORT)ini->CounterInterlockThreshold.Value);
}
AdamMessage SerialCounterUnit::CreateCommand(byte cmdNumber)
{
	AdamMessage cmd;
	cmd.StartMark = StartMarks::Request;
	cmd.UnitAddress = UnitAddress;
	cmd.CommandNumber = cmdNumber;
	return cmd;
}

template<class T>
bool UintTryParseHex(CString str, T& i, int minLength/* = 1*/)
{
	const char *src =  (LPCSTR)str;
	char *endOfParse;
	i = (T)strtol(src, &endOfParse, 16);
	return (endOfParse >= src + minLength);
}
