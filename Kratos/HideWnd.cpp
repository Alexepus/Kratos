// HideWnd.cpp : implementation file
//
/*
#include "stdafx.h"
#include "ProgNew.h"
#include "BigClientWnd.h"
#include "HideWnd.h"
*/
#include "stdafx.h"
#include "Main.h"

//#include <stdio.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHideWnd
extern UINT MY_WM_RESIZE;

CHideWnd::CHideWnd() : m_pBigClientWnd(0)
{
}

CHideWnd::~CHideWnd()
{
	if(m_pBigClientWnd)
		delete m_pBigClientWnd;
}


BEGIN_MESSAGE_MAP(CHideWnd, CWnd)
	//{{AFX_MSG_MAP(CHideWnd)
	ON_WM_HSCROLL()
	ON_WM_CREATE()
	ON_REGISTERED_MESSAGE(MY_WM_RESIZE, OnMyResize)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CHideWnd message handlers

void CHideWnd::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
//	AfxMessageBox("OnHScroll");
	
	CWnd::OnHScroll(nSBCode, nPos, pScrollBar);
}

int CHideWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	//if (CWnd::OnCreate(lpCreateStruct) == -1)
	//	return -1;
	
	// TODO: Add your specialized creation code here
	SetWindowText("CHideWnd");
	if(!CreateClient(lpCreateStruct)) return -1;
	return 0;
}

BOOL CHideWnd::CreateClient(LPCREATESTRUCT cs)   // CREATESTRUCT
{
//char str[80];
//sprintf(str, "x=%i\n y=%i\n cx=%i\n cy=%i", cs->x, cs->y, cs->cx, cs->cy);
//AfxMessageBox(str);
	// Create ScrollBars
	RECT r;
	::GetClientRect(this->m_hWnd, &r);
	
	/*
	//Create HScroll
	m_hHScroll = ::CreateWindowEx(0, "scrollbar", NULL, 
		WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS|SBS_HORZ, //|SBS_BOTTOMALIGN,
		r.left, r.bottom - ::GetSystemMetrics(SM_CYHSCROLL),
		r.right - ::GetSystemMetrics(SM_CXVSCROLL), ::GetSystemMetrics(SM_CYHSCROLL),
		this->m_hWnd, NULL, // -- or ID of scroll
		AfxGetInstanceHandle(), NULL);
	if(m_hHScroll == NULL) {AfxMessageBox("Can't create HScroll"); return FALSE;}
	
	//Create VScroll
	m_hVScroll = ::CreateWindowEx(0, "scrollbar", NULL, 
		WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS|SBS_VERT, 
		r.right - ::GetSystemMetrics(SM_CXVSCROLL), r.top,
		::GetSystemMetrics(SM_CXVSCROLL), r.bottom - ::GetSystemMetrics(SM_CYHSCROLL),
		this->m_hWnd, NULL, // -- or ID of scroll
		AfxGetInstanceHandle(), NULL);
	if(m_hVScroll == NULL) {AfxMessageBox("Can't create VScroll"); return FALSE;}
	*/
	
	
	// Create BigClientWnd
	m_pBigClientWnd = new CBigClientWnd;
	m_pBigClientWnd->m_pMainFrame = this->m_pMainFrame;
//	m_pBigClientWnd->m_class_name = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW,
//		AfxGetApp()->LoadStandardCursor(IDC_ARROW),
//		(HBRUSH) GetStockObject(BLACK_BRUSH), 0);
//	/*
	m_pBigClientWnd->CreateEx(0, m_pBigClientWnd->m_class_name, NULL,
		WS_CHILD|WS_CLIPCHILDREN|WS_VISIBLE,//|WS_HSCROLL|WS_VSCROLL, 
		0, 0, 
		//r.right - ::GetSystemMetrics(SM_CXVSCROLL), r.bottom - ::GetSystemMetrics(SM_CYHSCROLL),
		r.right, r.bottom,
		this->m_hWnd, NULL, 0);
//	*/
	if(m_pBigClientWnd->m_hWnd == NULL) return FALSE;
				
	return TRUE;
}


LRESULT CHideWnd::OnMyResize(WPARAM wp, LPARAM lp)
{
	//AfxMessageBox("This is MY_WM_RESIZE");
		//return (LRESULT) 1;
	SAndTBarHWND* Bar = (SAndTBarHWND*) lp;	
	HWND hWnd = (HWND) wp;
	RECT rc_sb, rc_tb;
	::GetWindowRect(Bar->Sb,  &rc_sb);
	::GetWindowRect(Bar->Tb, &rc_tb);
	::ScreenToClient(hWnd, (LPPOINT) &rc_sb); ::ScreenToClient(hWnd, (LPPOINT) &rc_sb.right);
	::ScreenToClient(hWnd, (LPPOINT) &rc_tb); ::ScreenToClient(hWnd, (LPPOINT) &rc_tb.right);
	
	::MoveWindow(this->m_hWnd,
				rc_tb.left, rc_tb.bottom, rc_tb.right - rc_tb.left, rc_sb.top - rc_tb.bottom,TRUE);
	RECT r;
	::GetClientRect(this->m_hWnd, &r);
	int cx = r.right;// - ::GetSystemMetrics(SM_CXVSCROLL);
	int cy = r.bottom;// - ::GetSystemMetrics(SM_CYHSCROLL);
	//::MoveWindow(m_hHScroll, r.left, cy, cx, ::GetSystemMetrics(SM_CYHSCROLL), TRUE);
	//::MoveWindow(m_hVScroll, cx, r.top, ::GetSystemMetrics(SM_CXVSCROLL), cy, TRUE);

	
	//m_pBigClientWnd->cx = cx;
	//m_pBigClientWnd->cy = cy;
	
	
	::MoveWindow(m_pBigClientWnd->m_hWnd, 0, 0, cx, cy, TRUE);
	m_pBigClientWnd->RecalcChildSize(cx, cy);
	return 0;
}



