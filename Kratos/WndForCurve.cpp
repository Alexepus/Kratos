// WndForCurve.cpp : implementation file
//

#include "stdafx.h"
#include "Main.h"
#include "Winuser.h"
//#include "ProgNew.h"
//#include "WndForCurve.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWndForCurve



CWndForCurve::CWndForCurve()
{
}

CWndForCurve::~CWndForCurve()
{
}


BEGIN_MESSAGE_MAP(CWndForCurve, CWnd)
	//{{AFX_MSG_MAP(CWndForCurve)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_NCMOUSEMOVE()
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CWndForCurve message handlers

void CWndForCurve::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	
	if(m_pGraph->m_pDataAll || m_pGraph->m_pDataShort)
		{ m_pGraph->m_NotifyCoord.x = ((double) (point.x+1))*m_Coord_X + m_pGraph->m_X1;
		  m_pGraph->m_NotifyCoord.y = m_pGraph->m_Y2 - (point.y)*m_Coord_Y;
		}
	else
		{ m_pGraph->m_NotifyCoord.x = 0;
		  m_pGraph->m_NotifyCoord.y = 0;
		}
	
	//m_pGraph->m_NotifyCoord.x = (double) point.x;
	//m_pGraph->m_NotifyCoord.y = (double) point.y;
	if(m_pGraph->m_pDataAll || m_pGraph->m_pDataShort)
		::SendMessage(::GetParent(m_pGraph->m_hWnd), WM_NOTIFY, 
					(WPARAM) ID_GRAPHICS_WND, (LPARAM) &m_pGraph->m_NotifyCoord);
	
	if(nFlags == MK_LBUTTON)
		{
		if( m_pGraph->m_Viewer == TRUE && m_MouseInWin == TRUE)
			{

			//AfxMessageBox("Parent is Viewer");
			HDC DC;
			DC = ::GetDC(this->m_hWnd);
			::SetBkColor(DC, (COLORREF) RGB(0,0,0));
			::SetROP2(DC, R2_NOT);
			LOGBRUSH LogBrush = {BS_SOLID, RGB(0,0,0), 0};
			HPEN NewPen = ::ExtCreatePen(PS_COSMETIC | PS_DOT, 1, &LogBrush, 0, 0);
			HPEN OldPen = (HPEN) ::SelectObject(DC, (HGDIOBJ) NewPen);
			::MoveToEx(DC, BeginPoint.x, BeginPoint.y, NULL);
			::LineTo(DC, BeginPoint.x, LastPoint.y);
			::LineTo(DC, LastPoint.x, LastPoint.y);
			::LineTo(DC, LastPoint.x, BeginPoint.y);
			::LineTo(DC, BeginPoint.x, BeginPoint.y);
			
			::MoveToEx(DC, BeginPoint.x, BeginPoint.y, NULL);
			::LineTo(DC, BeginPoint.x, point.y);
			::LineTo(DC, point.x, point.y);
			::LineTo(DC, point.x, BeginPoint.y);
			::LineTo(DC, BeginPoint.x, BeginPoint.y);

			//::Rectangle(DC, BeginPoint.x, BeginPoint.y, LastPoint.x, LastPoint.y);
			//::Rectangle(DC, BeginPoint.x, BeginPoint.y, point.x, point.y);
			LastPoint = point;
			::SelectObject(DC, (HGDIOBJ) OldPen);
			::DeleteObject((HGDIOBJ) NewPen);
			::ReleaseDC(this->m_hWnd, DC);
			}
		//else AfxMessageBox("Parent is MainFrame");
		}
	if((m_pGraph->m_pDataAll||m_pGraph->m_pDataShort) &&m_pGraph->m_GuideLines) //m_pGraph->m_Viewer == FALSE && 
	{//–исует направл€ющие линии
		RECT rect;
		GetWindowRect(&rect);
		HDC DC;
		DC = ::GetDC(this->m_hWnd);
		::SetBkColor(DC, (COLORREF) RGB(0,0,0));
		::SetROP2(DC, R2_NOT);
		LOGBRUSH LogBrush = {BS_SOLID, RGB(0,0,0), 0};
		HPEN NewPen = ::ExtCreatePen(PS_COSMETIC | PS_DOT, 1, &LogBrush, 0, 0);
		HPEN OldPen = (HPEN) ::SelectObject(DC, (HGDIOBJ) NewPen);
		if(!m_pGraph->m_WasOnPaint)
		{
//		if(abs(m_pGraph->m_LastUpdateRect.left-m_pGraph->m_LastUpdateRect.right)<800)
	//		Invalidate();
			if(nFlags != MK_LBUTTON)
			{
				//”даление предыдущих направл€ющих
				::MoveToEx(DC, LastPointGuide.x, 0, NULL);
				::LineTo(DC, LastPointGuide.x, rect.bottom);
				::MoveToEx(DC, 0, LastPointGuide.y, NULL);
				::LineTo(DC, rect.right, LastPointGuide.y);
			}
		}
		else
			m_pGraph->m_WasOnPaint=FALSE;
		if(nFlags != MK_LBUTTON )
		{
		//отрисовка новых направл€ющих
		::MoveToEx(DC, point.x, 0, NULL);
		::LineTo(DC, point.x, rect.bottom);
		::MoveToEx(DC, 0, point.y, NULL);
		::LineTo(DC, rect.right, point.y);
		}
		//::Rectangle(DC, BeginPoint.x, BeginPoint.y, LastPoint.x, LastPoint.y);
		//::Rectangle(DC, BeginPoint.x, BeginPoint.y, point.x, point.y);
		LastPointGuide = point;
		::SelectObject(DC, (HGDIOBJ) OldPen);
		::DeleteObject((HGDIOBJ) NewPen);
		::ReleaseDC(this->m_hWnd, DC);
	TRACKMOUSEEVENT TrMouseEvent;
	TrMouseEvent.cbSize=sizeof(TRACKMOUSEEVENT);
	TrMouseEvent.dwFlags=TME_LEAVE;
	TrMouseEvent.hwndTrack=m_hWnd;
	_TrackMouseEvent(&TrMouseEvent);
	}
		
	CWnd::OnMouseMove(nFlags, point);
}

