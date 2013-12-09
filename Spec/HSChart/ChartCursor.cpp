// ChartCursor.cpp: implementation of the CChartCursor class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ChartCursor.h"
#include "ChartCtrl.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CChartCursor::CChartCursor(CChartCtrl* pParent):CChartObject(pParent)
{
	m_bFixed=false;
	m_bHorizontal=false;
	m_iLineWidth=1;
	m_iPenStyle=PS_SOLID;

	m_pHorizontalAxis = pParent->GetBottomAxis();
	//m_pHorizontalAxis->RegisterSeries(this);
	m_pVerticalAxis = pParent->GetLeftAxis();
	//m_pVerticalAxis->RegisterSeries(this);
	m_dCoord=0.;
	RecalulateRect();
}

CChartCursor::~CChartCursor()
{
}
void CChartCursor::Draw(CDC *pDC)
{
	RecalulateRect();
	if (!m_bIsVisible)
		return;
	DrawEx(pDC);

}
void CChartCursor::DrawEx(CDC *pDC)
{
	CPen NewPen(m_iPenStyle,m_iLineWidth,m_ObjectColor);
	CPen* pOldPen;

	if (pDC->GetSafeHdc())
	{
		pDC->SetBkMode(TRANSPARENT);
		//To have lines limited in the drawing rectangle :
		pDC->IntersectClipRect(m_ObjectRect);
		pOldPen = pDC->SelectObject(&NewPen);

			
			CPoint ScreenPoint;
			if(!m_bHorizontal)
			{
				double Min, Max;
				m_pVerticalAxis->GetMinMax(Min,Max);
				ValueToScreen(m_dCoord,Min,ScreenPoint);
				pDC->MoveTo(ScreenPoint.x,ScreenPoint.y);
				ValueToScreen(m_dCoord,Max,ScreenPoint);
				pDC->LineTo(ScreenPoint.x,ScreenPoint.y);
			}
		pDC->SelectClipRgn(NULL);
		pDC->SelectObject(pOldPen);
	}
}

void CChartCursor::ValueToScreen(double XValue, double YValue, CPoint &ScreenPoint) const
{
	ScreenPoint.x = m_pHorizontalAxis->ValueToScreen(XValue);
	ScreenPoint.y = m_pVerticalAxis->ValueToScreen(YValue);
}

void CChartCursor::ScreenToValue(double &XValue, double &YValue, CPoint ScreenPoint) const
{
	XValue = m_pHorizontalAxis->ScreenToValue(ScreenPoint.x);
	YValue = m_pVerticalAxis->ScreenToValue(ScreenPoint.y);
}

void CChartCursor::SetCoord(double NewCoord)
{
	m_dCoord=NewCoord;
	RecalulateRect();
}

void CChartCursor::RecalulateRect()
{
	double Min, Max;
	CPoint ScreenPoint;
	m_pVerticalAxis->GetMinMax(Min,Max);
	ValueToScreen(m_dCoord,Max,ScreenPoint);
	m_rScreenRect.TopLeft()=ScreenPoint;
	ValueToScreen(m_dCoord,Min,ScreenPoint);
	m_rScreenRect.BottomRight()=ScreenPoint;
	m_rScreenRect.InflateRect(1,0,2,0);
}