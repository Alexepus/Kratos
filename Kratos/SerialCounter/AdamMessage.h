#pragma once
#include "..\stdafx.h"

namespace StartMarks
{
	const char Request = '#';
	const char Reply = '@';
	const char ReplyError = '%';
}


class AdamMessage
{
public:
	bool IsValidMessage;

	bool IsStartMarkSet;
	char StartMark;

	bool IsUnitAddressSet;
	byte UnitAddress;

	bool IsCommandNumberSet;
	byte CommandNumber;

	CString CommandData;

	AdamMessage() : IsValidMessage(false), IsStartMarkSet(false), IsUnitAddressSet(false),
		IsCommandNumberSet(false), StartMark('-'), UnitAddress(255), CommandNumber(255)	{}

	AdamMessage CreateReply()
	{
		AdamMessage rep;
		rep.StartMark = StartMarks::Reply;
		rep.UnitAddress = UnitAddress;
		rep.CommandNumber = CommandNumber;
		return rep;
	}

	static AdamMessage Parse(CString s)
	{
		AdamMessage cmd;
		if (s.GetLength() >= 1)
		{
			cmd.StartMark = s[0];
			cmd.IsStartMarkSet = true;
		}
		if (s.GetLength() >= 3)
		{
			byte b;
			if (ByteTryParseHex(s.Mid(1, 2), b))
			{				
				cmd.UnitAddress = b;
				cmd.IsUnitAddressSet = true;
			}
		}

		if (s.GetLength() >= 5)
		{
			byte b;
			if (ByteTryParseHex(s.Mid(3, 2), b))
			{
				cmd.CommandNumber = b;
				cmd.IsCommandNumberSet = true;
			}
		}
		if (s.GetLength() >= 6)
			cmd.CommandData = s.Mid(5, s.GetLength() - 5);

		cmd.IsValidMessage = cmd.IsStartMarkSet && cmd.IsUnitAddressSet && cmd.IsCommandNumberSet && 
			(cmd.StartMark == StartMarks::Request || cmd.StartMark == StartMarks::Reply || cmd.StartMark == StartMarks::ReplyError);
		return cmd;
	}

	static bool ByteTryParseHex(CString str, byte& b)
	{
		int a;
		const char *src =  (LPCSTR)str;
		char *endOfParse;
		a = strtol(src, &endOfParse, 16);
		b = (byte) a;
		return (endOfParse == src + 2);
	}

	CString ToString() const
	{
		CString str;
		str.Format("%c%.2hX%.2hX%s\r", StartMark, UnitAddress, CommandNumber, (LPCSTR)CommandData);
		return str;
	}
};