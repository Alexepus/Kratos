// DlgSetViewer.cpp : implementation file
//

#include "stdafx.h"
#include "Main.h"
//#include "ProgNew.h"
//#include "DlgSetViewer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgSetViewer dialog


CDlgSetViewer::CDlgSetViewer(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSetViewer::IDD, pParent)
{
/*	
	//{{AFX_DATA_INIT(CDlgSetViewer)
	m_CheckLine = FALSE;
	m_CheckPoints = FALSE;
	m_CheckGrid = FALSE;
	//}}AFX_DATA_INIT
*/
}


void CDlgSetViewer::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSetViewer)
	DDX_Check(pDX, IDC_CHECK_CURVE_LINE, m_CheckLine);
	DDX_Check(pDX, IDC_CHECK_CURVE_POINTS, m_CheckPoints);
	DDX_Check(pDX, IDC_CHECK_GRID, m_CheckGrid);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgSetViewer, CDialog)
	//{{AFX_MSG_MAP(CDlgSetViewer)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_CHECK_CURVE_LINE, OnCheckCurveLine)
	ON_BN_CLICKED(IDC_CHECK_CURVE_POINTS, OnCheckCurvePoints)
	ON_CBN_SELENDOK(IDC_COMBO_SIZE_CURVE, OnSelEndOkComboSizeCurve)
	ON_BN_CLICKED(IDC_BUTTON_CURVE_COLOR, OnButtonCurveColor)
	ON_BN_CLICKED(IDC_CHECK_GRID, OnCheckGrid)
	ON_BN_CLICKED(IDC_BUTTON_GRID_COLOR, OnButtonGridColor)
	ON_BN_CLICKED(IDC_BUTTON_FONT_COLOR, OnButtonFontColor)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSetViewer message handlers

void CDlgSetViewer::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	UpdateWndSimpleCurve();
	UpdateWndSimpleGrid();
	UpdateWndSimpleFont();
	// Do not call CDialog::OnPaint() for painting messages
}

//========
BOOL CDlgSetViewer::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
m_ColorCurve = m_pMainFrame->m_Doc.m_ViewWnd.m_ViewGraph.m_DataAllColor;
m_ColorGrid = m_pMainFrame->m_Doc.m_ViewWnd.m_ViewGraph.m_GridColor;
m_ColorFont = m_pMainFrame->m_Doc.m_ViewWnd.m_ViewGraph.m_ColorFont;
m_Font = m_pMainFrame->m_Doc.m_ViewWnd.m_ViewGraph.m_Font;
memmove((void*) &m_LogFont, (void*) m_pMainFrame->m_Doc.m_ViewWnd.m_ViewGraph.m_pLogFont, 
				sizeof(LOGFONT));
	
m_CheckLine = m_pMainFrame->m_Doc.m_ViewWnd.m_ViewGraph.m_LineAll;
m_CheckPoints = m_pMainFrame->m_Doc.m_ViewWnd.m_ViewGraph.m_PointsAll;
m_SizePoints = m_pMainFrame->m_Doc.m_ViewWnd.m_ViewGraph.m_SizePointsAll;

m_CheckGrid = m_pMainFrame->m_Doc.m_ViewWnd.m_ViewGraph.m_Grid;

HWND hWndChild = ::GetDlgItem(this->m_hWnd, IDC_CHECK_CURVE_LINE);	
if(m_CheckLine) ::SendMessage(hWndChild, BM_SETCHECK, 1, 0);
else ::SendMessage(hWndChild, BM_SETCHECK, 0, 0);
hWndChild = ::GetDlgItem(this->m_hWnd, IDC_CHECK_CURVE_POINTS);	
if(m_CheckPoints) ::SendMessage(hWndChild, BM_SETCHECK, 1, 0);
else ::SendMessage(hWndChild, BM_SETCHECK, 0, 0);