void CWndForCurve::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	//AfxMessageBox("OnLButtonDown");
	if( m_pGraph->m_Viewer == TRUE )	
		{
		m_MouseInWin = TRUE;
		BeginPoint = LastPoint = point;
		::SetCapture(this->m_hWnd);
		RECT r;
		::GetClientRect(this->m_hWnd, &r);
		::ClientToScreen(this->m_hWnd, (LPPOINT) &r);
		::ClientToScreen(this->m_hWnd, (LPPOINT) &r.right);
		::ClipCursor(&r);
		}
	OnMouseLeave(0,0);
	CWnd::OnLButtonDown(nFlags, point);
}

void CWndForCurve::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if( m_pGraph->m_Viewer == TRUE && m_MouseInWin == TRUE)
		{
		//AfxMessageBox("Parent is Viewer");
		m_MouseInWin = FALSE;
		HDC DC;
		DC = ::GetDC(this->m_hWnd);
		::SetBkColor(DC, (COLORREF) RGB(0,0,0));
		::SetROP2(DC, R2_NOT);
		LOGBRUSH LogBrush = {BS_SOLID, RGB(0,0,0), 0};
		HPEN NewPen = ::ExtCreatePen(PS_COSMETIC | PS_DOT, 1, &LogBrush, 0, 0);
		HPEN OldPen = (HPEN) ::SelectObject(DC, (HGDIOBJ) NewPen);
		::MoveToEx(DC, BeginPoint.x, BeginPoint.y, NULL);
		::LineTo(DC, BeginPoint.x, point.y);
		::LineTo(DC, point.x, point.y);
		::LineTo(DC, point.x, BeginPoint.y);
		::LineTo(DC, BeginPoint.x, BeginPoint.y);
			//::Rectangle(DC, BeginPoint.x, BeginPoint.y, LastPoint.x, LastPoint.y);
		//::Rectangle(DC, BeginPoint.x, BeginPoint.y, point.x, point.y);
		LastPoint = point;
		
		::ClipCursor(NULL);
		::ReleaseCapture();
		::SelectObject(DC, (HGDIOBJ) OldPen);
		::DeleteObject((HGDIOBJ) NewPen);
		::ReleaseDC(this->m_hWnd, DC);
		
		m_pGraph->m_NotifyZoom.xBeg = ((double) (BeginPoint.x+1))*m_Coord_X + m_pGraph->m_X1;
		m_pGraph->m_NotifyZoom.yBeg = m_pGraph->m_Y2 - (BeginPoint.y)*m_Coord_Y;
		m_pGraph->m_NotifyZoom.xEnd = ((double) (LastPoint.x+1))*m_Coord_X + m_pGraph->m_X1;
		m_pGraph->m_NotifyZoom.yEnd = m_pGraph->m_Y2 - (LastPoint.y)*m_Coord_Y;
		
		double a;
		if(m_pGraph->m_NotifyZoom.xBeg > m_pGraph->m_NotifyZoom.xEnd)
			{
			a = m_pGraph->m_NotifyZoom.xBeg;
			m_pGraph->m_NotifyZoom.xBeg = m_pGraph->m_NotifyZoom.xEnd;
			m_pGraph->m_NotifyZoom.xEnd = a;
			}
		if(m_pGraph->m_NotifyZoom.yBeg > m_pGraph->m_NotifyZoom.yEnd)
			{
			a = m_pGraph->m_NotifyZoom.yBeg;
			m_pGraph->m_NotifyZoom.yBeg = m_pGraph->m_NotifyZoom.yEnd;
			m_pGraph->m_NotifyZoom.yEnd = a;
			}
	
	//m_pGraph->m_NotifyCoord.x = (double) point.x;
	//m_pGraph->m_NotifyCoord.y = (double) point.y;
///*	
	m_pGraph->m_NotifyZoom.hdr.code = WM_LBUTTONUP;
	::SendMessage(::GetParent(m_pGraph->m_hWnd), WM_NOTIFY, 
				(WPARAM) ID_GRAPHICS_WND, (LPARAM) &m_pGraph->m_NotifyZoom);
//*/
		}
	m_pGraph->m_WasOnPaint=TRUE;
	CWnd::OnLButtonUp(nFlags, point);
}

