#ifndef _COMTHREAD_H_
#define _COMTHREAD_H_
#include <list>
class ComMsg
{
public:
	char Buf[512];
	int MsgSize;
	bool WaitForReply;
	ULONG TimeStamp;
};
UINT ComThread(LPVOID pParam);
int GetCorrectMsg(char *NewMsgPart, int NewPartSize, ComMsg* RecievedMsg, ComMsg* TailMsg);
int AnalyseCorrectMsg(ComMsg* RecievedMsg, ComMsg* SendMsg);
void AnalyseMsgWithWaitList(std::list<ComMsg>& List, ComMsg *RecievedMsg);
int SendComMessage(const ComMsg msg/*, ComMsg *Reply=NULL, BOOL WaitForReply=FALSE*/);
void CheckTailTimeout(ComMsg *TailMsg);
void Rs232Send(HANDLE hComPort, ComMsg* msg);
void ReportErrorAndExit();
void DumpDataToScreen(char* Discript, char* ReadBuf, int ReadFromPort);
void PrintToScreen(const char* str, int length);
void ReportJunkMessage(char *Buf, int Size, char Reason);


#endif