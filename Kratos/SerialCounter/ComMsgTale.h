#pragma once

class ComMsgTale
{
public:
	BYTE Buf[512];
	int MsgSize;
	ULONG TimeStamp; //—лужит дл€ вы€влени€ таймаутов, заполн€етс€ автоматически (после отправки и после 
	//получени€ целого сообщени€)

	ComMsgTale():MsgSize(0){}

	void ConcatWithNewData(byte newBuf[], int newLength)
	{
		memmove(Buf+MsgSize, newBuf, newLength);
		MsgSize += newLength;
	}
};