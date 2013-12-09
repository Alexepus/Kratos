// TermoDlg.h : header file
//

#if !defined(AFX_TERMODLG_H__79D69F46_0D12_11D7_AE0A_008048B920DA__INCLUDED_)
#define AFX_TERMODLG_H__79D69F46_0D12_11D7_AE0A_008048B920DA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CTermoDlg dialog

class CTermoDlg : public CDialog
{
// Construction
public:
	HANDLE hComPort;
	afx_msg void OnOK();
	char UartBuffer[16];
	void UartCommand(BYTE Byte0, BYTE Byte1);
	void UartCommand(int BytesToTransmit);
	CTermoDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CTermoDlg)
	enum { IDD = IDD_TERMO_DIALOG };
	CButton	m_ControlCheckSkip;
	CString	m_EditResponse;
	CString	m_EditCommand3_0;
	CString	m_EditCommand3_1;
	CString	m_EditCommand3_2;
	BOOL	m_CheckScan;
	BOOL	m_CheckSkipProcessing;
	BOOL	m_CheckViewRawData;
	BOOL	m_CheckViewSent;
	BOOL	m_CheckRequestReply;
	CString	m_EditCommand2_0;
	CString	m_EditCommand1_0;
	CString	m_EditCommand2_1;
	CString	m_EditCommand4_0;
	CString	m_EditCommand4_1;
	CString	m_EditCommand4_2;
	CString	m_EditCommand4_3;
	BOOL	m_CheckHex;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTermoDlg)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CTermoDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButtonClean();
	afx_msg void OnButtonCommand3();
	afx_msg void OnCheckScan();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnCheckRaw();
	afx_msg void OnCheckSkipProcessing();
	afx_msg void OnCheckViewSent();
	afx_msg void OnCheckRequestReply();
	afx_msg void OnButtonCommand1();
	afx_msg void OnButtonCommand2();
	afx_msg void OnButtonCommand4();
	afx_msg void OnCheckHex();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TERMODLG_H__79D69F46_0D12_11D7_AE0A_008048B920DA__INCLUDED_)
