#include "StdAfx.h"
#include "MailslotReader.h"

//	Create a named mail slot. This must be done on the local machine
//	thus we don't use the server name parameter.
CMailslotReader::CMailslotReader()
{
	m_hMailSlot = INVALID_HANDLE_VALUE;
}

bool CMailslotReader::Connect(LPCTSTR szSlotname)
{
	char fullSlotname[_MAX_PATH];
	//	Create our mail slot name
	_sntprintf_s(fullSlotname, _MAX_PATH, _MAX_PATH - 1, _T("\\\\.\\mailslot\\%s"), szSlotname);
	fullSlotname[_MAX_PATH - sizeof(TCHAR)] = TCHAR(0);

	if ((m_hMailSlot = CreateMailslot(fullSlotname, 0, 0, NULL)) != INVALID_HANDLE_VALUE)
		return true;

	return false;
}

//	Return a count of how many messages are waiting in the mailslot.
DWORD CMailslotReader::GetMessageCount(DWORD *pdwNextMessageLength)
{
	DWORD dwCount = 0;
	GetMailslotInfo(m_hMailSlot, NULL, pdwNextMessageLength, &dwCount, NULL);
	return dwCount;
}

//	Read a message from the mailslot and return it in a buffer allocated
//	on the heap. The caller must delete the buffer once it's done with it.
BYTE *CMailslotReader::Read(DWORD& dwBufferLength)
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

void CMailslotReader::Disconnect()
{
	if (m_hMailSlot != INVALID_HANDLE_VALUE)
		CloseHandle(m_hMailSlot);

	m_hMailSlot = INVALID_HANDLE_VALUE;
}

CMailslotReader::~CMailslotReader()
{
	Disconnect();
}
