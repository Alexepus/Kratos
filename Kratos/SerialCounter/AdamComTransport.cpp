#include "stdafx.h"
#include "AdamComTransport.h"
#include "ComFunctions.h"
#include "..\LogToFile.h"
#include "..\ProgNew.h"
#include "..\Exceptions.h"
#include "..\DomainLock.h"

extern CProgNewApp theApp;

AdamComTransport::AdamComTransport(void)
{
	m_bViewReceivedRawData = theApp.Ini.ViewReceivedRawData.Value;
	m_bViewSentRawData = theApp.Ini.ViewSentRawData.Value;
	m_ReplyTimeout = 300; // 300 мс
}

AdamComTransport::~AdamComTransport(void)
{
	if(m_hComPort!=INVALID_HANDLE_VALUE)
		CloseHandle(m_hComPort);
}


AdamMessage AdamComTransport::SendAndReceiveReply(AdamMessage message)
{
	DWORD sendMsgTimeStamp;
	const int readBufSize = 256;
	BYTE ReadBuf[readBufSize];
	DWORD readFromPort;
	ComMsgTale tailMsg;

	DomainLock lock(m_CsCom);

	Rs232Send(message);

	sendMsgTimeStamp = GetTickCount();
	bool isTimeout;
	bool anyDataReceived = false;
	do 
	{
		BOOL success = ReadFile(m_hComPort, ReadBuf, readBufSize, &readFromPort, NULL);
		if(!success)
			LogFileFormat("Не удалось считать данные из последовательного порта \"%s\"%s", theApp.Ini.CounterComPort.Value.GetString(),
			IsPortHandleValid()? "" : ": неверный порт");
		if(readFromPort == 0)
		{
			Sleep(1);
		}
		else
		{
			anyDataReceived = true;
			if(m_bViewReceivedRawData)
				DumpRawData("Raw<-", &ReadBuf[0], readFromPort);
			// Соединяем полученные данные с хвостом
			tailMsg.ConcatWithNewData(ReadBuf, readFromPort);
			std::list<CString> strings = SplitOnCr(tailMsg.Buf, tailMsg.MsgSize);
			for(std::list<CString>::iterator i = strings.begin(); i != strings.end(); i++)
			{
				AdamMessage aMsg = AdamMessage::Parse(*i);
				if(aMsg.IsValidMessage && aMsg.UnitAddress == message.UnitAddress
						&& aMsg.CommandNumber == message.CommandNumber)
					return aMsg;
			}

		}
		isTimeout = (GetTickCount() - sendMsgTimeStamp > m_ReplyTimeout);
	} while (!isTimeout);
	CString msg;
	CString sent = message.ToString();
	sent.Replace("\n","");
	if(anyDataReceived)
		msg.Format("От устройства получен некорректный ответ на запрос '%s'", sent.GetString());
	else
		msg.Format("Устройство не ответило на запрос '%s' в течение %i мс", sent.GetString(), GetTickCount() - sendMsgTimeStamp);
	LogFileFormat(msg.GetString());
	throw EXCEPTION(msg.GetString());
}

//_______________________________________________________________________________________
//Преобразует сообщение в формат канального уровня и отправляет данные в COM-порт
void AdamComTransport::Rs232Send(AdamMessage &msg)
{
	unsigned long RetCount=0;

	CString str = msg.ToString();

	BOOL success = WriteFile(m_hComPort, str.GetBuffer(), str.GetLength() , &RetCount, NULL);
	if(!success)
		LogFileFormat("Не удалось записать данные в последовательный порт \"%s\"%s", theApp.Ini.CounterComPort.Value.GetString(),
		IsPortHandleValid()? "" : ": неверный порт");
	if(m_bViewSentRawData)
		DumpRawData("Raw->", (byte*)str.GetBuffer(), str.GetLength());
}

void AdamComTransport::DumpRawData(const char* Discript, BYTE* ReadBuf, int ReadFromPort)
{
	CString s;
	for(int i = 0; i < ReadFromPort; i++)
		s += (char)ReadBuf[i];
	LogFileFormat("%s%s", Discript, s.GetString());
}

std::list<CString> AdamComTransport::SplitOnCr(byte buf[], int len)
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

// Открытие порта (с закрытием предыдущего)
bool AdamComTransport::ReconnectCom(const char *ComName, CString* message)
{
	LPTSTR lpMsgBuf;

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
		LogFileFormat("Ошибка открытия порта %s: %s", ComName,(LPCTSTR)lpMsgBuf);
		LocalFree(lpMsgBuf);
		return false;
	}
	ConfigureComPort(m_hComPort, theApp.Ini.CounterPortBaudRate.Value);

	return true;
}