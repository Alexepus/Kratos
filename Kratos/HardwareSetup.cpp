// HardwareSetup.cpp : implementation file
//

#include "stdafx.h"
#include "main.h"
#include "HardwareSetup.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CProgNewApp theApp;
/////////////////////////////////////////////////////////////////////////////
// CHardwareSetup dialog


CHardwareSetupDlg::CHardwareSetupDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CHardwareSetupDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CHardwareSetup)
	m_RetardCalibration = 0.0;
	//}}AFX_DATA_INIT
}


void CHardwareSetupDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHardwareSetupDlg)
	DDX_Text(pDX, IDC_EDIT_RETARD_CALIBRATION, m_RetardCalibration);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CHardwareSetupDlg, CDialog)
	//{{AFX_MSG_MAP(CHardwareSetupDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHardwareSetup message handlers

BOOL CHardwareSetupDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_RetardCalibration=m_pThreadCom->RetardCalibration;
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CHardwareSetupDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData();
	m_pThreadCom->RetardCalibration=m_RetardCalibration;
	theApp.Ini.RetardCalibration.Value=m_RetardCalibration;
	theApp.Ini.RetardCalibration.Write();
	CDialog::OnOK();
}
