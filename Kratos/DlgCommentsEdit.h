#if !defined(AFX_DLGCOMMENTSEDIT_H__F5D4D469_4F6A_4E1F_B8CE_3B42A7C6D02B__INCLUDED_)
#define AFX_DLGCOMMENTSEDIT_H__F5D4D469_4F6A_4E1F_B8CE_3B42A7C6D02B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgCommentsEdit.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgCommentsEdit dialog
class CDialogParamRegion;

class CDlgCommentsEdit : public CDialog
{
// Construction
public:
	CDialogParamRegion* m_pDlgParam;
	CDlgCommentsEdit(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgCommentsEdit)
	enum { IDD = IDD_DIALOG_COMMENTS_EDIT };
	CString	m_strComments;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgCommentsEdit)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgCommentsEdit)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGCOMMENTSEDIT_H__F5D4D469_4F6A_4E1F_B8CE_3B42A7C6D02B__INCLUDED_)
