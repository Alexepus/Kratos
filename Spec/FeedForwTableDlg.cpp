// FeedForwTableDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Spec.h"
#include "Functions.h"
#include "FeedForwTableDlg.h"
#include "ThermoReg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFeedForwTableDlg dialog


CFeedForwTableDlg::CFeedForwTableDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFeedForwTableDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFeedForwTableDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CFeedForwTableDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFeedForwTableDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_LIST_FF, m_List);

}


BEGIN_MESSAGE_MAP(CFeedForwTableDlg, CDialog)
	//{{AFX_MSG_MAP(CFeedForwTableDlg)
	ON_BN_CLICKED(IDC_BUTTON_LOAD_FF, OnButtonLoadFf)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_EDIT_COMMITTED, OnListEditCommited)

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFeedForwTableDlg message handlers

BOOL CFeedForwTableDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_List.SetExtendedStyle(LVS_EX_GRIDLINES);
	m_List.InsertColumn(0,"Temperature, C", LVCFMT_LEFT, 125);
	m_List.InsertColumn(1,"Value", LVCFMT_LEFT, 125);
	CString str;
	for(int i=0;i<4;i++)
	{
		str.Format("%.2f",theApp.Ini.FeedForwTemp[i]);
		m_List.InsertItem(i,(LPCSTR)str);
		str.Format("%.2f",theApp.Ini.FeedForwOut[i]);
		m_List.SetItemText(i,1,(LPCSTR)str);
	}
	m_List.SetEditable(TRUE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CFeedForwTableDlg::OnOK()
{
	SaveData();
	CDialog::OnOK();
}

void CFeedForwTableDlg::SaveData()
{
	m_List.EndEdit(TRUE);
	for(int i=0;i<4;i++)
	{
		theApp.Ini.FeedForwTemp[i]=atof((LPCSTR)m_List.GetItemText(i,0));
		theApp.Ini.FeedForwOut[i]=atof((LPCSTR)m_List.GetItemText(i,1));
	}
}



void CFeedForwTableDlg::OnButtonLoadFf() 
{
	SaveData();
	CThermoReg::UploadFfTable();
}

LRESULT CFeedForwTableDlg::OnListEditCommited(WPARAM WParam, LPARAM LParam)
{
	int ItemIndex=WParam, ColumnIndex=LParam;
	CString str;
	str.Format("%.2f",CorrectInt16Q4(atof((LPCSTR)m_List.GetItemText(ItemIndex,ColumnIndex))));
	m_List.SetItemText(ItemIndex,ColumnIndex,(LPCSTR)str);
	return 0;
}