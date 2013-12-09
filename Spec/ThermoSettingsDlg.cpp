// ThermoSettingsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Spec.h"
#include "IniFile.h"
#include "ThermoSettingsDlg.h"
#include "HeaterSetupDlg.h"
#include "Shlwapi.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#pragma comment( lib, "Shlwapi.lib" )
/////////////////////////////////////////////////////////////////////////////
// CThermoSettingsDlg dialog


CThermoSettingsDlg::CThermoSettingsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CThermoSettingsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CThermoSettingsDlg)
	//}}AFX_DATA_INIT
}


void CThermoSettingsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CThermoSettingsDlg)
	DDX_Control(pDX, IDC_EDIT_START_DELAY, m_EditStartDelay);
	DDX_Control(pDX, IDC_CHECK_REPEATE, m_CheckRepeat);
	DDX_Control(pDX, IDC_EDIT_COLD_LEAD, m_EditColdEnds);
	DDX_Control(pDX, IDC_EDIT_T2, m_EditTemp2);
	DDX_Control(pDX, IDC_EDIT_T1, m_EditTemp1);
	DDX_Control(pDX, IDC_EDIT_PRIME, m_EditPrimeTemp);
	DDX_Control(pDX, IDC_EDIT_HEATING_RATE, m_EditHeatingRate);
	DDX_Control(pDX, IDC_EDIT_DWELL1, m_EditTime1);
	DDX_Control(pDX, IDC_EDIT_DWELL2, m_EditTime2);
	DDX_Control(pDX, IDC_EDIT_COOLING_RATE, m_EditCoolingRate);
	DDX_Control(pDX, IDC_EDIT_BETA_PLUS, m_EditBetaPlus);
	DDX_Control(pDX, IDC_EDIT_BETA_MINUS, m_EditBetaMinus);
	DDX_Control(pDX, IDC_EDIT_COUPLE_FILE, m_EditCoupleFile);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CThermoSettingsDlg, CDialog)
	//{{AFX_MSG_MAP(CThermoSettingsDlg)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE_COUPLECALIB, OnButtonBrowseCoupleCalib)
	ON_BN_CLICKED(IDC_BUTTON_TR_SAVE, OnButtonSaveFunction)
	ON_BN_CLICKED(IDC_BUTTON_TR_LOAD, OnButtonLoadFunction)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CThermoSettingsDlg message handlers

