// ChartLinePointsSerie.cpp: implementation of the CChartLinePointsSerie class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ChartLinePointsSerie.h"
#include "ChartCtrl.h"
#include <math.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CChartLinePointsSerie::CChartLinePointsSerie(CChartCtrl* pParent): CChartSerie(pParent,stLinePointsSerie)
{
	m_iLineWidth = 1;
	m_iPenStyle = PS_SOLID;
	m_iPointType=ptEllipse;
	m_iXPointSize=5;
	m_iYPointSize=5;
	m_bShowPoints=FALSE;
	m_bShowLine=TRUE;
	SetColor(RGB(0,0,0));
}

CChartLinePointsSerie::~CChartLinePointsSerie()
{

}

CSize CChartLinePointsSerie::GetLegendSize() const
{
	CSize LegendSize(0,0);
	if(m_bShowLine)
	{
		LegendSize.cx = 9;
		LegendSize.cy = m_iLineWidth;
	}
	if(m_bShowPoints)
	{	
		LegendSize.cx = m_iXPointSize;
		LegendSize.cy = m_iYPointSize;
	}
	if(m_bShowLine&&m_bShowPoints)
	{	
		LegendSize.cx = max(9,m_iXPointSize);
		LegendSize.cy = max(m_iYPointSize, m_iYPointSize);
	}

	return LegendSize;
}

int CChartLinePointsSerie::DrawLegend(CDC *pDC, CPoint UpperLeft, int BitmapWidth) const
{
	if (m_strSerieName== _T(""))
		return 0;
	int TextHeight;
	COLORREF _ObjectColor, _PointsOutlineColor, _PointsInnerColor;
	CPoint _UpperLeft;
	POINT Displacement[9]={{0,0},{-1,0},{1,0},{0,-1},{0,1},{-1,-1},{-1,1},{1,1},{1,-1}};
	for(int DisplacementN=(m_bDrawLegendGalo)?9:0; DisplacementN>=0;DisplacementN--)
	{
		_UpperLeft=UpperLeft+Displacement[DisplacementN];
		if(DisplacementN==0)
		{
			_ObjectColor=m_ObjectColor;
			_PointsOutlineColor=m_PointsOutlineColor;
			_PointsInnerColor=m_PointsInnerColor;
		}
		else
			_ObjectColor=_PointsOutlineColor=_PointsInnerColor=GetBackColor();

		//Draw Text
		TextHeight = pDC->GetTextExtent(m_strSerieName.c_str()).cy;
		COLORREF OldTextColor=pDC->SetTextColor(_ObjectColor);
		pDC->ExtTextOut(_UpperLeft.x+BitmapWidth+6,_UpperLeft.y,ETO_CLIPPED,NULL,m_strSerieName.c_str(),NULL);
		pDC->SetTextColor(OldTextColor);
		if(m_bShowLine)
		{
			//Draw line:
			CPen NewPen(m_iPenStyle,m_iLineWidth,_ObjectColor);
			CPen* pOldPen = pDC->SelectObject(&NewPen);
			pDC->MoveTo(_UpperLeft.x+3,_UpperLeft.y+TextHeight/2);
			pDC->LineTo(_UpperLeft.x+BitmapWidth,_UpperLeft.y+TextHeight/2);
			pDC->SelectObject(pOldPen);
		}

		if(m_bShowPoints)
		{
			CPen PointsPen(PS_SOLID,1,_PointsOutlineColor); //For points
			CPen* pOldPen = pDC->SelectObject(&PointsPen);

			CRect PointRect;
			int XOffset = (BitmapWidth-3)/2;
			int YOffset=0;
			//if (TextHeight > m_iYPointSize)
				YOffset = TextHeight/2;
			PointRect.SetRect(_UpperLeft.x+3-m_iXPointSize/2+XOffset,_UpperLeft.y+YOffset-m_iYPointSize/2,_UpperLeft.x+m_iXPointSize/2+3+XOffset+1,_UpperLeft.y+YOffset+m_iYPointSize/2+1);

			CBrush NewBrush(_PointsInnerColor);
			CBrush* pOldBrush = pDC->SelectObject(&NewBrush);

			switch(m_iPointType)
			{
			case ptEllipse:
				pDC->Ellipse(PointRect);
				break;

			case ptRectangle:
				pDC->Rectangle(PointRect);
				break;

			case ptTriangle:
				{
					CPoint TrPoints[3];
					TrPoints[0].x = PointRect.left;
					TrPoints[0].y = PointRect.bottom;
					TrPoints[1].x = PointRect.right;
					TrPoints[1].y = PointRect.bottom;
					TrPoints[2].x = PointRect.left + (int)fabs((PointRect.left-PointRect.right)/2.0);
					TrPoints[2].y = PointRect.top;

					pDC->Polygon(TrPoints,3);
				}
				break;
			}

			pDC->SelectObject(pOldBrush);
			pDC->SelectObject(pOldPen);

		}
	}

	return (m_bShowPoints)? max(TextHeight, m_iYPointSize):TextHeight;
}

