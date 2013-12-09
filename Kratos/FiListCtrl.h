#if !defined(AFX_MYLISTCTRL_H__EF9ED613_8511_11D5_B81D_995740C4431E__INCLUDED_)
#define AFX_MYLISTCTRL_H__EF9ED613_8511_11D5_B81D_995740C4431E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyListCtrl.h : header file

#include "resource.h"		// main symbols
//
#define OFFSET_FIRST    2
#define OFFSET_OTHER    5

////////////////////////////////////////////////////
// CFiEdit Class
class CFiEdit : public CEdit
{
// Construction
public:
	 CFiEdit();
	int m_x;
	int m_y;
	int m_cx;
	int m_cy;
// Attributes
public:

// Operations
public:
 
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFiEdit)
	//}}AFX_VIRTUAL
	// Generated message map functions
protected:
	//{{AFX_MSG(CFiEdit)
	afx_msg void OnWindowPosChanging(WINDOWPOS FAR* lpwndpos);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// FiListCtrl window

class CFiListCtrl : public CListCtrl
{
// Construction
public:
	CFiListCtrl();
	enum { IDD = IDC_FI_LIST };
	void SetStyle(DWORD NewStile);
	int ColumnSort;
	BOOL m_bClientWidthSel;
	void OnColumnClick(NMHDR* pNMHDR, LRESULT* pResult);
// Attributes
public:
		
	COLORREF d1,d2;

	COLORREF m_text1;
	COLORREF m_text2;

	BOOL m_bFullRowSel;
	int m_cxClient;
	int m_cxStateImageOffset;
	BOOL		m_nEdit;
	CFiEdit		m_LVEdit;

protected:
	COLORREF d0;
	COLORREF m_clrText;
	COLORREF m_clrTextBk;
	COLORREF m_clrBkgnd;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFiListCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CFiListCtrl();

	
	// Generated message map functions
protected:
	void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	static LPCTSTR MakeShortString(CDC* pDC, LPCTSTR lpszLong, int nColumnLen, int nOffset);
	//{{AFX_MSG(CMyListCtrl)
	afx_msg void OnBeginlabeledit(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEndlabeledit(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYLISTCTRL_H__EF9ED613_8511_11D5_B81D_995740C4431E__INCLUDED_)
