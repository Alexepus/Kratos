#ifndef _LOGTOFILE_H_
#define _LOGTOFILE_H_

void Msg(LPCTSTR lpszFormat, ...);
void WriteSpecifiedFile(const char* FileName, const char* StrMsg, DWORD MaxSize=0, const char* HeadLine=NULL);
void LogFile(const char* StrMsg);
void LogFile(const char* SrcFile, int Line);
void LogFile(const char* StrMsg, const char* SrcFile, int Line);
void LogFileFormat(LPCTSTR lpszFormat, ...);
void LogSpecifiedFileFormat(const char* FileName, LPCTSTR lpszFormat, ...);

#endif