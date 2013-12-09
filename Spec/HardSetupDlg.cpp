// HardSetupDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Spec.h"
#include "HardSetupDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHardSetupDlg dialog


CHardSetupDlg::CHardSetupDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CHardSetupDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CHardSetupDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CHardSetupDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHardSetupDlg)
	DDX_Control(pDX, IDC_EDIT_COM_PORT_NAME, m_EditComPortName);
	DDX_Control(pDX, IDC_COMBO_RANGE, m_ComboRange);
	DDX_Control(pDX, IDC_COMBO_INTEGRATION, m_ComboIntegration);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CHardSetupDlg, CDialog)
	//{{AFX_MSG_MAP(CHardSetupDlg)
	ON_BN_CLICKED(IDC_BUTTON_COM_RECONNECT, OnButtonComReconnect)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHardSetupDlg message handlers

BOOL CHardSetupDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	m_EditComPortName.SetWindowText(theApp.Ini.ComPortName);	
	m_ComboIntegration.SetCurSel(theApp.Ini.AdcIntegration);	
	m_ComboRange.SetCurSel(theApp.Ini.AdcRange);	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CHardSetupDlg::OnOK()
{
	theApp.Ini.AdcIntegration=m_ComboIntegration.GetCurSel();	
	theApp.Ini.AdcRange=m_ComboRange.GetCurSel();	
	char ch[30];
	m_EditComPortName.GetWindowText(ch,sizeof(ch));
	theApp.Ini.ComPortName=ch;
	theApp.Ini.Save();
	CDialog::OnOK();
}

void CHardSetupDlg::OnButtonComReconnect() 
{
	char ch[20];
	m_EditComPortName.GetWindowText(ch,sizeof(ch));
	theApp.Ini.ComPortName=ch;
	theApp.m_SerialLink.ReconnectCom(ch);	
}
