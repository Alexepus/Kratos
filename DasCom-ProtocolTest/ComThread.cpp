#include "stdafx.h"
#include "ComThread.h"
#include "ComFunctions.h"
#include "ThreadSafeQueue2.h"
#include "Termo.h"
#include <list>


extern CThreadSafeQueue2 Queue; 
extern CTermoApp theApp;

UINT ComThread(LPVOID pParam)
{

	unsigned long ReadFromPort=0;
	char ReadBuf[256];
	LPTSTR lpMsgBuf;
	char ComName[20];
	ComMsg SendMsg, RecievedMsg, TailMsg;
	TailMsg.MsgSize=0;
	int MsgBytes;
	int ErrorMsgCount=0;
	std::list<ComMsg> WaitForResponseList;

	strcpy(ComName,(LPCSTR)theApp.ComPortName);
	theApp.pTermoDlg->hComPort=CreateFile(ComName, GENERIC_WRITE|GENERIC_READ, 0, NULL, OPEN_EXISTING , 
	FILE_ATTRIBUTE_NORMAL, NULL);
	if(theApp.pTermoDlg->hComPort==INVALID_HANDLE_VALUE)
	{

		FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_SYSTEM| 
		FORMAT_MESSAGE_IGNORE_INSERTS, NULL, GetLastError(),MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR) &lpMsgBuf, 0, NULL);
		Msg("������ �������� ����� %s.\n%s", &ComName[0],(LPCTSTR)lpMsgBuf);
		LocalFree(lpMsgBuf);
		return 0;
	}
	ConfigureComPort(theApp.pTermoDlg->hComPort);


	while(!theApp.ExitThreads)
	{
		if(Queue.Pop(&SendMsg)>0)
		{
			Rs232Send(theApp.pTermoDlg->hComPort, &SendMsg);
			if(SendMsg.WaitForReply)
			{
				SendMsg.TimeStamp=GetTickCount();
				WaitForResponseList.push_front(SendMsg);
			}
		}
		else
		{
			if (!ReadFile(theApp.pTermoDlg->hComPort, ReadBuf, 16, &ReadFromPort, NULL)) 
				ReportErrorAndExit();
			if(!ReadFromPort && (WaitForResponseList.size()==0))
			{
				CheckTailTimeout(&TailMsg);
				Sleep(1);
				continue;
			}
			else
			{
				if(theApp.ViewRawData)
				{
					DumpDataToScreen("Raw<-", &ReadBuf[0], ReadFromPort);
					if(theApp.SkipProcessing)
					{
						WaitForResponseList.clear();
						continue;
					}
				}
				do   //�������� ������ �� 0, 1 ��� 2 �������
				{
					MsgBytes=GetCorrectMsg(ReadBuf, ReadFromPort, &RecievedMsg, &TailMsg);
					if(MsgBytes>0)
						AnalyseMsgWithWaitList(WaitForResponseList,&RecievedMsg);
					else
						AnalyseMsgWithWaitList(WaitForResponseList, NULL);
					ReadFromPort=0;
				}while(MsgBytes!=0);
			}

		}
	}

return 0;
}


//_______________________________________________________________________________________
//���������� ����� ��������� ��� START-, END-����� � ESCAPE-�������. ����� �������� ��������� ���, 
//���� ������������ �������� �� ������ =0.
int GetCorrectMsg(char *NewMsgPart, int NewPartSize, ComMsg* RecievedMsg, ComMsg* TailMsg)
{
char *Tail=TailMsg->Buf;
int &TailSize=TailMsg->MsgSize;
char TmpMsg[512];
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
	if(Tail[i]==(char)RS485_START)
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
	else if(Tail[i]==(char)RS485_END)
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
	if(TmpMsg[i]==(char)RS485_ESCAPE)
	{
	  	TmpMsg[i]=TmpMsg[i+1]+RS485_START;
	 	memmove(TmpMsg+i+1, TmpMsg+i+2, TmpMsgSize-i-2);
	 	TmpMsgSize--;
	  
	}
}

//����������� � �������� ����� ����������� ���������
memmove(RecievedMsg->Buf, TmpMsg, TmpMsgSize);
RecievedMsg->MsgSize=TmpMsgSize;
RecievedMsg->TimeStamp=GetTickCount();
return RecievedMsg->MsgSize;
}

