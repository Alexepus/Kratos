// BigClientWnd.cpp : implementation file
//
/*
#include "stdafx.h"
#include "string.h"
#include "ProgNew.h"
#include "TitleXWnd.h"
#include "BigClientWnd.h"
#include "CRegion.h"
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
// CBigClientWnd

CBigClientWnd::CBigClientWnd()
{
///*	
	m_class_name = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW,
		AfxGetApp()->LoadStandardCursor(IDC_ARROW),
		(HBRUSH) GetStockObject(BLACK_BRUSH), 0);
//*/


	/*
	m_class_name = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW,
		AfxGetApp()->LoadStandardCursor(IDC_ARROW),
		(HBRUSH) ::CreateSolidBrush((COLORREF) RGB(0,100,200)), 0);
	*/
}

CBigClientWnd::~CBigClientWnd()
{
}


BEGIN_MESSAGE_MAP(CBigClientWnd, CWnd)
	//{{AFX_MSG_MAP(CBigClientWnd)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CBigClientWnd message handlers

int CBigClientWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	//if (CWnd::OnCreate(lpCreateStruct) == -1)
	//	return -1;
	
	// TODO: Add your specialized creation code here
	SetWindowText("CBigClientWnd");
	if(!CreateClient(lpCreateStruct)) return -1;
	return 0;
}

BOOL CBigClientWnd::CreateClient(LPCREATESTRUCT cs)
{
	RECT r;
	::GetClientRect(this->m_hWnd, &r);
/*
	const char* name = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW,
		::LoadCursor(NULL, IDC_CROSS),
		(HBRUSH) GetStockObject(WHITE_BRUSH), 0);

	m_pMainFrame->m_Doc.m_Graph.CreateEx(0, name, NULL,
		WS_CHILD|WS_CLIPCHILDREN|WS_VISIBLE,//|WS_HSCROLL|WS_VSCROLL, 
		0, 0, r.right/2, r.bottom/2, this->m_hWnd, NULL, 0);
*/		
///*
	m_pMainFrame->m_Doc.m_Graph.CreateEx(0, m_class_name, NULL,
		WS_CHILD|WS_CLIPCHILDREN|WS_VISIBLE,//|WS_HSCROLL|WS_VSCROLL, 
		0, 0, r.right, r.bottom, this->m_hWnd, NULL, 0);

	if(m_pMainFrame->m_Doc.m_Graph.m_hWnd == NULL) return FALSE;
//*/
	
	
	/*
	HCURSOR CursorNew = ::LoadCursor(NULL, IDC_CROSS);
	::SetClassLong(m_pMainFrame->m_Doc.m_Graph.m_hWnd, GCL_HCURSOR,
									(LONG) CursorNew);
	*/
	
	//CursorNew = ::LoadCursor(NULL, IDC_ARROW);
	//::SetClassLong(this->m_hWnd, GCL_HCURSOR, (LONG) CursorNew);
	
	//m_pMainFrame->m_Doc.m_Graph.ShowWindow(SW_SHOWNORMAL);

	//	if(!CreateTitleX(&r)) return FALSE;
	return TRUE;
}

BOOL CBigClientWnd::CreateTitleX(RECT* r)
{
	m_pTitleXWnd = new CTitleXWnd;
	if(!m_pTitleXWnd) return FALSE;
	
	m_pTitleXWnd->m_Font = ::CreateFontIndirect(m_pTitleXWnd->m_LogFont);
	//if(!m_pTitleXWnd->m_Font) AfxMessageBox("m_pTitleXWnd->m_Font == NULL");
	//else AfxMessageBox("m_pTitleXWnd->m_Font != NULL");
	
	/*
	int cy = ((int) m_pTitleXWnd->m_LogFont->lfHeight+2);
	m_pTitleXWnd->CreateEx(0, this->m_class_name, NULL,
		WS_CHILD|WS_CLIPCHILDREN|WS_VISIBLE,//|WS_HSCROLL|WS_VSCROLL, 
		0, r->bottom - cy, 
		r->right, cy,
		this->m_hWnd, NULL, 0);
	if(m_pTitleXWnd->m_hWnd == NULL) return FALSE;
	
	//::SetClassLong(m_pTitleXWnd->m_hWnd, GCL_HBRBACKGROUND, (LONG) GetStockObject(BLACK_BRUSH));
	::UpdateWindow(m_pTitleXWnd->m_hWnd);
	*/
	return TRUE;
}

void CBigClientWnd::RecalcChildSize(int cx, int cy)
{
::MoveWindow(m_pMainFrame->m_Doc.m_Graph.m_hWnd, 0, 0, cx, cy, TRUE);
if(!m_pMainFrame->m_Doc.m_Graph.m_pDataAll && !m_pMainFrame->m_Doc.m_Graph.m_pDataShort)
	::MoveWindow(m_pMainFrame->m_Doc.m_Graph.m_WndForCurve.m_hWnd, 0, 0, cx, cy, TRUE);
	//::MoveWindow(m_pMainFrame->m_Doc.m_Graph.m_WndForCurve.m_hWnd, 0, 0, 100, 100, TRUE);
}



BOOL CBigClientWnd::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	// TODO: Add your specialized code here and/or call the base class
	//AfxMessageBox("OnNotify");
	char str[1024];
	char FormatStr[64];
	NOTIFY_COORD* NotifyCoord = (NOTIFY_COORD*) lParam;
	if(NotifyCoord->hdr.hwndFrom == m_pMainFrame->m_Doc.m_Graph.m_hWnd)
		{
		if(NotifyCoord->hdr.code == WM_MOUSEMOVE)
			{
			if(fabs(NotifyCoord->x)>1e10 || fabs(NotifyCoord->y)>1e10)
			{
				//Msg("Invalid Coordinates");
				return CWnd::OnNotify(wParam, lParam, pResult);
			}
			sprintf(FormatStr, " X = %c%c%i%c%c  Y = %c%c%i%c%c",
				'%','.',NotifyCoord->n_digitsX,'l','f', '%','.',NotifyCoord->n_digitsY,'l','f');
			sprintf(str, FormatStr, NotifyCoord->x, NotifyCoord->y);
			::SendMessage(m_pMainFrame->m_hStatusBar, SB_SETTEXT, 
				CMainFrame::StatusBarPartCoordinates, (LPARAM) (LPSTR) str);
			}
		}// end if(NotifyCoord->hdr.hwndFrom == m_pMainFrame->m_Doc.m_Graph.m_hWnd)

	return CWnd::OnNotify(wParam, lParam, pResult);
}
