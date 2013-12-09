// ViewGraphics.cpp : implementation file
//

#include "stdafx.h"
#include "Main.h"

//#include "ProgNew.h"
//#include "ViewGraphics.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CViewGraphics

CViewGraphics::CViewGraphics()
{
	ReadLOGFONTFromIniFile("ViewTextFont", m_pLogFont);
	CWinApp* App = AfxGetApp();
	m_ColorFont = App->GetProfileInt("ViewTextFont", "ColorFont", 0xff);
	m_TextColor = m_ColorFont;
	if(m_Font) ::DeleteObject((HGDIOBJ) m_Font);
	m_Font = ::CreateFontIndirect(m_pLogFont);

	m_DataAllColor = (COLORREF) App->GetProfileInt("SettingsViewer", "ColorLine", 0xff);
	m_GridColor = (COLORREF) App->GetProfileInt("SettingsViewer", "ColorGrid", 0xff);
	m_GuideLines=(BOOL) App->GetProfileInt("SettingsGraph","GuideLines", 1);

	m_LineAll = (BOOL) App->GetProfileInt("SettingsViewer","Line", 1);
	m_PointsAll = (BOOL) App->GetProfileInt("SettingsViewer","Points", 1);
	m_Grid = (BOOL) App->GetProfileInt("SettingsViewer","Grid", 1);
	m_SizePointsAll = (BOOL) App->GetProfileInt("SettingsViewer","SizePoints", 2);

	m_Viewer = TRUE;
	
}

CViewGraphics::~CViewGraphics()
{
}


BEGIN_MESSAGE_MAP(CViewGraphics, CGraphics)
	//{{AFX_MSG_MAP(CViewGraphics)
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CViewGraphics message handlers

BOOL CViewGraphics::OnEraseBkgnd(CDC* pDC) 
{
	RECT rect={1,1,2,2};
	// TODO: Add your message handler code here and/or call default
	GetClientRect( &rect);
	pDC->FillSolidRect( &rect, RGB(0,0,0));

	return 1;//CGraphics::OnEraseBkgnd(pDC);
}
