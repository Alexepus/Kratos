#include "stdafx.h"
#include "main.h"

//Записывает строку текста StrMsg (добавляя "\n" в конец). Если MaxSize>0, то
//усекает начало файла до величины MaxSize*0.95, причем доусекает последнюю строчку
//до начала новой строки
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
if(GetFileSize(hFile,NULL)>MaxSize) //Если размер файла >MaxSize, удаляем первые строки до MaxSize*0.95
{
	DWORD ReadBytes, FilePointer=GetFileSize(hFile,NULL)-(DWORD)(MaxSize*0.95), BigBufferSize;
	char SmallReadBuf[2]=" ", *BigReadBuf;
	SetFilePointer(hFile,FilePointer, NULL, FILE_BEGIN);
	do //Ищем конец строки
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
	BigBufferSize=GetFileSize(hFile,NULL)-FilePointer+1;
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
	//Запись заголовка
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

//Запись заголовка
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

//Добавляет строку сообщения об ошибке в log файл программы "Kratos.log",
//сопровождая сообщение датой и временем
void LogFile(const std::string msg)
{
	LogFile(msg.c_str());
}

//Добавляет строку сообщения об ошибке в log файл программы "Kratos.log",
//сопровождая сообщение датой и временем
void LogFile(const CString msg)
{
	LogFile(msg.GetString());
}

//Добавляет строку сообщения об ошибке в log файл программы "Kratos.log",
//сопровождая сообщение датой и временем
void LogFile(const char* StrMsg)
{
char LogFileName[_MAX_PATH];
::GetModuleFileName(NULL, (char *)LogFileName, _MAX_PATH);
CString str=LogFileName;
int EndPath=str.ReverseFind('\\');
str.Delete(EndPath+1,str.GetLength()-EndPath-1);
str+="Kratos.log";
strcpy((char *)LogFileName,(char*)(LPCTSTR)str);

SYSTEMTIME t;
GetLocalTime(&t);
str.Format("%i.%.2i.%.2i %.2i:%.2i:%.2i.%.3i ", t.wYear, t.wMonth, t.wDay,
		   t.wHour, t.wMinute, t.wSecond, t.wMilliseconds);
str+=StrMsg;
WriteSpecifiedFile(LogFileName, str, 4*1024*1024);
}

//Добавляет строку сообщения о текущем файле и строке в log файл программы "Kratos.log",
//сопровождая сообщение датой и временем
void LogFile(const char* StrMsg, const char* SrcFile, int Line)
{
CString str, SrcF(SrcFile);
SrcF.Delete(0, SrcF.ReverseFind('\\')+1);
str.Format("%s in %s, line %i", StrMsg, (LPCSTR)SrcF, Line);
LogFile(str);
}

//Добавляет строку сообщения о текущем файле и строке в log файл программы "Kratos.log",
//сопровождая сообщение датой и временем
void LogFileFormat(LPCTSTR lpszFormat, ...)
{
va_list args;
va_start(args, lpszFormat);
CString str;
str.FormatV(lpszFormat, args);
LogFile(str);
}