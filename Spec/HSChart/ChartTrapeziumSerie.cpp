// ChartTrapeziumSerie.cpp: implementation of the CChartTrapeziumSerie class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ChartTrapeziumSerie.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CChartTrapeziumSerie::CChartTrapeziumSerie(CChartCtrl* pParent):CChartLineSerie(pParent)
{
	m_iSerieType = stTrapeziumSerie;
}

CChartTrapeziumSerie::~CChartTrapeziumSerie()
{

}

void CChartTrapeziumSerie::DrawAll(CDC *pDC)
{
	if (!m_bIsVisible)
		return;

	CPen NewPen(m_iPenStyle,m_iLineWidth,m_ObjectColor);
	CBrush NewBrush(m_ObjectColor);

	CPen* pOldPen;
	CBrush* pOldBrush;

	if (pDC->GetSafeHdc())
	{
		pDC->SetBkMode(TRANSPARENT);
		//To have lines limited in the drawing rectangle :
		pDC->IntersectClipRect(m_ObjectRect);
		pOldPen = pDC->SelectObject(&NewPen);
		pOldBrush = pDC->SelectObject(&NewBrush);
		for (int i=1;i<(int)m_vPoints.size();i++)
		{
			CPoint TrPoints[4];
			ValueToScreen(m_vPoints[i-1].X,0,TrPoints[0]);
			ValueToScreen(m_vPoints[i-1].X,m_vPoints[i-1].Y,TrPoints[1]);
			ValueToScreen(m_vPoints[i].X,m_vPoints[i].Y,TrPoints[2]);
			ValueToScreen(m_vPoints[i].X,0,TrPoints[3]);
			pDC->Polygon(TrPoints,4);
		}
		m_iLastDrawnPoint=(int)m_vPoints.size()-1;
		pDC->SelectClipRgn(NULL);
		pDC->SelectObject(pOldPen);
		pDC->SelectObject(pOldBrush);

	}
}

void CChartTrapeziumSerie::Draw(CDC* pDC)
{
	if (!m_bIsVisible)
		return;

	CPen NewPen(m_iPenStyle,m_iLineWidth,m_ObjectColor);
	CBrush NewBrush(m_ObjectColor);

	CPen* pOldPen;
	CBrush* pOldBrush;

	if (pDC->GetSafeHdc())
	{
		pDC->SetBkMode(TRANSPARENT);
		//To have lines limited in the drawing rectangle :
		pDC->IntersectClipRect(m_ObjectRect);
		pOldPen = pDC->SelectObject(&NewPen);
		pOldBrush = pDC->SelectObject(&NewBrush);
		for (int i=max(1,m_iLastDrawnPoint);i<(int)m_vPoints.size();i++)
		{
			CPoint TrPoints[4];
			ValueToScreen(m_vPoints[i-1].X,0,TrPoints[0]);
			ValueToScreen(m_vPoints[i-1].X,m_vPoints[i-1].Y,TrPoints[1]);
			ValueToScreen(m_vPoints[i].X,m_vPoints[i].Y,TrPoints[2]);
			ValueToScreen(m_vPoints[i].X,0,TrPoints[3]);
			pDC->Polygon(TrPoints,4);
		}
		m_iLastDrawnPoint=(int)m_vPoints.size()-1;
		pDC->SelectClipRgn(NULL);
		pDC->SelectObject(pOldPen);
		pDC->SelectObject(pOldBrush);

	}
}