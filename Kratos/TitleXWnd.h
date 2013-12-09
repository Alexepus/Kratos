#if !defined(AFX_TITLEXWND_H__7F5E5188_75D1_11D5_9A4B_008048FD9845__INCLUDED_)
#define AFX_TITLEXWND_H__7F5E5188_75D1_11D5_9A4B_008048FD9845__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// TitleXWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTitleXWnd window

class CTitleXWnd : public CWnd
{
// Construction
public:
	CTitleXWnd();

// Attributes
public:
	
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTitleXWnd)
	//}}AFX_VIRTUAL

// Implementation
public:
	char* m_TextString;
	COLORREF m_TextColor;
//	char m_FontStyle[LF_FACESIZE];
	char* m_lpszSection;
	LOGFONT* m_LogFont;
	HFONT m_Font;
	//CTitleXWnd* m_pTitleX;
	virtual ~CTitleXWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CTitleXWnd)
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TITLEXWND_H__7F5E5188_75D1_11D5_9A4B_008048FD9845__INCLUDED_)
