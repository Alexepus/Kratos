// ChartLinePointsSerie.h: interface for the CChartLinePointsSerie class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHARTLINEPOINTSSERIE_H__C5D635CC_D0F8_4212_881C_B3B73DA8406C__INCLUDED_)
#define AFX_CHARTLINEPOINTSSERIE_H__C5D635CC_D0F8_4212_881C_B3B73DA8406C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ChartSerie.h"

class CChartLinePointsSerie : public CChartSerie  
{
public:
	enum PointType
	{
		ptEllipse=0,
		ptRectangle=1,
		ptTriangle=2
	};
	BOOL m_bShowLine;
	BOOL m_bShowPoints;
	COLORREF m_PointsInnerColor;
	COLORREF m_PointsOutlineColor;
	int  GetPenStyle() const        { return m_iPenStyle; }
	void SetPenStyle(int NewStyle)  { m_iPenStyle = NewStyle; }

	int	 GetWidth() const       { return m_iLineWidth; }
	void SetWidth(int NewValue)  { m_iLineWidth = NewValue; }

	void SetPointSize(int XSize, int YSize)
	{
		m_iXPointSize = XSize;
		m_iYPointSize = YSize;
	}
	void GetPointSize(int& XSize, int& YSize) const
	{
        XSize = m_iXPointSize;
        YSize = m_iYPointSize;
    }
	void SetPointType(PointType Type)  { m_iPointType = Type; }
	PointType GetPointType() const     { return m_iPointType; }


	CChartLinePointsSerie(CChartCtrl* pParent);
	virtual ~CChartLinePointsSerie();
	virtual void SetColor(COLORREF NewColor) { m_ObjectColor=m_PointsInnerColor=m_PointsOutlineColor=NewColor; }

protected:
	int DrawLegend(CDC* pDC, CPoint UpperLeft, int BitmapWidth) const;
	CSize GetLegendSize() const;
	virtual void Draw(CDC* pDC);
	virtual void DrawAll(CDC *pDC);


private:
	int m_iLineWidth;
	int m_iPenStyle;
	int m_iXPointSize;		// Width of the points
	int m_iYPointSize;		// Height of the points
	PointType m_iPointType;

};

/*class CChartLinePointsSerie : public CChartLineSerie,public CChartPointsSerie
{
public:
	CChartLinePointsSerie(CChartCtrl* pParent):CChartSerie(pParent,stLinePointsSerie), CChartLineSerie(pParent),CChartPointsSerie(pParent){};

private:
	void Draw(CDC* pDC){CChartLineSerie::Draw(pDC); CChartPointsSerie::Draw(pDC);};
	void DrawAll(CDC *pDC){CChartLineSerie::DrawAll(pDC); CChartPointsSerie::DrawAll(pDC);};
	int DrawLegend(CDC* pDC, CPoint UpperLeft, int BitmapWidth) const{return max(CChartLineSerie::DrawLegend(pDC, UpperLeft, BitmapWidth),CChartPointsSerie::DrawLegend(pDC, UpperLeft, BitmapWidth));};
	CSize GetLegendSize() const{return CChartLineSerie::GetLegendSize();};

};*/


#endif // !defined(AFX_CHARTLINEPOINTSSERIE_H__C5D635CC_D0F8_4212_881C_B3B73DA8406C__INCLUDED_)
