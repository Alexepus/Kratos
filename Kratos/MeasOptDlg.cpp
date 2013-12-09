// MeasOptDlg.cpp : implementation file
//

#include "stdafx.h"
#include "main.h"
#include "MeasOptDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMeasOptDlg dialog


CMeasOptDlg::CMeasOptDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMeasOptDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMeasOptDlg)
	m_NSigma = _T("");
	m_Attempts = _T("");
	m_Submeasurings = _T("");
	m_CheckRemeasure = FALSE;
	//}}AFX_DATA_INIT
}


void CMeasOptDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMeasOptDlg)
	DDX_Text(pDX, IDC_STATIC_NSIGMA, m_NSigma);
	DDX_Text(pDX, IDC_STATIC_ATTEMPTS, m_Attempts);
	DDX_Text(pDX, IDC_STATIC_SUBMEASURINGS, m_Submeasurings);
	DDX_Control(pDX, IDC_SLIDER_NSIGMA, SliderNSigma);
	DDX_Control(pDX, IDC_SLIDER_NATTEMPTS, SliderAttempts);
	DDX_Control(pDX, IDC_SLIDER_NSUBMEASURINGS, SliderSubmeasurings);
	DDX_Check(pDX, IDC_CHECK_REMEASURE, m_CheckRemeasure);
	//}}AFX_DATA_MAP
}

BOOL CMeasOptDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
   
	SliderNSigma.SetRangeMin(10,TRUE);
	SliderNSigma.SetRangeMax(300,TRUE);
	SliderNSigma.SetPos((int)(m_pThreadCom->NSigma*10.));
	SliderNSigma.SetTicFreq(10);

	SliderAttempts.SetRangeMin(1,TRUE);
	SliderAttempts.SetRangeMax(10,TRUE);
	SliderAttempts.SetPos(m_pThreadCom->Attempts);

	SliderSubmeasurings.SetRangeMin(1,TRUE);
	SliderSubmeasurings.SetRangeMax(10,TRUE);
	SliderSubmeasurings.SetPos(m_pThreadCom->SubMeasurings);

	m_CheckRemeasure=m_pThreadCom->EnableRemeasure;
	UpdateData(FALSE);
	OnCheckRemeasure();
	return TRUE;
}


BEGIN_MESSAGE_MAP(CMeasOptDlg, CDialog)
	//{{AFX_MSG_MAP(CMeasOptDlg)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_NSIGMA, OnCustomdrawSliderNsigma)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_NATTEMPTS, OnCustomdrawSliderNattempts)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_NSUBMEASURINGS, OnCustomdrawSliderNsubmeasurings)
	ON_BN_CLICKED(IDC_CHECK_REMEASURE, OnCheckRemeasure)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMeasOptDlg message handlers

void CMeasOptDlg::OnCustomdrawSliderNsigma(NMHDR* pNMHDR, LRESULT* pResult) 
{
	int NSigma=SliderNSigma.GetPos();
	m_NSigma.Format("%4.1f", NSigma/10.);
	UpdateData(FALSE);
	*pResult = 0;
}

void CMeasOptDlg::OnCustomdrawSliderNattempts(NMHDR* pNMHDR, LRESULT* pResult) 
{
	int Attempts=SliderAttempts.GetPos();
	m_Attempts.Format("%i", Attempts);
	UpdateData(FALSE);
	
	*pResult = 0;
}

void CMeasOptDlg::OnCustomdrawSliderNsubmeasurings(NMHDR* pNMHDR, LRESULT* pResult) 
{
	int Submeasurings=SliderSubmeasurings.GetPos();
	m_Submeasurings.Format("%i", Submeasurings);
	UpdateData(FALSE);
	
	*pResult = 0;
}


void CMeasOptDlg::OnOK() 
{
	UpdateData(TRUE);
	m_pThreadCom->NSigma=SliderNSigma.GetPos()/10.;
	m_pThreadCom->Attempts=SliderAttempts.GetPos();
	m_pThreadCom->SubMeasurings=SliderSubmeasurings.GetPos();
	m_pThreadCom->EnableRemeasure=m_CheckRemeasure;

	CWinApp* App=AfxGetApp();
	App->WriteProfileInt("Measuring Options", "NSigma", SliderNSigma.GetPos());
	App->WriteProfileInt("Measuring Options", "Attempts", SliderAttempts.GetPos());
	App->WriteProfileInt("Measuring Options", "SubMeasurings", SliderSubmeasurings.GetPos());
	App->WriteProfileInt("Measuring Options", "EnableRemeasure", m_CheckRemeasure);

	CDialog::OnOK();
}

void CMeasOptDlg::OnCheckRemeasure() 
{
	UpdateData(TRUE);
	SliderNSigma.EnableWindow(m_CheckRemeasure);
	SliderAttempts.EnableWindow(m_CheckRemeasure);
	GetDlgItem(IDC_STATIC_NSIGMA)->EnableWindow(m_CheckRemeasure);
	GetDlgItem(IDC_STATIC_NSIGMA_MIN)->EnableWindow(m_CheckRemeasure);
	GetDlgItem(IDC_STATIC_NSIGMA_MAX)->EnableWindow(m_CheckRemeasure);
	GetDlgItem(IDC_GROUP_NSIGMA)->EnableWindow(m_CheckRemeasure);
	GetDlgItem(IDC_STATIC_ATTEMPTS)->EnableWindow(m_CheckRemeasure);
	GetDlgItem(IDC_STATIC_ATTEMPTS_MIN)->EnableWindow(m_CheckRemeasure);
	GetDlgItem(IDC_STATIC_ATTEMPTS_MAX)->EnableWindow(m_CheckRemeasure);
	GetDlgItem(IDC_GROUP_ATTEMPTS)->EnableWindow(m_CheckRemeasure);
}
