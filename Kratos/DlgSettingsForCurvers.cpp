// DlgSettingsForCurvers.cpp : implementation file
//

#include "stdafx.h"
#include "Main.h"

//#include "ProgNew.h"
//#include "DlgSettingsForCurvers.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgSettingsForCurvers dialog
//class CMainFrame;

CDlgSettingsForCurvers::CDlgSettingsForCurvers(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSettingsForCurvers::IDD, pParent)
{
/*	
	//{{AFX_DATA_INIT(CDlgSettingsForCurvers)
	m_CheckCurrLine = FALSE;
	m_CheckCurrPoints = FALSE;
	m_CheckResLine = FALSE;
	m_CheckResPoints = FALSE;
	m_CheckGrid = FALSE;
	m_CheckGuideLines = FALSE;
	//}}AFX_DATA_INIT
//	m_CurrSizePoints = 2;
*/

/*
m_CheckCurrLine = m_pMainFrame->m_Doc.m_Graph.m_LineShort;
m_CheckCurrPoints = m_pMainFrame->m_Doc.m_Graph.m_PointsShort;
m_SizePointsCurr = m_pMainFrame->m_Doc.m_Graph.m_SizePointsShort;

m_CheckResLine = m_pMainFrame->m_Doc.m_Graph.m_LineAll;
m_CheckResPoints = m_pMainFrame->m_Doc.m_Graph.m_PointsAll;
m_SizePointsRes = m_pMainFrame->m_Doc.m_Graph.m_SizePointsAll;
*/


//m_CheckGrid = TRUE;
//if(m_CheckCurrLine) AfxMessageBox("m_CheckCurrLine==TRUE");
//else AfxMessageBox("m_CheckCurrLine==FALSE");
}


void CDlgSettingsForCurvers::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSettingsForCurvers)
	DDX_Check(pDX, IDC_CHECK_CURR_LINE, m_CheckCurrLine);
	DDX_Check(pDX, IDC_CHECK_CURR_POINTS, m_CheckCurrPoints);
	DDX_Check(pDX, IDC_CHECK_RES_LINE, m_CheckResLine);
	DDX_Check(pDX, IDC_CHECK_RES_POINTS, m_CheckResPoints);
	DDX_Check(pDX, IDC_CHECK_GRID, m_CheckGrid);
	DDX_Check(pDX, IDC_CHECK_GUIDE_LINES, m_CheckGuideLines);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgSettingsForCurvers, CDialog)
	//{{AFX_MSG_MAP(CDlgSettingsForCurvers)
	ON_BN_CLICKED(IDC_CHECK_CURR_LINE, OnCheckCurrLine)
	ON_BN_CLICKED(IDC_BUTTON_CURR_COLOR, OnButtonCurrColor)
	ON_BN_CLICKED(IDC_CHECK_CURR_POINTS, OnCheckCurrPoints)
	ON_WM_PAINT()
	ON_CBN_SELENDOK(IDC_COMBO_SIZE_CURR, OnSelEndOkComboSizeCurr)
	ON_BN_CLICKED(IDC_BUTTON_RES_COLOR, OnButtonResColor)
	ON_BN_CLICKED(IDC_CHECK_RES_LINE, OnCheckResLine)
	ON_BN_CLICKED(IDC_CHECK_RES_POINTS, OnCheckResPoints)
	ON_CBN_SELENDOK(IDC_COMBO_SIZE_RES, OnSelEndOkComboSizeRes)
	ON_BN_CLICKED(IDC_CHECK_GRID, OnCheckGrid)
	ON_BN_CLICKED(IDC_BUTTON_GRID_COLOR, OnButtonGridColor)
	ON_BN_CLICKED(IDC_CHECK_GUIDE_LINES, OnCheckGuideLines)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSettingsForCurvers message handlers


BOOL CDlgSettingsForCurvers::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
m_ColorCurr = m_pMainFrame->m_Doc.m_Graph.m_DataShortColor;
m_ColorRes = m_pMainFrame->m_Doc.m_Graph.m_DataAllColor;
m_ColorGrid = m_pMainFrame->m_Doc.m_Graph.m_GridColor;
	
