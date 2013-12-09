#if !defined(AFX_DXPSPARAMDLG_H__2360A35A_6D50_4528_B8D4_60DA5A6DC502__INCLUDED_)
#define AFX_DXPSPARAMDLG_H__2360A35A_6D50_4528_B8D4_60DA5A6DC502__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DxpsParamDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDxpsParamDlg dialog

class CDxpsParamDlg : public CDialog
{
// Construction
public:
	void EnableDisableControls();
	BOOL CheckBE();
	BOOL m_NewRegion;
	int m_RegionID;
	CWnd* m_pParentWindow;
	DxpsRegPar* pParameters;
	CDxpsRegion *m_pReg;
	CDxpsParamDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDxpsParamDlg)
	enum { IDD = IDD_DIALOG_PARAM_DXPS };
	CComboBox	m_ComboDivisor;
	CComboBox	m_ComboDivident;
	CComboBox	m_ComboHV;
	CButton	m_ButtonResetReg;
	CButton	m_ButtonResetAll;
	CComboBox	m_ComboColor;
	CComboBox	m_ComboAnode;
	int		m_TypeBE;
	CString	m_Comments;
	double	m_Dwell;
	int	m_HV;
	int		m_SwitchOn;
	double	m_BE;
	double	m_Delay;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDxpsParamDlg)
	public:
	virtual int DoModal();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	BOOL CheckDividentDivisor();
	BOOL CheckHV();

	// Generated message map functions
	//{{AFX_MSG(CDxpsParamDlg)
	afx_msg void OnRadioDxpsParTypeBE();
	afx_msg void OnRadioDxpsParTypeDiv();
	afx_msg void OnButtonDxpsParHvTable();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg void OnButtonResetReg();
	afx_msg void OnButtonResetAll();
	afx_msg void OnKillfocusEditHv();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DXPSPARAMDLG_H__2360A35A_6D50_4528_B8D4_60DA5A6DC502__INCLUDED_)
