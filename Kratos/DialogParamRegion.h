#include "afxwin.h"
#if !defined(AFX_DIALOGPARAMREGION_H__078C4E45_871B_11D5_9A4B_008048FD9845__INCLUDED_)
#define AFX_DIALOGPARAMREGION_H__078C4E45_871B_11D5_9A4B_008048FD9845__INCLUDED_

//#include "FiTableDlg.h"	// Added by ClassView --- йнгек !!!!!
#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// DialogParamRegion.h : header file
//

class CDlgKEEnd;
class CFiListCtrl;
class CFiTableDlg;

/////////////////////////////////////////////////////////////////////////////
// CDialogParamRegion dialog


class CDialogParamRegion : public CDialog
{
// Construction
public:
	BOOL CheckHV();

	void SubClassingWindows();
	DATA_IN::EnergyType m_KE_BE;
	BOOL m_Off;
	
	CMainFrame* m_pMainFrame;
	CDlgKEEnd* m_pDlgKEEnd;
	CRegion* m_pReg;
	CDialogParamRegion(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDialogParamRegion)
	enum { IDD = IDD_DIALOG_PARAM_REGION };
	CComboBox	m_ComboHV;
	CStatic m_AnodeTxtControl;
	CComboBox m_ComboAnodeControl;
	double	m_HV;
	double	m_KE_Start;
	double	m_KE_End;
	int		m_N;
	double	m_Step;
	CString	m_Comments;
	int		m_Anode;
	double	m_Time;
	int m_Priority;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogParamRegion)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	virtual void OnOK();
	virtual BOOL OnInitDialog();

	// Generated message map functions
	//{{AFX_MSG(CDialogParamRegion)
	afx_msg void OnRadioOn();
	afx_msg void OnRadioOff();
	void SetKeBe(DATA_IN::EnergyType energyType);
	afx_msg void OnRadioKE();
	afx_msg void OnRadioBE();
	afx_msg void OnSelEndOkComboAnode();
	afx_msg void OnKillFocusEditKEBegin();
	afx_msg void OnKillFocusEditKEEnd();
	afx_msg void OnKillFocusEditHV();
	afx_msg void OnKillFocusEditStep();
	afx_msg void OnKillFocusEditN();
	afx_msg void OnKillFocusEditTime();
	afx_msg void OnKillFocusEditComments();
	afx_msg void OnButtonReset();
	afx_msg void OnButtonHVTable();
	afx_msg void OnButtonCommentsEdit();
	afx_msg void OnBnClickedButtonResetAll();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	CString m_BeginTime;
	CString m_EndTime;
	CString m_LastEditTime;
};

#endif // !defined(AFX_DIALOGPARAMREGION_H__078C4E45_871B_11D5_9A4B_008048FD9845__INCLUDED_)
