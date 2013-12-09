// CommentsWnd.cpp : implementation file
//

#include "stdafx.h"
#include "ProgNew.h"
#include "CommentsWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCommentsWnd

CCommentsWnd::CCommentsWnd()
{
}

CCommentsWnd::~CCommentsWnd()
{
}


BEGIN_MESSAGE_MAP(CCommentsWnd, CWnd)
	//{{AFX_MSG_MAP(CCommentsWnd)
	ON_WM_CLOSE()
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CCommentsWnd message handlers

void CCommentsWnd::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
//	AfxMessageBox("Comments Close");
	RECT r;
	::GetWindowRect(this->m_hWnd, &r);
	
	//AfxMessageBox("CViewWnd::OnClose()");
	// Записываю вершины углов. Размеры cx, cy вычисляются при создании окна 
	// см. CRegionWnd::OnCommand (кнопка View)
	//AfxMessageBox("Comments Close");
	CWinApp* App = AfxGetApp();
	
	if(!::IsIconic(this->m_hWnd) && !::IsZoomed(this->m_hWnd))
		{
		App->WriteProfileInt("SizeOfCommentsWindow", "Left", r.left);
		App->WriteProfileInt("SizeOfCommentsWindow", "Top", r.top);
		App->WriteProfileInt("SizeOfCommentsWindow", "Right", r.right);
		App->WriteProfileInt("SizeOfCommentsWindow", "Bottom", r.bottom);
		}
	CWnd::OnClose();
}

int CCommentsWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	RECT r;
	::GetClientRect(m_hWnd, &r);

	this->m_hWndEdit = ::CreateWindowEx(0, "edit",NULL,
		WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_AUTOVSCROLL | ES_READONLY,
		0, 0, r.right, r.bottom,
		this->m_hWnd, NULL, AfxGetInstanceHandle(), NULL);

	::SendMessage(m_hWndEdit, EM_SETMARGINS, EC_LEFTMARGIN | EC_RIGHTMARGIN,
								(LPARAM) MAKELONG(10, 10));

	return 0;
}

void CCommentsWnd::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	RECT r;
	::GetClientRect(m_hWnd, &r);
	::MoveWindow(this->m_hWndEdit, 0, 0, r.right, r.bottom, TRUE);
}

HBRUSH CCommentsWnd::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CWnd::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	COLORREF Color = RGB(190,210,45);
	HBRUSH hBrush;
	LOGBRUSH LogBrush = {BS_SOLID, Color, NULL};
	hBrush = ::CreateBrushIndirect(&LogBrush);
	::SetBkColor(pDC->m_hDC, Color);
	::SetTextColor(pDC->m_hDC, (COLORREF) RGB(0,0,0));
	if(hBrush) return hBrush;	
//	hbr = (HBRUSH) ::GetStockObject(WHITE_BRUSH);	
//	::SetTextColor(pDC->m_hDC, (COLORREF) RGB(255,0,0));
//	::SetBkColor(pDC->m_hDC, (COLORREF) RGB(255,0,0));
	
	// TODO: Return a different brush if the default is not desired
	return hbr;
}
