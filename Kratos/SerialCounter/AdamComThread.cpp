#include "stdafx.h"
#include "AdamComThread.h"
#include "ProcessableMessage.h"
#include "ComFunctions.h"
#include "..\LogToFile.h"
#include "..\ProgNew.h"
#include "..\Exceptions.h"

extern CProgNewApp theApp;

AdamComThread::AdamComThread(void)
{
	m_hComPort=INVALID_HANDLE_VALUE;
	m_hComThread=NULL;
	m_ReplyTimeout=200; //0.2 сек
	m_CmdExitThread=false;
	m_bViewSentRawData=theApp.Ini.ViewSentRawData.Value;
	m_bViewReceivedRawData=theApp.Ini.ViewReceivedRawData.Value;
}

AdamComThread::~AdamComThread(void)
{
	if(m_hComThread!=NULL)
	{
		m_CmdExitThread=true;
		WaitForSingleObject(m_hComThread,100);
		DWORD exitCode=0;
		GetExitCodeThread(m_hComThread,&exitCode);
		if(exitCode == STILL_ACTIVE)
			TerminateThread(m_hComThread, -1);
	}

	if(m_hComPort!=INVALID_HANDLE_VALUE)
		CloseHandle(m_hComPort);
}

// Поток по одному вычитывает сообщения из очереди на отправку, записывает их в порт и ждет ответа.
// Если ответ получен или истук таймаут, то переходит к слудеющему сообщению
ULONG WINAPI AdamComThread::ComThread(LPVOID pParam)
{
	AdamComThread *pComObj=static_cast<AdamComThread *>(pParam);
	unsigned long ReadFromPort=0;
	BYTE ReadBuf[256];
	ProcessableMessage *BeingSendedMsg;
	ComMsgTale TailMsg;	
	DWORD sendMsgTimeStamp;
	while(!pComObj->m_CmdExitThread)
	{	
		TailMsg.MsgSize=0;
		WaitForSingleObject(pComObj->m_ComSendQueue.m_EventHasData, 10);
		if(pComObj->m_ComSendQueue.try_front_pop(BeingSendedMsg))
		{
			if(BeingSendedMsg->FreeToDelete)
			{
				delete BeingSendedMsg;
				continue;
			}
			pComObj->m_CsReconnect.Lock();
			pComObj->Rs232Send(BeingSendedMsg->Message);
			pComObj->m_CsReconnect.Unlock();			

			sendMsgTimeStamp = GetTickCount();
			bool isTimeout;
			bool replyReceived = false;
			bool anyDataReceived = false;
			do 
			{
				pComObj->m_CsReconnect.Lock();
				BOOL success = ReadFile(pComObj->m_hComPort, ReadBuf, 32, &ReadFromPort, NULL);
				pComObj->m_CsReconnect.Unlock();
				if(!success)
					LogFileFormat("Не удалось считать данные из последовательного порта \"%s\"%s", pComObj->m_PortName.GetString(),
					pComObj->IsPortHandleValid()? "" : ": неверный порт");
				if(ReadFromPort == 0)
				{
					Sleep(1);
				}
				else
				{
					anyDataReceived = true;
					if(pComObj->m_bViewReceivedRawData)
						pComObj->DumpRawData("Raw<-", &ReadBuf[0], ReadFromPort);
					// Соединяем полученные данные с хвостом
					TailMsg.ConcatWithNewData(ReadBuf, ReadFromPort);
					std::list<CString> strings = SplitOnCr(TailMsg.Buf, TailMsg.MsgSize);
					for(std::list<CString>::iterator i = strings.begin(); i != strings.end(); i++)
					{
						AdamMessage aMsg = AdamMessage::Parse(*i);
						if(aMsg.IsValidMessage && aMsg.UnitAddress == BeingSendedMsg->Message.UnitAddress
							&& aMsg.CommandNumber == BeingSendedMsg->Message.CommandNumber)
						{
							NotifyReplyReceived(BeingSendedMsg, aMsg);
							replyReceived = true;
							break;
						}
					}

				}
				isTimeout = (GetTickCount() - sendMsgTimeStamp > pComObj->m_ReplyTimeout);
			} while (!isTimeout && !replyReceived);
			if(!replyReceived)
			{ 
				AdamMessage aMsg;
				aMsg.IsStartMarkSet = anyDataReceived;
				NotifyReplyReceived(BeingSendedMsg, aMsg);
			}
		}
	}

	return 1;
}
void AdamComThread::NotifyReplyReceived(ProcessableMessage* beingSendedMsg, AdamMessage reply)
{
	if(beingSendedMsg->ProcessingFinishedCallback != nullptr)
	{
		beingSendedMsg->ProcessingFinishedCallback(reply, beingSendedMsg->CallbackParam);
		delete beingSendedMsg;
	}
	else
	{
		if(beingSendedMsg->FreeToDelete)
		{
			delete beingSendedMsg;
			return;
		}
		beingSendedMsg->Reply = reply;
		beingSendedMsg->ReplyReady.SetEvent();
	}
}
std::list<CString> AdamComThread::SplitOnCr(byte buf[], int len)
{
	std::list<CString> list;
	CString s;
	for(int i = 0; i < len; i++)
	{
		char c = (char)buf[i];
		if(c != '\r')
			s += c;
		else
		{
			list.push_back(s);
			s = "";
		}
	}
	return list;
}

