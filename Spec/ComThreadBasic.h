#ifndef _COMTHREADBASIC_H_
#define _COMTHREADBASIC_H_
#include <list>
#include "ThreadSafeQueue.h"

#define RS485_START   (BYTE)0xAA
#define RS485_END     (BYTE)0xAB
#define RS485_ESCAPE  (BYTE)0xAC

class ComMsg
{
public:
	ComMsg(){WaitForReply=true;}
	BYTE Buf[20];
	int MsgSize;
	bool WaitForReply;
	ULONG TimeStamp; //Служит для выявления таймаутов, заполняется автоматически (после отправки и после 
					 //получения целого сообщения)
};

class ComMsgTale
{
public:
	BYTE Buf[512];
	int MsgSize;
	ULONG TimeStamp; //Служит для выявления таймаутов, заполняется автоматически (после отправки и после 
					 //получения целого сообщения)
};

class CComThreadBasic
{
protected:
	static ULONG WINAPI ComThread(LPVOID pParam);

	CThreadSafeQueue<ComMsg> m_ComSendQueue;
	HANDLE m_hComPort;
	CCriticalSection m_CsReconnect;

	int GetCorrectMsg(BYTE *NewMsgPart, int NewPartSize, ComMsg* ReceivedMsg, ComMsgTale* TailMsg);
	void AnalyseMsgWithWaitList(std::list<ComMsg>& List, ComMsg *ReceivedMsg);
	void Rs232Send(ComMsg* msg);
	void CheckTailTimeout(ComMsgTale *TailMsg);
	
	//********************************************************************************
	//Следующие функции могут быть переопределены для специфических способов оповещения 
	//о событиях или ошибках 

	//Анализирует сообщение, выполняет соответствующее действие, 
	//возвращает 1, если принято ожидаемое сообщение, 0 - в остальных случаях
	//Если SendMsg==NULL, анализирует асинхронное сообщение. Возвращает TRUE, если
	//ReceivedMsg - правильный ответ на SentMsg и сообщение обработано
	virtual int AnalyseCorrectMsg(ComMsg* ReceivedMsg, ComMsg* SentMsg){return TRUE;};

	//Может запрашивать действие пользователя и заканчивать поток
	virtual void ReportComErrorAndExit(){AfxEndThread(0);};
	
	//Может выводить сырые данные при установленных флагах m_bViewSentRawData и
	//m_bViewReceivedRawData
	virtual void DumpRawData(const char* Discript, BYTE* ReadBuf, int ReadFromPort)=0;
	
	//Вызывается после отправки сообщения
	virtual void DumpSentMsg(ComMsg* SentMsg){};

	//Вызывается при некорректных/незаконченных сообщениях в приемном буфере
	virtual void ReportJunkMessage(BYTE *Buf, int Size, char Reason){};
	//Вызывается при истекшем тайм-ауте ожидания ответа
	virtual void ReportTimeOut(ComMsg* SentMsg){};

public:
	CComThreadBasic();
	~CComThreadBasic();

public:
	bool m_bViewSentRawData;
	bool m_bViewReceivedRawData;
	int m_BaudRate;
	double m_ReplyTimeout;		//Таймаут на ожидание ответа
	HANDLE m_hComThread;		//Флаг факта завершения потока
	volatile bool m_CmdExitThread;//Команда потоку на завершение 
	void SendComMessage(const ComMsg &msg/*, ComMsg *Reply=NULL, BOOL WaitForReply=FALSE*/);
								//ThreadSafe-функция для помещения сообщения в очередь
	void ReconnectCom(const char *ComName); //ThreadSafe-функция для соединения/пересоединения
								//на другой порт или после ошибки
};

#endif