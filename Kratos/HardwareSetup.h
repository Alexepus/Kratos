#if !defined(AFX_HARDWARESETUP_H__C8CCBD20_8D2C_11DB_AAE8_E749DF642562__INCLUDED_)
#define AFX_HARDWARESETUP_H__C8CCBD20_8D2C_11DB_AAE8_E749DF642562__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HardwareSetup.h : header file
//
#include "main.h"
/////////////////////////////////////////////////////////////////////////////
// CHardwareSetup dialog

class CHardwareSetupDlg : public CDialog
{
// Construction
public:
	CHardwareSetupDlg(CWnd* pParent = NULL);   // standard constructor
	THREAD_COMMON* m_pThreadCom;

// Dialog Data
	//{{AFX_DATA(CHardwareSetup)
	enum { IDD = IDD_DLG_HARDWARE };
	double	m_RetardCalibration;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHardwareSetup)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CHardwareSetup)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HARDWARESETUP_H__C8CCBD20_8D2C_11DB_AAE8_E749DF642562__INCLUDED_)
