#if !defined(AFX_DLGSETTINGSFORCURVERS_H__D6BEAE23_C42A_4182_806F_1978696B07D8__INCLUDED_)
#define AFX_DLGSETTINGSFORCURVERS_H__D6BEAE23_C42A_4182_806F_1978696B07D8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSettingsForCurvers.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgSettingsForCurvers dialog
class CMainFrame;
//class CDoc;
//class CGraphics;

class CDlgSettingsForCurvers : public CDialog
{
// Construction
public:
	void UpdateWndSimpleGuideLines();
	void UpdateWndSimpleGrid();
	HWND m_hWndSimpleGrid;
	COLORREF m_ColorGrid;
	void UpdateWndSimpleRes();
	int m_SizePointsRes;
	int m_SizePointsCurr;
	COLORREF WndColorSet(COLORREF rgb);
	void UpdateWndSimpleCurr();
	CMainFrame* m_pMainFrame;
	HBRUSH m_Brush;
	HWND m_hWndSimpleCurr;
	HWND m_hWndSimpleRes;
	HWND m_hWndSimpleGuideLines;
	COLORREF m_ColorCurr;
	COLORREF m_ColorRes;
//	int m_SizePointsCurr;
	
	CDlgSettingsForCurvers(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgSettingsForCurvers)
	enum { IDD = IDD_DLG_SETTINGS_FOR_CURVERS };
	BOOL	m_CheckCurrLine;
	BOOL	m_CheckCurrPoints;
	BOOL	m_CheckResLine;
	BOOL	m_CheckResPoints;
	BOOL	m_CheckGrid;
	BOOL	m_CheckGuideLines;
	//}}AFX_DATA
	//int m_SizePointsCurr;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgSettingsForCurvers)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgSettingsForCurvers)
	virtual BOOL OnInitDialog();
	afx_msg void OnCheckCurrLine();
	afx_msg void OnButtonCurrColor();
	afx_msg void OnCheckCurrPoints();
	afx_msg void OnPaint();
	afx_msg void OnSelEndOkComboSizeCurr();
	afx_msg void OnButtonResColor();
	afx_msg void OnCheckResLine();
	afx_msg void OnCheckResPoints();
	afx_msg void OnSelEndOkComboSizeRes();
	afx_msg void OnCheckGrid();
	afx_msg void OnButtonGridColor();
	afx_msg void OnCheckGuideLines();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSETTINGSFORCURVERS_H__D6BEAE23_C42A_4182_806F_1978696B07D8__INCLUDED_)
