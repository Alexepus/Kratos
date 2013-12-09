/*
 *
 *	ChartAxisLabel.cpp
 *
 *	Written by C�dric Moonen (cedric_moonen@hotmail.com)
 *
 *
 *
 *	This code may be used for any non-commercial and commercial purposes in a compiled form.
 *	The code may be redistributed as long as it remains unmodified and providing that the 
 *	author name and this disclaimer remain intact. The sources can be modified WITH the author 
 *	consent only.
 *	
 *	This code is provided without any garanties. I cannot be held responsible for the damage or
 *	the loss of time it causes. Use it at your own risks
 *
 *	An e-mail to notify me that you are using this code is appreciated also.
 *
 *
 */

#include "stdafx.h"
#include "ChartAxisLabel.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CChartAxisLabel::CChartAxisLabel(CChartCtrl* pParent, bool bHorizontal):CChartObject(pParent)
{
	m_bIsHorizontal = bHorizontal;

	m_iFontSize = 80;
	m_strFontName = "Microsoft Sans Serif";

	m_strLabelText = "";
}

CChartAxisLabel::~CChartAxisLabel()
{

}

CSize CChartAxisLabel::GetSize(CDC *pDC) const
{
	CSize LabelSize;
	LabelSize.cx = 0;
	LabelSize.cy = 0;

	if (!m_bIsVisible)
		return LabelSize;
	if (!pDC->GetSafeHdc())
		return LabelSize;
	if (m_strLabelText == "")
		return LabelSize;

	CFont NewFont;
	CFont* pOldFont;
	NewFont.CreatePointFont(m_iFontSize,m_strFontName.c_str(),pDC);
	pOldFont = pDC->SelectObject(&NewFont);

	LabelSize = pDC->GetTextExtent(m_strLabelText.c_str());
	LabelSize.cx += 4;
	LabelSize.cy += 4;
	if (!m_bIsHorizontal)
	{
		int Width = LabelSize.cy;
		int Height = LabelSize.cx;
		LabelSize.cx = Width;
		LabelSize.cy = Height;
	}
	pDC->SelectObject(pOldFont);

	return LabelSize;
}

void CChartAxisLabel::Draw(CDC *pDC)
{
	if (!m_bIsVisible)
		return;
	if (!pDC->GetSafeHdc())
		return;
	if (m_strLabelText == "")
		return;

	CFont NewFont;
	CFont* pOldFont;
	NewFont.CreatePointFont(m_iFontSize,m_strFontName.c_str(),pDC);

	if (!m_bIsHorizontal)
	{
		LOGFONT* pLogFont = new LOGFONT;
		NewFont.GetLogFont(pLogFont);
		pLogFont->lfOrientation = 900;
		pLogFont->lfEscapement = 900;

		CFont VertFont;
		VertFont.CreateFontIndirect(pLogFont);
		pOldFont = pDC->SelectObject(&VertFont);
		delete pLogFont;

		pDC->ExtTextOut(m_ObjectRect.left + 2,m_ObjectRect.top,
					ETO_CLIPPED,NULL,m_strLabelText.c_str(),NULL);
	}
	else
	{
		pOldFont = pDC->SelectObject(&NewFont);
		pDC->ExtTextOut(m_ObjectRect.left,m_ObjectRect.top + 2,
					ETO_CLIPPED,NULL,m_strLabelText.c_str(),NULL);
	}

	pDC->SelectObject(pOldFont);
}

void CChartAxisLabel::SetPosition(int LeftBorder, int TopBorder, CDC *pDC)
{
	CSize NewSize = GetSize(pDC);
	CRect NewRect;
	NewRect.top = TopBorder;
	NewRect.bottom = TopBorder + NewSize.cy;
	NewRect.left = LeftBorder;
	NewRect.right = LeftBorder + NewSize.cx;

	CChartObject::SetRect(NewRect);
}
