// Spec.h : main header file for the SPEC application
//

#if !defined(AFX_SPEC_H__3AE50321_BAB5_484C_BE9F_70692B90ADE1__INCLUDED_)
#define AFX_SPEC_H__3AE50321_BAB5_484C_BE9F_70692B90ADE1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols
#include "MassCalibDoc.h"
#include "ComThread.h"
#include "Properties.h"

#define WINDOW_CLASS_NAME _T("SpecWindowClass")

/////////////////////////////////////////////////////////////////////////////
// CSpecApp:
// See Spec.cpp for the implementation of this class
//
class CSpecApp : public CWinApp
{
public:
	CProperties Ini;
	CComThread m_SerialLink;
	char* m_psIniFileName;
	CMassCalibDoc* m_pMassCalibDoc;
	CSpecView* m_pSpecView;

	CWinThread *m_pThread;
	HANDLE m_hThermoRegThread;
	HANDLE m_EventThreadExit;	//—обытие, говор€щее потокам о запросе на завершение
	LARGE_INTEGER Freq, Count1,Count2;

	CSpecApp();
	~CSpecApp();
	BOOL FirstInstance();
	void RegisterWindowClass();
	int ExitInstance();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSpecApp)
	public:
	virtual BOOL InitInstance();
	virtual CDocument* OpenDocumentFile(LPCTSTR lpszFileName);
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CSpecApp)
	afx_msg void OnAppAbout();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SPEC_H__3AE50321_BAB5_484C_BE9F_70692B90ADE1__INCLUDED_)
