// InspectDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Spec.h"
#include "InspectDlg.h"
#include "MassCalibDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInspectDlg dialog


CInspectDlg::CInspectDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CInspectDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CInspectDlg)
	m_EditA = _T("");
	m_EditB = _T("");
	m_EditC = _T("");
	//}}AFX_DATA_INIT
}


void CInspectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInspectDlg)
	DDX_Text(pDX, IDC_EDIT_INSPECT_A, m_EditA);
	DDX_Text(pDX, IDC_EDIT_INSPECT_B, m_EditB);
	DDX_Text(pDX, IDC_EDIT_INSPECT_C, m_EditC);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CInspectDlg, CDialog)
	//{{AFX_MSG_MAP(CInspectDlg)
	ON_BN_CLICKED(IDC_BUTTON_INSPECT_DEFAULTS, OnButtonInspectDefaults)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInspectDlg message handlers

BOOL CInspectDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	m_pDoc=theApp.m_pMassCalibDoc;
	m_EditA.Format("%.8g",m_pDoc->a);
	m_EditB.Format("%.8g",m_pDoc->b);
	m_EditC.Format("%.8g",m_pDoc->c);
	UpdateData(FALSE);	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CInspectDlg::OnOK() 
{
	UpdateData();
	m_pDoc->a = atof(m_EditA);
	m_pDoc->b = atof(m_EditB);
	m_pDoc->c = atof(m_EditC);
	
	CDialog::OnOK();
}

void CInspectDlg::OnButtonInspectDefaults() 
{
	m_EditA.Format("%.8g", 0.);
	m_EditB.Format("%.8g",5./120.); // 5В <=> масса 120
	m_EditC.Format("%.8g", 0.);
	
	UpdateData(FALSE);
}