//_______________________________________________________________________________________
//����������� ���������, ��������� ��������������� ��������, 
//���������� 1, ���� ������� ��������� ���������, 0 - � ��������� �������
//���� SendMsg==NULL, ����������� ����������� ���������
int AnalyseCorrectMsg(ComMsg* RecievedMsg, ComMsg* SendMsg)
{
	char *ch;
	int Return=FALSE;
	if(SendMsg==NULL)
	{
		int len=theApp.pTermoDlg->GetDlgItem(IDC_EDIT_RESPONSE)->SendMessage(WM_GETTEXTLENGTH,NULL,NULL);
		ch=new char[len+101];
		theApp.pTermoDlg->GetDlgItemText(IDC_EDIT_RESPONSE,ch,len+1);
		sprintf(&ch[strlen(ch)], "Async Msg: ");
		if (RecievedMsg->MsgSize==1)
			sprintf(&ch[strlen(ch)], "1 Byte: 0x%.2X (%i)\r\n", (BYTE)RecievedMsg->Buf[0],(BYTE)RecievedMsg->Buf[0]);
		else if (RecievedMsg->MsgSize==2)
			sprintf(&ch[strlen(ch)], "2 Bytes: 0x%.2X, 0x%.2X\r\n", (BYTE)RecievedMsg->Buf[0], (BYTE)RecievedMsg->Buf[1]);
		else if (RecievedMsg->MsgSize==3)
			sprintf(&ch[strlen(ch)], "3 Bytes: 0x%.2X, 0x%.2X, 0x%.2X\r\n", (BYTE)RecievedMsg->Buf[0], (BYTE)RecievedMsg->Buf[1], (BYTE)RecievedMsg->Buf[2]);
		else 		
			sprintf(&ch[strlen(ch)], "%i Bytes: 0x%.2X,0x%.2X,0x%.2X,0x%.2X...\r\n", RecievedMsg->MsgSize,(BYTE)RecievedMsg->Buf[0], (BYTE)RecievedMsg->Buf[1],(BYTE)RecievedMsg->Buf[2],(BYTE)RecievedMsg->Buf[3]);
		theApp.pTermoDlg->SetDlgItemText(IDC_EDIT_RESPONSE,ch);
		delete[] ch;
		Return=TRUE;
	}
	else if((RecievedMsg->MsgSize>0) && (RecievedMsg->Buf[0]==SendMsg->Buf[0]))
	{
		int len=theApp.pTermoDlg->GetDlgItem(IDC_EDIT_RESPONSE)->SendMessage(WM_GETTEXTLENGTH,NULL,NULL);
		ch=new char[len+101];
		theApp.pTermoDlg->GetDlgItemText(IDC_EDIT_RESPONSE,ch,len+1);
		if (RecievedMsg->MsgSize==1)
			sprintf(&ch[strlen(ch)], "1 Byte: 0x%.2X (%i)\r\n", (BYTE)RecievedMsg->Buf[0],(BYTE)RecievedMsg->Buf[0]);
		else if (RecievedMsg->MsgSize==2)
			sprintf(&ch[strlen(ch)], "2 Bytes: 0x%.2X, 0x%.2X\r\n", (BYTE)RecievedMsg->Buf[0], (BYTE)RecievedMsg->Buf[1]);
		else if (RecievedMsg->MsgSize==3)
			sprintf(&ch[strlen(ch)], "3 Bytes: 0x%.2X, 0x%.2X, 0x%.2X\r\n", (BYTE)RecievedMsg->Buf[0], (BYTE)RecievedMsg->Buf[1], (BYTE)RecievedMsg->Buf[2]);
		else 
			sprintf(&ch[strlen(ch)], "%i Bytes: 0x%.2X,0x%.2X,0x%.2X,0x%.2X...\r\n", RecievedMsg->MsgSize,(BYTE)RecievedMsg->Buf[0], (BYTE)RecievedMsg->Buf[1],(BYTE)RecievedMsg->Buf[2],(BYTE)RecievedMsg->Buf[3]);
		theApp.pTermoDlg->SetDlgItemText(IDC_EDIT_RESPONSE,ch);
		delete[] ch;
		Return=TRUE;
	}
return Return;
}

//_______________________________________________________________________________________
//������ � ThreadSafe-������� �������� ����� ���������
//����� ���������� �� ������� ������ 
int SendComMessage(const ComMsg msg)
{
Queue.Push(msg);
return 1;
}

//_______________________________________________________________________________________
//����������� ��������� � ������ ���������� ������ � ���������� ������ � COM-����
void Rs232Send(HANDLE hComPort, ComMsg* msg)
{
  unsigned char sum=0;
  int i;
  unsigned Count=0;
  unsigned long RetCount=0;
  char Buf[50]={0};

	Buf[Count++]=RS485_START;
	for(i=0; i<msg->MsgSize; i++)
	{
 		//if((msg->Buf[i]==RS485_START)||(msg->Buf[i]==RS485_END)||(msg->Buf[i]==RS485_ESCAPE))
		//{   
		//	Buf[Count++]=RS485_ESCAPE;
		//	Buf[Count++]=msg->Buf[i]-(char)RS485_START;
		//}
		//else
			Buf[Count++]=msg->Buf[i];
	}
	Buf[Count++]=RS485_END;
	WriteFile(hComPort, &Buf, Count , &RetCount, NULL);
	if(theApp.ViewSentData && (Count>0))
		DumpDataToScreen("Raw->", Buf, Count);
}

