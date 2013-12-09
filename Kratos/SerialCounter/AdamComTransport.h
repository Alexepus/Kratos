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

	//����� �������� ����� ������ ��� ������������� ������ m_bViewSentRawData �
	//m_bViewReceivedRawData
	virtual void DumpRawData(const char* Discript, BYTE* ReadBuf, int ReadFromPort);

	// ��������� ������������������ ������ �� ����� �� <CR>
	static std::list<CString> SplitOnCr(byte buf[], int len);

public:		
	AdamMessage SendAndReceiveReply(AdamMessage message);
	// ��� �� ������ ����
	bool IsPortHandleValid() {return (m_hComPort != INVALID_HANDLE_VALUE);}

	bool ReconnectCom(const char *ComName, CString* message); //������� ��� ����������/��������������
	//�� ������ ���� ��� ����� ������
};
