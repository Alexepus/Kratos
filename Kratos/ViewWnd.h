#if !defined(AFX_VIEWWND_H__ADB7E605_C2D7_45E1_A09B_A9292F8FAA09__INCLUDED_)
#define AFX_VIEWWND_H__ADB7E605_C2D7_45E1_A09B_A9292F8FAA09__INCLUDED_


#include "ViewGraphics.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ViewWnd.h : header file
//

class CMainFrame;
/////////////////////////////////////////////////////////////////////////////
// CViewWnd window

class CViewWnd : public CWnd
{
// Construction
public:
	CViewWnd();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CViewWnd)
	protected:
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

// Implementation
public:
	CMainFrame* m_pMainFrame;
	CViewGraphics m_ViewGraph;

	HWND m_hStatusBar;
	RECT m_rectWnd;
	WNDCLASS m_WC;
	virtual ~CViewWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CViewWnd)
	afx_msg void OnClose();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIEWWND_H__ADB7E605_C2D7_45E1_A09B_A9292F8FAA09__INCLUDED_)
