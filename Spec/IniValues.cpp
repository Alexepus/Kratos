#include "stdafx.h"
#include "IniValues.h"

IniValues::IniValues()
{
char ModuleName[MAX_PATH];
::GetModuleFileName(NULL, ModuleName, MAX_PATH);
CString cstr=ModuleName, cstr2;
int EndPath=cstr.ReverseFind('\\');
cstr.Delete(EndPath+1,cstr.GetLength()-EndPath-1);
cstr2=cstr+"Spec.ini";
strcpy(IniFileName,(char*)(LPCTSTR)cstr2);

ProjectFile.Describe("", "RECENT FILES", "ProjectFile", IniFileName);
ShowList.Describe(TRUE, "Common", "ShowList", IniFileName);
ListWidth.Describe(100, "Common", "ListWidth", IniFileName);

}

//--------------------------------------------------------------------------------------------------

ValueSvLd::ValueSvLd()
{
m_Section=m_Key=DefStr=m_IniFileName=NULL;sVal=NULL; 
Type=(ValType)0;

char ModuleName[MAX_PATH];
::GetModuleFileName(NULL, ModuleName, MAX_PATH);
CString cstr=ModuleName;
int EndPath=cstr.ReverseFind('\\');
cstr.Delete(EndPath+1,cstr.GetLength()-EndPath-1);
cstr+="Spec.ini";
m_IniFileName=new char[cstr.GetLength()+1];
strcpy(m_IniFileName, (LPCTSTR)cstr);
}

void ValueSvLd::Describe(LPCTSTR DefaultVal, LPCTSTR Section, LPCTSTR Key, LPCTSTR IniFileName)
{
	Type=t_str;
	DefStr=new char[strlen(DefaultVal)+1];
	strcpy(DefStr, DefaultVal);
	sVal=new CString;
	m_Section=new char[strlen(Section)+1];
	strcpy(m_Section, Section);
	m_Key=new char[strlen(Key)];
	strcpy(m_Key, Key);
	if(IniFileName)
	{
		m_IniFileName=new char[strlen(IniFileName)+1];
		strcpy(m_IniFileName, IniFileName);
	}
	Read();
}

void ValueSvLd::Describe(int DefaultVal,LPCTSTR Section, LPCTSTR Key, LPCTSTR IniFileName)
{
	Type=t_int;
	DefInt=DefaultVal;
	m_Section=new char[strlen(Section)+1];
	strcpy(m_Section, Section);
	m_Key=new char[strlen(Key)+1];
	strcpy(m_Key, Key);
	if(IniFileName)
	{
		m_IniFileName=new char[strlen(IniFileName)+1];
		strcpy(m_IniFileName, IniFileName);
	}
	Read();
}

void ValueSvLd::Describe(double DefaultVal,LPCTSTR Section, LPCTSTR Key, LPCTSTR IniFileName)
{
	Type=t_double; 
	DefDouble=DefaultVal;
	m_Section=new char[strlen(Section)+1];
	strcpy(m_Section, Section);
	m_Key=new char[strlen(Key)+1];
	strcpy(m_Key, Key);
	if(IniFileName)
	{
		m_IniFileName=new char[strlen(IniFileName)+1];
		strcpy(m_IniFileName, IniFileName);
	}
	Read();
}

ValueSvLd::~ValueSvLd()
{
	Write();
	if(m_IniFileName) delete[] m_IniFileName;
	if(DefStr) delete[] DefStr;
	if(sVal) delete sVal;
	if(m_Section) delete[] m_Section;
	if(m_Key) delete[] m_Key;
}

void ValueSvLd::Read()
{
	char StrIn[MAX_PATH], StrOut[MAX_PATH];
	switch(Type)
	{
	case t_str:
		sprintf(StrIn, "%s",DefStr);
		break;
	case t_double:
		sprintf(StrIn, "%f",DefDouble);
		break;
	case t_int:
		sprintf(StrIn, "%i",DefInt);
	}
	GetPrivateProfileString(m_Section, m_Key, StrIn, StrOut, MAX_PATH, m_IniFileName);
	switch(Type)
	{
	case t_str:
		*sVal=StrOut;
		break;
	case t_double:
		dVal=atof(StrOut);
		break;
	case t_int:
		iVal=atoi(StrOut);
	}
}

