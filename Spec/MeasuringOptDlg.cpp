// MeasuringOptDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Spec.h"
#include "MeasuringOptDlg.h"
#include "MainFrame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CSpecApp theApp;
/////////////////////////////////////////////////////////////////////////////
// CMeasuringOptDlg dialog


CMeasuringOptDlg::CMeasuringOptDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMeasuringOptDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMeasuringOptDlg)
	m_MassSetupDelay = _T("");
	m_MassStart = _T("");
	m_MassStop = _T("");
	m_MassStep = _T("");
	m_ScanTime = 0;
	m_CalibMassSetupDelay = _T("");
	//}}AFX_DATA_INIT
}


void CMeasuringOptDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMeasuringOptDlg)
	DDX_Text(pDX, IDC_EDIT_MASS_SETUP_DELAY, m_MassSetupDelay);
	DDX_Text(pDX, IDC_EDIT_MASS_START, m_MassStart);
	DDX_Text(pDX, IDC_EDIT_MASS_STOP, m_MassStop);
	DDX_Text(pDX, IDC_EDIT_MASS_STEP, m_MassStep);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER, m_ScanTime);
	DDX_Text(pDX, IDC_EDIT_CALIB_MASS_SETUP_DELAY, m_CalibMassSetupDelay);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMeasuringOptDlg, CDialog)
	//{{AFX_MSG_MAP(CMeasuringOptDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMeasuringOptDlg message handlers

BOOL CMeasuringOptDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	m_MassStart.Format("%.0f", theApp.Ini.CalibMassStart);
	m_MassStop.Format("%.0f", theApp.Ini.CalibMassStop);
	m_MassStep.Format("%.2f", theApp.Ini.CalibMassStep);
	m_MassSetupDelay.Format("%.3f", theApp.Ini.MassSetupDelay);
	m_CalibMassSetupDelay.Format("%.3f", theApp.Ini.CalibMassSetupDelay);
	CMassSpecDoc* pDoc=(CMassSpecDoc*) ((CMainFrame*)theApp.GetMainWnd())->GetActiveDocument();

	int Sec,Min,Hour;
	Sec=pDoc->m_ScanTime/1000%60;
	Min=pDoc->m_ScanTime/1000/60%60;
	Hour=pDoc->m_ScanTime/1000/3600;
	m_ScanTime=CTime(2000,1,1,Hour,Min,Sec,0);
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CMeasuringOptDlg::OnOK()
{
	UpdateData();
	theApp.Ini.CalibMassStart=atof((LPCSTR)m_MassStart);
	theApp.Ini.CalibMassStop=atof((LPCSTR)m_MassStop);
	theApp.Ini.CalibMassStep=atof((LPCSTR)m_MassStep);
	theApp.Ini.MassSetupDelay=atof((LPCSTR)m_MassSetupDelay);
	theApp.Ini.CalibMassSetupDelay=atof((LPCSTR)m_CalibMassSetupDelay);
    CMassSpecDoc* pDoc=(CMassSpecDoc*)((CMainFrame*)theApp.GetMainWnd())->GetActiveDocument();
	pDoc->m_ScanTime=1000*(m_ScanTime.GetSecond()+m_ScanTime.GetMinute()*60+m_ScanTime.GetHour()*3600);
	theApp.Ini.Save();
	CDialog::OnOK();
}
