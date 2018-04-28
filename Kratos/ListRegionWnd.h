#if !defined(AFX_LISTREGIONWND_H__ECBB88A5_84C3_11D5_9A4B_008048FD9845__INCLUDED_)
#define AFX_LISTREGIONWND_H__ECBB88A5_84C3_11D5_9A4B_008048FD9845__INCLUDED_

//#include "CommentsWnd.h"	// Added by ClassView
#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// ListRegionWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CListRegionWnd window

class CListRegionWnd : public CListCtrl
{
// Construction
public:
	CListRegionWnd();
	virtual ~CListRegionWnd();

	// Attributes
public:
	CCommentsWnd m_CommentsWnd;
	CMainFrame* m_pMainFrame;
	CImageList m_ImageIcon;
	int N_Column;

// Operations
public:
	void CreateColumns();
	void SetNewRegionItem(CRegion* pReg);
	void UpdateItem(CRegion* pReg);
	int FindSelectedItem();
	std::vector<CRegion*> GetSelectedRegions();
	void SelectItems(std::vector<CRegion*> regions);
	void SetIconForReg(CRegion* pReg, int Image);
	void SetOnOffIcon(CRegion* pReg);
	void SaveColumnsWidthsToIni();
	void Create(CWnd* parentCWnd, RECT controlPosition);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CListRegionWnd)
	protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);	
	//}}AFX_VIRTUAL
	void OnCustomdraw(NMHDR* pNMHDR, LRESULT* pResult);

private:
	CString m_iniSectionName;
	std::vector<CString> m_columnsNames; // Названия колонок
	static bool IsItemDisabledAtIndex(int index);

	// Generated message map functions
protected:
	//{{AFX_MSG(CListRegionWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LISTREGIONWND_H__ECBB88A5_84C3_11D5_9A4B_008048FD9845__INCLUDED_)
