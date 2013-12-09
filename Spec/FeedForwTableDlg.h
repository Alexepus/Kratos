#if !defined(AFX_FEEDFORWTABLEDLG_H__3CF150F9_2A73_4FEC_B85A_8AE33203D907__INCLUDED_)
#define AFX_FEEDFORWTABLEDLG_H__3CF150F9_2A73_4FEC_B85A_8AE33203D907__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FeedForwTableDlg.h : header file
//
#include "ReportCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CFeedForwTableDlg dialog

class CFeedForwTableDlg : public CDialog
{
// Construction
public:
	void SaveData();
	void OnOK();
	CFeedForwTableDlg(CWnd* pParent = NULL);   // standard constructor
	LRESULT OnListEditCommited(WPARAM WParam, LPARAM LParam);

// Dialog Data
	//{{AFX_DATA(CFeedForwTableDlg)
	enum { IDD = IDD_FF_TABLE_DLG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFeedForwTableDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	CReportCtrl m_List;

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFeedForwTableDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonLoadFf();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FEEDFORWTABLEDLG_H__3CF150F9_2A73_4FEC_B85A_8AE33203D907__INCLUDED_)