void CThermoSettingsDlg::OnButtonBrowseCoupleCalib() 
{
	char fullpath[260]="*.*"; char dir[256]; char ext[4];
	CString OldPath=theApp.Ini.ThermoCoupleFile.Mid(0,theApp.Ini.ThermoCoupleFile.ReverseFind('\\'));
	if(PathFileExists(OldPath))
	{
		sprintf(fullpath, (LPCSTR)theApp.Ini.ThermoCoupleFile);
		sprintf(dir, (LPCSTR)OldPath);
	}
	else
		sprintf(dir, "C:\\");
	OPENFILENAME ofn;
	memset(&ofn, 0, sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = theApp.m_pMainWnd->m_hWnd;
	ofn.hInstance = AfxGetInstanceHandle();
	ofn.lpstrFilter = "ThermoCouple file (*.*)\0*.*\0";
	sprintf(ext, "");
	ofn.lpstrDefExt = ext;
	ofn.nFilterIndex = 1;
	ofn.lpstrFile = fullpath; //filename;
	ofn.nMaxFile = sizeof(fullpath);
	ofn.lpstrFileTitle = NULL; //out: file.ext
	//ofn.nMaxFileTitle = sizeof(filename);
	ofn.lpstrInitialDir = dir;
	ofn.lpstrTitle = "Select File...";
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_EXPLORER;

	int Result;
	EnableWindow(FALSE);
	Result = ::GetOpenFileName(&ofn);
	if(Result)
	{
		CString Path=ofn.lpstrFile;
		theApp.Ini.ThermoCoupleFile=ofn.lpstrFile;
		m_EditCoupleFile.SetWindowText((LPCSTR)Path.Mid(Path.ReverseFind('\\')+1));
	}
	EnableWindow();
	SetFocus();
}

BOOL CThermoSettingsDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	CString str;
	m_EditCoupleFile.SetWindowText((LPCSTR)theApp.Ini.ThermoCoupleFile.Mid(theApp.Ini.ThermoCoupleFile.ReverseFind('\\')+1));
	str.Format("%g",theApp.Ini.TRFunc.StartDelay);
	m_EditStartDelay.SetWindowText((LPCSTR)str);
	str.Format("%g",theApp.Ini.TRFunc.PrimeTemp);
	m_EditPrimeTemp.SetWindowText((LPCSTR)str);
	str.Format("%g",theApp.Ini.TRFunc.BetaPlus);
	m_EditBetaPlus.SetWindowText((LPCSTR)str);
	str.Format("%g",theApp.Ini.TRFunc.Temp1);
	m_EditTemp1.SetWindowText((LPCSTR)str);
	str.Format("%g",theApp.Ini.TRFunc.Time1);
	m_EditTime1.SetWindowText((LPCSTR)str);
	str.Format("%g",theApp.Ini.TRFunc.BetaMinus);
	m_EditBetaMinus.SetWindowText((LPCSTR)str);
	str.Format("%g",theApp.Ini.TRFunc.Temp2);
	m_EditTemp2.SetWindowText((LPCSTR)str);
	str.Format("%g",theApp.Ini.TRFunc.Time2);
	m_EditTime2.SetWindowText((LPCSTR)str);
	str.Format("%g",theApp.Ini.TRFunc.HeatingRate);
	m_EditHeatingRate.SetWindowText((LPCSTR)str);
	str.Format("%g",theApp.Ini.TRFunc.CoolingRate);
	m_EditCoolingRate.SetWindowText((LPCSTR)str);
	m_CheckRepeat.SetCheck(theApp.Ini.TRFunc.Repeat);

	str.Format("%g",theApp.Ini.ColdEndsTemp);
	m_EditColdEnds.SetWindowText((LPCSTR)str);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CThermoSettingsDlg::OnButtonSaveFunction() 
{
	char fullpath[260]="*.trf"; char dir[256]; char ext[4];
	if(!theApp.Ini.TRFunctionFile.IsEmpty())
		strcpy(fullpath,(LPCSTR)theApp.Ini.TRFunctionFile);
	sprintf(dir, (LPCSTR)theApp.Ini.TRFunctionFile);
	OPENFILENAME ofn;
	memset(&ofn, 0, sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = theApp.m_pMainWnd->m_hWnd;
	ofn.hInstance = AfxGetInstanceHandle();
	ofn.lpstrFilter = "TR Function file (*.trf)\0*.trf\0All files (*.*)\0*.*\0";
	sprintf(ext, "trf");
	ofn.lpstrDefExt = ext;
	ofn.nFilterIndex = 1;
	ofn.lpstrFile = fullpath; //filename;
	ofn.nMaxFile = sizeof(fullpath);
	ofn.lpstrFileTitle = NULL; //out: file.ext
	//ofn.nMaxFileTitle = sizeof(filename);
	ofn.lpstrInitialDir = dir;
	ofn.lpstrTitle = "Save...";
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_EXPLORER;

	int Result;
	EnableWindow(FALSE);
	Result = ::GetSaveFileName(&ofn);
	if(Result)
	{
		theApp.Ini.TRFunctionFile=ofn.lpstrFile;
		CString str;
		CIniFile Ini(ofn.lpstrFile);

		m_EditStartDelay.GetWindowText(str);
		Ini.SetValueF("ThermoRegulator Function","StartDelay",atof((LPCSTR)str));
		m_EditPrimeTemp.GetWindowText(str);
		Ini.SetValueF("ThermoRegulator Function","PrimeTemp",atof((LPCSTR)str));
		m_EditBetaPlus.GetWindowText(str);
		Ini.SetValueF("ThermoRegulator Function","BetaPlus",atof((LPCSTR)str));
		m_EditTemp1.GetWindowText(str);
		Ini.SetValueF("ThermoRegulator Function","Temp1",atof((LPCSTR)str));
		m_EditTime1.GetWindowText(str);
		Ini.SetValueF("ThermoRegulator Function","Time1",atof((LPCSTR)str));
		m_EditBetaMinus.GetWindowText(str);
		Ini.SetValueF("ThermoRegulator Function","BetaMinus",atof((LPCSTR)str));
		m_EditTemp2.GetWindowText(str);
		Ini.SetValueF("ThermoRegulator Function","Temp2",atof((LPCSTR)str));
		m_EditTime2.GetWindowText(str);
		Ini.SetValueF("ThermoRegulator Function","Time2",atof((LPCSTR)str));
		m_EditHeatingRate.GetWindowText(str);
		Ini.SetValueF("ThermoRegulator Function","HeatingRate",atof((LPCSTR)str));
		m_EditCoolingRate.GetWindowText(str);
		Ini.SetValueF("ThermoRegulator Function","CoolingRate",atof((LPCSTR)str));
		Ini.SetValueI("ThermoRegulator Function","Repeat",m_CheckRepeat.GetCheck());

		Ini.WriteFile();
	}
	EnableWindow();
	SetFocus();
}

void CThermoSettingsDlg::OnButtonLoadFunction() 
{
	char fullpath[260]="*.trf"; char dir[256]; char ext[4];
	if(!theApp.Ini.TRFunctionFile.IsEmpty())
		strcpy(fullpath,(LPCSTR)theApp.Ini.TRFunctionFile);
	sprintf(dir, (LPCSTR)theApp.Ini.TRFunctionFile);
	OPENFILENAME ofn;
	memset(&ofn, 0, sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = theApp.m_pMainWnd->m_hWnd;
	ofn.hInstance = AfxGetInstanceHandle();
	ofn.lpstrFilter = "TR Function file (*.trf)\0*.trf\0All files (*.*)\0*.*\0";
	sprintf(ext, "trf");
	ofn.lpstrDefExt = ext;
	ofn.nFilterIndex = 1;
	ofn.lpstrFile = fullpath; //filename;
	ofn.nMaxFile = sizeof(fullpath);
	ofn.lpstrFileTitle = NULL; //out: file.ext
	//ofn.nMaxFileTitle = sizeof(filename);
	ofn.lpstrInitialDir = dir;
	ofn.lpstrTitle = "Load...";
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_EXPLORER;

	int Result;
	EnableWindow(FALSE);
	Result = ::GetOpenFileName(&ofn);
	if(Result)
	{
		theApp.Ini.TRFunctionFile=ofn.lpstrFile;
		CString str;
		CIniFile Ini(ofn.lpstrFile);
		if(Ini.ReadFile())
		{
			str.Format("%g",Ini.GetValueF("ThermoRegulator Function","StartDelay",theApp.Ini.TRFunc.PrimeTemp));
			m_EditStartDelay.SetWindowText((LPCSTR)str);
			str.Format("%g",Ini.GetValueF("ThermoRegulator Function","PrimeTemp",theApp.Ini.TRFunc.PrimeTemp));
			m_EditPrimeTemp.SetWindowText((LPCSTR)str);
			str.Format("%g",Ini.GetValueF("ThermoRegulator Function","BetaPlus",theApp.Ini.TRFunc.BetaPlus));
			m_EditBetaPlus.SetWindowText((LPCSTR)str);
			str.Format("%g",Ini.GetValueF("ThermoRegulator Function","Temp1",theApp.Ini.TRFunc.Temp1));
			m_EditTemp1.SetWindowText((LPCSTR)str);
			str.Format("%g",Ini.GetValueF("ThermoRegulator Function","Time1",theApp.Ini.TRFunc.Time1));
			m_EditTime1.SetWindowText((LPCSTR)str);
			str.Format("%g",Ini.GetValueF("ThermoRegulator Function","BetaMinus",theApp.Ini.TRFunc.BetaMinus));
			m_EditBetaMinus.SetWindowText((LPCSTR)str);
			str.Format("%g",Ini.GetValueF("ThermoRegulator Function","Temp2",theApp.Ini.TRFunc.Temp2));
			m_EditTemp2.SetWindowText((LPCSTR)str);
			str.Format("%g",Ini.GetValueF("ThermoRegulator Function","Time2",theApp.Ini.TRFunc.Time2));
			m_EditTime2.SetWindowText((LPCSTR)str);
			str.Format("%g",Ini.GetValueF("ThermoRegulator Function","HeatingRate",theApp.Ini.TRFunc.HeatingRate));
			m_EditHeatingRate.SetWindowText((LPCSTR)str);
			str.Format("%g",Ini.GetValueF("ThermoRegulator Function","CoolingRate",theApp.Ini.TRFunc.CoolingRate));
			m_EditCoolingRate.SetWindowText((LPCSTR)str);
			m_CheckRepeat.SetCheck(Ini.GetValueI("ThermoRegulator Function", "Repeat",theApp.Ini.TRFunc.Repeat));
	
		}
	}
	EnableWindow();
	SetFocus();
}

void CThermoSettingsDlg::OnOK()
{
	CString str;
	m_EditStartDelay.GetWindowText(str);
	theApp.Ini.TRFunc.StartDelay=atof((LPCSTR)str);
	m_EditPrimeTemp.GetWindowText(str);
	theApp.Ini.TRFunc.PrimeTemp=atof((LPCSTR)str);
	m_EditBetaPlus.GetWindowText(str);
	theApp.Ini.TRFunc.BetaPlus=atof((LPCSTR)str);
	m_EditTemp1.GetWindowText(str);
	theApp.Ini.TRFunc.Temp1=atof((LPCSTR)str);
	m_EditTime1.GetWindowText(str);
	theApp.Ini.TRFunc.Time1=atof((LPCSTR)str);
	m_EditBetaMinus.GetWindowText(str);
	theApp.Ini.TRFunc.BetaMinus=atof((LPCSTR)str);
	m_EditTemp2.GetWindowText(str);
	theApp.Ini.TRFunc.Temp2=atof((LPCSTR)str);
	m_EditTime2.GetWindowText(str);
	theApp.Ini.TRFunc.Time2=atof((LPCSTR)str);
	m_EditHeatingRate.GetWindowText(str);
	theApp.Ini.TRFunc.HeatingRate=atof((LPCSTR)str);
	m_EditCoolingRate.GetWindowText(str);
	theApp.Ini.TRFunc.CoolingRate=atof((LPCSTR)str);
	theApp.Ini.TRFunc.Repeat=m_CheckRepeat.GetCheck();

	m_EditColdEnds.GetWindowText(str);
	theApp.Ini.ColdEndsTemp=atof((LPCSTR)str);
	theApp.Ini.Save();

	CDialog::OnOK();
}
