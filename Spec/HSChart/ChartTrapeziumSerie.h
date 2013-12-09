// ChartTrapeziumSerie.h: interface for the CChartTrapeziumSerie class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHARTTRAPEZIUMSERIE_H__957E0B5B_4620_4225_B53C_E2BEC8F39D55__INCLUDED_)
#define AFX_CHARTTRAPEZIUMSERIE_H__957E0B5B_4620_4225_B53C_E2BEC8F39D55__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ChartLineSerie.h"

class CChartTrapeziumSerie : public CChartLineSerie  
{
public:
	COLORREF m_OutlineColor;
	CChartTrapeziumSerie(CChartCtrl* pParent);
	virtual ~CChartTrapeziumSerie();
	virtual void SetColor(COLORREF NewColor) { m_ObjectColor=m_OutlineColor=NewColor; }

protected:
	int DrawLegend(CDC* pDC, CPoint UpperLeft, int BitmapWidth) const{return CChartLineSerie::DrawLegend(pDC, UpperLeft, BitmapWidth);};
	CSize GetLegendSize() const{return CChartLineSerie::GetLegendSize();};
	void Draw(CDC* pDC);
	void DrawAll(CDC *pDC);

};

#endif // !defined(AFX_CHARTTRAPEZIUMSERIE_H__957E0B5B_4620_4225_B53C_E2BEC8F39D55__INCLUDED_)
