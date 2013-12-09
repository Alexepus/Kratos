// FiTableDlg.cpp : implementation file
//

#include "stdafx.h"
#include "main.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CFiTableDlg dialog

CFiTableDlg::CFiTableDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFiTableDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFiTableDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CFiTableDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFiTableDlg)
		DDX_Control(pDX, IDC_FI_LIST, m_list);
	//}}AFX_DATA_MAP
}

BOOL CFiTableDlg::OnInitDialog()
{
	
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
//	SetIcon(m_hIcon, TRUE);			// Set big icon
//	SetIcon(m_hIcon, FALSE);		// Set small icon

///////////////////////////////////////////////////////////////////////////////////////
//
//	Цвет для фона

	m_list.d1=RGB(255,255,255);
	m_list.d2=RGB(255,255,255);
//	Цвет для текста
	m_list.m_text1=RGB(0,0,0);
	m_list.m_text2=RGB(0,0,0);

	m_list.SetStyle(LVS_REPORT | LVS_OWNERDRAWFIXED |LVS_EDITLABELS    );
	::SendMessage(m_list.m_hWnd, LVM_SETEXTENDEDLISTVIEWSTYLE, 0,LVS_EX_GRIDLINES);
	long style = GetWindowLong(m_list.m_hWnd,GWL_STYLE);
	SetWindowLong(m_list.m_hWnd,GWL_STYLE, style|LVS_SINGLESEL);

// Добавили две колонки
	m_list.InsertColumn(0,_T("HV"),LVCFMT_LEFT,72);
	m_list.InsertColumn(1,_T("Fi"),LVCFMT_LEFT,95);
	
	char str[100];
// Добавили записей
	for (int i=0; i<256; i++)
	{
		if(i<m_pFiTable->GetNValues())
		{
			sprintf(str,"%i",m_pFiTable->GetHV(i));
			m_list.InsertItem(i,str);
			sprintf(str,"%g",m_pFiTable->GetFi(i));
			m_list.SetItemText(i,1,str);
		}
		else
		{
			m_list.InsertItem(i,"");
			m_list.SetItemText(i,1,"");
		}
	}

	m_list.ColumnSort=0;
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

BEGIN_MESSAGE_MAP(CFiTableDlg, CDialog)
	//{{AFX_MSG_MAP(CFiTableDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFiTableDlg message handlers

void CFiTableDlg::OnOK() 
{
	CString str0, str1;
	m_pFiTable->NValues=0;
for (int i=0; i<256; i++)
	{
		str0=m_list.GetItemText(i,0);
		str1=m_list.GetItemText(i,1);
		if(!str0.IsEmpty()&&(!str1.IsEmpty()))
			m_pFiTable->Append((float)atof(m_list.GetItemText(i,0)),
			(float)atof(m_list.GetItemText(i,1)));
	}
m_pFiTable->WriteToFile();
CDialog::OnOK();
}


