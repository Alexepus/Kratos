#ifndef _HEATER_H_
#define _HEATER_H_

#include "ThreadSafeQueue.h"
#include "Functions.h"

extern CSpecApp theApp;
enum eCmd{CmdStart,CmdPrime,CmdGo,CmdWait,CmdContinue,CmdMainOff};

//Класс, обрабатывающий команды пользователя по управлению нагревателем, 
//и отправлящший их в поток HeatingThread. Родительский объект - CSpecView

class CHeaterCommands
{
public:
	CHeaterCommands(){CommandMainOff();}
	~CHeaterCommands(){CommandMainOff();}

	eCmd State;
	bool WaitState;
	CThreadSafeQueue<eCmd> Queue;

	void CommandStart()
	{
		int Result=VerifyThermoFunction(&theApp.Ini.TRFunc);
		ReportThermoFunctionError(Result, theApp.m_pMainWnd);

		if(Result!=TF_OK)
			return;
		State=CmdStart;
		WaitState=false;
		Queue.Push(State);
	}

	void CommandPrime()
	{
		State=CmdPrime;
		WaitState=false;
		Queue.Push(State);
	}

	void CommandGo()
	{
		State=CmdGo;
		WaitState=false;
		Queue.Push(State);
	}

	void CommandWait()
	{
		if(State!=CmdMainOff)
			WaitState=true;
		eCmd Cmd=(WaitState)?CmdWait:CmdContinue;
		Queue.Push(Cmd);
	}

	void CommandContinue()
	{
		WaitState=false;
		eCmd Cmd=(WaitState)?CmdWait:CmdContinue;
		Queue.Push(Cmd);
	}

	void CommandWaitContinue()
	{
		if(State!=CmdMainOff)
			WaitState=!WaitState;
		else
			WaitState=false;
		eCmd Cmd=(WaitState)?CmdWait:CmdContinue;
		Queue.Push(Cmd);
	}

	void CommandMainOff()
	{
		State=CmdMainOff;
		WaitState=false;
		Queue.Push(State);
	}

};

#endif