// ChartCursor.h: interface for the CChartCursor class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHARTCURSOR_H__0ACA7FF3_3C8A_458C_97C2_F3D3F73280C4__INCLUDED_)
#define AFX_CHARTCURSOR_H__0ACA7FF3_3C8A_458C_97C2_F3D3F73280C4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ChartObject.h"
class CChartAxis;

class CChartCursor : public CChartObject  
{
friend CChartCtrl;

public:
	CChartCursor(CChartCtrl* pParent);
	virtual ~CChartCursor();

	int  GetPenStyle() const        { return m_iPenStyle; }
	void SetPenStyle(int NewStyle)  { m_iPenStyle = NewStyle; }

	int	 GetWidth() const       { return m_iLineWidth; }
	void SetWidth(int NewValue)  { m_iLineWidth = NewValue; }

	double	 GetCoord() const       { return m_dCoord; }
	void SetCoord(double NewCoord);
	CRect m_rScreenRect;

private:
	double m_dCoord;
	bool m_bHorizontal;
	bool m_bFixed;
	int m_iLineWidth;
	int m_iPenStyle;
	void Draw(CDC* pDC);
	void DrawEx(CDC* pDC);
	CChartAxis* m_pHorizontalAxis;
	CChartAxis* m_pVerticalAxis;
	void ValueToScreen(double XValue, double YValue, CPoint &ScreenPoint) const;
	void ScreenToValue(double &XValue, double &YValue, CPoint ScreenPoint) const;
	bool IsMouseOver(CPoint ScreenPoint){return m_rScreenRect.PtInRect(ScreenPoint) && !m_bFixed;}
	void RecalulateRect();
};

#endif // !defined(AFX_CHARTCURSOR_H__0ACA7FF3_3C8A_458C_97C2_F3D3F73280C4__INCLUDED_)
