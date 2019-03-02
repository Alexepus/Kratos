#pragma once
#include "afxwin.h"
#include <memory>

// CSerialCounterDlg dialog
class SerialCounterUnit;

class CSerialCounterDlg : public CDialog
{
	DECLARE_DYNAMIC(CSerialCounterDlg)

public:
	CSerialCounterDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSerialCounterDlg();
	virtual BOOL OnInitDialog();

// Dialog Data
	enum { IDD = IDD_DLG_COUNTER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()

private:
	std::unique_ptr <SerialCounterUnit> m_counterUnit;
	bool m_firstActivating;
	bool m_portChanged;
	bool m_disableMsgBox;

	LRESULT OnPostCreateWindow(WPARAM WParam, LPARAM LParam);
	afx_msg void OnChangeComPort();
	CEdit EditComPort;
	CButton CheckModuleAvailable;
	CButton CheckRunMode;
	CButton CheckCoolingOk;
	CEdit EditCountTime;
	CEdit EditDetectThreshold;
	CEdit EditCoolingThreshold;
	CButton ButtonApply;
	CStatic StaticModuleName;
	CStatic StaticPortAvailable;
	CStatic StaticCurCountTime;
	CStatic StaticCurDetectThreshold;
	CStatic StaticCurInterlockThreshold;
	CStatic StaticCurCoolingFreq;

	afx_msg void OnChangeCountTime();
	afx_msg void OnChangeDetectThreshold();
	afx_msg void OnChangeCoolingThreshold();
	afx_msg void OnKillfocusCountTime();
	afx_msg void OnKillfocusDetectThreshold();
	afx_msg void OnKillfocusCoolingThreshold();
	afx_msg void OnButtonApplyClicked();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	void CheckUnitState();
	int Coerce(int val, int minVal, int maxVal);
	void Coerce(CEdit &edit, int minVal, int maxVal, int factor =1);
	void CheckDetectThreshold();
	void CheckCoolingParams();
	void CheckCountTime();
public:
	afx_msg void OnBnClickedOk();
	CButton CheckModuleConfigured;
};