m_CheckCurrLine = m_pMainFrame->m_Doc.m_Graph.m_LineShort;
m_CheckCurrPoints = m_pMainFrame->m_Doc.m_Graph.m_PointsShort;
m_SizePointsCurr = m_pMainFrame->m_Doc.m_Graph.m_SizePointsShort;

m_CheckResLine = m_pMainFrame->m_Doc.m_Graph.m_LineAll;
m_CheckResPoints = m_pMainFrame->m_Doc.m_Graph.m_PointsAll;
m_SizePointsRes = m_pMainFrame->m_Doc.m_Graph.m_SizePointsAll;

m_CheckGrid = m_pMainFrame->m_Doc.m_Graph.m_Grid;
m_CheckGuideLines = m_pMainFrame->m_Doc.m_Graph.m_GuideLines;


HWND hWndChild = ::GetDlgItem(this->m_hWnd, IDC_CHECK_CURR_LINE);	
if(m_CheckCurrLine) ::SendMessage(hWndChild, BM_SETCHECK, 1, 0);
else ::SendMessage(hWndChild, BM_SETCHECK, 0, 0);
hWndChild = ::GetDlgItem(this->m_hWnd, IDC_CHECK_CURR_POINTS);	
if(m_CheckCurrPoints) ::SendMessage(hWndChild, BM_SETCHECK, 1, 0);
else ::SendMessage(hWndChild, BM_SETCHECK, 0, 0);

hWndChild = ::GetDlgItem(this->m_hWnd, IDC_CHECK_RES_LINE);	
if(m_CheckResLine) ::SendMessage(hWndChild, BM_SETCHECK, 1, 0);
else ::SendMessage(hWndChild, BM_SETCHECK, 0, 0);
hWndChild = ::GetDlgItem(this->m_hWnd, IDC_CHECK_RES_POINTS);	
if(m_CheckResPoints) ::SendMessage(hWndChild, BM_SETCHECK, 1, 0);
else ::SendMessage(hWndChild, BM_SETCHECK, 0, 0);

hWndChild = ::GetDlgItem(this->m_hWnd, IDC_CHECK_GRID);	
if(m_CheckGrid) ::SendMessage(hWndChild, BM_SETCHECK, 1, 0);
else ::SendMessage(hWndChild, BM_SETCHECK, 0, 0);

	
	int i;
	char strCombo[16];

	//m_CheckCurrLine = TRUE;	
	
	
	// CURRENT CURVE
	hWndChild = ::GetDlgItem(this->m_hWnd, IDC_EDIT_CURR_SAMPLE);
	RECT r;
	::GetClientRect(hWndChild, &r);
	m_hWndSimpleCurr = ::CreateWindow(m_pMainFrame->m_Doc.m_Graph.m_cl_name_for_curve,
		NULL, WS_CHILD|WS_CLIPCHILDREN|WS_VISIBLE,//|WS_HSCROLL|WS_VSCROLL, 
		0, 0, r.right, r.bottom, 
		hWndChild, NULL, AfxGetInstanceHandle(),0);
	UpdateWndSimpleCurr();

	hWndChild = ::GetDlgItem(this->m_hWnd, IDC_COMBO_SIZE_CURR);
	for(i=0; i<19; ++i)
		{
		sprintf(strCombo, "%i", 2+i);
		::SendMessage(hWndChild, CB_ADDSTRING, 0, (LPARAM) strCombo);
		}
	::SendMessage(hWndChild, CB_SETCURSEL , (WPARAM) (m_SizePointsCurr-2), 0);
	//::SendMessage(hWndChild, CB_SETITEMHEIGHT , 0, 100);


	// RESULT CURVE
	hWndChild = ::GetDlgItem(this->m_hWnd, IDC_EDIT_RES_SAMPLE);
	::GetClientRect(hWndChild, &r);
	m_hWndSimpleRes = ::CreateWindow(m_pMainFrame->m_Doc.m_Graph.m_cl_name_for_curve,
		NULL, WS_CHILD|WS_CLIPCHILDREN|WS_VISIBLE,//|WS_HSCROLL|WS_VSCROLL, 
		0, 0, r.right, r.bottom, 
		hWndChild, NULL, AfxGetInstanceHandle(),0);
	UpdateWndSimpleRes();

	hWndChild = ::GetDlgItem(this->m_hWnd, IDC_COMBO_SIZE_RES);
	for(i=0; i<19; ++i)
		{
		sprintf(strCombo, "%i", 2+i);
		::SendMessage(hWndChild, CB_ADDSTRING, 0, (LPARAM) strCombo);
		}
	::SendMessage(hWndChild, CB_SETCURSEL , (WPARAM) (m_SizePointsRes-2), 0);
	
	
	// GRID
	hWndChild = ::GetDlgItem(this->m_hWnd, IDC_EDIT_GRID_SAMPLE);
