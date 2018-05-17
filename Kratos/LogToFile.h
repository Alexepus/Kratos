#ifndef _LOGTOFILE_H_
#define _LOGTOFILE_H_

void WriteSpecifiedFile(const char* FileName, const char* StrMsg, DWORD MaxSize=0, const char* HeadLine=NULL);
void LogFile(const char* StrMsg);
void LogFile(const std::string msg);
void LogFile(const CString msg);
void LogFile(const char* StrMsg, const char* SrcFile, int Line);
void LogFileFormat(LPCTSTR lpszFormat, ...);

#endif