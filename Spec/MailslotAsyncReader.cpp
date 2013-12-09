#include "stdafx.h"
#include "MailslotAsyncReader.h"

CMailslotAsyncReader::CMailslotAsyncReader()
{
	m_hStopEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
}

CMailslotAsyncReader::~CMailslotAsyncReader()
{
	Disconnect();
	WaitForSingleObject(m_hThreadHandle, 100);
	CloseHandle(m_hStopEvent);
}

bool CMailslotAsyncReader::Connect(LPCTSTR szSlotName, IAsyncReaderEvent *eventReceiver)
{
	char fullSlotname[_MAX_PATH];
	//	Create our mail slot name
	_sntprintf_s(fullSlotname, _MAX_PATH, _MAX_PATH - 1, _T("\\\\.\\mailslot\\%s"), szSlotName);
	fullSlotname[_MAX_PATH - sizeof(TCHAR)] = TCHAR(0);

	if ((m_hMailSlot = CreateMailslot(fullSlotname, 0, 0, NULL)) != INVALID_HANDLE_VALUE)
	{
		m_Subscriber = eventReceiver;
		DWORD stub;
		m_hThreadHandle = CreateThread(NULL, 0, &CMailslotAsyncReader::ThreadProc, this, NULL, &stub);
		return true;
	}

	return false;
}

DWORD WINAPI CMailslotAsyncReader::ThreadProc(LPVOID lpParameter)
{
	CMailslotAsyncReader *reader = (CMailslotAsyncReader*) lpParameter;
	while(WaitForSingleObject(reader->m_hStopEvent, 50) == WAIT_TIMEOUT)
	{
		while(reader->GetMessageCount() > 0)
		{
			DWORD dwBufferLength;
			BYTE *data = reader->Read(dwBufferLength);
			reader->m_Subscriber->OnIpcMessage(data, dwBufferLength);
		}
	}
	return 0;
}


void CMailslotAsyncReader::Disconnect()
{
	if (m_hMailSlot != INVALID_HANDLE_VALUE)
		CloseHandle(m_hMailSlot);

	m_hMailSlot = INVALID_HANDLE_VALUE;
	SetEvent(m_hStopEvent);
}

DWORD CMailslotAsyncReader::GetMessageCount()
{
	DWORD dwCount = 0;
	DWORD dwNextMessageLength;
	GetMailslotInfo(m_hMailSlot, NULL, &dwNextMessageLength, &dwCount, NULL);
	return dwCount;
}

//	Read a message from the mailslot and return it in a buffer allocated
//	on the heap. The caller must delete the buffer once it's done with it.
BYTE *CMailslotAsyncReader::Read(DWORD& dwBufferLength)
{
	//	We have to allocate a large buffer for incoming messages because
	//	we have no idea how much data is coming...
	BYTE  *pbData = (BYTE *) NULL;

	dwBufferLength = 0;
	const int dwBufferSize = 65536;
	pbData = new BYTE[dwBufferSize];

	//	Now read the data
	//Read(pbData, 65536 - sizeof(TCHAR), &dwBufferLength, m_hStopEvent) && dwBufferLength);
	::ReadFile(m_hMailSlot, pbData, dwBufferSize, &dwBufferLength, NULL);

	return pbData;
}