void CWndForCurve::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if( m_pGraph->m_Viewer == TRUE)
		{
		m_pGraph->m_NotifyZoom.hdr.code = WM_RBUTTONDOWN;
		::SendMessage(::GetParent(m_pGraph->m_hWnd), WM_NOTIFY, 
					(WPARAM) ID_GRAPHICS_WND, (LPARAM) &m_pGraph->m_NotifyZoom);
		}
	CWnd::OnRButtonDown(nFlags, point);
}

LRESULT CWndForCurve::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{	//”даление направл€ющих линий при выходе мыши из окна
	HDC DC;
	DC = ::GetDC(this->m_hWnd);
	::SetBkColor(DC, (COLORREF) RGB(0,0,0));
	::SetROP2(DC, R2_NOT);
	LOGBRUSH LogBrush = {BS_SOLID, RGB(0,0,0), 0};
	HPEN NewPen = ::ExtCreatePen(PS_COSMETIC | PS_DOT, 1, &LogBrush, 0, 0);
	HPEN OldPen = (HPEN) ::SelectObject(DC, (HGDIOBJ) NewPen);
	RECT rect;
	GetWindowRect(&rect);

	//if(!m_pGraph->m_WasOnPaint)
	{
	::MoveToEx(DC, LastPointGuide.x, 0, NULL);
	::LineTo(DC, LastPointGuide.x, rect.bottom);
	::MoveToEx(DC, 0, LastPointGuide.y, NULL);
	::LineTo(DC, rect.right, LastPointGuide.y);
	}
	m_pGraph->m_WasOnPaint=TRUE;

	::SelectObject(DC, (HGDIOBJ) OldPen);
	::DeleteObject((HGDIOBJ) NewPen);
	::ReleaseDC(this->m_hWnd, DC);

return TRUE;			
}

int CWndForCurve::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	SetWindowText("CWndForCurve");
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	
	return 0;
}