//	RECT r;
	::GetClientRect(hWndChild, &r);
	m_hWndSimpleGrid = ::CreateWindow(m_pMainFrame->m_Doc.m_Graph.m_cl_name_for_curve,
		NULL, WS_CHILD|WS_CLIPCHILDREN|WS_VISIBLE,//|WS_HSCROLL|WS_VSCROLL, 
		0, 0, r.right, r.bottom, 
		hWndChild, NULL, AfxGetInstanceHandle(),0);
	UpdateWndSimpleGrid();

	//GUIDE LINES
	hWndChild = ::GetDlgItem(this->m_hWnd, IDC_EDIT_CURSOR_SAMPLE);
	::GetClientRect(hWndChild, &r);
	m_hWndSimpleGuideLines = ::CreateWindow(m_pMainFrame->m_Doc.m_Graph.m_cl_name_for_curve,
		NULL, WS_CHILD|WS_CLIPCHILDREN|WS_VISIBLE,//|WS_HSCROLL|WS_VSCROLL, 
		0, 0, r.right, r.bottom, 
		hWndChild, NULL, AfxGetInstanceHandle(),0);
	UpdateWndSimpleGuideLines();
	
	
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgSettingsForCurvers::OnCheckCurrLine() 
{
	// TODO: Add your control notification handler code here
	if(m_CheckCurrLine) m_CheckCurrLine = FALSE;
	else m_CheckCurrLine = TRUE;
	UpdateWndSimpleCurr();
}
//=========
void CDlgSettingsForCurvers::OnCheckResLine() 
{
	// TODO: Add your control notification handler code here
	if(m_CheckResLine) m_CheckResLine = FALSE;
	else m_CheckResLine = TRUE;
	UpdateWndSimpleRes();
}

//========
void CDlgSettingsForCurvers::OnCheckCurrPoints() 
{
	// TODO: Add your control notification handler code here
	//HWND hWndChild;// = ::GetDlgItem(this->m_hWnd, IDC_COMBO_SIZE_CURR);
	if(m_CheckCurrPoints) m_CheckCurrPoints = FALSE;
	else m_CheckCurrPoints = TRUE;
	UpdateWndSimpleCurr();	
}
//=========

void CDlgSettingsForCurvers::OnCheckResPoints() 
{
	// TODO: Add your control notification handler code here
	if(m_CheckResPoints) m_CheckResPoints = FALSE;
	else m_CheckResPoints = TRUE;
	UpdateWndSimpleRes();	
}

//=========
void CDlgSettingsForCurvers::OnSelEndOkComboSizeCurr() 
{
	// TODO: Add your control notification handler code here
	//AfxMessageBox("OnSelEndOkComboSizeCurr() ");
	UINT res;
	HWND hWndChild = ::GetDlgItem(this->m_hWnd, IDC_COMBO_SIZE_CURR);
	res = (UINT) ::SendMessage(hWndChild, CB_GETCURSEL , 0, 0);
	if(res!=CB_ERR) m_SizePointsCurr = res+2;
	UpdateWndSimpleCurr();
}
//===========
void CDlgSettingsForCurvers::OnSelEndOkComboSizeRes() 
{
	// TODO: Add your control notification handler code here
	UINT res;
	HWND hWndChild = ::GetDlgItem(this->m_hWnd, IDC_COMBO_SIZE_RES);
	res = (UINT) ::SendMessage(hWndChild, CB_GETCURSEL , 0, 0);
	if(res!=CB_ERR) m_SizePointsRes = res+2;
	UpdateWndSimpleRes();
}


