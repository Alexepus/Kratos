#include "stdafx.h"
#include "ComThreadBasic.h"
#include "ComFunctions.h"
#include "ThreadSafeQueue.h"
#include "LogToFile.h"
#include <list>


CComThreadBasic::CComThreadBasic()
{
	m_hComPort=INVALID_HANDLE_VALUE;
	m_hComThread=NULL;
	m_ReplyTimeout=0.2; //0.2 ���
	m_BaudRate=57600;
	m_CmdExitThread=false;
	m_bViewSentRawData=false;
	m_bViewReceivedRawData=false;
}

//��������� �����, ���� �� ��� ����, ��������� ����, ������ �������
CComThreadBasic::~CComThreadBasic()
{
	if(m_hComThread!=NULL)
	{
		m_CmdExitThread=true;
		WaitForSingleObject(m_hComThread,100);
	}

	if(m_hComPort!=INVALID_HANDLE_VALUE)
		CloseHandle(m_hComPort);
}

ULONG WINAPI CComThreadBasic::ComThread(LPVOID pParam)
{
	CComThreadBasic *pComObj=static_cast<CComThreadBasic *>(pParam);
	unsigned long ReadFromPort=0;
	BYTE ReadBuf[256];
	ComMsg SendMsg, ReceivedMsg;
	ComMsgTale TailMsg;
	TailMsg.MsgSize=0;
	int MsgBytes;
	int ErrorMsgCount=0;
	std::list<ComMsg> WaitForResponseList;
	Sleep(200); // �������, ����� �� ��������� ������ ��� ��������� � MainFrame

	while(!pComObj->m_CmdExitThread)
	{	
		if(pComObj->m_ComSendQueue.try_front_pop(SendMsg))
		{
			pComObj->m_CsReconnect.Lock();
			pComObj->Rs232Send(&SendMsg);
			pComObj->m_CsReconnect.Unlock();
			
			pComObj->DumpSentMsg(&SendMsg);
			if(SendMsg.WaitForReply)
			{
				SendMsg.TimeStamp=GetTickCount();
				WaitForResponseList.push_back(SendMsg);
			}
		}

		pComObj->m_CsReconnect.Lock();
		if (!ReadFile(pComObj->m_hComPort, ReadBuf, 32, &ReadFromPort, NULL))
		{
			pComObj->m_CsReconnect.Unlock();
			pComObj->ReportComErrorAndExit();
		}
		pComObj->m_CsReconnect.Unlock();
		if(!ReadFromPort)
		{
			pComObj->CheckTailTimeout(&TailMsg);
			pComObj->AnalyseMsgWithWaitList(WaitForResponseList, NULL);
			if(pComObj->m_ComSendQueue.IsEmpty())
				Sleep(1);
			continue;
		}
		else
		{
			if(pComObj->m_bViewReceivedRawData)
				pComObj->DumpRawData("Raw<-", &ReadBuf[0], ReadFromPort);
			do   //�������� ������ �� 0, 1 ��� ����� �������
			{
				MsgBytes=pComObj->GetCorrectMsg(ReadBuf, ReadFromPort, &ReceivedMsg, &TailMsg);
				if(MsgBytes>0)
					pComObj->AnalyseMsgWithWaitList(WaitForResponseList,&ReceivedMsg);
				else
					pComObj->AnalyseMsgWithWaitList(WaitForResponseList, NULL);
				ReadFromPort=0;
			}while(MsgBytes!=0);
		}

	}

return 1;
}

void CComThreadBasic::ReconnectCom(const char *ComName)
{
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
		Msg("������ �������� ����� %s.\n%s", &ComName[0],(LPCTSTR)lpMsgBuf);
		LocalFree(lpMsgBuf);
		m_CsReconnect.Unlock();
		return;
	}
	ConfigureComPort(m_hComPort, m_BaudRate);
	
	if(m_hComThread!=NULL)
	{
		DWORD ExitCode=0;
		GetExitCodeThread(m_hComThread,&ExitCode);
		if(ExitCode!=STILL_ACTIVE) //����� ����� �� ������
		{
			DWORD Unused;
			m_hComThread=CreateThread(NULL,NULL, &CComThreadBasic::ComThread,this,0,&Unused);
		}
	}
	else //��������� �������� ������
	{
		DWORD Unused;
		m_hComThread=CreateThread(NULL,NULL, &CComThreadBasic::ComThread,this,0,&Unused);
	}
	m_CsReconnect.Unlock();
}

