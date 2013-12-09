#pragma once
#include "afxwin.h"


// CProtectionSetupDlg dialog

class CProtectionSetupDlg : public CDialog
{
	DECLARE_DYNAMIC(CProtectionSetupDlg)

public:
	CProtectionSetupDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CProtectionSetupDlg();

// Dialog Data
	enum { IDD = IDD_PROTECTION_SETUP_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
	CEdit m_MaxCurrentEdit;
	CEdit m_MaxVoltageEdit;
	CEdit m_MaxPowerEdit;
	afx_msg void OnKillfocusEditMaxcurrent();
	afx_msg void OnKillfocusEditMaxvoltage();
	afx_msg void OnKillfocusEditMaxpower();
	BOOL OnInitDialog();

	double MaxI, MaxU, MaxP, LimitI;
	void ParseValues();
	CEdit m_LimitCurrentEdit;
};
