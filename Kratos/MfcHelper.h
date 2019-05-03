#pragma once
#include "StdAfx.h"
#include <functional>

template <typename... Args>
CString Format(const char* format, Args&&... args)
{
	CString result;
	result.Format(format, std::forward<Args>(args)...);
	return result;
}

std::vector<CString> SplitString(const CString source, const char delimeterChars[]);

CString JoinStrings(const std::vector<CString>& strings, const char delimeter[]);
void TryWithMessageBox(std::function<void()> lambda, CWnd* win);
