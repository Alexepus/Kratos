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
	ULONG TimeStamp; //������ ��� ��������� ���������, ����������� ������������� (����� �������� � ����� 
					 //��������� ������ ���������)
};

class ComMsgTale
{
public:
	BYTE Buf[512];
	int MsgSize;
	ULONG TimeStamp; //������ ��� ��������� ���������, ����������� ������������� (����� �������� � ����� 
					 //��������� ������ ���������)
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
	//��������� ������� ����� ���� �������������� ��� ������������� �������� ���������� 
	//� �������� ��� ������� 

	//����������� ���������, ��������� ��������������� ��������, 
	//���������� 1, ���� ������� ��������� ���������, 0 - � ��������� �������
	//���� SendMsg==NULL, ����������� ����������� ���������. ���������� TRUE, ����
	//ReceivedMsg - ���������� ����� �� SentMsg � ��������� ����������
	virtual int AnalyseCorrectMsg(ComMsg* ReceivedMsg, ComMsg* SentMsg){return TRUE;};

	//����� ����������� �������� ������������ � ����������� �����
	virtual void ReportComErrorAndExit(){AfxEndThread(0);};
	
	//����� �������� ����� ������ ��� ������������� ������ m_bViewSentRawData �
	//m_bViewReceivedRawData
	virtual void DumpRawData(const char* Discript, BYTE* ReadBuf, int ReadFromPort)=0;
	
	//���������� ����� �������� ���������
	virtual void DumpSentMsg(ComMsg* SentMsg){};

	//���������� ��� ������������/������������� ���������� � �������� ������
	virtual void ReportJunkMessage(BYTE *Buf, int Size, char Reason){};
	//���������� ��� �������� ����-���� �������� ������
	virtual void ReportTimeOut(ComMsg* SentMsg){};

public:
	CComThreadBasic();
	~CComThreadBasic();

public:
	bool m_bViewSentRawData;
	bool m_bViewReceivedRawData;
	int m_BaudRate;
	double m_ReplyTimeout;		//������� �� �������� ������
	HANDLE m_hComThread;		//���� ����� ���������� ������
	volatile bool m_CmdExitThread;//������� ������ �� ���������� 
	void SendComMessage(const ComMsg &msg/*, ComMsg *Reply=NULL, BOOL WaitForReply=FALSE*/);
								//ThreadSafe-������� ��� ��������� ��������� � �������
	void ReconnectCom(const char *ComName); //ThreadSafe-������� ��� ����������/��������������
								//�� ������ ���� ��� ����� ������
};

#endif