hWndChild = ::GetDlgItem(this->m_hWnd, IDC_CHECK_GRID);	
if(m_CheckGrid) ::SendMessage(hWndChild, BM_SETCHECK, 1, 0);
else ::SendMessage(hWndChild, BM_SETCHECK, 0, 0);

	
	int i;
	char strCombo[16];
	
	// CURVE
	hWndChild = ::GetDlgItem(this->m_hWnd, IDC_EDIT_CURVE_SAMPLE);
	RECT r;
	::GetClientRect(hWndChild, &r);
	m_hWndSimpleCurve = ::CreateWindow(m_pMainFrame->m_Doc.m_Graph.m_cl_name_for_curve,
		NULL, WS_CHILD|WS_CLIPCHILDREN|WS_VISIBLE,//|WS_HSCROLL|WS_VSCROLL, 
		0, 0, r.right, r.bottom, 
		hWndChild, NULL, AfxGetInstanceHandle(),0);
	//if(!m_hWndSimpleCurve) AfxMessageBox("m_hWndSimpleCurve == NULL");
	//else AfxMessageBox("m_hWndSimpleCurve != NULL");
	UpdateWndSimpleCurve();

	hWndChild = ::GetDlgItem(this->m_hWnd, IDC_COMBO_SIZE_CURVE);
	for(i=0; i<19; ++i)
		{
		sprintf(strCombo, "%i", 2+i);
		::SendMessage(hWndChild, CB_ADDSTRING, 0, (LPARAM) strCombo);
		}
	::SendMessage(hWndChild, CB_SETCURSEL , (WPARAM) (m_SizePoints-2), 0);
	//::SendMessage(hWndChild, CB_SETITEMHEIGHT , 0, 100);
	
	
	// GRID
	hWndChild = ::GetDlgItem(this->m_hWnd, IDC_EDIT_GRID_SAMPLE);
//	RECT r;
	::GetClientRect(hWndChild, &r);
	m_hWndSimpleGrid = ::CreateWindow(m_pMainFrame->m_Doc.m_Graph.m_cl_name_for_curve,
		NULL, WS_CHILD|WS_CLIPCHILDREN|WS_VISIBLE,//|WS_HSCROLL|WS_VSCROLL, 
		0, 0, r.right, r.bottom, 
		hWndChild, NULL, AfxGetInstanceHandle(),0);
	UpdateWndSimpleGrid();
	
	// FONT
	hWndChild = ::GetDlgItem(this->m_hWnd, IDC_EDIT_FONT_SAMPLE);
