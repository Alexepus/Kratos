#if !defined(AFX_THERMOSETTINGSDLG_H__6F8C8AB1_2DFB_4CC0_B784_52552A36D14C__INCLUDED_)
#define AFX_THERMOSETTINGSDLG_H__6F8C8AB1_2DFB_4CC0_B784_52552A36D14C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ThermoSettingsDlg.h : header file
//
#include "resource.h"
/////////////////////////////////////////////////////////////////////////////
// CThermoSettingsDlg dialog

class CThermoSettingsDlg : public CDialog
{
// Construction
public:
	CThermoSettingsDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CThermoSettingsDlg)
	enum { IDD = IDD_TR_SETTINGS_DLG };
	CEdit	m_EditStartDelay;
	CButton	m_CheckRepeat;
	CEdit	m_EditColdEnds;
	CEdit	m_EditTemp2;
	CEdit	m_EditTemp1;
	CEdit	m_EditPrimeTemp;
	CEdit	m_EditHeatingRate;
	CEdit	m_EditTime1;
	CEdit	m_EditTime2;
	CEdit	m_EditCoolingRate;
	CEdit	m_EditBetaPlus;
	CEdit	m_EditBetaMinus;
	CEdit	m_EditCoupleFile;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CThermoSettingsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void OnOK();

	// Generated message map functions
	//{{AFX_MSG(CThermoSettingsDlg)
	afx_msg void OnButtonBrowseCoupleCalib();
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonSaveFunction();
	afx_msg void OnButtonLoadFunction();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_THERMOSETTINGSDLG_H__6F8C8AB1_2DFB_4CC0_B784_52552A36D14C__INCLUDED_)
