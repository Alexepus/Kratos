#pragma once
#include "RegionWndButtons.h"

class CMainFrame;

// CRegionWnd window

class CRegionWnd : public CWnd
{
	HICON m_hIcon;
// Construction
	public:
	CRegionWnd(CMainFrame* pMainFrame);
	virtual ~CRegionWnd();

	// Operations
	void Show();
	RECT GetWindowRect() const;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRegionWnd)
	//}}AFX_VIRTUAL

public:
	CListRegionWnd* m_pListRegionWnd;

private:
	CMainFrame* m_pMainFrame;
	CDialogParamRegion* m_pDlgParamReg;
	CRegionWndButtons m_Buttons;
	RECT m_rectWnd;
	BOOL RegisterRegionWndClass();
	WNDCLASS m_WC;	

	// Generated message map functions
protected:
	//{{AFX_MSG(CRegionWnd)
	afx_msg void OnDestroy();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnClose();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnMove(int x, int y);
	afx_msg void OnButtonAddNew();
	afx_msg void OnButtonEdit();
	afx_msg void OnButtonDelete();
	afx_msg void OnButtonView();
	afx_msg void OnButtonOnOff();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.
