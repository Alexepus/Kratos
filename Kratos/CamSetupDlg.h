#include "afxwin.h"
#if !defined(AFX_CAMSETUPDLG_H__D2617940_8222_11D5_932C_FD1903C506AF__INCLUDED_)
#define AFX_CAMSETUPDLG_H__D2617940_8222_11D5_932C_FD1903C506AF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CamSetupDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCamSetupDlg dialog

class CCamSetupDlg : public CDialog
{
// Construction
public:
	CCamSetupDlg(CWinApp* pApp, CWnd* pParent = NULL);   // standard constructor
	CWinApp* pTheApp;
	char pIniPath[_MAX_PATH];

// Dialog Data
	//{{AFX_DATA(CCamSetupDlg)
	enum { IDD = IDD_CAM_SETUP_DLG };
	UINT	m_CountPos;
	UINT	m_CrateNum;
	UINT	m_RegistPos;
	UINT	m_TimerPos;
	CString	m_CountTest;
	CString	m_CrateTest;
	CString	m_RegistTest;
	CString	m_TimerTest;
	bool m_UsbCounter;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCamSetupDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCamSetupDlg)
	virtual void OnOK();
	afx_msg void OnTest();
	afx_msg void OnKillfocusEditCrateNum();
	afx_msg void OnKillfocusEditCountPos();
	afx_msg void OnKillfocusEditRegistPos();
	afx_msg void OnKillfocusEditTimerPos();
	//}}AFX_MSG
	BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	CEdit EditCountPos;
	CEdit EditTimerPos;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CAMSETUPDLG_H__D2617940_8222_11D5_932C_FD1903C506AF__INCLUDED_)
