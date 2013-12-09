// ProtectionSetupDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Spec.h"
#include "ProtectionSetupDlg.h"
#include "ThermoReg.h"

// CProtectionSetupDlg dialog

IMPLEMENT_DYNAMIC(CProtectionSetupDlg, CDialog)

CProtectionSetupDlg::CProtectionSetupDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CProtectionSetupDlg::IDD, pParent)
{

}

CProtectionSetupDlg::~CProtectionSetupDlg()
{
}

void CProtectionSetupDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_MAXCURRENT, m_MaxCurrentEdit);
	DDX_Control(pDX, IDC_EDIT_MAXVOLTAGE, m_MaxVoltageEdit);
	DDX_Control(pDX, IDC_EDIT_MAXPOWER, m_MaxPowerEdit);
	DDX_Control(pDX, IDC_EDIT_LIMITCURRENT, m_LimitCurrentEdit);
}


BEGIN_MESSAGE_MAP(CProtectionSetupDlg, CDialog)
	ON_BN_CLICKED(IDCANCEL, &CProtectionSetupDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &CProtectionSetupDlg::OnBnClickedOk)
	ON_EN_KILLFOCUS(IDC_EDIT_MAXCURRENT, &CProtectionSetupDlg::OnKillfocusEditMaxcurrent)
	ON_EN_KILLFOCUS(IDC_EDIT_MAXVOLTAGE, &CProtectionSetupDlg::OnKillfocusEditMaxvoltage)
	ON_EN_KILLFOCUS(IDC_EDIT_MAXPOWER, &CProtectionSetupDlg::OnKillfocusEditMaxpower)
END_MESSAGE_MAP()


void CProtectionSetupDlg::OnBnClickedCancel()
{
	OnCancel();
}

void CProtectionSetupDlg::OnBnClickedOk()
{
	ParseValues();
	theApp.Ini.ProtectionMaxI = MaxI;
	theApp.Ini.ProtectionMaxU = MaxU;
	theApp.Ini.ProtectionMaxP = MaxP;
	theApp.Ini.LimitationMaxI = LimitI;

	theApp.Ini.Save();
	CThermoReg::LoadProtectionVals();
	OnOK();
}

void CProtectionSetupDlg::OnKillfocusEditMaxcurrent()
{
	CString str;
	ParseValues();
	str.Format("%.1f", MaxI);
	m_MaxCurrentEdit.SetWindowText(str);	
}

void CProtectionSetupDlg::OnKillfocusEditMaxvoltage()
{
	CString str;
	ParseValues();
	str.Format("%.1f", MaxU);
	m_MaxVoltageEdit.SetWindowText(str);
}

void CProtectionSetupDlg::OnKillfocusEditMaxpower()
{
	CString str;
	ParseValues();
	str.Format("%.0f", MaxP);
	m_MaxPowerEdit.SetWindowText(str);	
}

BOOL CProtectionSetupDlg::OnInitDialog()
{
	BOOL ret = CDialog::OnInitDialog();

	CString str;
	str.Format("%.1f", theApp.Ini.ProtectionMaxI);
	m_MaxCurrentEdit.SetWindowText(str);

	str.Format("%.1f", theApp.Ini.ProtectionMaxU);
	m_MaxVoltageEdit.SetWindowText(str);

	str.Format("%.0f", theApp.Ini.ProtectionMaxP);
	m_MaxPowerEdit.SetWindowText(str);

	str.Format("%.1f", theApp.Ini.LimitationMaxI);
	m_LimitCurrentEdit.SetWindowText(str);

	return ret;
}

void CProtectionSetupDlg::ParseValues()
{
	char str[100];
	m_MaxCurrentEdit.GetWindowText(str, 100);
	MaxI = atof(str);

	m_MaxVoltageEdit.GetWindowText(str, 100);
	MaxU = atof(str);

	m_MaxPowerEdit.GetWindowText(str, 100);
	MaxP = atof(str);

	m_LimitCurrentEdit.GetWindowText(str, 100);
	LimitI = atof(str);
}