//===========
void CDlgSettingsForCurvers::UpdateWndSimpleCurr()
{
HWND hWndChild;
HDC DC;
DC = ::GetDC(m_hWndSimpleCurr);
RECT r;
::GetClientRect(m_hWndSimpleCurr, &r);
//LOGBRUSH LogBrush = {BS_SOLID, m_pMainFrame->m_Doc.m_Graph.m_DataShortColor,0};
LOGBRUSH LogBrush = {BS_SOLID, m_ColorCurr, 0};
HPEN NewPen = ::ExtCreatePen(	PS_COSMETIC | PS_SOLID, 1, &LogBrush,0,0);
HPEN OldPen = (HPEN) ::SelectObject(DC, NewPen);
HBRUSH NewBrush = ::CreateBrushIndirect(&LogBrush);
HBRUSH OldBrush = (HBRUSH) ::SelectObject(DC, NewBrush);
//::SetBkColor(DC, m_ColorCurr);

::InvalidateRect(m_hWndSimpleCurr, &r, TRUE);
::UpdateWindow(m_hWndSimpleCurr);
if(m_CheckCurrLine) 
	{ ::MoveToEx(DC, 0, r.bottom, NULL);
		::LineTo(DC, r.right, 0);
	}
if(m_CheckCurrPoints)
	::Ellipse(DC, r.right/2-m_SizePointsCurr/2, r.bottom/2-m_SizePointsCurr/2,
							(int)(r.right/2+m_SizePointsCurr/2.+0.5), 
							(int)(r.bottom/2+m_SizePointsCurr/2.+0.5) );

::SelectObject(DC, OldBrush);
::SelectObject(DC, OldPen);
::DeleteObject(NewBrush);
::DeleteObject(NewPen);
::ReleaseDC(m_hWndSimpleCurr, DC);

if(!m_CheckCurrPoints)
	{
	hWndChild = ::GetDlgItem(this->m_hWnd, IDC_COMBO_SIZE_CURR);
	::EnableWindow(hWndChild, FALSE);
	hWndChild = ::GetDlgItem(this->m_hWnd, IDC_STATIC_CURR);
	::EnableWindow(hWndChild, FALSE);
	}
else
	{
	hWndChild = ::GetDlgItem(this->m_hWnd, IDC_COMBO_SIZE_CURR);
	::EnableWindow(hWndChild, TRUE);
	hWndChild = ::GetDlgItem(this->m_hWnd, IDC_STATIC_CURR);
	::EnableWindow(hWndChild, TRUE);
	}

hWndChild = ::GetDlgItem(this->m_hWnd, IDC_BUTTON_CURR_COLOR);
if(!m_CheckCurrPoints && !m_CheckCurrLine) ::EnableWindow(hWndChild, FALSE);
else ::EnableWindow(hWndChild, TRUE);
}

COLORREF CDlgSettingsForCurvers::WndColorSet(COLORREF rgb)
{
	COLORREF CustColor[16];
	CHOOSECOLOR cc;
	CWinApp* App = AfxGetApp();
	//rgb = (COLORREF) App->GetProfileInt("COLOR","FirstColor",0xff0000);
	char* str[] = {"1","2","3","4","5","6","7","8","9","10","11","12","13","14","15","16"};
	//rgb=RGB(0,0,255);
	int i;
	for(i=0; i<16; ++i) 
		CustColor[i]=(COLORREF) App->GetProfileInt("COLOR",str[i],0xffffff);
	memset(&cc, 0, sizeof(CHOOSECOLOR));
	cc.lStructSize=sizeof(CHOOSECOLOR);
	cc.hwndOwner=m_hWnd;
//	cc.hInstance=AfxGetInstanceHandle();
	cc.lpCustColors=CustColor;
	cc.Flags=CC_RGBINIT|CC_FULLOPEN;
	cc.rgbResult=rgb;
	if(::ChooseColor(&cc)) rgb=cc.rgbResult;

	//App->WriteProfileInt("COLOR","FirstColor",(int) rgb);
	for(i=0; i<16; ++i) 
		App->WriteProfileInt("COLOR",str[i],cc.lpCustColors[i]);

	/*
	m_Doc.m_Graph.m_DataAllColor = (COLORREF) App->GetProfileInt("COLOR","1",0xff0000);
	m_Doc.m_Graph.m_DataShortColor = (COLORREF) App->GetProfileInt("COLOR","2",0xff0000);
	m_Doc.m_Graph.m_GridColor = (COLORREF) App->GetProfileInt("COLOR","3",0xff0000);
//REDRAW
	m_Doc.m_Graph.ReDrawAll();
*/

	return rgb;
}
//==========

