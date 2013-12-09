#pragma once

#include "AdamMessage.h"
#include "ProcessableMessage.h"
#include "..\ThreadSafeQueue.h"
#include <list>
#include "ComMsgTale.h"

class AdamComThread
{
public:
	AdamComThread(void);
	virtual ~AdamComThread(void);
protected:
	static ULONG WINAPI ComThread(LPVOID pParam);

	CThreadSafeQueue<ProcessableMessage*> m_ComSendQueue;
	HANDLE m_hComPort;
	CString m_PortName;
	CCriticalSection m_CsReconnect;

	int GetCorrectMsg(BYTE *NewMsgPart, int NewPartSize, AdamMessage* ReceivedMsg, ComMsgTale* TailMsg);
	void AnalyseMsgWithWaitList(std::list<AdamMessage>& List, AdamMessage *ReceivedMsg);
	void Rs232Send(AdamMessage &msg);
	

	//Анализирует сообщение, выполняет соответствующее действие, 
	//возвращает 1, если принято ожидаемое сообщение, 0 - в остальных случаях
	//Если SendMsg==NULL, анализирует асинхронное сообщение. Возвращает TRUE, если
	//ReceivedMsg - правильный ответ на SentMsg и сообщение обработано
	virtual int AnalyseCorrectMsg(AdamMessage* ReceivedMsg, AdamMessage* SentMsg){return TRUE;};

	//Может запрашивать действие пользователя и заканчивать поток
	virtual void ReportComErrorAndExit(){AfxEndThread(0);};
	
	//Вызывается после отправки сообщения
	virtual void DumpSentMsg(AdamMessage* SentMsg){};

	//Вызывается при некорректных/незаконченных сообщениях в приемном буфере
	virtual void ReportJunkMessage(BYTE *Buf, int Size, char Reason){};
	//Вызывается при истекшем тайм-ауте ожидания ответа
	virtual void ReportTimeOut(AdamMessage* SentMsg){};

	//Может выводить сырые данные при установленных флагах m_bViewSentRawData и
	//m_bViewReceivedRawData
	virtual void DumpRawData(const char* Discript, BYTE* ReadBuf, int ReadFromPort);

	// Разбиение последовательности байтов на части по <CR>
	static std::list<CString> SplitOnCr(byte buf[], int len);

	static void NotifyReplyReceived(ProcessableMessage* beingSendedMsg, AdamMessage reply);

public:
	bool m_bViewSentRawData;
	bool m_bViewReceivedRawData;
	DWORD m_ReplyTimeout;		//Таймаут на ожидание ответа в мс
	HANDLE m_hComThread;		//Флаг факта завершения потока
	volatile bool m_CmdExitThread;//Команда потоку на завершение 

	// Был ли открыт порт
	bool IsPortHandleValid() {return (m_hComPort != INVALID_HANDLE_VALUE);}
	
	// Синхронная отправка сообщения с возвратом ответа устройства
	AdamMessage SendComMessage(const AdamMessage &msg);
	
	// Асинхронная отправка сообщения
	void PostComMessage(const AdamMessage &msg, TwoParamFunc replyCallback, void* param);

	bool ReconnectCom(const char *ComName, CString* message); //ThreadSafe-функция для соединения/пересоединения
								//на другой порт или после ошибки
};

