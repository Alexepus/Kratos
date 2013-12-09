#if !defined(AFX_INSPECTDLG_H__0D147AA0_9773_4DCB_A14E_51190815E409__INCLUDED_)
#define AFX_INSPECTDLG_H__0D147AA0_9773_4DCB_A14E_51190815E409__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InspectDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CInspectDlg dialog

class CMassCalibDoc;
class CInspectDlg : public CDialog
{
// Construction
public:
	CInspectDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CInspectDlg)
	enum { IDD = IDD_INSPECT_DLG };
	CString	m_EditA;
	CString	m_EditB;
	CString	m_EditC;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInspectDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CMassCalibDoc *m_pDoc;
	// Generated message map functions
	//{{AFX_MSG(CInspectDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnButtonInspectDefaults();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INSPECTDLG_H__0D147AA0_9773_4DCB_A14E_51190815E409__INCLUDED_)