void CDlgSettingsForCurvers::OnButtonCurrColor() 
{
	// TODO: Add your control notification handler code here
	m_ColorCurr = WndColorSet(m_ColorCurr);
	UpdateWndSimpleCurr();
}
//==========
void CDlgSettingsForCurvers::OnButtonResColor() 
{
	// TODO: Add your control notification handler code here
	m_ColorRes = WndColorSet(m_ColorRes);
	UpdateWndSimpleRes();
}




void CDlgSettingsForCurvers::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	UpdateWndSimpleCurr();
	UpdateWndSimpleRes();
	UpdateWndSimpleGrid();
	UpdateWndSimpleGuideLines();
	// Do not call CDialog::OnPaint() for painting messages
}



void CDlgSettingsForCurvers::UpdateWndSimpleRes()
{
HWND hWndChild;
HDC DC;
DC = ::GetDC(m_hWndSimpleRes);
RECT r;
::GetClientRect(m_hWndSimpleRes, &r);
//LOGBRUSH LogBrush = {BS_SOLID, m_pMainFrame->m_Doc.m_Graph.m_DataShortColor,0};
LOGBRUSH LogBrush = {BS_SOLID, m_ColorRes, 0};
HPEN NewPen = ::ExtCreatePen(	PS_COSMETIC | PS_SOLID, 1, &LogBrush,0,0);
HPEN OldPen = (HPEN) ::SelectObject(DC, NewPen);
HBRUSH NewBrush = ::CreateBrushIndirect(&LogBrush);
HBRUSH OldBrush = (HBRUSH) ::SelectObject(DC, NewBrush);
//::SetBkColor(DC, m_ColorCurr);

::InvalidateRect(m_hWndSimpleRes, &r, TRUE);
::UpdateWindow(m_hWndSimpleRes);
if(m_CheckResLine) 
	{ ::MoveToEx(DC, 0, r.bottom, NULL);
		::LineTo(DC, r.right, 0);
	}
if(m_CheckResPoints)
	::Ellipse(DC, r.right/2-m_SizePointsRes/2, r.bottom/2-m_SizePointsRes/2,
							(int)(r.right/2+m_SizePointsRes/2.+0.5), 
							(int)(r.bottom/2+m_SizePointsRes/2.+0.5) );

::SelectObject(DC, OldBrush);
::SelectObject(DC, OldPen);
::DeleteObject(NewBrush);
::DeleteObject(NewPen);
::ReleaseDC(m_hWndSimpleRes, DC);

if(!m_CheckResPoints)
	{
	hWndChild = ::GetDlgItem(this->m_hWnd, IDC_COMBO_SIZE_RES);
	::EnableWindow(hWndChild, FALSE);
	hWndChild = ::GetDlgItem(this->m_hWnd, IDC_STATIC_RES);
	::EnableWindow(hWndChild, FALSE);
	}
else
	{
	hWndChild = ::GetDlgItem(this->m_hWnd, IDC_COMBO_SIZE_RES);
	::EnableWindow(hWndChild, TRUE);
	hWndChild = ::GetDlgItem(this->m_hWnd, IDC_STATIC_RES);
	::EnableWindow(hWndChild, TRUE);
	}

hWndChild = ::GetDlgItem(this->m_hWnd, IDC_BUTTON_RES_COLOR);
if(!m_CheckResPoints && !m_CheckResLine) ::EnableWindow(hWndChild, FALSE);
else ::EnableWindow(hWndChild, TRUE);

}





void CDlgSettingsForCurvers::OnCheckGrid() 
{
	// TODO: Add your control notification handler code here
	if(m_CheckGrid) m_CheckGrid = FALSE;
	else m_CheckGrid = TRUE;
	UpdateWndSimpleGrid();
}

