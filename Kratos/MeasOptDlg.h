#if !defined(AFX_MEASOPTDLG_H__CDC03EA1_C148_11D2_932F_B48A64D85031__INCLUDED_)
#define AFX_MEASOPTDLG_H__CDC03EA1_C148_11D2_932F_B48A64D85031__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MeasOptDlg.h : header file
//
//#include "region.h"
//#include "threads.h"
class CMainFrame;
/////////////////////////////////////////////////////////////////////////////
// CMeasOptDlg dialog

class CMeasOptDlg : public CDialog
{
// Construction
public:
	CMeasOptDlg(CWnd* pParent = NULL);   // standard constructor
	BOOL CMeasOptDlg::OnInitDialog();
	int m_SliderNSigma;
	int m_SliderNSubmeasurings;
// Dialog Data
	//{{AFX_DATA(CMeasOptDlg)
	enum { IDD = IDD_DLG_MEASURE_OPT };
	CString	m_NSigma;
	CString	m_Attempts;
	CString	m_Submeasurings;
	BOOL	m_CheckRemeasure;
	//}}AFX_DATA
	
	CSliderCtrl  SliderNSigma;
	CSliderCtrl  SliderAttempts;
	CSliderCtrl  SliderSubmeasurings;

	THREAD_COMMON* m_pThreadCom;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMeasOptDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMeasOptDlg)
	afx_msg void OnCustomdrawSliderNsigma(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCustomdrawSliderNattempts(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCustomdrawSliderNsubmeasurings(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCustomdrawSliderRegiondelay(NMHDR* pNMHDR, LRESULT* pResult);
	virtual void OnOK();
	afx_msg void OnCheckRemeasure();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MEASOPTDLG_H__CDC03EA1_C148_11D2_932F_B48A64D85031__INCLUDED_)