//_______________________________________________________________________________________
//���������� ����� ��������� ��� START-, END-����� � ESCAPE-�������. ����� �������� ��������� ���, 
//���� ������������ �������� �� ������ =0.
int CComThreadBasic::GetCorrectMsg(BYTE *NewMsgPart, int NewPartSize, ComMsg* ReceivedMsg, ComMsgTale* TailMsg)
{
BYTE *Tail=TailMsg->Buf;
int &TailSize=TailMsg->MsgSize;
BYTE TmpMsg[512];
int i, TmpMsgSize=0;
char CRC=0;
//���������� ������ � ����� ������
if(TailSize+NewPartSize>512) //������������ ������
{
TailSize=0;
return -1;
}
memmove(Tail+TailSize,NewMsgPart,NewPartSize);
TailSize+=NewPartSize;

//������������ ������ ������ � ����� �������
bool WasStart=false;
for(i=0; i<TailSize; i++)
{
	if(Tail[i]==(BYTE)RS485_START)
	{
		if(i!=0)
		{
			memmove(TmpMsg, Tail, i);
			memmove(Tail, Tail+i, TailSize-i);
			TailSize=TailSize-i;
			ReportJunkMessage(TmpMsg, i, 'S');
			i=-1;
		}
		WasStart=true;
	}
	else if(Tail[i]==(BYTE)RS485_END)
	{
		if(WasStart)
		{
			memmove(TmpMsg, Tail+1, i-1);
			TmpMsgSize=i-1;
			memmove(Tail, Tail+i+1, TailSize-i-1);
			TailSize=TailSize-i-1;
			break;
		}
		else
		{
			memmove(TmpMsg, Tail, i+1);
			memmove(Tail, Tail+i+1, TailSize-i-1);
			TailSize=TailSize-i-1;
			ReportJunkMessage(TmpMsg, i+1, 'E');
			i=-1;
		}
	}
}
TailMsg->TimeStamp=GetTickCount();

if(TmpMsgSize<=0)  //��������� �� �������� ������
	return 0;

//��������� ESCAPE-�������
for(i=0; i<TmpMsgSize-1; i++)
{
	if(TmpMsg[i]==(BYTE)RS485_ESCAPE)
	{
	  	TmpMsg[i]=TmpMsg[i+1]+RS485_START;
	 	memmove(TmpMsg+i+1, TmpMsg+i+2, TmpMsgSize-i-2);
	 	TmpMsgSize--;
	  
	}
}

//����������� � �������� ����� ����������� ���������
memmove(ReceivedMsg->Buf, TmpMsg, TmpMsgSize);
ReceivedMsg->MsgSize=TmpMsgSize;
ReceivedMsg->TimeStamp=GetTickCount();
return ReceivedMsg->MsgSize;
}

//_______________________________________________________________________________________
//������ � ThreadSafe-������� �������� ����� ���������
//����� ���������� �� ������� ������ 
void CComThreadBasic::SendComMessage(const ComMsg &msg)
{
m_ComSendQueue.Push(msg);
}

//_______________________________________________________________________________________
//����������� ��������� � ������ ���������� ������ � ���������� ������ � COM-����
void CComThreadBasic::Rs232Send(ComMsg* msg)
{
  unsigned char sum=0;
  int i;
  unsigned Count=0;
  unsigned long RetCount=0;
  BYTE Buf[50]={0};
BYTE b;
	Buf[Count++]=RS485_START;
	for(i=0; i<msg->MsgSize; i++)
	{
		b=msg->Buf[i];
 		if((msg->Buf[i]==RS485_START)||(msg->Buf[i]==RS485_END)||(msg->Buf[i]==RS485_ESCAPE))
		{   
			Buf[Count++]=RS485_ESCAPE;
			Buf[Count++]=msg->Buf[i]-(char)RS485_START;
		}
		else
			Buf[Count++]=msg->Buf[i];
	}
	Buf[Count++]=RS485_END;
	WriteFile(m_hComPort, &Buf, Count , &RetCount, NULL);
	if(m_bViewSentRawData && (Count>0))
		DumpRawData("Raw->", Buf, Count);
}

//_______________________________________________________________________________________
//������������ ������� ������������ ���������, ������� �� ����� ������. ���� ReceivedMsg!=NULL,
//�� ������� ����������� �������� ��������� �� ������������ ������. ���� ������������
//�������, ��������� ��������, ������� ������������. ���� �� �������, ���������
//����������� ���������.
//����� ������� ��������� ������ �� ������� (������ ��������� � �������).  
void CComThreadBasic::AnalyseMsgWithWaitList(std::list<ComMsg>& List, ComMsg *ReceivedMsg)
{
	std::list<ComMsg>::iterator iter;
	//ComMsg SentMsg;
	bool Processed=false;
	if(ReceivedMsg!=NULL)
	{
		for (iter = List.begin(); iter != List.end();)
		{
			if(AnalyseCorrectMsg(ReceivedMsg, &(*iter)))
			{
				//��������� ����������. �������� ������� �� �������, �����
				List.erase(iter);
				Processed=true;
				break;
			}
			else
				++iter;
		}

		if(!Processed)
		{
			//����� ������������ (����������������� ���� ����������) ���������
			AnalyseCorrectMsg(ReceivedMsg, NULL);
		}
	}

	//�������� �� �������
	ULONG TickCount=GetTickCount();
	for (iter = List.begin(); iter != List.end();)
	{
		if(TickCount-iter->TimeStamp>m_ReplyTimeout*1000)
		{
			//�������.
			ReportTimeOut(&(*iter));
			iter=List.erase(iter);
		}
		else
			++iter;
	}
}

//�������� ������ ��������� �� �������. ����� - ��� ���� �� ��������, �� � �� �����������, 
//����� ���������. ���� ������� (�.�. ����� � ������ ���������� �����) �����, �� ����� ������
//��������� � �������� ���������  
void CComThreadBasic::CheckTailTimeout(ComMsgTale *TailMsg)
{
	if(TailMsg->MsgSize>0)
		if(TailMsg->TimeStamp+m_ReplyTimeout*1000<GetTickCount())
		{
			//�������.
			ComMsg msg;
			msg.MsgSize=min(TailMsg->MsgSize,sizeof(msg.Buf));
			memmove(msg.Buf,TailMsg->Buf,msg.MsgSize);
			ReportJunkMessage(msg.Buf, msg.MsgSize, 'T');
			TailMsg->MsgSize=0;
		}
}


