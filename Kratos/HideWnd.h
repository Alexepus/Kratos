#if !defined(AFX_HIDEWND_H__A3346704_7533_11D5_9A4B_008048FD9845__INCLUDED_)
#define AFX_HIDEWND_H__A3346704_7533_11D5_9A4B_008048FD9845__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// HideWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CHideWnd window
//class CBigClientWnd;
class CMainFrame;

class CHideWnd : public CWnd
{
// Construction
public:
	CHideWnd();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHideWnd)
	//}}AFX_VIRTUAL

// Implementation
public:
	CMainFrame* m_pMainFrame;
	
	CBigClientWnd* m_pBigClientWnd;
	BOOL CreateClient(LPCREATESTRUCT cs);
	HWND m_hVScroll;
	HWND m_hHScroll;
	virtual ~CHideWnd();


	// Generated message map functions
protected:
	//{{AFX_MSG(CHideWnd)
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT OnMyResize(WPARAM wp, LPARAM lp);
	//}}AFX_MSG
	//void OnMyResize(WPARAM wp, LPARAM lp);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HIDEWND_H__A3346704_7533_11D5_9A4B_008048FD9845__INCLUDED_)
