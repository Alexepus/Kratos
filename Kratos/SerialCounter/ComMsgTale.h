#pragma once

class ComMsgTale
{
public:
	BYTE Buf[512];
	int MsgSize;
	ULONG TimeStamp; //������ ��� ��������� ���������, ����������� ������������� (����� �������� � ����� 
	//��������� ������ ���������)

	ComMsgTale():MsgSize(0){}

	void ConcatWithNewData(byte newBuf[], int newLength)
	{
		memmove(Buf+MsgSize, newBuf, newLength);
		MsgSize += newLength;
	}
};