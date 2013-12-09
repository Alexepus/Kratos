#ifndef _HEATER_COMMANDS_H_
#define _HEATER_COMMANDS_H_

#include "ComThread.h"
#include "Functions.h"
//#include "Threads.h"
//#include "ThermoReg.h"

enum eCmd{CmdStart,CmdPrime,CmdWait,CmdContinue,CmdMainOff};

//Класс, обрабатывающий команды пользователя по управлению нагревателем 
class CSpecView;
class CThermoReg;

class CHeaterCommands
{
public:
	CHeaterCommands();
	~CHeaterCommands();
	CSpecView* m_pView;
	CThermoReg *m_pThermoReg;
	ComMsg msg;
	double CurrentTime, StartTime, WaitBeginTime, TotalWaitTime;
	double CurrentSlope,TimeToInflection;
	double EndTemp;

	eCmd State;
	bool WaitState;

	void Init();
	void OnTimer(UINT nIDEvent);
	void SendMsgTempChange(double TempEnd, double Beta);

	void CommandStart();
	void CommandPrime();
	void CommandGo();
	void CommandWait();
	void CommandContinue();
	void CommandWaitContinue();
	void CommandMainOff();
};

#endif