//	RECT r;
	::GetClientRect(hWndChild, &r);
	m_hWndSimpleFont = ::CreateWindow(m_pMainFrame->m_Doc.m_Graph.m_cl_name_for_curve,
		NULL, WS_CHILD|WS_CLIPCHILDREN|WS_VISIBLE,//|WS_HSCROLL|WS_VSCROLL, 
		0, 0, r.right, r.bottom, 
		hWndChild, NULL, AfxGetInstanceHandle(),0);
	UpdateWndSimpleFont();
	
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgSetViewer::UpdateWndSimpleCurve()
{
HWND hWndChild;
HDC DC;
DC = ::GetDC(m_hWndSimpleCurve);
//if(!DC) AfxMessageBox("DC == NULL");
//else AfxMessageBox("DC != NULL");
RECT r;
::GetClientRect(m_hWndSimpleCurve, &r);
//LOGBRUSH LogBrush = {BS_SOLID, m_pMainFrame->m_Doc.m_Graph.m_DataShortColor,0};
LOGBRUSH LogBrush = {BS_SOLID, m_ColorCurve, 0};
//LOGBRUSH LogBrush = {BS_SOLID, (COLORREF) RGB(100,100,100), 0};
HPEN NewPen = ::ExtCreatePen(	PS_COSMETIC | PS_SOLID, 1, &LogBrush,0,0);
HPEN OldPen = (HPEN) ::SelectObject(DC, NewPen);
HBRUSH NewBrush = ::CreateBrushIndirect(&LogBrush);
HBRUSH OldBrush = (HBRUSH) ::SelectObject(DC, NewBrush);
//::SetBkColor(DC, m_ColorCurr);

::InvalidateRect(m_hWndSimpleCurve, &r, TRUE);
::UpdateWindow(m_hWndSimpleCurve);

if(m_CheckLine) 
	{ ::MoveToEx(DC, 0, r.bottom, NULL);
		::LineTo(DC, r.right, 0);
	//AfxMessageBox("LINE OK");
	}
if(m_CheckPoints)
	::Ellipse(DC, r.right/2-m_SizePoints/2, r.bottom/2-m_SizePoints/2,
							(int)(r.right/2+m_SizePoints/2.+0.5), 
							(int)(r.bottom/2+m_SizePoints/2.+0.5) );

CDC MemDC, cdc;
cdc.Attach(DC);
MemDC.CreateCompatibleDC(&cdc);
CBitmap memBitmap1, memBitmap2;
memBitmap1.CreateCompatibleBitmap(&MemDC, r.right*2, r.bottom*2);
cdc.SelectObject(&memBitmap1);
//HPEN NewPen1 = ::ExtCreatePen(	PS_GEOMETRIC | PS_SOLID, 1, &LogBrush,0,0);
//HBRUSH NewBrush1 = ::CreateBrushIndirect(&LogBrush);
MemDC.SelectObject(&NewPen);
MemDC.SelectObject(&NewBrush);
cdc.MoveTo(0, r.bottom*2);
cdc.LineTo(r.right*2, 0);
//BitBlt(DC,0,0,r.right,r.bottom,MemDC.GetSafeHdc(),0,0, SRCCOPY);

::SelectObject(DC, OldBrush);
::SelectObject(DC, OldPen);
::DeleteObject(NewBrush);
::DeleteObject(NewPen);
::ReleaseDC(m_hWndSimpleCurve, DC);

if(!m_CheckPoints)
	{
	hWndChild = ::GetDlgItem(this->m_hWnd, IDC_COMBO_SIZE_CURVE);
	::EnableWindow(hWndChild, FALSE);
	hWndChild = ::GetDlgItem(this->m_hWnd, IDC_STATIC_CURVE_SIZE);
	::EnableWindow(hWndChild, FALSE);
	}
else
	{
	hWndChild = ::GetDlgItem(this->m_hWnd, IDC_COMBO_SIZE_CURVE);
	::EnableWindow(hWndChild, TRUE);
	hWndChild = ::GetDlgItem(this->m_hWnd, IDC_STATIC_CURVE_SIZE);
	::EnableWindow(hWndChild, TRUE);
	}

hWndChild = ::GetDlgItem(this->m_hWnd, IDC_BUTTON_CURVE_COLOR);
if(!m_CheckPoints && !m_CheckLine) ::EnableWindow(hWndChild, FALSE);
else ::EnableWindow(hWndChild, TRUE);

}


void CDlgSetViewer::UpdateWndSimpleGrid()
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

void CDlgSetViewer::UpdateWndSimpleFont()
{
	HDC DC = ::GetDC(this->m_hWndSimpleFont);
	
	HFONT OldFont = (HFONT) ::SelectObject(DC, this->m_Font);
	::SetBkColor(DC, (COLORREF) RGB(0,0,0));
	//m_ColorFont = m_pMainFrame->m_Doc.m_ViewWnd.m_ViewGraph.m_TextColor;
	::SetTextColor(DC, m_ColorFont);
	char str[16];
	sprintf(str, "%s", "123");
	SIZE size;
	::GetTextExtentPoint32(DC, str, strlen(str), &size);
	RECT r;
	::GetClientRect(this->m_hWndSimpleFont, &r);
	::InvalidateRect(m_hWndSimpleFont, &r, TRUE);
	::UpdateWindow(m_hWndSimpleFont);

	::TextOut(DC, r.right/2 - size.cx/2, r.bottom/2 - size.cy/2, str, strlen(str));
	//::TextOut(DC, r.right/2 , r.bottom/2, str, strlen(str));
	::SelectObject(DC, OldFont);
	::ReleaseDC(this->m_hWndSimpleFont, DC);

}

void CDlgSetViewer::OnCheckCurveLine() 
{
	// TODO: Add your control notification handler code here
	if(m_CheckLine) m_CheckLine = FALSE;
	else m_CheckLine = TRUE;
	UpdateWndSimpleCurve();
	MessageBox("");
}

