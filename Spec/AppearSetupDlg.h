#if !defined(AFX_APPEARSETUPDLG_H__8EDDAD70_F875_4C8D_8B7C_1F713A923B94__INCLUDED_)
#define AFX_APPEARSETUPDLG_H__8EDDAD70_F875_4C8D_8B7C_1F713A923B94__INCLUDED_

#include "HSCHART\ChartCtrl.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AppearSetupDlg.h : header file
//
#include "resource.h"
#include "ColourPicker.h"
#include "HSChart\\ChartCtrl.h"
/////////////////////////////////////////////////////////////////////////////
// CAppearSetupDlg dialog
#define SERIES_TYPE_LINE_DOTS	0
#define SERIES_TYPE_LINE		1
#define SERIES_TYPE_DOTS		2
#define SERIES_TYPE_TRAPEZIUM	3 

class CAppearSetupDlg : public CDialog
{
// Construction
public:
	void LoadFromStruct(CSeriesSetup *Opt);
	void EnableSeriesControls(BOOL Enable=TRUE);
	int m_Series1Type;
	int m_Series1Width;
	int m_Series1DotSize;
	int m_Series1Color;
	int m_Series1DotColor;
	int m_Series2Type;
	int m_Series2Width;
	int m_Series2DotSize;
	int m_Series2Color;
	int m_Series2DotColor;


	CChartCtrl m_Chart;
	CAppearSetupDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAppearSetupDlg)
	enum { IDD = IDD_APPEARANCE_SETUP_DLG };
	CTreeCtrl	m_Tree;
	CDateTimeCtrl	m_KeepTime;
	CButton	m_CheckShowPower;
	CColourPicker m_ButtonSeries1Color;
	CStatic	m_StaticSize1;
	CSliderCtrl	m_SliderSeries1Size;
	CComboBox	m_ComboSeries1Type;
	CComboBox	m_ComboSeries1Element;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAppearSetupDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAppearSetupDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnCustomdrawSliderSeries1Size(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCloseupComboSeries1Element();
	afx_msg void OnCloseupComboSeries1Type();
	afx_msg void OnSelchangedAppearTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg LRESULT OnButtonSeries1Color(WPARAM Color, LPARAM DlgId);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_APPEARSETUPDLG_H__8EDDAD70_F875_4C8D_8B7C_1F713A923B94__INCLUDED_)
