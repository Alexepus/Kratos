#if !defined(AFX_CALIBDLG_H__3506A98A_065A_457F_9EEE_EEA6D7F4B9C8__INCLUDED_)
#define AFX_CALIBDLG_H__3506A98A_065A_457F_9EEE_EEA6D7F4B9C8__INCLUDED_

#include "ReportCtrl.h"	// Added by ClassView
#include "HSChart\\ChartSerie.h"
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CalibDlg.h : header file
//
class CSpecView;
class CChartTrapeziumSerie;
class CChartLinePointsSerie;

/////////////////////////////////////////////////////////////////////////////
// CCalibDlg dialog

#define WM_NEW_CALIB_MSG (WM_USER+9)

struct CMassCalibMsg
{
	enum{eNewPoint, eEndSweep} Action;
	double Mass;
	double Voltage;
};

class CCalibDlg : public CDialog
{
// Construction
public:
	void RedrawChart();
	void StopCalib();
	void FormatChartToCalib();
	void StartCalib();
	void FillTable();
	void CopyToClipboard();
	CString PrintEasyPlotData();
	void SaveEasyPlot(char *FileName);


	CSpecView* m_pSpecView;
	CReportCtrl m_List;
	CRect m_ButtonOKRect;
	CRect m_ListRect;
	CCalibDlg(CWnd* pParent = NULL);   // standard constructor
	CThreadSafeQueue<CMassCalibMsg> m_CalibMsgQueue;
	volatile bool m_bMeasureStart;
	CChartTrapeziumSerie* m_pTSerie;
	CChartLinePointsSerie* m_pLPSerie;
	std::vector<CChartSerie::SChartPoint> m_vPointsCurrentPass, m_vPointsLastPass;
// Dialog Data
	//{{AFX_DATA(CCalibDlg)
	enum { IDD = IDD_CALIB_DLG };
	CButton	m_ButtonInspect;
	CButton	m_ButtonCancel;
	CButton	m_ButtonOK;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCalibDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	LRESULT OnNewMsg(WPARAM WParam, LPARAM LParam);
	void OnCancel();
	void OnOK();

	// Generated message map functions
	//{{AFX_MSG(CCalibDlg)
	afx_msg void OnButtonCalibOk();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	virtual BOOL OnInitDialog();
	afx_msg void OnClickListCalib(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnButtonCancel();
	afx_msg void OnKeydownListCalib(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonCalibInspect();
	//}}AFX_MSG
	LRESULT OnListCheckBox(WPARAM WParam, LPARAM LParam);
	LRESULT OnListEditCommited(WPARAM WParam, LPARAM LParam);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CALIBDLG_H__3506A98A_065A_457F_9EEE_EEA6D7F4B9C8__INCLUDED_)
