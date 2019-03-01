// DlgCommentsEdit.cpp : implementation file
//

#include "stdafx.h"
#include "Main.h"
//#include "ProgNew.h"
//#include "DlgCommentsEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgCommentsEdit dialog


CDlgCommentsEdit::CDlgCommentsEdit(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgCommentsEdit::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgCommentsEdit)
	m_strComments = _T("");
	//}}AFX_DATA_INIT
}


void CDlgCommentsEdit::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgCommentsEdit)
	DDX_Text(pDX, IDC_EDIT, m_strComments);
	DDV_MaxChars(pDX, m_strComments, 255);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgCommentsEdit, CDialog)
	//{{AFX_MSG_MAP(CDlgCommentsEdit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgCommentsEdit message handlers

BOOL CDlgCommentsEdit::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	this->m_strComments = this->m_pDlgParam->m_Comments;
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
