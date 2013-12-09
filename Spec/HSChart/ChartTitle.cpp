/*
 *
 *	ChartTitle.cpp
 *
 *	Written by Cédric Moonen (cedric_moonen@hotmail.com)
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
#include "ChartTitle.h"
#include "ChartCtrl.h"
#include "Math.h"

using namespace std;

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CChartTitle::CChartTitle(CChartCtrl* pParent) : CChartObject(pParent)
{
	CChartObject::SetColor(RGB(0,0,0));
	m_iFontSize = 12;
	m_strFontName = "Microsoft Sans Serif";
	m_bBold=false;
	m_bItalic=false;
}

CChartTitle::~CChartTitle()
{
	if(m_Font.GetSafeHandle())
		m_Font.DeleteObject(); 
}

void CChartTitle::AddString(std::string NewString)
{
	m_StringArray.push_back(NewString);
}

size_t CChartTitle::GetStringCount() const
{
	return m_StringArray.size();
}

string CChartTitle::GetString(size_t Index) const
{
	if ( (Index<0) || (Index>=m_StringArray.size()) )
		return "";
	return m_StringArray[Index];
}

void CChartTitle::RemoveAll()
{
	m_StringArray.clear();
}

void CChartTitle::SetFont(int iPointSize,std::string strFaceName, bool bBold, bool bItalic)
{
	m_iFontSize = iPointSize;
	m_strFontName = strFaceName;
	m_bBold=bBold;
	m_bItalic=bItalic;
	if(m_Font.GetSafeHandle())
		m_Font.DeleteObject(); 
}


void CChartTitle::Draw(CDC *pDC)
{
	if (!pDC->GetSafeHdc())
		return;
	if (!m_bIsVisible)
		return;
	if(!m_Font.GetSafeHandle())
	{
	VERIFY(m_Font.CreateFont(
	   -MulDiv(m_iFontSize, GetDeviceCaps(pDC->GetSafeHdc(), LOGPIXELSY), 72),// nHeight
	   0,                         // nWidth
	   0,                         // nEscapement
	   0,                         // nOrientation
	   m_bBold? FW_BOLD:FW_NORMAL, // nWeight
	   m_bItalic,                  // bItalic
	   FALSE,                     // bUnderline
	   0,                         // cStrikeOut
	   ANSI_CHARSET,              // nCharSet
	   OUT_DEFAULT_PRECIS,        // nOutPrecision
	   CLIP_DEFAULT_PRECIS,       // nClipPrecision
	   DEFAULT_QUALITY,           // nQuality
	   DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
	   m_strFontName.c_str()));   
	}
	CPen SolidPen(PS_SOLID,0,m_ObjectColor);
	CPen* pOldPen;
	CFont* pOldFont;
	pOldFont = pDC->SelectObject(&m_Font);
	pOldPen = pDC->SelectObject(&SolidPen);

	//Draw all entries
	int YPos = 0;
	size_t TitleCount = m_StringArray.size();
	for (size_t i=0;i<TitleCount;i++)
	{
		//Draw Text
		int TextWidth = pDC->GetTextExtent(m_StringArray[i].c_str()).cx;
		int TextHeigh = pDC->GetTextExtent(m_StringArray[i].c_str()).cy;

		int XPos = m_ObjectRect.left + (int)fabs((m_ObjectRect.left-m_ObjectRect.right)/2.0) - TextWidth/2;
		pDC->ExtTextOut(XPos,m_ObjectRect.top+YPos,ETO_CLIPPED,NULL,m_StringArray[i].c_str(),NULL);

		YPos += TextHeigh + 0;
	}

	pDC->SelectObject(pOldFont);
	pDC->SelectObject(pOldPen);
}

CSize CChartTitle::GetSize(CDC *pDC)
{
	CSize TitleSize;

	if (!m_bIsVisible)
	{
		TitleSize.cx = TitleSize.cy = 0;
		return TitleSize;
	}

	CFont* pOldFont;
	pOldFont = pDC->SelectObject(&m_Font);

	int Height = 0;		//Upper space
	CSize TextSize = 0;
	int MaxTextWidth = 0;

	size_t TitleCount = m_StringArray.size();
	if (TitleCount==0)
	{
		TitleSize.cx = TitleSize.cy = 0;
		return TitleSize;
	}

	for (size_t i=0;i<TitleCount;i++)
	{
		TextSize = pDC->GetTextExtent(m_StringArray[i].c_str());
		Height += TextSize.cy;
		if (TextSize.cx > MaxTextWidth)
			MaxTextWidth = TextSize.cx;
	}

	TitleSize.cx = MaxTextWidth + 2;
	TitleSize.cy = Height-10;

	m_ObjectRect.bottom = m_ObjectRect.top + Height;

	pDC->SelectObject(pOldFont);
	return TitleSize;
}

