// Termo.h : main header file for the TERMO application
//

#if !defined(AFX_TERMO_H__79D69F44_0D12_11D7_AE0A_008048B920DA__INCLUDED_)
#define AFX_TERMO_H__79D69F44_0D12_11D7_AE0A_008048B920DA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif
#include "resource.h"
#include "TermoDlg.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CTermoApp:
// See Termo.cpp for the implementation of this class
//

class CTermoApp : public CWinApp
{
public:
	CString ComPortName;
	CTermoApp();
	CWinThread* hComThread;
	volatile BOOL ExitThreads;
	volatile BOOL ViewRawData;
	volatile BOOL SkipProcessing;
	volatile BOOL ViewSentData;

	double ComTimeout;
	int BaudRate;
	CTermoDlg *pTermoDlg;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTermoApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CTermoApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

void Msg(LPCTSTR lpszFormat, ...);
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TERMO_H__79D69F44_0D12_11D7_AE0A_008048B920DA__INCLUDED_)