void CDlgSetViewer::OnCheckCurvePoints() 
{
	// TODO: Add your control notification handler code here
	if(m_CheckPoints) m_CheckPoints = FALSE;
	else m_CheckPoints = TRUE;
	UpdateWndSimpleCurve();	
}

void CDlgSetViewer::OnSelEndOkComboSizeCurve() 
{
	// TODO: Add your control notification handler code here
	UINT res;
	HWND hWndChild = ::GetDlgItem(this->m_hWnd, IDC_COMBO_SIZE_CURVE);
	res = (UINT) ::SendMessage(hWndChild, CB_GETCURSEL , 0, 0);
	if(res!=CB_ERR) m_SizePoints = res+2;
	UpdateWndSimpleCurve();	
}

void CDlgSetViewer::OnButtonCurveColor() 
{
	// TODO: Add your control notification handler code here
	m_ColorCurve = WndColorSet(m_ColorCurve);
	UpdateWndSimpleCurve();	
}

COLORREF CDlgSetViewer::WndColorSet(COLORREF rgb)
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
//	for(i=0; i<16; ++i) 
//		App->WriteProfileInt("COLOR",str[i],cc.lpCustColors[i]);
	return rgb;

}

void CDlgSetViewer::OnCheckGrid() 
{
	// TODO: Add your control notification handler code here
	if(m_CheckGrid) m_CheckGrid = FALSE;
	else m_CheckGrid = TRUE;
	UpdateWndSimpleGrid();		
}

void CDlgSetViewer::OnButtonGridColor() 
{
	// TODO: Add your control notification handler code here
	m_ColorGrid = WndColorSet(m_ColorGrid);
	UpdateWndSimpleGrid();		
}

void CDlgSetViewer::OnButtonFontColor() 
{
	// TODO: Add your control notification handler code here
LOGFONT NewLogFont;
memmove((void*) &NewLogFont, (void*) &m_LogFont, sizeof(LOGFONT));	
if(ChooseNewFont(this->m_hWnd, &NewLogFont, &m_ColorFont))	
	{
	HWND hWnd = m_pMainFrame->m_Doc.m_ViewWnd.m_ViewGraph.m_hWnd;
	HDC DC = ::GetDC(hWnd);
	HFONT NewFont = ::CreateFontIndirect(&NewLogFont);
	HFONT OldFont = (HFONT) ::SelectObject(DC, NewFont);
	RECT r; 
	::GetClientRect(hWnd, &r);
	int cy = r.bottom - r.top;
	char* str = "A";
	SIZE size;
	::GetTextExtentPoint32(DC, str, strlen(str), &size);
	::SelectObject(DC, OldFont);
	
	if((10*size.cy) < cy)
		{
		WriteLOGFONTToIniFile("ViewTextFont", &NewLogFont);
		//CWinApp* App = AfxGetApp();
		//App->WriteProfileInt("ViewTextFont", "ViewColorFont", m_ColorFont);
		//m_Doc.m_ViewWnd.m_ViewGraph.m_TextColor = m_Doc.m_Graph.m_TextColor;
		//if(m_Font) ::DeleteObject( (HGDIOBJ) m_Font);
		//::DeleteObject( (HGDIOBJ) NewFont);
		memmove( (void*) &m_LogFont, (void*) &NewLogFont, sizeof(LOGFONT));	
		if(m_Font) ::DeleteObject( (HGDIOBJ) m_Font);
		m_Font = ::CreateFontIndirect(&m_LogFont);
		//m_Doc.m_ViewWnd.m_ViewGraph.m_Font = m_Doc.m_Graph.m_Font;
		//if(::IsWindow(m_Doc.m_ViewWnd.m_hWnd)) m_Doc.m_ViewWnd.m_ViewGraph.ReDrawAll();
		//m_pMainFrame->m_Doc.m_ViewWnd.m_ViewGraph.ReDrawAll();
		UpdateWndSimpleFont();
		}
	else AfxMessageBox("ERROR !\n\n Can't create font");
	::SelectObject(DC, OldFont);
	::DeleteObject( (HGDIOBJ) NewFont);
	::ReleaseDC(hWnd, DC);


	}

	
}