void CDlgSettingsForCurvers::UpdateWndSimpleGrid()
{
HWND hWndChild;
HDC DC;
DC = ::GetDC(m_hWndSimpleGrid);
RECT r;
::GetClientRect(m_hWndSimpleGrid, &r);
//LOGBRUSH LogBrush = {BS_SOLID, m_pMainFrame->m_Doc.m_Graph.m_DataShortColor,0};
LOGBRUSH LogBrush = {BS_SOLID, m_ColorGrid, 0};
HPEN NewPen = ::ExtCreatePen(	PS_GEOMETRIC | PS_DOT, 1, &LogBrush,0,0);
HPEN OldPen = (HPEN) ::SelectObject(DC, NewPen);

//HBRUSH NewBrush = ::CreateBrushIndirect(&LogBrush);
//HBRUSH OldBrush = (HBRUSH) ::SelectObject(DC, NewBrush);
::SetBkColor(DC, m_ColorGrid);

::InvalidateRect(m_hWndSimpleGrid, &r, TRUE);
::UpdateWindow(m_hWndSimpleGrid);
if(m_CheckGrid) 
	{ ::MoveToEx(DC, 0, r.bottom/3, NULL);
		::LineTo(DC, r.right, r.bottom/3);
		::MoveToEx(DC, 0, 2*r.bottom/3, NULL);
		::LineTo(DC, r.right, 2*r.bottom/3);
		::MoveToEx(DC, r.right/3, 0, NULL);
		::LineTo(DC, r.right/3, r.bottom);
		::MoveToEx(DC, 2*r.right/3, 0, NULL);
		::LineTo(DC, 2*r.right/3, r.bottom);
	}

::SelectObject(DC, OldPen);
//::DeleteObject(NewBrush);
::DeleteObject(NewPen);
::ReleaseDC(m_hWndSimpleGrid, DC);

hWndChild = ::GetDlgItem(this->m_hWnd, IDC_BUTTON_GRID_COLOR);
if(!m_CheckGrid) ::EnableWindow(hWndChild, FALSE);
else ::EnableWindow(hWndChild, TRUE);

}

void CDlgSettingsForCurvers::OnButtonGridColor() 
{
	// TODO: Add your control notification handler code here
	m_ColorGrid = WndColorSet(m_ColorGrid);
	UpdateWndSimpleGrid();
}

void CDlgSettingsForCurvers::OnCheckGuideLines() 
{
	UpdateData();
	UpdateWndSimpleGuideLines();
}

void CDlgSettingsForCurvers::UpdateWndSimpleGuideLines()
{
HDC DC;
DC = ::GetDC(m_hWndSimpleGuideLines);
RECT r;
::GetClientRect(m_hWndSimpleGuideLines, &r);
LOGBRUSH LogBrush = {BS_SOLID, m_ColorGrid, 0};
HPEN NewPen;
HPEN OldPen;

::SetBkColor(DC, (COLORREF) RGB(0,0,0));
::SetROP2(DC, R2_NOT);

::InvalidateRect(m_hWndSimpleGuideLines, &r, TRUE);
::UpdateWindow(m_hWndSimpleGuideLines);
if(m_CheckGuideLines) 
	{ 
		NewPen = ::ExtCreatePen(PS_COSMETIC | PS_DOT, 1, &LogBrush, 0, 0);
		OldPen = (HPEN) ::SelectObject(DC, NewPen);
		::MoveToEx(DC, 0, r.bottom/2, NULL);
		::LineTo(DC, r.right, r.bottom/2);
		::MoveToEx(DC, r.right/2, 0, NULL);
		::LineTo(DC, r.right/2, r.bottom);
	}
else
	{ 
		NewPen = ::ExtCreatePen(	PS_GEOMETRIC , 1, &LogBrush,0,0);
		OldPen = (HPEN) ::SelectObject(DC, NewPen);
		::MoveToEx(DC, r.right/2-8, r.bottom/2, NULL);
		::LineTo(DC, r.right/2+8, r.bottom/2);
		::MoveToEx(DC, r.right/2, r.bottom/2-8, NULL);
		::LineTo(DC, r.right/2, r.bottom/2+8);
	}

::SelectObject(DC, OldPen);
::DeleteObject(NewPen);
::ReleaseDC(m_hWndSimpleGrid, DC);
}
