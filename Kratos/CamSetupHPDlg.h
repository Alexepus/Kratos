#include "afxwin.h"
#if !defined(AFX_CAMSETUPHPDLG_H__2548011E_8A91_4440_8A12_4A2E52607CBE__INCLUDED_)
#define AFX_CAMSETUPHPDLG_H__2548011E_8A91_4440_8A12_4A2E52607CBE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CamSetupHPDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCamSetupHPDlg dialog

class CCamSetupHPDlg : public CDialog
{
// Construction
public:
	CCamSetupHPDlg(CWinApp* pApp, CWnd* pParent = NULL);   // standard constructor
	CWinApp* pTheApp;
	char pIniPath[_MAX_PATH];

// Dialog Data
	//{{AFX_DATA(CCamSetupHPDlg)
	enum { IDD = IDD_CAM_SETUP_HP_DLG };
	UINT	m_CountPos;
	UINT	m_CrateNum;
	UINT	m_RegistRetardPos;
	UINT	m_RegistHVPos;
	UINT	m_TimerPos;
	CString	m_CountTest;
	CString	m_CrateTest;
	CString	m_RegistRetardTest;
	CString	m_RegistHVTest;
	CString	m_TimerTest;
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA
	bool m_UsbCounter;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCamSetupHPDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCamSetupHPDlg)
	virtual void OnOK();
	afx_msg void OnTest();
	afx_msg void OnKillfocusEditCrateNum();
	afx_msg void OnKillfocusEditCountPos();
	afx_msg void OnKillfocusEditRegistRetardPos();
	afx_msg void OnKillfocusEditRegistHVPos();
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

#endif // !defined(AFX_CAMSETUPHPDLG_H__2548011E_8A91_4440_8A12_4A2E52607CBE__INCLUDED_)
