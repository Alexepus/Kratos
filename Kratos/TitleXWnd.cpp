// TitleXWnd.cpp : implementation file
//
/*
#include "stdafx.h"
#include <string.h>
#include "ProgNew.h"
#include "TitleXWnd.h"
#include "Region.h"
#include "DialogParamRegion.h"
#include "function.h"
*/
#include "stdafx.h"
#include "Main.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTitleXWnd

CTitleXWnd::CTitleXWnd()
{
	m_Font = NULL;
	m_LogFont = new LOGFONT;
	m_lpszSection = new char[7]; 
	sprintf(m_lpszSection, "%s", "TitleX");
	ReadLOGFONTFromIniFile(m_lpszSection, m_LogFont);
	//sprintf(m_FontStyle, "%s", m_LogFont->lfFaceName);	
	CWinApp* App = AfxGetApp();
	CString str = App->GetProfileString(m_lpszSection, "TextString", "Title X");
	m_TextString = new char[str.GetLength()+1];
	sprintf(m_TextString, "%s", (LPCSTR)str);
	m_TextColor = App->GetProfileInt(m_lpszSection, "TextColor", 0xff);
}

CTitleXWnd::~CTitleXWnd()
{
	if(m_Font) ::DeleteObject((HFONT) m_Font);
	delete m_TextString;
	delete m_lpszSection;
	delete m_LogFont;
}


BEGIN_MESSAGE_MAP(CTitleXWnd, CWnd)
	//{{AFX_MSG_MAP(CTitleXWnd)
	ON_WM_RBUTTONDOWN()
	ON_WM_PAINT()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CTitleXWnd message handlers

void CTitleXWnd::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	//AfxMessageBox("RBUTTONDOWN");
	if(ChooseNewFont(this->m_hWnd, m_LogFont, &m_TextColor))
	{
	AfxMessageBox("ChooseNewFont OK");
	//WriteLOGFONTToIniFile(m_lpszSection, m_LogFont);
	if(m_Font) ::DeleteObject( (HGDIOBJ) m_Font);
	m_Font = ::CreateFontIndirect(m_LogFont);
	//RecalcSizeOllWindows
	RECT r;
	::GetClientRect(this->m_hWnd, &r);
	::InvalidateRect(this->m_hWnd, &r, TRUE);
	::UpdateWindow(this->m_hWnd);
	//::SendMessage(this->m_hWnd, WM_PAINT, 0, 0);
	
	}


	CWnd::OnRButtonDown(nFlags, point);
}

void CTitleXWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	RECT r; 
	::GetClientRect(this->m_hWnd, &r);
	HFONT old_font;
	old_font = (HFONT) ::SelectObject(dc.m_hDC, (HGDIOBJ) m_Font);
	//if(!old_font) AfxMessageBox("old_font == NULL");
	//else AfxMessageBox("old_font != NULL");
	COLORREF OldTextColor = ::SetTextColor(dc.m_hDC, m_TextColor);
	::SetTextAlign(dc.m_hDC, TA_CENTER);
	//::SetTextAlign(dc.m_hDC, VTA_CENTER|VTA_BASELINE);
	//if((::SetTextAlign(dc.m_hDC, VTA_CENTER)) == TA_CENTER|VTA_CENTER) AfxMessageBox("TA_CENTER|VTA_CENTER");
	::TextOut(dc.m_hDC, r.right/2, 0, m_TextString, strlen(m_TextString));
	//::TextOut(dc.m_hDC, 0, r.bottom/2, m_TextString, strlen(m_TextString));
	
	::SetTextColor(dc.m_hDC, OldTextColor);
	::SelectObject(dc.m_hDC, (HGDIOBJ) old_font);

	::MoveToEx(dc.m_hDC, 1, 1, NULL);
	::LineTo(dc.m_hDC, r.right-1, r.top+1);
	::LineTo(dc.m_hDC, r.right-1, r.bottom-1);
	::LineTo(dc.m_hDC, r.left+1, r.bottom-1);
	::LineTo(dc.m_hDC, r.left+1, r.top+1);
	
	// Do not call CWnd::OnPaint() for painting messages
}

int CTitleXWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	SetWindowText("CTitleXWnd");
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	
	return 0;
}
