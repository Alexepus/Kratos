// ProgNew.h : main header file for the PROGNEW application
//

#if !defined(AFX_PROGNEW_H__113AB9A9_6F91_11D5_9A4B_008048FD9845__INCLUDED_)
#define AFX_PROGNEW_H__113AB9A9_6F91_11D5_9A4B_008048FD9845__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
#include "IniValues.h"
#include "SerialCounter/AdamComTransport.h"

//FARPROC EditNormalProc;
//WNDPROC EditNormalProc;
LRESULT CALLBACK EditSuperNumberProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
/////////////////////////////////////////////////////////////////////////////
// CProgNewApp:
// See ProgNew.cpp for the implementation of this class
//
class CMainFrame;
typedef struct _SandTBarHWND
	{
		HWND Sb; 
		HWND Tb;
	} SAndTBarHWND;

class CProgNewApp : public CWinApp
{
public:
	CMainFrame *m_pMainFrame;
	IniValues Ini;
	AdamComTransport m_AdamCom;
	CProgNewApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CProgNewApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CProgNewApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
protected:
	BOOL FirstInstance();
};
CString Format(LPCTSTR lpszFormat, ...);
void Msg(LPCTSTR lpszFormat, ...);
void MsgLog(LPCTSTR lpszFormat, ...);
void Msg(const std::string message);
void Msg(const CString message);


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROGNEW_H__113AB9A9_6F91_11D5_9A4B_008048FD9845__INCLUDED_)
