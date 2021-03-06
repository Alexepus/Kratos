#include "stdafx.h"
#include "LogToFile.h"

void Msg(LPCTSTR lpszFormat, ...)
{
//����� �������� ������� ��� ������ ��������� ��� �������. 
//������: Msg("Current Point=%i, Problem Value=%f", k, pReg->m_pDataOut[k].y);
va_list args;
va_start(args, lpszFormat);
CString str;
str.FormatV(lpszFormat, args);
::AfxMessageBox(str,MB_OK|MB_ICONINFORMATION);
}

//���������� ������ ������ StrMsg (�������� "\n" � �����). ���� MaxSize>0, ��
//������� ������ ����� �� �������� MaxSize*0.95, ������ ��������� ��������� �������
//�� ������ ����� ������
void WriteSpecifiedFile(const char* FileName, const char* StrMsg, DWORD MaxSize, const char* HeadLine)
{
DWORD WrittenToPort=0;
HANDLE hFile;
char *lpMsgBuf;
DWORD ShareMode=(GetVersion()< 0x80000000)?
		FILE_SHARE_READ|FILE_SHARE_WRITE|FILE_SHARE_DELETE //Windows NT, 2000 or Later
		:FILE_SHARE_READ|FILE_SHARE_WRITE;//Windows 95, 98, ME
hFile=CreateFile(FileName, GENERIC_WRITE|GENERIC_READ, ShareMode, 
						   NULL, OPEN_ALWAYS , FILE_ATTRIBUTE_NORMAL, NULL);
if (hFile==INVALID_HANDLE_VALUE)
	{
		FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_SYSTEM| 
			FORMAT_MESSAGE_IGNORE_INSERTS, NULL, GetLastError(),MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPTSTR) &lpMsgBuf, 0, NULL);
		::Msg("CreateFile: error opening log file \"%s\".\n%s",FileName,lpMsgBuf);
		LocalFree(lpMsgBuf);
		return;
	}
if((GetFileSize(hFile,NULL)>MaxSize) && (MaxSize>0)) //���� ������ ����� >MaxSize, ������� ������ ������ �� MaxSize*0.95
{
	DWORD ReadBytes, FilePointer=GetFileSize(hFile,NULL)-(DWORD)(MaxSize*0.95), BigBufferSize;
	char SmallReadBuf[2]=" ", *BigReadBuf;
	SetFilePointer(hFile,FilePointer, NULL, FILE_BEGIN);
	do //���� ����� ������
	{
		if (!ReadFile(hFile, SmallReadBuf, 1, &ReadBytes, NULL))
		{
			FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_SYSTEM| 
				FORMAT_MESSAGE_IGNORE_INSERTS, NULL, GetLastError(),MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
				(LPTSTR) &lpMsgBuf, 0, NULL);
			::Msg("ReadFile: error reading log file \"%s\".\n%s",FileName,lpMsgBuf);
			LocalFree(lpMsgBuf);
			return;
		}
		FilePointer++;
	}while(ReadBytes!=0 && SmallReadBuf[0]!='\n');
	BigBufferSize=max(GetFileSize(hFile,NULL)-FilePointer+1,2);
	BigReadBuf=(char*)malloc(BigBufferSize);
	if (!ReadFile(hFile, BigReadBuf, BigBufferSize-1, &ReadBytes, NULL))
		{
			FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_SYSTEM| 
				FORMAT_MESSAGE_IGNORE_INSERTS, NULL, GetLastError(),MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
				(LPTSTR) &lpMsgBuf, 0, NULL);
			::Msg("ReadFile: error reading log file \"%s\".\n%s",FileName,lpMsgBuf);
			LocalFree(lpMsgBuf);
			return;
		}
	ASSERT(BigBufferSize-1==ReadBytes);
	CloseHandle(hFile);
	hFile=CreateFile(FileName, GENERIC_WRITE|GENERIC_READ, ShareMode,
						   NULL, TRUNCATE_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile==INVALID_HANDLE_VALUE)
		{
			FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_SYSTEM| 
				FORMAT_MESSAGE_IGNORE_INSERTS, NULL, GetLastError(),MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
				(LPTSTR) &lpMsgBuf, 0, NULL);
			::Msg("CreateFile: error opening log file \"%s\".\n%s",FileName,lpMsgBuf);
			LocalFree(lpMsgBuf);
			return;
		}
	//������ ���������
	if(HeadLine!=NULL)
		{	
			CString str3=HeadLine;
			str3+="\r\n";
			if (!WriteFile(hFile, str3, str3.GetLength(), &WrittenToPort, NULL))
			{
				FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_SYSTEM| 
					FORMAT_MESSAGE_IGNORE_INSERTS, NULL, GetLastError(),MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
					(LPTSTR) &lpMsgBuf, 0, NULL);
				::Msg("WriteFile: error writing log file \"%s\".\n%s",FileName,lpMsgBuf);
				LocalFree(lpMsgBuf);
				return;
			}
		}
	if (!WriteFile(hFile, BigReadBuf, BigBufferSize-1, &WrittenToPort, NULL))
		{
			FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_SYSTEM| 
				FORMAT_MESSAGE_IGNORE_INSERTS, NULL, GetLastError(),MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
				(LPTSTR) &lpMsgBuf, 0, NULL);
			::Msg("WriteFile: error writing log file \"%s\".\n%s",FileName,lpMsgBuf);
			LocalFree(lpMsgBuf);
			return;
		}
	free(BigReadBuf);
}

