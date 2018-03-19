#include "stdafx.h"
#include "Time.h"

/**
* \brief Время в секундах с 1.01.1601 (местное время)
*/
doubleTime GetTimeDouble()
{
	FILETIME FileTime; //FILETIME type is is the number 100 ns increments since January 1 1601
	SYSTEMTIME localTime;
	const double secondsInTick = 100.0e-9;
	GetLocalTime(&localTime);
	SystemTimeToFileTime(&localTime, &FileTime);
	return ((((ULONGLONG)FileTime.dwHighDateTime) << 32) + FileTime.dwLowDateTime) * secondsInTick;
}

CString FormatTime(time_t time, char* format)
{
	return CTime(time).Format(format);
}
