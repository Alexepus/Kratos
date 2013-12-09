#if !defined(AFX_DXPSDLG_H__4A32C1E9_F1B4_4928_BF0B_065C2B2CCCAC__INCLUDED_)
#define AFX_DXPSDLG_H__4A32C1E9_F1B4_4928_BF0B_065C2B2CCCAC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DxpsDlg.h : header file
//
#include "Main.h"
#include "DxpsRegion.h"	// Added by ClassView
/////////////////////////////////////////////////////////////////////////////
// CDxpsDlg dialog

#define DXPS_TIMER_INIT 1
class CDxpsDlg : public CDialog
{
// Construction
public:
	BOOL EditScanTimeActualized;
	BOOL m_SetFocusEditScanTime;
	CBrush m_EditBkBrush;
	void LoadRegionPreferences();
	void SaveRegionPreferences();
	DWORD m_ColorIndex[15];
	void SetIcon(int Row, int ColorIndex, int Status);
	CImageList ImLst;
	int FindSelectedItem();
	void FillTable();
	void FillTableRow(int Row, DxpsRegPar* Param);
	void MoveControls();
	BOOL Create();
	afx_msg void OnGetMinMaxInfo( MINMAXINFO FAR* lpMMI );
	CDxpsDlg(CWnd* pParent = NULL);   // standard constructor
	DxpsRegPar PreferredParams;
// Dialog Data
	//{{AFX_DATA(CDxpsDlg)
	enum { IDD = IDD_DLG_DXPS };
	CListCtrl	m_ListDxps;
	CString	m_EditTime;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDxpsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

// Implementation
protected:
	void OnOK();
	BOOL AdjustListLastColomn();
	CFont m_Font;
	BOOL Init;
	HICON m_hIcon;
	// Generated message map functions
	//{{AFX_MSG(CDxpsDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDblclkListDxps(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRclickListDxps(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonDxpsEdit();
	afx_msg void OnMove(int x, int y);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnKillfocusEditDxpsTime();
	afx_msg void OnButtonDxpsAddNew();
	afx_msg void OnRootAddNew();
	afx_msg void OnButtonDxpsDelete();
	afx_msg void OnRootDelete();
	afx_msg void OnRootEdit();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnButtonDxpsOnoff();
	afx_msg void OnPaint();
	afx_msg void OnButtonDxpsView();
	afx_msg void OnChangeEditDxpsTime();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnButtonDxpsClose();
	afx_msg void OnSetFocusEditDxpsTime();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClickedCheckDxpsHideOff();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DXPSDLG_H__4A32C1E9_F1B4_4928_BF0B_065C2B2CCCAC__INCLUDED_)