SetFilePointer(hFile,0, NULL, FILE_END);
CString str=StrMsg;
str+="\r\n";

//������ ���������
if((HeadLine!=NULL) && (GetFileSize(hFile,NULL)==0))
	{
	CString str3=HeadLine;
	str3+="\r\n";
	if (!WriteFile(hFile, str3, str3.GetLength(), &WrittenToPort, NULL))
		{
			FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_SYSTEM| 
				FORMAT_MESSAGE_IGNORE_INSERTS, NULL, GetLastError(),MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
				(LPTSTR) &lpMsgBuf, 0, NULL);
			::Msg("WriteFile: error writing log file \"%s\".\n%s",FileName,lpMsgBuf);
			LocalFree(lpMsgBuf);
			return;
		}
	}
if (!WriteFile(hFile, (LPCSTR)str, str.GetLength(), &WrittenToPort, NULL))
		{
					FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_SYSTEM| 
						FORMAT_MESSAGE_IGNORE_INSERTS, NULL, GetLastError(),MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
						(LPTSTR) &lpMsgBuf, 0, NULL);
					::Msg("WriteFile: error writing log file \"%s\".\n%s",FileName,lpMsgBuf);
					return;
		}
CloseHandle(hFile);
}

//��������� ������ ��������� �� ������ � log ���� ��������� "Spec.log",
//����������� ��������� ����� � ��������
void LogFile(const char* StrMsg)
{
char LogFileName[_MAX_PATH];
::GetModuleFileName(NULL, (char *)LogFileName, _MAX_PATH);
CString str=LogFileName;
int EndPath=str.ReverseFind('\\');
str.Delete(EndPath+1,str.GetLength()-EndPath-1);
str+="Spec.log";
strcpy((char *)LogFileName,(char*)(LPCTSTR)str);

CTime t = CTime::GetCurrentTime();
str.Format("%i.%.2i.%.2i %.2i:%.2i:%.2i ", t.GetYear(), t.GetMonth(), t.GetDay(),
		   t.GetHour(), t.GetMinute(), t.GetSecond());
str+=StrMsg;
WriteSpecifiedFile(LogFileName, str, 4*1024*1024);
}

//��������� ������ ��������� � ������� ����� � ������ � log ���� ��������� "Spec.log",
//����������� ��������� ����� � ��������
void LogFile(const char* SrcFile, int Line)
{
char LogFileName[_MAX_PATH];
::GetModuleFileName(NULL, (char *)LogFileName, _MAX_PATH);
CString str=LogFileName;
int EndPath=str.ReverseFind('\\');
str.Delete(EndPath+1,str.GetLength()-EndPath-1);
str+="Spec.log";
strcpy((char *)LogFileName,(char*)(LPCTSTR)str);

CString str2;
CTime t = CTime::GetCurrentTime();
str.Format("%i.%.2i.%.2i %.2i:%.2i:%.2i ", t.GetYear(), t.GetMonth(), t.GetDay(),
		   t.GetHour(), t.GetMinute(), t.GetSecond());
str2.Format(" Line: %i", Line);
CString SrcF(SrcFile);
SrcF.Delete(0, SrcF.ReverseFind('\\')+1);
str+=SrcF;
str+=str2;
WriteSpecifiedFile(LogFileName, str, 4*1024*1024);
}

//��������� ������ ��������� � ������� ����� � ������ � log ���� ��������� "Spec.log",
//����������� ��������� ����� � ��������
void LogFile(const char* StrMsg, const char* SrcFile, int Line)
{
CString str, SrcF(SrcFile);
SrcF.Delete(0, SrcF.ReverseFind('\\')+1);
str.Format("%s in %s, line %i", StrMsg, SrcF, Line);
LogFile(str);
}

//��������� ������ ��������� � log ���� ��������� "Spec.log",
//����������� ��������� ����� � ��������
void LogFileFormat(LPCTSTR lpszFormat, ...)
{
va_list args;
va_start(args, lpszFormat);
CString str;
str.FormatV(lpszFormat, args);
LogFile(str);
}

//��������� ������ ��������� � ��������� ����,
//����������� ��������� ����� � ��������
void LogSpecifiedFileFormat(const char* FileName, LPCTSTR lpszFormat, ...)
{
	va_list args;
	va_start(args, lpszFormat);
	CString str, StrMsg;
	StrMsg.FormatV(lpszFormat, args);

	SYSTEMTIME st;
	GetLocalTime(&st);
	str.Format("%i.%.2i.%.2i %.2i:%.2i:%.2i.%.3i ", st.wYear, st.wMonth, st.wDay,
			   st.wHour, st.wMinute, st.wMinute, st.wMilliseconds);
	str+=StrMsg;
	WriteSpecifiedFile(FileName, str, 1024*1024);
}
