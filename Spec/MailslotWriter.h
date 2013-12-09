#pragma once

class CMailslotWriter
{
public:
	CMailslotWriter();
	virtual		~CMailslotWriter(void);
	bool		Connect(LPCTSTR szSlotname, LPCTSTR szServer = _T("."));
	DWORD		Write(BYTE *pbData, DWORD dwDataLength);
	void		Disconnect();
protected:
	HANDLE		m_hMailSlot;
};
