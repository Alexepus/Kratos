#if !defined(AFX_HeaterSetupDLG_H__1CF26151_115E_4841_984D_CF2270CB8D56__INCLUDED_)
#define AFX_HeaterSetupDLG_H__1CF26151_115E_4841_984D_CF2270CB8D56__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HeaterSetupDlg.h : header file
//
#include "resource.h"

#define TIMER_POLL_PID_VALS 1
/////////////////////////////////////////////////////////////////////////////
// CHeaterSetupDlg dialog

class CHeaterSetupDlg : public CDialog
{
// Construction
public:
	bool m_bExist;
	void OnCancel();
	void SavePidParam();
	void SavePidCoefs();
	CHeaterSetupDlg(CWnd* pParent = NULL);   // standard constructor
	LRESULT OnComEvent(WPARAM WParam, LPARAM LParam);

// Dialog Data
	//{{AFX_DATA(CHeaterSetupDlg)
	enum { IDD = IDD_CONTROLLER_SETUP_DLG };
	CEdit	m_EditViDecay;
	CEdit	m_EditDerivSamples;
	CStatic	m_StaticVpF;
	CStatic	m_StaticViF;
	CStatic	m_StaticVdF;
	CStatic	m_StaticPidOut;
	CStatic	m_StaticFeedForwVF;
	CStatic	m_StaticFeedForwV;
	CEdit	m_EditZeroVi;
	CEdit	m_EditViMax;
	CEdit	m_EditKff;
	CEdit	m_EditKp;
	CEdit	m_EditKi;
	CEdit	m_EditKd;
	CStatic	m_StaticOutput;
	CStatic	m_StaticVP;
	CStatic	m_StaticVI;
	CStatic	m_StaticVD;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHeaterSetupDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CHeaterSetupDlg)
	afx_msg void OnButtonPidCoefLoad();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	virtual BOOL OnInitDialog();
	afx_msg void OnKillfocusEditKd();
	afx_msg void OnKillfocusEditKi();
	afx_msg void OnKillfocusEditKp();
	virtual void OnOK();
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnButtonAdcDac();
	afx_msg void OnButtonPidParamLoad();
	afx_msg void OnKillfocusEditKff();
	afx_msg void OnKillfocusEditDerivSamples();
	afx_msg void OnKillfocusEditViMax();
	afx_msg void OnKillfocusEditZeroVi();
	afx_msg void OnKillfocusEditViDecay();
	afx_msg void OnButtonFfTable();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HeaterSetupDLG_H__1CF26151_115E_4841_984D_CF2270CB8D56__INCLUDED_)
