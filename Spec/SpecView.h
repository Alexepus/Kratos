// SpecView.h : interface of the CSpecView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_SPECVIEW_H__26AA1D20_E093_42CC_82EE_F4A17917C28A__INCLUDED_)
#define AFX_SPECVIEW_H__26AA1D20_E093_42CC_82EE_F4A17917C28A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include"IniValues.h"
#include"CalibDlg.h"
#include"ThermoReg.h"

class CChartCtrl;
class CReportCtrl;
class CMassSpecDoc;
class CHeaterSetupDlg;

struct CSpecMsg
{
	int RegionN;
	double Intensity;
	double Time;
};


class CSpecView : public CView
{
protected: // create from serialization only
	CSpecView();
	DECLARE_DYNCREATE(CSpecView)

// Attributes
public:
	CMassSpecDoc* GetDocument();
	SvLdInt m_bShowList;
	SvLdInt m_iListWidth;
	SvLdInt	m_iColumn0Width;
	SvLdInt	m_iColumn1Width;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSpecView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL
	LRESULT OnListCheckBox(WPARAM WParam, LPARAM LParam);
	LRESULT OnListItemSorted(WPARAM WParam, LPARAM LParam);
	LRESULT OnListEditCommited(WPARAM WParam, LPARAM LParam);
	LRESULT OnMeasThreadMessage(WPARAM WParam, LPARAM LParam);
	LRESULT OnSerialEvent(WPARAM WParam, LPARAM LParam);
	LRESULT OnRemoteDxpsState(WPARAM WParam, LPARAM LParam);

// Implementation
public:
	void UpdateView();
	void FinishDrawMassChart();
	CThermoReg m_ThermoReg;
	void RedrawChart();
	void FormatChartToMassHist();
	void FillTable();
	volatile BOOL m_bMeasureStart;
	CCalibDlg* m_pCalibDlg;
	CRect m_CalibDlgRect;
	void OnButtonCalibOK();
	CButton* m_pButtonOK;
	HCURSOR m_hCursorDivSize;
	void DrawDivider(CDC* pDC=NULL);
	void ResizeClient(int x=-1, int y=-1);
	CPoint m_ClickPoint;
	CPoint m_LastPoint;
	BOOL m_bWasLButtonDown;
	enum DispMode{eMassSpec, eMassCalib, eThermoChart} m_DisplayMode, m_OldDispMode;
	//BOOL m_bCalibration;
	//BOOL m_bThermoChart;
	CChartCtrl* m_pChart;
	CReportCtrl* m_pList;
	CRect m_rDivider;
	CHeaterSetupDlg *m_pHeaterSetupDlg;
	virtual ~CSpecView();
	void StartMsp(){OnToolBarStart();}
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CSpecView)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnViewMasses();
	afx_msg void OnUpdateViewMasses(CCmdUI* pCmdUI);
	afx_msg void OnSettingsCalibration();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnToolBarStart();
	afx_msg void OnUpdateToolBarButtonStart(CCmdUI* pCmdUI);
	afx_msg void OnToolBarStop();
	afx_msg void OnUpdateToolBarButtonStop(CCmdUI* pCmdUI);
	afx_msg void OnDestroy();
	afx_msg void OnSettingsAppearance();
	afx_msg void OnViewMassChart();
	afx_msg void OnUpdateViewMassChart(CCmdUI* pCmdUI);
	afx_msg void OnViewTemperatureChart();
	afx_msg void OnUpdateViewTemperatureChart(CCmdUI* pCmdUI);
	afx_msg void OnTrStart();
	afx_msg void OnTrWait();
	afx_msg void OnTrWaitCont();
	afx_msg void OnTrContinue();
	afx_msg void OnTrMainOff();
	afx_msg void OnTrPrime();
	afx_msg void OnUpdateTrMainoff(CCmdUI* pCmdUI);
	afx_msg void OnUpdateTrPrime(CCmdUI* pCmdUI);
	afx_msg void OnUpdateTrStart(CCmdUI* pCmdUI);
	afx_msg void OnUpdateTrWait(CCmdUI* pCmdUI);
	afx_msg void OnUpdateTrWaitCont(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditCut(CCmdUI* pCmdUI);
	afx_msg void OnEditCut();
	afx_msg void OnUpdateSettingsCalibration(CCmdUI* pCmdUI);
	afx_msg void OnSettingsHeaterSetup();
	afx_msg void OnUpdateSettingsHeaterSetup(CCmdUI* pCmdUI);
	afx_msg void OnEditCopy();
	afx_msg void OnUpdateEditCopy(CCmdUI* pCmdUI);
	afx_msg void OnFileSaveasEasyplot();
	afx_msg void OnUpdateTrSyncDxps(CCmdUI* pCmdUI);
	afx_msg void OnUpdateTrSyncMsp(CCmdUI* pCmdUI);
	afx_msg void OnTrSyncDxps();
	afx_msg void OnTrSyncMsp();
	afx_msg void OnSettingsMeasuringOptions();
	afx_msg void OnSettingsHardwareSetup();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSettingsAdcsdacs();
};

#ifndef _DEBUG  // debug version in SpecView.cpp
inline CMassSpecDoc* CSpecView::GetDocument()
   { return (CMassSpecDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SPECVIEW_H__26AA1D20_E093_42CC_82EE_F4A17917C28A__INCLUDED_)
