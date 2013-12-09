// ComThread.cpp: implementation of the CComThread class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Spec.h"
#include "ComThread.h"
#include "ComFunctions.h"
#include "LogToFile.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CComThread::CComThread() :CComThreadBasic()
{
	char chLogFileName[_MAX_PATH];
	::GetModuleFileName(NULL, (char *)chLogFileName, _MAX_PATH);
	LogFileName=chLogFileName;
	int EndPath=LogFileName.ReverseFind('\\');
	LogFileName.Delete(EndPath+1,LogFileName.GetLength()-EndPath-1);
	LogFileName+="SerialLink.log";
	m_bViewSentRawData=false;
	m_bViewReceivedRawData=false;
	m_Pool.SetPoolSize(128);
}

	//Анализирует сообщение, выполняет соответствующее действие, 
	//возвращает 1, если принято ожидаемое сообщение, 0 - в остальных случаях
	//Если SendMsg==NULL, анализирует асинхронное сообщение. Возвращает TRUE, если
	//ReceivedMsg - правильный ответ на SentMsg и сообщение обработано
int CComThread::AnalyseCorrectMsg(ComMsg* ReceivedMsg, ComMsg* SentMsg)
{
	int Return=FALSE;
	if(SentMsg==NULL) //Асинхронное сообщение
	{
		CString str, str2;
		str.Format("Async Msg: 0x%.2X", (int)(BYTE)ReceivedMsg->Buf[0]);  
		for(int i=1;i<ReceivedMsg->MsgSize;i++)
		{
			str2.Format("%s,0x%.2X", (PCSTR)str, (int)(BYTE)ReceivedMsg->Buf[i]);
			str=str2;
		}
		LogSpecifiedFileFormat(LogFileName, str);
		m_Pool.PutMsg(ReceivedMsg,SERIAL_ASYNC_MSG);

		Return=TRUE;
	}
	else if((ReceivedMsg->MsgSize>0) && (ReceivedMsg->Buf[0]==SentMsg->Buf[0]))
	{				//Синхронное сообщение
		CString str, str2;
		str.Format("Received: 0x%.2X", (int)(BYTE)ReceivedMsg->Buf[0]);  
		for(int i=1;i<ReceivedMsg->MsgSize;i++)
		{
			str2.Format("%s,0x%.2X", (PCSTR)str, (int)(BYTE)ReceivedMsg->Buf[i]);
			str=str2;
		}
		//LogSpecifiedFileFormat(LogFileName, str);
		m_Pool.PutMsg(ReceivedMsg,SERIAL_SYNC_MSG);

		Return=TRUE;
	}
return Return;
}

	//Может запрашивать действие пользователя и заканчивать поток
void CComThread::ReportComErrorAndExit()
{
	LPTSTR lpMsgBuf;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_SYSTEM| 
	FORMAT_MESSAGE_IGNORE_INSERTS, NULL, GetLastError(),MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
	(LPTSTR) &lpMsgBuf, 0, NULL);
	CString str2;
	DetectSerialErrors(m_hComPort,&str2);
	LogSpecifiedFileFormat(LogFileName, "ReadFile: ошибка чтения из порта.%s %s",lpMsgBuf,(LPCSTR)str2);
	LogFileFormat("ReadFile: ошибка чтения из порта.%s %s",lpMsgBuf,(LPCSTR)str2);
	LocalFree(&lpMsgBuf);

	m_Pool.PutMsg(NULL,SERIAL_ERROR_COM_FAILED);
	AfxEndThread(0);
}
	
	//Может выводить сырые данные при установленных флагах m_bViewSentRawData и
	//m_bViewReceivedRawData
void CComThread::DumpRawData(const char* Discript, BYTE* ReadBuf, int ReadFromPort)
{
	if(ReadFromPort>0)
	{
		CString str,str2;
		str.Format("%s%i Bytes: 0x%.2X",Discript, ReadFromPort, (int)(unsigned char)ReadBuf[0]);
		for (int i=1; i<ReadFromPort; i++)
		{
			str2.Format("%s,0x%.2X", (LPCSTR)str,(int)(unsigned char)ReadBuf[i]);
			str=str2;
		}
		LogSpecifiedFileFormat(LogFileName, str);
	}
}

	//Вызывается после отправки сообщения
void CComThread::DumpSentMsg(ComMsg* SentMsg)
{
	CString str, str2;
	str.Format("Sent: 0x%.2X", (int)(BYTE)SentMsg->Buf[0]);  
	for(int i=1;i<SentMsg->MsgSize;i++)
	{
		str2.Format("%s,0x%.2X", (PCSTR)str, (int)(BYTE)SentMsg->Buf[i]);
		str=str2;
	}
	//LogSpecifiedFileFormat(LogFileName, str);
}
	
	//Вызывается при некорректных/незаконченных сообщениях в приемном буфере
void CComThread::ReportJunkMessage(BYTE *Buf, int Size, char Reason)
{
	CString str,str2;
	str.Format("Junk Msg-%c (%i): 0x%.2X",Reason, Size, (int)(unsigned char)Buf[0]);
	for (int i=1; i<Size; i++)
	{
		str2.Format("%s,0x%.2X", (LPCSTR)str,(int)(unsigned char)Buf[i]);
		str=str2;
	}
	LogSpecifiedFileFormat(LogFileName, str);
}

	//Вызывается при истекшем тайм-ауте ожидания ответа
void CComThread::ReportTimeOut(ComMsg* SentMsg)
{
	CString str, str2;
	str.Format("No reply. Cmd:0x%.2X", (int)(BYTE)SentMsg->Buf[0]);  
	for(int i=1;i<SentMsg->MsgSize;i++)
	{
		str2.Format("%s,0x%.2X", (PCSTR)str, (int)(BYTE)SentMsg->Buf[i]);
		str=str2;
	}
	LogSpecifiedFileFormat(LogFileName, str);
	m_Pool.PutMsg(SentMsg,SERIAL_ERROR_TIMEOUT);

}

bool CComThread::SendWaitForReply(ComMsg* msg)
{
	return m_Pool.WaitForMsgReply(msg, &m_ComSendQueue);
}
