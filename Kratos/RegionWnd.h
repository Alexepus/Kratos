#if !defined(AFX_REGIONWND_H__C7EC35A2_81A9_11D5_9A4B_008048FD9845__INCLUDED_)
#define AFX_REGIONWND_H__C7EC35A2_81A9_11D5_9A4B_008048FD9845__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// RegionWnd.h : header file
//

//class CListRegionWnd;

typedef struct _BUTTON
{
char* Name;
HWND hWnd;
} BUTTON;

typedef struct _ALL_BUTTONS
{
int cx;
int cy;
BUTTON AddNew;
BUTTON Edit;
BUTTON Delete;
BUTTON Close;
BUTTON OnOff;
BUTTON View;
} ALL_BUTTONS;


/////////////////////////////////////////////////////////////////////////////

class CMainFrame;

// CRegionWnd window

class CRegionWnd : public CWnd
{
	HICON m_hIcon;
// Construction
	public:
	CRegionWnd();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRegionWnd)
	protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	CMainFrame* m_pMainFrame;
	CDialogParamRegion* m_pDlgParamReg;
	CListRegionWnd* m_pListRegionWnd;

	ALL_BUTTONS m_Button;
	RECT m_rectWnd;
	BOOL RegisterRegionWndClass();
//	const char* m_ClassNameRegionWnd;
	WNDCLASS m_WC;

	
	virtual ~CRegionWnd();

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
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REGIONWND_H__C7EC35A2_81A9_11D5_9A4B_008048FD9845__INCLUDED_)
