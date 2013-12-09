#if !defined(AFX_BIGCLIENTWND_H__7F5E5187_75D1_11D5_9A4B_008048FD9845__INCLUDED_)
#define AFX_BIGCLIENTWND_H__7F5E5187_75D1_11D5_9A4B_008048FD9845__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// BigClientWnd.h : header file
//

class CMainFrame;
class CTitleXWnd;
/////////////////////////////////////////////////////////////////////////////
// CBigClientWnd window

class CBigClientWnd : public CWnd
{
// Construction
public:
	CBigClientWnd();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBigClientWnd)
	protected:
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

// Implementation
public:
	CMainFrame* m_pMainFrame;
	void RecalcChildSize(int cx, int cy);
//	int cy;
//	int cx;
	CTitleXWnd* m_pTitleXWnd;
	BOOL CreateTitleX(RECT* r);
	BOOL CreateClient(LPCREATESTRUCT cs);
	const char* m_class_name;
	virtual ~CBigClientWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CBigClientWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BIGCLIENTWND_H__7F5E5187_75D1_11D5_9A4B_008048FD9845__INCLUDED_)
