// MainFrame.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRAME_H__C304ADF7_3A1B_484E_919D_C2B101CA93E6__INCLUDED_)
#define AFX_MAINFRAME_H__C304ADF7_3A1B_484E_919D_C2B101CA93E6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "SpecView.h"
#include "MassSpecDoc.h"
#include "ThermoSettingsDlg.h"
#include "XStatusBar.h"
#include <set>

class CMainFrame : public CFrameWnd
{
	
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:
	struct Indicators
	{
		enum IndicatorIds
		{
			Protection = 1,
			Power,
			RefTemp,
			CurTemp,
			RemainTime
		};
	};

	struct ProtectionStatus
	{
		enum State
		{
			OverloadI = 1,
			OverloadU = 2,
			OverloadP = 4,
			LimitationI = 8,
			OK = 16,
			Unknown = 32
		};
	};
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetStatusProtection(int state); // ProtectionStatus::State
	void SetStatusPower(double Power, bool On=true);
	void SetStatusTRef(double TRef, bool On=true);
	void SetStatusTCur(double TCur, bool Known=true);
	void SetStatusRemainTime(double Time);
	CMassSpecDoc* m_pMassSpecDoc;
	virtual ~CMainFrame();

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	XStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;
	CToolBar    m_wndToolBarTR;
protected:  // control bar embedded members
	double m_dRemainTime;
	double m_dTRef;
	double m_dTCur;
	static const int TimerIdStatusProtection = 23;
	int ProtectionSignalingCount;
	CMenu m_PopupMenuProtection;
// Generated message map functions
protected:
	afx_msg void OnUpdateStatusRemainTime(CCmdUI *pCmdUI);
	afx_msg void OnUpdateStatusCurTemp(CCmdUI *pCmdUI);
	afx_msg void OnUpdateStatusRefTemp(CCmdUI *pCmdUI);
	afx_msg void CMainFrame::OnTimer(UINT_PTR nIDEvent);
	LRESULT OnRemoteDxpsState(WPARAM WParam, LPARAM LParam);

	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSettingsHardware();
	afx_msg void OnClose();
	afx_msg void OnTrTrsettings();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	void SetStatusProtectionHighlight();
public:
	afx_msg void OnPopupResetProtection();
	afx_msg void OnPopupProtectionParameters();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRAME_H__C304ADF7_3A1B_484E_919D_C2B101CA93E6__INCLUDED_)
