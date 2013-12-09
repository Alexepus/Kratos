#if !defined(AFX_VIEWGRAPHICS_H__2488C79E_E069_4068_9723_A560DC46E8D2__INCLUDED_)
#define AFX_VIEWGRAPHICS_H__2488C79E_E069_4068_9723_A560DC46E8D2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ViewGraphics.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CViewGraphics window
//#include "Graphics.h"

class CViewGraphics : public CGraphics
{
// Construction
public:
	CViewGraphics();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CViewGraphics)
	//}}AFX_VIRTUAL

// Implementation
public:
//	CRegion* m_pReg;
	//HFONT m_Font;
	COLORREF m_ColorFont;
	virtual ~CViewGraphics();

	// Generated message map functions
protected:
	//{{AFX_MSG(CViewGraphics)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIEWGRAPHICS_H__2488C79E_E069_4068_9723_A560DC46E8D2__INCLUDED_)