//_______________________________________________________________________________________
void ReportErrorAndExit()
{
	LPTSTR lpMsgBuf;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_SYSTEM| 
	FORMAT_MESSAGE_IGNORE_INSERTS, NULL, GetLastError(),MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
	(LPTSTR) &lpMsgBuf, 0, NULL);
	CString str,str2;
	DetectSerialErrors(theApp.pTermoDlg->hComPort,&str2);
	str.Format("ReadFile: ������ ������ �� �����.\n%s%s\n���������� ����������?",lpMsgBuf,(LPCSTR)str2);
	LocalFree(&lpMsgBuf);
	if(AfxMessageBox((LPCSTR)str,MB_YESNO,NULL)==IDNO)
	{
		CloseHandle(theApp.pTermoDlg->hComPort);
		theApp.pTermoDlg->hComPort=INVALID_HANDLE_VALUE;
		AfxEndThread(0);
	}
}

//_______________________________________________________________________________________
//������������ ������� ������������ ���������, ������� �� ����� ������. ���� RecievedMsg!=NULL,
//�� ������� ����������� �������� ��������� �� ������������ ������. ���� ������������
//�������, ��������� ��������, ������� ������������. ���� �� �������, ���������
//����������� ���������.
//����� ������� ��������� ������ �� ������� (������ ��������� � �������).  
void AnalyseMsgWithWaitList(std::list<ComMsg>& List, ComMsg *RecievedMsg)
{
	std::list<ComMsg>::iterator iter;
	//ComMsg SentMsg;
	bool Processed=false;
	if(RecievedMsg!=NULL)
	{
		for (iter = List.begin(); iter != List.end();)
		{
			if(AnalyseCorrectMsg(RecievedMsg, &(*iter)))
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
			AnalyseCorrectMsg(RecievedMsg, NULL);
		}
	}

	//�������� �� �������
	ULONG TickCount=GetTickCount();
	for (iter = List.begin(); iter != List.end();)
	{
		if(iter->TimeStamp+theApp.ComTimeout*1000<TickCount)
		{
			//�������.
			CString str, str2;
			str.Format("No reply. Cmd:0x%.2X", (int)(BYTE)iter->Buf[0]);  
			for(int i=1;i<iter->MsgSize;i++)
			{
				str2.Format("%s,0x%.2X", (PCSTR)str, (int)(BYTE)iter->Buf[i]);
				str=str2;
			}
			str2.Format("%s, Timeout:%.3f", (PCSTR)str, (TickCount-iter->TimeStamp)*0.001);
			PrintToScreen((PCSTR)str2, str2.GetLength());
			iter=List.erase(iter);
		}
		else
			++iter;
	}
}

//�������� ������ ��������� �� �������. ����� - ��� ���� �� ��������, �� � �� �����������, 
//����� ���������. ���� ������� (�.�. ����� � ������ ���������� �����) �����, �� ����� ������
//��������� � �������� ���������  
void CheckTailTimeout(ComMsg *TailMsg)
{
	if(TailMsg->MsgSize>0)
		if(TailMsg->TimeStamp+theApp.ComTimeout*1000<GetTickCount())
		{
			//�������.
			ReportJunkMessage(TailMsg->Buf, TailMsg->MsgSize, 'T');
			TailMsg->MsgSize=0;
		}
}


void DumpDataToScreen(char* Discript, char* ReadBuf, int ReadFromPort)
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
		PrintToScreen((LPCSTR)str, str.GetLength());
	}
}

void PrintToScreen(const char* str, int length)
{
	char *ch;
	int len=theApp.pTermoDlg->GetDlgItem(IDC_EDIT_RESPONSE)->SendMessage(WM_GETTEXTLENGTH,NULL,NULL);
	ch=new char[len+length+4];
	theApp.pTermoDlg->GetDlgItemText(IDC_EDIT_RESPONSE,ch,len+1);
	sprintf(&ch[len], "%s\r\n", str);  
	theApp.pTermoDlg->SetDlgItemText(IDC_EDIT_RESPONSE,ch);
	delete[] ch;
}

void ReportJunkMessage(char *Buf, int Size, char Reason)
{
	CString str,str2;
	str.Format("Junk Msg-%c (%i): 0x%.2X",Reason, Size, (int)(unsigned char)Buf[0]);
	for (int i=1; i<Size; i++)
	{
		str2.Format("%s,0x%.2X", (LPCSTR)str,(int)(unsigned char)Buf[i]);
		str=str2;
	}
	PrintToScreen((LPCSTR)str, str.GetLength());
}