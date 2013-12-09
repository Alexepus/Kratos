#include "stdafx.h"
#include "MailslotWriter.h"

CMailslotWriter::CMailslotWriter(void)
{
}

CMailslotWriter::~CMailslotWriter(void)
{
}

bool CMailslotWriter::Connect(LPCTSTR szSlotname, LPCTSTR szServer)
{
	//	Close any existing mail slot
	Disconnect();

	char fullSlotname[_MAX_PATH];
	//	Create our mail slot name
	_sntprintf_s(fullSlotname, _MAX_PATH, _MAX_PATH - 1, _T("\\\\%s\\mailslot\\%s"), szServer, szSlotname);
	fullSlotname[_MAX_PATH - sizeof(TCHAR)] = TCHAR(0);

	//	Now open the mail slot for overlapped I/O
	if ((m_hMailSlot = CreateFile(fullSlotname, 
		GENERIC_WRITE, 
		FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 
		FILE_ATTRIBUTE_NORMAL, NULL)
		) != INVALID_HANDLE_VALUE)
		return true;

	return false;
}

void CMailslotWriter::Disconnect()
{
	if (m_hMailSlot != INVALID_HANDLE_VALUE)
		CloseHandle(m_hMailSlot);

	m_hMailSlot = INVALID_HANDLE_VALUE;
}

DWORD CMailslotWriter::Write(BYTE *pbData, DWORD dwDataLength)
{
	DWORD dwBytesWritten;
	::WriteFile(m_hMailSlot, pbData, dwDataLength, &dwBytesWritten, NULL);
	return dwBytesWritten;
}
