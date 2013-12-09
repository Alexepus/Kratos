#if !defined(AFX_COMMENTSWND_H__C6356799_E9F3_4009_A7A9_E8BEC765748D__INCLUDED_)
#define AFX_COMMENTSWND_H__C6356799_E9F3_4009_A7A9_E8BEC765748D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CommentsWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCommentsWnd window
class CMainFrame;

class CCommentsWnd : public CWnd
{
// Construction
public:
	CCommentsWnd();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCommentsWnd)
	//}}AFX_VIRTUAL

// Implementation
public:
	HWND m_hWndEdit;
	CMainFrame* m_pMainFrame;
	virtual ~CCommentsWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CCommentsWnd)
	afx_msg void OnClose();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COMMENTSWND_H__C6356799_E9F3_4009_A7A9_E8BEC765748D__INCLUDED_)
