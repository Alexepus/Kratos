#pragma once
#include "..\ThreadSafeQueue.h"
#include "AdamMessage.h"
#include <list>
#include "ComMsgTale.h"

class AdamComTransport
{

public:
	AdamComTransport(void);
	~AdamComTransport(void);

protected:
	HANDLE m_hComPort;
	CCriticalSection m_CsCom;	
	bool m_bViewSentRawData;
	bool m_bViewReceivedRawData;
	DWORD m_ReplyTimeout;	

	int GetCorrectMsg(BYTE *NewMsgPart, int NewPartSize, AdamMessage* ReceivedMsg, ComMsgTale* TailMsg);

	void Rs232Send(AdamMessage &msg);

	//Может выводить сырые данные при установленных флагах m_bViewSentRawData и
	//m_bViewReceivedRawData
	virtual void DumpRawData(const char* Discript, BYTE* ReadBuf, int ReadFromPort);

	// Разбиение последовательности байтов на части по <CR>
	static std::list<CString> SplitOnCr(byte buf[], int len);

public:		
	AdamMessage SendAndReceiveReply(AdamMessage message);
	// Был ли открыт порт
	bool IsPortHandleValid() {return (m_hComPort != INVALID_HANDLE_VALUE);}

	bool ReconnectCom(const char *ComName, CString* message); //функция для соединения/пересоединения
	//на другой порт или после ошибки
};