void CChartLinePointsSerie::Draw(CDC* pDC)
{
	if (!m_bIsVisible)
		return;

	CPen NewPen(m_iPenStyle,m_iLineWidth,m_ObjectColor); //For line
	CPen PointsPen(PS_SOLID,1,m_PointsOutlineColor); //For points
	CBrush NewBrush(m_PointsInnerColor); //For points

	CPen* pOldPen;
	CBrush* pOldBrush;

	if (pDC->GetSafeHdc())
	{
		pDC->SetBkMode(TRANSPARENT);
		//To have lines limited in the drawing rectangle :
		pDC->IntersectClipRect(m_ObjectRect);
		pOldPen = pDC->SelectObject(&NewPen);
		pOldBrush = pDC->SelectObject(&NewBrush);

		CPoint ScreenPoint;
		int i;

		if(m_bShowLine)
		{

			for (i=max(1,m_iLastDrawnPoint);i<(int)m_vPoints.size();i++)
			{
				ValueToScreen(m_vPoints[i-1].X,m_vPoints[i-1].Y,ScreenPoint);
				pDC->MoveTo(ScreenPoint.x,ScreenPoint.y);
				ValueToScreen(m_vPoints[i].X,m_vPoints[i].Y,ScreenPoint);
				pDC->LineTo(ScreenPoint.x,ScreenPoint.y);
			}
		}
		if(m_bShowPoints)
		{	
			pDC->SelectObject(&PointsPen);
			if((m_vPoints.size()-m_iLastDrawnPoint==1) && (m_iLastDrawnPoint!=0) && m_bShowLine)
				m_iLastDrawnPoint--;
			if(m_iLastDrawnPoint < 0)
				m_iLastDrawnPoint = 0;
			for (i=m_iLastDrawnPoint;i<(int)m_vPoints.size();i++)
			{
				ValueToScreen(m_vPoints[i].X,m_vPoints[i].Y,ScreenPoint);

				CRect PointRect;
				PointRect.SetRect(ScreenPoint.x-m_iXPointSize/2,ScreenPoint.y-m_iYPointSize/2,ScreenPoint.x+m_iXPointSize/2+1,ScreenPoint.y+m_iYPointSize/2+1);

				switch(m_iPointType)
				{
				case ptEllipse:
					pDC->Ellipse(PointRect);
					break;

				case ptRectangle:
					pDC->Rectangle(PointRect);
					break;

				case ptTriangle:
					{
						CPoint TrPoints[3];
						TrPoints[0].x = PointRect.left;
						TrPoints[0].y = PointRect.bottom;
						TrPoints[1].x = PointRect.right;
						TrPoints[1].y = PointRect.bottom;
						TrPoints[2].x = PointRect.left + (int)fabs((PointRect.left-PointRect.right)/2.0);
						TrPoints[2].y = PointRect.top;

						pDC->Polygon(TrPoints,3);
					}
					break;
				}
			}
		}
		m_iLastDrawnPoint=(int)m_vPoints.size()-1;
		pDC->SelectClipRgn(NULL);
		pDC->SelectObject(pOldPen);
		pDC->SelectObject(pOldBrush);
	}
}

void CChartLinePointsSerie::DrawAll(CDC *pDC)
{
	if (!m_bIsVisible)
		return;

	CPen NewPen(m_iPenStyle,m_iLineWidth,m_ObjectColor); //For line
	CPen PointsPen(PS_SOLID,1,m_PointsOutlineColor); //For points
	CBrush NewBrush(m_PointsInnerColor); //For points

	CPen* pOldPen;
	CBrush* pOldBrush;

	if (pDC->GetSafeHdc())
	{
		pDC->SetBkMode(TRANSPARENT);
		//To have lines limited in the drawing rectangle :
		pDC->IntersectClipRect(m_ObjectRect);
		pOldPen = pDC->SelectObject(&NewPen);
		pOldBrush = pDC->SelectObject(&NewBrush);

		CPoint ScreenPoint;
		int i;

		if(m_bShowLine)
		{
			if((int)m_vPoints.size()>0)
			{
				ValueToScreen(m_vPoints[0].X,m_vPoints[0].Y,ScreenPoint);
				pDC->MoveTo(ScreenPoint.x,ScreenPoint.y);
			}

			for (i=1;i<(int)m_vPoints.size();i++)
			{
				ValueToScreen(m_vPoints[i].X,m_vPoints[i].Y,ScreenPoint);
				pDC->LineTo(ScreenPoint.x,ScreenPoint.y);
			}
		}
		if(m_bShowPoints)
		{	
			pDC->SelectObject(&PointsPen);
			for (i=0;i<(int)m_vPoints.size();i++)
			{
				ValueToScreen(m_vPoints[i].X,m_vPoints[i].Y,ScreenPoint);

				CRect PointRect;
				PointRect.SetRect(ScreenPoint.x-m_iXPointSize/2,ScreenPoint.y-m_iYPointSize/2,ScreenPoint.x+m_iXPointSize/2+1,ScreenPoint.y+m_iYPointSize/2+1);

				switch(m_iPointType)
				{
				case ptEllipse:
					pDC->Ellipse(PointRect);
					break;

				case ptRectangle:
					pDC->Rectangle(PointRect);
					break;

				case ptTriangle:
					{
						CPoint TrPoints[3];
						TrPoints[0].x = PointRect.left;
						TrPoints[0].y = PointRect.bottom;
						TrPoints[1].x = PointRect.right;
						TrPoints[1].y = PointRect.bottom;
						TrPoints[2].x = PointRect.left + (int)fabs((PointRect.left-PointRect.right)/2.0);
						TrPoints[2].y = PointRect.top;

						pDC->Polygon(TrPoints,3);
					}
					break;
				}
			}
		}
		m_iLastDrawnPoint=(int)m_vPoints.size()-1;
		pDC->SelectClipRgn(NULL);
		pDC->SelectObject(pOldPen);
		pDC->SelectObject(pOldBrush);
	}
}