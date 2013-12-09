#if !defined(AFX_ADCDACDLG_H__059873AE_72C7_48C0_8D02_616A8B8107C7__INCLUDED_)
#define AFX_ADCDACDLG_H__059873AE_72C7_48C0_8D02_616A8B8107C7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AdcDacDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAdcDacDlg dialog
#define TIMER_POLL_SERIAL 1
#define TIMER_HEATER 2
#define TIMER_MSP 3

class CAdcDacDlg : public CDialog
{
// Construction
public:
	CAdcDacDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAdcDacDlg)
	enum { IDD = IDD_ADCDAC_DLG };
	CButton	m_CheckAdcOn;
	CStatic	m_StaticDac1;
	CStatic	m_StaticDac0;
	CButton	m_CheckManualMsp;
	CButton	m_CheckManualHeater;
	CSliderCtrl	m_SliderMsp;
	CSliderCtrl	m_SliderHeater;
	CStatic	m_StaticAverAdc2;
	CStatic	m_StaticAverAdc3;
	CStatic	m_StaticAverAdc1;
	CStatic	m_StaticAverAdc0;
	CStatic	m_StaticAdc3;
	CStatic	m_StaticAdc2;
	CStatic	m_StaticAdc1;
	CStatic	m_StaticAdc0;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAdcDacDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAdcDacDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnCheckManualHeater();
	afx_msg void OnCheckManualMsp();
	afx_msg void OnCustomdrawSliderHeater(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCustomdrawSliderMsp(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCheckAdcOn();
	//}}AFX_MSG
	LRESULT OnSerialLinkEvent(WPARAM WParam, LPARAM LParam);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADCDACDLG_H__059873AE_72C7_48C0_8D02_616A8B8107C7__INCLUDED_)
