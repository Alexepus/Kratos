// MsgPool.cpp: implementation of the CMsgPool class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Spec.h"
#include "MsgPool.h"
#include "SpecIpcdata0.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMsgPool::CMsgPool()
{
	MailslotAsyncReader.Connect("Spec_SerialIn", static_cast<IAsyncReaderEvent*>(this));
	SetPoolSize(16);
	m_Pos=0;
}

CMsgPool::~CMsgPool()
{

}

void CMsgPool::SetPoolSize(size_t Size)
{
	m_CS.Lock();
	m_Size=Size;
	m_Pool.resize(m_Size);
	m_SendList.reserve(10);
	m_CS.Unlock();
}

// Помещение полученного сообщения в буфер. При помещении в буфер отправляются уведомления подписчикам
// <param name="Type">Тип сообщения из потока: {SERIAL_SYNC_MSG, SERIAL_ASYNC_MSG,SERIAL_ERROR_COM_FAILED,SERIAL_ERROR_TIMEOUT}</param>
void CMsgPool::PutMsg(ComMsg* msg, short Type)
{
	m_CS.Lock();
	if(msg)
	{
		CWaitList::iterator Iter;
		for(Iter=m_WaitForReplyList.begin(); Iter!=m_WaitForReplyList.end();Iter++)
			if(Iter->msg.Buf[0]==msg->Buf[0])
			{
				if(Type==SERIAL_SYNC_MSG)
				{
					Iter->Recieved=true;
					Iter->msg=*msg;
				}
				else
					Iter->Recieved=false;
				SetEvent(Iter->Event);
				break;
			}
		m_Pos++;
		m_Pos=m_Pos%m_Size;
		m_Pool[m_Pos]=*msg;
		for(size_t i=0; i<m_SendList.size();i++)
			PostMessage(m_SendList[i], WM_COM_EVENT,m_Pos,(Type<<16)|msg->Buf[0]);
	}
	else
		for(size_t i=0; i<m_SendList.size();i++)
			PostMessage(m_SendList[i], WM_COM_EVENT,0,(Type<<16)|0);
	TryMailslotsWrite(msg, Type);
	m_CS.Unlock();
}

// Считывание полученного сообщения по Id. Потокобезопасно.
ComMsg CMsgPool::GetMsg(size_t Id)
{
	m_CS.Lock();
	ComMsg msg;
	msg=m_Pool[Id%m_Size];
	m_CS.Unlock();
	return msg;
}

void CMsgPool::RegisterMsgReception(HWND hWnd)
{
	m_CS.Lock();
	bool found=false;
	for(size_t i=0; i<m_SendList.size();i++)
		if(m_SendList[i]==hWnd)
		{
			found=true;
			break;
		}
	if(!found)
		m_SendList.push_back(hWnd);
	m_CS.Unlock();
}

void CMsgPool::UnRegisterMsgReception(HWND hWnd)
{
	m_CS.Lock();
	vector<HWND>::iterator it;
	for(it=m_SendList.begin(); it!=m_SendList.end(); it++)
	{
		if(*it==hWnd)
		{
			m_SendList.erase(it);
			break;
		}
	}
	m_CS.Unlock();
}

bool  CMsgPool::WaitForMsgReply(ComMsg* msg, CThreadSafeQueue<ComMsg>* ComSendQueue)
{
	HANDLE Event=CreateEvent(NULL, TRUE, FALSE, NULL);
	Trio trio;
	trio.msg=*msg;
	trio.Event=Event;
	trio.Recieved=false;
	CWaitList::iterator Iter;	
	m_CS.Lock();
	Iter=m_WaitForReplyList.insert(m_WaitForReplyList.end(),trio);
	m_CS.Unlock();
	ComSendQueue->Push(*msg);
	DWORD WaitResult=WaitForSingleObject(Event,2000);
	if(WaitResult!=WAIT_OBJECT_0)
	{
		m_CS.Lock();
		m_WaitForReplyList.erase(Iter);
		m_CS.Unlock();
		CloseHandle(Event);
		return false;
	}
	else
	{
		bool Result;
		m_CS.Lock();
		Result=Iter->Recieved;
		if(Iter->Recieved)
			*msg=Iter->msg;
		m_WaitForReplyList.erase(Iter);
		m_CS.Unlock();
		CloseHandle(Event);
		return Result;
	}
}

// Отправка принятого сообщения в мейл-слот(ы), если они созданы получателями
// <param name="Type">Тип сообщения из потока: {SERIAL_SYNC_MSG, SERIAL_ASYNC_MSG,SERIAL_ERROR_COM_FAILED,SERIAL_ERROR_TIMEOUT}</param>
void CMsgPool::TryMailslotsWrite( ComMsg* msg, short Type )
{
	if(MailslotWriter.Connect("Spec_SerialOut", theApp.Ini.SerialLinkOutServer))
	{
		SpecIpcdata0 data;
		data.CommandType = Type;
		data.comMsg = *msg;
		memset(data.StringMgs, 0, 65);
		MailslotWriter.Write((BYTE*)&data, sizeof(SpecIpcdata0));
	}
}

void CMsgPool::OnIpcMessage(BYTE *pbMessage, DWORD dwMessageLength)
{
	if(dwMessageLength != sizeof(SpecIpcdata0))
	{
		LogFileFormat("CMsgPool::OnIpcMessage Получено сообщение IPC длиной %i вместо %i байт", dwMessageLength, sizeof(SpecIpcdata0));
		return;
	}
	SpecIpcdata0 *data = (SpecIpcdata0 *)pbMessage;
	theApp.m_SerialLink.SendComMessage(data->comMsg);
	delete[] pbMessage;
}
