#if !defined(AFX_MEASURINGOPTDLG_H__D7E1382C_2747_4C6B_BC34_8589F753E380__INCLUDED_)
#define AFX_MEASURINGOPTDLG_H__D7E1382C_2747_4C6B_BC34_8589F753E380__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MeasuringOptDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMeasuringOptDlg dialog

class CMeasuringOptDlg : public CDialog
{
// Construction
public:
	void OnOK();
	CMeasuringOptDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CMeasuringOptDlg)
	enum { IDD = IDD_MEASURING_OPT_DLG };
	CString	m_MassSetupDelay;
	CString	m_MassStart;
	CString	m_MassStop;
	CString	m_MassStep;
	CTime	m_ScanTime;
	CString	m_CalibMassSetupDelay;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMeasuringOptDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMeasuringOptDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MEASURINGOPTDLG_H__D7E1382C_2747_4C6B_BC34_8589F753E380__INCLUDED_)