void ValueSvLd::Write()
{
	char StrIn[MAX_PATH];
	switch(Type)
	{
	case t_str:
		sprintf(StrIn, "%s",(LPCSTR)(*sVal));
		break;
	case t_double:
		sprintf(StrIn, "%f",dVal);
		break;
	case t_int:
		sprintf(StrIn, "%i",iVal);
	}
	WritePrivateProfileString(m_Section, m_Key, StrIn, m_IniFileName);
}
//__________________________________________________________________________________________

SvLdDouble::SvLdDouble()
{
	m_Section=m_Key=m_IniFileName=NULL; 

	char ModuleName[MAX_PATH];
	::GetModuleFileName(NULL, ModuleName, MAX_PATH);
	CString cstr=ModuleName;
	int EndPath=cstr.ReverseFind('\\');
	cstr.Delete(EndPath+1,cstr.GetLength()-EndPath-1);
	cstr+="Spec.ini";
	m_IniFileName=new char[cstr.GetLength()+1];
	strcpy(m_IniFileName, (LPCTSTR)cstr);
}

SvLdDouble::~SvLdDouble()
{
	char StrIn[MAX_PATH];
	sprintf(StrIn, "%f",dVal);
	WritePrivateProfileString(m_Section, m_Key, StrIn, m_IniFileName);
	if(m_IniFileName) delete[] m_IniFileName;
	if(m_Section) delete[] m_Section;
	if(m_Key) delete[] m_Key;

}

void SvLdDouble::Describe(double DefaultVal,LPCTSTR Section, LPCTSTR Key, LPCTSTR IniFileName)
{
	m_Section=new char[strlen(Section)+1];
	strcpy(m_Section, Section);
	m_Key=new char[strlen(Key)+1];
	strcpy(m_Key, Key);
	if(IniFileName)
	{
		m_IniFileName=new char[strlen(IniFileName)+1];
		strcpy(m_IniFileName, IniFileName);
	}
	char StrIn[MAX_PATH], StrOut[MAX_PATH];
	sprintf(StrIn, "%f",DefaultVal);
	GetPrivateProfileString(m_Section, m_Key, StrIn, StrOut, MAX_PATH, m_IniFileName);
	dVal=atof(StrOut);
}

//__________________________________________________________________________________________

SvLdInt::SvLdInt()
{
	m_Section=m_Key=m_IniFileName=NULL; 

	char ModuleName[MAX_PATH];
	::GetModuleFileName(NULL, ModuleName, MAX_PATH);
	CString cstr=ModuleName;
	int EndPath=cstr.ReverseFind('\\');
	cstr.Delete(EndPath+1,cstr.GetLength()-EndPath-1);
	cstr+="Spec.ini";
	m_IniFileName=new char[cstr.GetLength()+1];
	strcpy(m_IniFileName, (LPCTSTR)cstr);
}

SvLdInt::~SvLdInt()
{
	char StrIn[MAX_PATH];
	sprintf(StrIn, "%i",iVal);
	WritePrivateProfileString(m_Section, m_Key, StrIn, m_IniFileName);
	if(m_IniFileName) delete[] m_IniFileName;
	if(m_Section) delete[] m_Section;
	if(m_Key) delete[] m_Key;

}

void SvLdInt::Describe(int DefaultVal,LPCTSTR Section, LPCTSTR Key, LPCTSTR IniFileName)
{
	m_Section=new char[strlen(Section)+1];
	strcpy(m_Section, Section);
	m_Key=new char[strlen(Key)+1];
	strcpy(m_Key, Key);
	if(IniFileName)
	{
		m_IniFileName=new char[strlen(IniFileName)+1];
		strcpy(m_IniFileName, IniFileName);
	}
	char StrIn[MAX_PATH], StrOut[MAX_PATH];
	sprintf(StrIn, "%i",DefaultVal);
	GetPrivateProfileString(m_Section, m_Key, StrIn, StrOut, MAX_PATH, m_IniFileName);
	iVal=atoi(StrOut);
}