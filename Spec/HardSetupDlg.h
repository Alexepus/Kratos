#if !defined(AFX_HARDSETUPDLG_H__A4003EE4_E3B8_47F5_AEE6_1E3112887113__INCLUDED_)
#define AFX_HARDSETUPDLG_H__A4003EE4_E3B8_47F5_AEE6_1E3112887113__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HardSetupDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CHardSetupDlg dialog

class CHardSetupDlg : public CDialog
{
// Construction
public:
	void OnOK();
	CHardSetupDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CHardSetupDlg)
	enum { IDD = IDD_HARDWARE_SETUP_DLG };
	CEdit	m_EditComPortName;
	CComboBox	m_ComboRange;
	CComboBox	m_ComboIntegration;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHardSetupDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CHardSetupDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonComReconnect();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HARDSETUPDLG_H__A4003EE4_E3B8_47F5_AEE6_1E3112887113__INCLUDED_)
