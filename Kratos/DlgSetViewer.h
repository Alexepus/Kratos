#if !defined(AFX_DLGSETVIEWER_H__BA07B962_E1E1_4550_9C19_FDB111D289F0__INCLUDED_)
#define AFX_DLGSETVIEWER_H__BA07B962_E1E1_4550_9C19_FDB111D289F0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSetViewer.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgSetViewer dialog
class CMainFrame;

class CDlgSetViewer : public CDialog
{
// Construction
public:
//	C0LORREF m_ColorText;
	LOGFONT m_LogFont;
	COLORREF WndColorSet(COLORREF rgb);
	void UpdateWndSimpleFont();
	HWND m_hWndSimpleFont;
	void UpdateWndSimpleGrid();
	HWND m_hWndSimpleGrid;
	void UpdateWndSimpleCurve();
	HWND m_hWndSimpleCurve;
	int m_SizePoints;
	HFONT m_Font;
	CMainFrame* m_pMainFrame;
	COLORREF m_ColorFont;
	COLORREF m_ColorGrid;
	COLORREF m_ColorCurve;
	CDlgSetViewer(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgSetViewer)
	enum { IDD = IDD_DLG_VIEWER_SET };
	BOOL	m_CheckLine;
	BOOL	m_CheckPoints;
	BOOL	m_CheckGrid;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgSetViewer)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgSetViewer)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnCheckCurveLine();
	afx_msg void OnCheckCurvePoints();
	afx_msg void OnSelEndOkComboSizeCurve();
	afx_msg void OnButtonCurveColor();
	afx_msg void OnCheckGrid();
	afx_msg void OnButtonGridColor();
	afx_msg void OnButtonFontColor();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSETVIEWER_H__BA07B962_E1E1_4550_9C19_FDB111D289F0__INCLUDED_)
