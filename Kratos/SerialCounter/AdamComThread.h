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
	

	//����������� ���������, ��������� ��������������� ��������, 
	//���������� 1, ���� ������� ��������� ���������, 0 - � ��������� �������
	//���� SendMsg==NULL, ����������� ����������� ���������. ���������� TRUE, ����
	//ReceivedMsg - ���������� ����� �� SentMsg � ��������� ����������
	virtual int AnalyseCorrectMsg(AdamMessage* ReceivedMsg, AdamMessage* SentMsg){return TRUE;};

	//����� ����������� �������� ������������ � ����������� �����
	virtual void ReportComErrorAndExit(){AfxEndThread(0);};
	
	//���������� ����� �������� ���������
	virtual void DumpSentMsg(AdamMessage* SentMsg){};

	//���������� ��� ������������/������������� ���������� � �������� ������
	virtual void ReportJunkMessage(BYTE *Buf, int Size, char Reason){};
	//���������� ��� �������� ����-���� �������� ������
	virtual void ReportTimeOut(AdamMessage* SentMsg){};

	//����� �������� ����� ������ ��� ������������� ������ m_bViewSentRawData �
	//m_bViewReceivedRawData
	virtual void DumpRawData(const char* Discript, BYTE* ReadBuf, int ReadFromPort);

	// ��������� ������������������ ������ �� ����� �� <CR>
	static std::list<CString> SplitOnCr(byte buf[], int len);

	static void NotifyReplyReceived(ProcessableMessage* beingSendedMsg, AdamMessage reply);

public:
	bool m_bViewSentRawData;
	bool m_bViewReceivedRawData;
	DWORD m_ReplyTimeout;		//������� �� �������� ������ � ��
	HANDLE m_hComThread;		//���� ����� ���������� ������
	volatile bool m_CmdExitThread;//������� ������ �� ���������� 

	// ��� �� ������ ����
	bool IsPortHandleValid() {return (m_hComPort != INVALID_HANDLE_VALUE);}
	
	// ���������� �������� ��������� � ��������� ������ ����������
	AdamMessage SendComMessage(const AdamMessage &msg);
	
	// ����������� �������� ���������
	void PostComMessage(const AdamMessage &msg, TwoParamFunc replyCallback, void* param);

	bool ReconnectCom(const char *ComName, CString* message); //ThreadSafe-������� ��� ����������/��������������
								//�� ������ ���� ��� ����� ������
};

