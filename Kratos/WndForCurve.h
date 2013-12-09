#if !defined(AFX_WNDFORCURVE_H__EE98C7BE_0D79_42A3_BC9D_585353B25F99__INCLUDED_)
#define AFX_WNDFORCURVE_H__EE98C7BE_0D79_42A3_BC9D_585353B25F99__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WndForCurve.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CWndForCurve window
class CGraphics;

class CWndForCurve : public CWnd
{
// Construction
public:
	CWndForCurve();

// Attributes
public:
	CGraphics* m_pGraph;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWndForCurve)
	//}}AFX_VIRTUAL

// Implementation
public:
	BOOL m_MouseInWin;
	CPoint LastPoint;
	CPoint LastPointGuide;
	CPoint BeginPoint;
	double m_Coord_Y;
	double m_Coord_X;

	virtual ~CWndForCurve();

	// Generated message map functions
protected:
	//{{AFX_MSG(CWndForCurve)
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
#endif // !defined(AFX_WNDFORCURVE_H__EE98C7BE_0D79_42A3_BC9D_585353B25F99__INCLUDED_)