// Открытие порта (с закрытием предыдущего), перезапуск рабочего потока, если он не запущен
bool AdamComThread::ReconnectCom(const char *ComName, CString* message)
{
	m_PortName = ComName;
	LPTSTR lpMsgBuf;

	m_CsReconnect.Lock();

	if(m_hComPort!=INVALID_HANDLE_VALUE)
	{
		CloseHandle(m_hComPort);
		m_hComPort=INVALID_HANDLE_VALUE;
	}
	m_hComPort=CreateFile(ComName, GENERIC_WRITE|GENERIC_READ, 0, NULL, OPEN_EXISTING , 
		FILE_ATTRIBUTE_NORMAL, NULL);
	if(m_hComPort==INVALID_HANDLE_VALUE)
	{

		FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_SYSTEM| 
			FORMAT_MESSAGE_IGNORE_INSERTS, NULL, GetLastError(),MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPTSTR) &lpMsgBuf, 0, NULL);
		if(message != nullptr)
			*message = lpMsgBuf;
		LogFileFormat("Ошибка открытия порта %s: %s", &ComName[0],(LPCTSTR)lpMsgBuf);
		LocalFree(lpMsgBuf);
		m_CsReconnect.Unlock();
		return false;
	}
	ConfigureComPort(m_hComPort, theApp.Ini.CounterPortBaudRate.Value);

	if(m_hComThread!=NULL)
	{
		DWORD ExitCode=0;
		GetExitCodeThread(m_hComThread,&ExitCode);
		if(ExitCode!=STILL_ACTIVE) //Поток вышел по ошибке
		{
			DWORD Unused;
			m_hComThread=CreateThread(NULL,NULL, &AdamComThread::ComThread,this,0,&Unused);
		}
	}
	else //Начальное создание потока
	{
		DWORD Unused;
		m_hComThread=CreateThread(NULL,NULL, &AdamComThread::ComThread,this,0,&Unused);
	}
	m_CsReconnect.Unlock();
	return true;
}

//_______________________________________________________________________________________
//Преобразует сообщение в формат канального уровня и отправляет данные в COM-порт
void AdamComThread::Rs232Send(AdamMessage &msg)
{
	unsigned long RetCount=0;
	
	CString str = msg.ToString();

	BOOL success = WriteFile(m_hComPort, str.GetBuffer(), str.GetLength() , &RetCount, NULL);
	if(!success)
		LogFileFormat("Не удалось записать данные в последовательный порт \"%s\"%s", m_PortName.GetString(),
		IsPortHandleValid()? "" : ": неверный порт");
	if(m_bViewSentRawData)
		DumpRawData("Raw->", (byte*)str.GetBuffer(), str.GetLength());
}

// Синхронная отправка сообщения с возвратом ответа устройства
AdamMessage AdamComThread::SendComMessage(const AdamMessage &msg)
{
	if(IsPortHandleValid())
	{
		ProcessableMessage* pm = new ProcessableMessage(msg);
		m_ComSendQueue.Push(pm);
		if(WaitForSingleObject(pm->ReplyReady.m_hObject, m_ReplyTimeout) == WAIT_OBJECT_0)//INFINITE
		{
			AdamMessage reply = pm->Reply;
			delete pm;
			return reply;
		}
		pm->FreeToDelete = true;
	}
	return AdamMessage();
}

// Асинхронная отправка сообщения
void AdamComThread::PostComMessage(const AdamMessage &msg, TwoParamFunc replyCallback, void* param)
{
	ProcessableMessage* pm = new ProcessableMessage(msg, replyCallback, param);
	m_ComSendQueue.Push(pm);
}

void AdamComThread::DumpRawData(const char* Discript, BYTE* ReadBuf, int ReadFromPort)
{
	CString s;
	for(int i = 0; i < ReadFromPort; i++)
		s += (char)ReadBuf[i];
	LogFileFormat("%s%s", Discript, s.GetString());
}


