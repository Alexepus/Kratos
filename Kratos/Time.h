#pragma once
#include "StdAfx.h"

typedef double doubleTime;
/**
* \brief Время в секундах с 1.01.1601
*/
doubleTime GetTimeDouble();
CString FormatTime(time_t time, char* format);