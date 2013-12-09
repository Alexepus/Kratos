// MsgPool.h: interface for the CMsgPool class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSGPOOL_H__C453CF0D_C02E_4945_B589_3469FE99DF5D__INCLUDED_)
#define AFX_MSGPOOL_H__C453CF0D_C02E_4945_B589_3469FE99DF5D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector>
#include "ComThreadBasic.h"
#include "MailslotAsyncReader.h"
#include "MailslotWriter.h"

using namespace std;
// Позволяет окну подписаться на получение уведомлений о сообщениях; обратившись к объекту 
// с использованием полученного Id, можно получить всё сообщение
// Формат сообщений: Msg = WM_COM_EVENT, wParam = Id, lParam = (ThreadEventType<<16)|Command)

class CMsgPool : IAsyncReaderEvent
{
	friend class CComThread;
	struct Trio
	{
		ComMsg msg;
		HANDLE Event; // Используется для ожидания ответа на сообщение, посланное SerialLink::SendWaitForReply()
		bool Recieved;
	};
	typedef list<Trio> CWaitList;
protected:
	vector<ComMsg> m_Pool;
	vector<HWND> m_SendList;
	CWaitList m_WaitForReplyList;
	size_t m_Size;
	size_t m_Pos;
	CCriticalSection m_CS;
	bool WaitForMsgReply(ComMsg* msg, CThreadSafeQueue<ComMsg>* ComSendQueue); // Используется для ожидания ответа на сообщение, посланное SerialLink::SendWaitForReply()
	void PutMsg(ComMsg* msg, short Type); // Помещение полученного сообщения в буфер. При помещении в буфер отправляются уведомления подписчикам 
	CMailslotAsyncReader MailslotAsyncReader;
	CMailslotWriter MailslotWriter;
	void TryMailslotsWrite(ComMsg* msg, short Type);

	// Интерфейс IAsyncReaderEvent
	void OnIpcMessage(BYTE *pbMessage, DWORD dwMessageLength);

public:
	CMsgPool();
	virtual ~CMsgPool();
	void SetPoolSize(size_t Size);
	void RegisterMsgReception(HWND hWnd);// Подписка на получение уведомлений о сообщениях
	void UnRegisterMsgReception(HWND hWnd);// Отписка от получения уведомлений о сообщениях
	ComMsg GetMsg(size_t Id); // Считывание полученного сообщения по Id. Потокобезопасно.
};

#endif // !defined(AFX_MSGPOOL_H__C453CF0D_C02E_4945_B589_3469FE99DF5D__INCLUDED_)
