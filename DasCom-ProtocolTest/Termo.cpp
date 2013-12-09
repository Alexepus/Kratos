// Termo.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "Termo.h"
#include "TermoDlg.h"
#include "ThreadSafeQueue.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTermoApp

BEGIN_MESSAGE_MAP(CTermoApp, CWinApp)
	//{{AFX_MSG_MAP(CTermoApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTermoApp construction

CTermoApp::CTermoApp()
{
free((void*)m_pszProfileName);
m_pszProfileName= (char*)malloc(_MAX_PATH);
::GetModuleFileName(NULL, (char *)m_pszProfileName, _MAX_PATH);
CString cstr=m_pszProfileName;
int EndPath=cstr.ReverseFind('\\');
cstr.Delete(EndPath+1,cstr.GetLength()-EndPath-1);
cstr+="Termo.ini";
strcpy((char *)m_pszProfileName,(char*)(LPCTSTR)cstr);
ComPortName=GetProfileString("RS232", "ComPortName", "None");
if(ComPortName=="None")
{
	ComPortName="COM1";
	WriteProfileString("RS232", "ComPortName", ComPortName);
}
ComTimeout=atof((LPCSTR)GetProfileString("RS232", "ComTimeOut", "0.100"));
CString str;
str.Format("%.3f", ComTimeout);
WriteProfileString("RS232", "ComTimeOut", (LPCSTR)str);
BaudRate=GetProfileInt("RS232", "BaudRate", 57600);
WriteProfileInt("RS232", "BaudRate", BaudRate);
ViewRawData=GetProfileInt("Interface", "ViewRawData", 0);
SkipProcessing=GetProfileInt("Interface", "SkipProcessing", 0);
ViewSentData=GetProfileInt("Interface", "ViewSentData", 0);
ExitThreads=FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CTermoApp object

CTermoApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CTermoApp initialization

BOOL CTermoApp::InitInstance()
{
	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	CTermoDlg dlg;
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

void Msg(LPCTSTR lpszFormat, ...)
{
//Очень полезная функция для вывода сообщений при отладке. 
//Пример: Msg("Current Point=%i, Problem Value=%f", k, pReg->m_pDataOut[k].y);
va_list args;
va_start(args, lpszFormat);
CString str;
str.FormatV(lpszFormat, args);
::AfxMessageBox(str,MB_OK|MB_ICONINFORMATION);
}

