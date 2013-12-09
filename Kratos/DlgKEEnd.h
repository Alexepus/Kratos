#if !defined(AFX_DLGKEEND_H__BF523EE2_8B3A_11D5_9A4B_008048FD9845__INCLUDED_)
#define AFX_DLGKEEND_H__BF523EE2_8B3A_11D5_9A4B_008048FD9845__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// DlgKEEnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgKEEnd dialog

class CDlgKEEnd : public CDialog
{
// Construction
public:
	CDlgKEEnd(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgKEEnd)
	enum { IDD = IDD_DLG_KE_END };
	int		m_Radio;
	CString	m_StaticBig;
	CString	m_StaticSmall;
	CString	m_StaticCaption;
	CString	m_StaticCaptionBig;
	CString	m_StaticCaptionSmall;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgKEEnd)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgKEEnd)
	afx_msg void OnRadioBig();
	afx_msg void OnRadioOver();
	afx_msg void OnRadioSmall();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGKEEND_H__BF523EE2_8B3A_11D5_9A4B_008048FD9845__INCLUDED_)
