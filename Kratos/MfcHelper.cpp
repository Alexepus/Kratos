#include "StdAfx.h"
#include "MfcHelper.h"
#include "Exceptions.h"
#include "function.h"
#include "LogToFile.h"

std::vector<CString> SplitString(const CString source, const char delimeterChars[])
{
std::vector<CString> res;
int curPos = 0;
auto resToken = source.Tokenize(delimeterChars, curPos);
while (resToken.GetLength() > 0)
{
	res.push_back(resToken);
	resToken = source.Tokenize(delimeterChars, curPos);
}
return res;
}

CString JoinStrings(const std::vector<CString>& strings, const char delimeter[])
{
	CString res;
	for (size_t i = 0; i < strings.size(); i++)
	{
		res += strings[i];
		if (i != strings.size() - 1)
			res += delimeter;
	}
	return res;
}

void TryWithMessageBox(std::function<void()> lambda, CWnd* win)
{
	try
	{
		lambda();
	}
	catch(std::exception &ex)
	{
		win->MessageBox(DetailedException::TryGetDetailedWhat(ex).c_str(), GetAppTitle(), MB_OK | MB_ICONERROR);
		LogFile(DetailedException::TryGetDetailedWhat(ex).c_str());
	}
}