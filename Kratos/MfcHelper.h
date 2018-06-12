#pragma once
#include "StdAfx.h"

template <typename... Args>
CString Format(const char* format, Args&&... args)
{
	CString result;
	result.Format(format, std::forward<Args>(args)...);
	return result;
}

std::vector<CString> SplitString(const CString source, const char delimeterChars[]);

CString JoinStrings(const std::vector<CString>& strings, const char delimeter[]);