#pragma once

class IAsyncReaderEvent
{
public:
	void virtual OnIpcMessage(BYTE *pbMessage, DWORD dwMessageLength) = 0;
};

class CMailslotAsyncReader
{
public:
	CMailslotAsyncReader();
	virtual ~CMailslotAsyncReader();
	
	bool	Connect(LPCTSTR szSlotName, IAsyncReaderEvent *eventReceiver);
	void	Disconnect();
protected:
	bool	OnMessage(BYTE *pbMessage, DWORD dwMessageLength);
	static unsigned int __stdcall ThreadStub(LPVOID data);

	HANDLE		m_hStopEvent;
	HANDLE		m_hThreadHandle;
	IAsyncReaderEvent *m_Subscriber;
	static DWORD WINAPI ThreadProc(LPVOID lpParameter);
	HANDLE		m_hMailSlot;
	BYTE 		*Read(DWORD& dwBufferLength);
	DWORD		GetMessageCount();
};


