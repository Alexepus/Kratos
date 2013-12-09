#if !defined(AFX_FITABLEDLG_H__58CC0340_DC2A_11D5_932F_801150622D24__INCLUDED_)
#define AFX_FITABLEDLG_H__58CC0340_DC2A_11D5_932F_801150622D24__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FiTableDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFiTableDlg dialog

class CFiTableDlg : public CDialog
{
// Construction
public:
	CFiTableDlg(CWnd* pParent = NULL);   // standard constructor
	CFiTable *m_pFiTable;
	// Dialog Data
	//{{AFX_DATA(CFiTableDlg)
	enum { IDD = IDD_DLG_FI_TABLE };
	CFiListCtrl	m_list;
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFiTableDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFiTableDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnColumnclickFiList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FITABLEDLG_H__58CC0340_DC2A_11D5_932F_801150622D24__INCLUDED_)
