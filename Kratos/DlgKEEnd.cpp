// DlgKEEnd.cpp : implementation file
//
/*
#include "stdafx.h"
#include "ProgNew.h"
#include "DlgKEEnd.h"
*/
#include "stdafx.h"
#include "Main.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgKEEnd dialog


CDlgKEEnd::CDlgKEEnd(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgKEEnd::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgKEEnd)
	m_Radio = 0;
	m_StaticBig = _T("");
	m_StaticSmall = _T("");
	m_StaticCaption = _T("");
	m_StaticCaptionBig = _T("");
	m_StaticCaptionSmall = _T("");
	//}}AFX_DATA_INIT
}


void CDlgKEEnd::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgKEEnd)
	DDX_Radio(pDX, IDC_RADIO_BIG, m_Radio);
	DDX_Text(pDX, IDC_STATIC_BIG, m_StaticBig);
	DDV_MaxChars(pDX, m_StaticBig, 25);
	DDX_Text(pDX, IDC_STATIC_SMALL, m_StaticSmall);
	DDV_MaxChars(pDX, m_StaticSmall, 25);
	DDX_Text(pDX, IDC_STATIC_CAPTION, m_StaticCaption);
	DDV_MaxChars(pDX, m_StaticCaption, 80);
	DDX_Text(pDX, IDC_STATIC_CAPTION_BIG, m_StaticCaptionBig);
	DDV_MaxChars(pDX, m_StaticCaptionBig, 16);
	DDX_Text(pDX, IDC_STATIC_CAPTION_SMALL, m_StaticCaptionSmall);
	DDV_MaxChars(pDX, m_StaticCaptionSmall, 16);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgKEEnd, CDialog)
	//{{AFX_MSG_MAP(CDlgKEEnd)
	ON_BN_CLICKED(IDC_RADIO_BIG, OnRadioBig)
	ON_BN_CLICKED(IDC_RADIO_OVER, OnRadioOver)
	ON_BN_CLICKED(IDC_RADIO_SMALL, OnRadioSmall)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgKEEnd message handlers

void CDlgKEEnd::OnRadioBig() 
{
	// TODO: Add your control notification handler code here
	m_Radio = 0;
}

void CDlgKEEnd::OnRadioOver() 
{
	// TODO: Add your control notification handler code here
	m_Radio = 2;
}

void CDlgKEEnd::OnRadioSmall() 
{
	// TODO: Add your control notification handler code here
	m_Radio = 1;
}

BOOL CDlgKEEnd::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_Radio = 0;
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
