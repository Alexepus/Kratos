
// Kratos.Test.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include <stdio.h>
#include "gtest/gtest.h"
#include "Kratos.Test.h"
#include "Kratos.TestDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CKratosTestApp

BEGIN_MESSAGE_MAP(CKratosTestApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CKratosTestApp construction

CKratosTestApp::CKratosTestApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CKratosTestApp object

CKratosTestApp theApp;


// CKratosTestApp initialization

BOOL CKratosTestApp::InitInstance()
{
	CWinApp::InitInstance();
//
//	CKratosTestDlg dlg;
//	m_pMainWnd = &dlg;
//
//#if !defined(_AFXDLL) && !defined(_AFX_NO_MFC_CONTROLS_IN_DIALOGS)
//	ControlBarCleanUp();
//#endif

	int argc = __argc - 1;
	if (argc < 0)
		argc = 0;
	testing::InitGoogleTest(&argc, __argv);
	auto res = RUN_ALL_TESTS();
	return FALSE;
}