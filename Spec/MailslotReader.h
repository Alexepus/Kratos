#if !defined(AFX_MAILSLOTREADER_H__ADA1148A_6361_4A0C_AB3F_5A1C821615E5__INCLUDED_)
#define AFX_MAILSLOTREADER_H__ADA1148A_6361_4A0C_AB3F_5A1C821615E5__INCLUDED_

#pragma once

class CMailslotReader
{
public:
	CMailslotReader();
	virtual		~CMailslotReader();

	bool		Connect(LPCTSTR szSlotname);

	BYTE 		*Read(DWORD& dwBufferLength);
	DWORD		GetMessageCount(DWORD *pdwNextMessageLength = (DWORD *) NULL);
	void		Disconnect();
protected:
	HANDLE		m_hMailSlot;
};
#endif