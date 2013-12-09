/*
 *
 *	ChartAxis.cpp
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
 *	History:
 *		- 16/05/2006: Bug fix in ScreenToValue function
 *		- 18/05/2006: Added support for panning
 *		- 11/08/2006: Changes done for the automatic axis (Un/RegisterSeries, ...)
 *		- 12/08/2006: Tick increment can be set manually also 
 *
 */

#include "stdafx.h"
#include "ChartAxis.h"
#include "ChartAxisLabel.h"
#include "ChartGrid.h"
#include "ChartCtrl.h"

#include "Math.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CChartAxis::CChartAxis(CChartCtrl* pParent,bool bHoriz):CChartObject(pParent)
{
	m_bIsHorizontal = bHoriz;
	m_bIsInverted = false;
	m_bIsAutomatic = false;
	m_bIsLogarithmic = false;
	m_bExpandOnly=false;
	m_bIsSecondary = false;
	m_bIsTimeFormat = false;
	m_ExpandOnLower=m_ExpandOnUpper=0;
	m_MaxValue = m_UnzoomMax = 10;
	m_MinValue = m_UnzoomMin = 0;
	m_LowerOffset=0;
	m_HigherOffset=0;

	m_bAutoTicks = true;
	m_TickIncrement = 1;
	m_FirstTickVal = 0;
	m_DecCount = 0;
	m_StartPos = m_EndPos = 0;

	m_nFontSize = 80;
	m_strFontName = "Microsoft Sans Serif";

	m_pAxisGrid = new CChartGrid(pParent,this,bHoriz);
	m_pAxisLabel = new CChartAxisLabel(pParent,bHoriz);
	m_strTimeFormat="hh:mm:ss";
	m_bUseReduceLimits=false;
	m_ReduceLimitLower=0;
	m_ReduceLimitUpper=0;

}

CChartAxis::~CChartAxis()
{
	if (m_pAxisGrid)
	{
		delete m_pAxisGrid;
		m_pAxisGrid = NULL;
	}
	if (m_pAxisLabel)
	{
		delete m_pAxisLabel;
		m_pAxisLabel = NULL;
	}
}	


int CChartAxis::ClipMargin(CRect ControlRect,CRect& MarginRect,CDC* pDC) const
{
	if (!m_bIsVisible)
		return 0;

	int Size = 0;	

	CSize LabelSize = m_pAxisLabel->GetSize(pDC);
	CFont* pOldFont;
	CFont NewFont;
	NewFont.CreatePointFont(m_nFontSize,m_strFontName.c_str(),pDC);
	pOldFont = pDC->SelectObject(&NewFont);

	CString Buffer;
	Buffer.Format(_T("%.*f"),m_DecCount,m_MaxValue);
	if (m_bIsHorizontal)
	{
		Size += 4 + 1;		//Space above and under the text

		CSize TextSize = pDC->GetTextExtent(Buffer);
		Size += TextSize.cy;
		Size +=LabelSize.cy;

		if (!m_bIsSecondary)
		{
			ControlRect.bottom -= Size;
			ControlRect.right -= TextSize.cx/2+3;

			if (ControlRect.bottom < MarginRect.bottom)
				MarginRect.bottom = ControlRect.bottom;
			if (ControlRect.right < MarginRect.right)
				MarginRect.right = ControlRect.right;
		}
		else
		{
			ControlRect.top += Size;
			ControlRect.right -= TextSize.cx/2+3;

			if (ControlRect.top > MarginRect.top)
				MarginRect.top = ControlRect.top;
			if (ControlRect.right < MarginRect.right)
				MarginRect.right = ControlRect.right;
		}

	}
	else
	{
		Size += 7 + 1;		//Space before and after the text + Tick

        char szBuffer[255];
        if (!m_bIsLogarithmic)
        {
  
    		int MaxChars = abs( (int)log10(fabs(m_MaxValue) )) + 1;
    		int MinChars = abs( (int)log10(fabs(m_MinValue) )) + 1;
    		if (m_MinValue<0)
    			MinChars++;
    		if (m_MaxValue<0)
    			MaxChars++;
    		if (MaxChars>MinChars)
    			sprintf(szBuffer,"%.*f",m_DecCount,m_MaxValue);
    		else
    			sprintf(szBuffer,"%.*f",m_DecCount,m_MinValue);
       }
       else
       {
           char BuffMax[255];
           char BuffMin[255];
           int MaxDecCount = (int)log10(m_MaxValue);
           if (MaxDecCount < 0)
               MaxDecCount = -MaxDecCount;
           else
               MaxDecCount = 0;
           sprintf(BuffMax,"%.*f",MaxDecCount,m_MaxValue);
           
           int MinDecCount = (int)log10(m_MinValue);
           if (MinDecCount < 0)
                MinDecCount = -MinDecCount;
           else
               MinDecCount = 0;
           sprintf(BuffMin,"%.*f",MinDecCount,m_MinValue);
           
           if (strlen(BuffMin) > strlen(BuffMax) )
               strcpy(szBuffer,BuffMin);
           else
               strcpy(szBuffer,BuffMax);
       }

		CSize TextSize = pDC->GetTextExtent(szBuffer);
		Size += TextSize.cx;
		Size += LabelSize.cx + 2;

		if (!m_bIsSecondary)
		{
			ControlRect.left += Size;
			ControlRect.top += TextSize.cy/2+3;

			if (ControlRect.top > MarginRect.top)
				MarginRect.top = ControlRect.top;
			if (ControlRect.left > MarginRect.left)
				MarginRect.left = ControlRect.left;
		}
		else
		{
			ControlRect.right -= Size;
			ControlRect.top += TextSize.cy/2+3;

			if (ControlRect.top > MarginRect.top)
				MarginRect.top = ControlRect.top;
			if (ControlRect.right < MarginRect.right)
				MarginRect.right = ControlRect.right;
		}
	}

	pDC->SelectObject(pOldFont);
	return Size;
}

void CChartAxis::SetAutomatic(bool bNewValue)  
{ 
	m_bIsAutomatic = bNewValue; 
	//if (m_bIsAutomatic)
		//m_MinValue = m_MaxValue = 0;

	RefreshAutoAxis();
}

void CChartAxis::SetTickIncrement(bool bAuto, double Increment)
{
	m_bAutoTicks = bAuto;
	if (!m_bAutoTicks)
		m_TickIncrement = Increment;
	else
		CalculateTicksIncrement();

	CalculateFirstTick();
	m_pParent->RefreshCtrl();
}

void CChartAxis::SetAxisSize(CRect ControlRect,CRect MarginRect)
{
	if (m_bIsHorizontal)
	{
		m_StartPos = MarginRect.left;	
		m_EndPos = MarginRect.right;

		if (!m_bIsSecondary)
		{
			CRect AxisSize = ControlRect;
			AxisSize.top = MarginRect.bottom;
			SetRect(AxisSize);	
		}
		else
		{
			CRect AxisSize = ControlRect;
			AxisSize.bottom = MarginRect.top;
			SetRect(AxisSize);	
		}
	}
	else
	{
		m_StartPos = MarginRect.bottom;
		m_EndPos = MarginRect.top;

		if (!m_bIsSecondary)
		{
			CRect AxisSize = ControlRect;
			AxisSize.right = MarginRect.left;
			SetRect(AxisSize);
		}
		else
		{
			CRect AxisSize = ControlRect;
			AxisSize.left = MarginRect.right;
			SetRect(AxisSize);
		}
	}
}

void CChartAxis::Draw(CDC *pDC)
{
	if (!m_bIsVisible)
		return;

	CPen SolidPen(PS_SOLID,0,m_ObjectColor);
	CPen* pOldPen;

	CFont NewFont;
	CFont* pOldFont;
	bool EvenTick=true;
	bool DecimateLabels=false;
	if (pDC->GetSafeHdc() == NULL)
		return;

	CSize LabelSize = m_pAxisLabel->GetSize(pDC);
	int HalfAxisPos = (int)fabs((m_EndPos + m_StartPos)/2.0);
	int XPos = 0;
	int YPos = 0;
	if (m_bIsHorizontal)
	{
		if (!m_bIsSecondary)
		{
			YPos = m_ObjectRect.top  + LabelSize.cy + 2;
			XPos = HalfAxisPos - LabelSize.cx/2;
		}
		else
		{
			YPos = m_ObjectRect.top  + 2;
			XPos = HalfAxisPos - LabelSize.cx/2;
		}
	}
	else
	{
		if (!m_bIsSecondary)
		{
			YPos = HalfAxisPos + LabelSize.cy/2;
			XPos = m_ObjectRect.left + 0;
		}
		else
		{
			YPos = HalfAxisPos + LabelSize.cy/2;
			XPos = m_ObjectRect.right - LabelSize.cx - 2;
		}
	}
	m_pAxisLabel->SetPosition(XPos,YPos,pDC);
	m_pAxisLabel->Draw(pDC);

	NewFont.CreatePointFont(m_nFontSize,m_strFontName.c_str(),pDC) ;
	pOldPen = pDC->SelectObject(&SolidPen);
	pOldFont = pDC->SelectObject(&NewFont);

	CalculateTicksIncrement();

	char szBuffer[255];
	int TickCount = 0;
	int FirstTickPos = ValueToScreen(m_FirstTickVal);
    double TickValue = m_FirstTickVal;
	if (m_bIsHorizontal)
	{
		if (!m_bIsSecondary)
		{
			pDC->MoveTo(m_StartPos,m_ObjectRect.top+1);
			pDC->LineTo(m_EndPos,m_ObjectRect.top+1);
		}
		else
		{
			pDC->MoveTo(m_StartPos,m_ObjectRect.bottom-1);
			pDC->LineTo(m_EndPos,m_ObjectRect.bottom-1);
		}

		if (m_MinValue == m_MaxValue)
		{
			int HorizPos = m_StartPos + (int)fabs((m_EndPos-m_StartPos)/2.0);
			if (!m_bIsSecondary)
			{
				pDC->MoveTo(HorizPos,m_ObjectRect.top+1);
				pDC->LineTo(HorizPos,m_ObjectRect.top+4);
			}
			else
			{
				pDC->MoveTo(HorizPos,m_ObjectRect.bottom-1);
				pDC->LineTo(HorizPos,m_ObjectRect.bottom-4);
			}

            if (!m_bIsLogarithmic)
			   sprintf(szBuffer,"%.*f",m_DecCount,m_MinValue);
            else
            {
                int LogDecCount = (int)log10(m_MinValue);
                sprintf(szBuffer,"%.*f",LogDecCount,m_MinValue);
            }

			CSize TextSize;
			TextSize = pDC->GetTextExtent(szBuffer);

			if (!m_bIsSecondary)
				pDC->ExtTextOut(HorizPos-TextSize.cx/2,m_ObjectRect.top+5,ETO_CLIPPED|ETO_OPAQUE,NULL,szBuffer,NULL);
			else
				pDC->ExtTextOut(HorizPos-TextSize.cx/2,m_ObjectRect.bottom-5,ETO_CLIPPED|ETO_OPAQUE,NULL,szBuffer,NULL);
		}
		else while ( (TickValue<=m_MaxValue)  )//|| (TickValue-m_MaxValue < 0.0000001)
		{
			int HorizPos = ValueToScreen(TickValue);
			if (m_bIsInverted)
			   FirstTickPos = HorizPos;

			if (!m_bIsSecondary)
			{
				pDC->MoveTo(HorizPos,m_ObjectRect.top+1);
				pDC->LineTo(HorizPos,m_ObjectRect.top+4);
			}
			else
			{
				pDC->MoveTo(HorizPos,m_ObjectRect.bottom-1);
				pDC->LineTo(HorizPos,m_ObjectRect.bottom-4);
			}
			if(!m_bIsTimeFormat)
			{
				if (!m_bIsLogarithmic)
				   sprintf(szBuffer,"%.*f",m_DecCount,TickValue);
				else
				{
					int LogDecCount = (int)log10(TickValue);
					if (LogDecCount<0)
						LogDecCount = -(LogDecCount)-1;
					else
						LogDecCount = 0;
					sprintf(szBuffer,"%.*f",LogDecCount,TickValue);
				}
			}
			else
				strcpy(szBuffer,(LPCSTR)GetInTimeFormat(TickValue));
			CSize TextSize = pDC->GetTextExtent(szBuffer);
			int a=HorizPos-ValueToScreen(TickValue+m_TickIncrement);
			if(TextSize.cx+2>=abs(HorizPos-ValueToScreen(TickValue+m_TickIncrement)))
				DecimateLabels=true;
			if(!DecimateLabels || EvenTick)
			{
				if (!m_bIsSecondary)
					pDC->ExtTextOut(HorizPos-TextSize.cx/2,m_ObjectRect.top+5,ETO_CLIPPED|ETO_OPAQUE,NULL,szBuffer,NULL);
				else
					pDC->ExtTextOut(HorizPos-TextSize.cx/2,m_ObjectRect.bottom-5-TextSize.cy,ETO_CLIPPED|ETO_OPAQUE,NULL,szBuffer,NULL);
			}
            if (!m_bIsLogarithmic)
			   TickValue += m_TickIncrement;
            else
			   TickValue *= m_TickIncrement;
			TickCount++;
			EvenTick=!EvenTick;
		}
	} //if (m_bIsHorizontal)
	else
	{
		if (!m_bIsSecondary)
		{
			pDC->MoveTo(m_ObjectRect.right-1,m_StartPos);
			pDC->LineTo(m_ObjectRect.right-1,m_EndPos);
		}
		else
		{
			pDC->MoveTo(m_ObjectRect.left+1,m_StartPos);
			pDC->LineTo(m_ObjectRect.left+1,m_EndPos);
		}

		if (m_MinValue == m_MaxValue)
		{
			int VertPos = m_EndPos + (int)fabs((m_EndPos-m_StartPos)/2.0);

			if (!m_bIsSecondary)
			{
				pDC->MoveTo(m_ObjectRect.right-1,VertPos);
				pDC->LineTo(m_ObjectRect.right-4,VertPos);
			}
			else
			{
				pDC->MoveTo(m_ObjectRect.left+1,VertPos);
				pDC->LineTo(m_ObjectRect.left+4,VertPos);
			}

            if (!m_bIsLogarithmic)
			   sprintf(szBuffer,"%.*f",m_DecCount,TickValue);
            else
            {
                int LogDecCount = (int)(log10(TickValue));
                sprintf(szBuffer,"%.*f",LogDecCount,TickValue);
            }			
			CSize TextSize = pDC->GetTextExtent(szBuffer);

			if (!m_bIsSecondary) //Left
				pDC->ExtTextOut(LabelSize.cx+m_ObjectRect.left+2,VertPos-TextSize.cy/2,ETO_CLIPPED|ETO_OPAQUE,NULL,szBuffer,NULL);
			else				//Right
				pDC->ExtTextOut(m_ObjectRect.left+6,VertPos-TextSize.cy/2,ETO_CLIPPED|ETO_OPAQUE,NULL,szBuffer,NULL);
		}
		else while ( (TickValue<=m_MaxValue)  )//|| (TickValue-m_MaxValue < 0.0000001)
		{
			int VertPos = ValueToScreen(TickValue);
			if (!m_bIsInverted)
			   FirstTickPos = VertPos;

			if (!m_bIsSecondary)
			{
				pDC->MoveTo(m_ObjectRect.right-1,VertPos);
				pDC->LineTo(m_ObjectRect.right-4,VertPos);
			}
			else
			{
				pDC->MoveTo(m_ObjectRect.left+1,VertPos);
				pDC->LineTo(m_ObjectRect.left+4,VertPos);
			}

            if (!m_bIsLogarithmic)
			   sprintf(szBuffer,"%.*f",m_DecCount,TickValue);
            else
            {
                int LogDecCount = (int)(log10(TickValue));
                if (LogDecCount<0)
                    LogDecCount = (int)fabs(LogDecCount*1.0)+1;
                else
                    LogDecCount = 0;
                sprintf(szBuffer,"%.*f",LogDecCount,TickValue);
            }
			CSize TextSize = pDC->GetTextExtent(szBuffer);

			if(!m_bIsSecondary) //Left
				pDC->ExtTextOut(m_ObjectRect.right-TextSize.cx-7,VertPos-TextSize.cy/2,ETO_CLIPPED|ETO_OPAQUE,NULL,szBuffer,NULL);
				//pDC->ExtTextOut(m_ObjectRect.left+LabelSize.cx+4,VertPos-TextSize.cy/2,ETO_CLIPPED|ETO_OPAQUE,NULL,szBuffer,NULL);
			else				//Right
				pDC->ExtTextOut(m_ObjectRect.left+6,VertPos-TextSize.cy/2,ETO_CLIPPED|ETO_OPAQUE,NULL,szBuffer,NULL);

            if (!m_bIsLogarithmic)
			   TickValue += m_TickIncrement;
            else
			   TickValue *= m_TickIncrement;
			TickCount++;
		}
	}

	CRect Size = m_pParent->GetPlottingRect();
	m_pAxisGrid->SetRect(Size);
	double TickSpace = 0;
	if (m_bIsLogarithmic)
	{
       m_TickIncrement = 10;
	   TickSpace = log10(m_TickIncrement) * GetAxisLenght()/(log10(m_MaxValue)-log10(m_MinValue));
    }
    else
        TickSpace = m_TickIncrement * GetAxisLenght()/(m_MaxValue-m_MinValue);
    m_pAxisGrid->SetTicksCount(TickCount);
	m_pAxisGrid->Draw(pDC);

	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldFont);
}


void CChartAxis::SetMinMax(double Minimum, double Maximum)
{
	if (Minimum > Maximum)
	{
		TRACE("Maximum axis value must be > minimum axis value");
		return;
	}

	m_MinValue = m_UnzoomMin = Minimum;
	m_MaxValue = m_UnzoomMax = Maximum;
}


void CChartAxis::SetInverted(bool bNewValue)
{
	m_bIsInverted = bNewValue;
}

int CChartAxis::GetAxisLenght() const
{
    int Length = (int)fabs( (m_EndPos-m_StartPos) * 1.0);
    return max(1,Length);
}

void CChartAxis::CalculateTicksIncrement()
{
	if (!m_bAutoTicks)
	{
		CalculateFirstTick();
		return;
	}

	//Calculate the appropriate TickSpace (1 tick every 30 pixel +/-)
	if (m_bIsTimeFormat)
	{
    	int PixelSpace;
		int OptUSec[]={1,2,5,10,20,50};
		int OptMinSec[]={1,2,3,5,10,15,20,30};
		int OptHours[]={1,2,3,4,5,6,8,10,12};
    	if (m_bIsHorizontal)
    		PixelSpace = 32;
    	else
    		PixelSpace = 20;
    
    	int MaxTickNumber = (int)fabs((m_EndPos-m_StartPos)/PixelSpace * 1.0);

    	//Best Tick Space
    	double TempTickSpace = (m_MaxValue-m_MinValue)/MaxTickNumber;
		double MinTickSpace=.01;
		int i;
		for (i=0;i<sizeof(OptUSec)/sizeof(int);i++)
		{
			if (MinTickSpace*OptUSec[i]>=TempTickSpace)
			{
	    		m_TickIncrement = MinTickSpace*OptUSec[i];
				goto End;
			}
		}
		MinTickSpace=1;
		for (i=0;i<sizeof(OptMinSec)/sizeof(int);i++)
		{
			if (MinTickSpace*OptMinSec[i]>=TempTickSpace)
			{
	    		m_TickIncrement = MinTickSpace*OptMinSec[i];
				goto End;
			}
		}
		MinTickSpace=60;
		for (i=0;i<sizeof(OptMinSec)/sizeof(int);i++)
		{
			if (MinTickSpace*OptMinSec[i]>=TempTickSpace)
	    	{
	    		m_TickIncrement = MinTickSpace*OptMinSec[i];
				goto End;
			}
		}
		MinTickSpace=3600;
		for (i=0;i<sizeof(OptHours)/sizeof(int);i++)
		{
			if (MinTickSpace*OptHours[i]>=TempTickSpace)
			{
	    		m_TickIncrement = MinTickSpace*OptHours[i];
				goto End;
			}
		}
		MinTickSpace=24*3600;
	}
	else if (m_bIsLogarithmic)
	   m_TickIncrement = 10;
    else
    {
    	int PixelSpace;
    	if (m_bIsHorizontal)
    		PixelSpace = 25;
    	else
    		PixelSpace = 20;
    
    	int MaxTickNumber = (int)fabs((m_EndPos-m_StartPos)/PixelSpace * 1.0);
    
    	//Best Tick Space
    	double TempTickSpace = (m_MaxValue-m_MinValue)/MaxTickNumber;
    
    	// Calculate appropriate tickSpace (not rounded on 'strange values' but 
    	// on something like 1, 2 or 5*10^X  where X is optimalized for showing the most
    	// significant digits)
    	int Zeros = (int)floor(log10(TempTickSpace));
    	double MinTickSpace = pow(10.0,Zeros);
    
    	int Digits = 0;
    	if (Zeros<0)		//must set digits
    	{
    		Digits = (int)fabs(Zeros*1.0);
    	}
    
    	if (MinTickSpace>=TempTickSpace)
    	{
    		m_TickIncrement = MinTickSpace;
    		SetDecimals(Digits);
    	}
    	else if (MinTickSpace*2>=TempTickSpace)
    	{
    		m_TickIncrement = MinTickSpace*2;
    		SetDecimals(Digits);
    	}
    	else if (MinTickSpace*5>=TempTickSpace)
    	{
    		m_TickIncrement = MinTickSpace*5;
    		SetDecimals(Digits);
    	}
    	else if (MinTickSpace*10>=TempTickSpace)
    	{
    		m_TickIncrement = MinTickSpace*10;
    		if (Digits)
    			SetDecimals(Digits-1);
    		else
    			SetDecimals(Digits);
    	}
    }
End:
	if(m_MinValue+m_TickIncrement==m_MinValue)
		m_TickIncrement=m_MaxValue-m_MinValue;
	CalculateFirstTick();
}
	
void CChartAxis::CalculateFirstTick()
{
	if (m_bIsLogarithmic)
	{
        int LogBase = (int)log10(m_MinValue);
        m_FirstTickVal = pow(10.0,LogBase);
     }
    else
    {
    	m_FirstTickVal = 0;
    	if (m_TickIncrement!=0)
    	{
    		if (m_MinValue == 0)
    			m_FirstTickVal = 0;
    		else if (m_MinValue>0)
    		{
				double Ticks=m_MinValue/m_TickIncrement;
				Ticks=(Ticks>=0)?floor(Ticks):ceil(Ticks);
    			m_FirstTickVal = Ticks * m_TickIncrement;
    			while (m_FirstTickVal<m_MinValue)
    				m_FirstTickVal += m_TickIncrement;
    		}
    		else
    		{
				double Ticks=m_MinValue/m_TickIncrement;
				Ticks=(Ticks>=0)?floor(Ticks):ceil(Ticks);
    			m_FirstTickVal = Ticks * m_TickIncrement;
    			while (m_FirstTickVal>m_MinValue)
    				m_FirstTickVal -= m_TickIncrement;
    			if (!(m_FirstTickVal == m_MinValue))
    				m_FirstTickVal += m_TickIncrement;
    		}
    	}
     }
}

long CChartAxis::GetTickScreenPos(int TickIndex) const
{
	double TickVal = 0;
    if (!m_bIsLogarithmic)
		TickVal = m_FirstTickVal + TickIndex * m_TickIncrement;
	else
	   TickVal = m_FirstTickVal * pow(m_TickIncrement,TickIndex);
	return ValueToScreen(TickVal);
}

long CChartAxis::ValueToScreen(double Value) const
{
    long Offset = 0;
    if (m_bIsLogarithmic)
        Offset = (int)floor((log10(Value)-log10(m_MinValue)) * GetAxisLenght()/(log10(m_MaxValue)-log10(m_MinValue)) );
    else
    {
		if(m_MaxValue!=m_MinValue)
			Offset = (int)floor( (Value - m_MinValue) * GetAxisLenght()/(m_MaxValue-m_MinValue) );
		else
			Offset=GetAxisLenght()/2;
		//double a=(Value - m_MinValue) * GetAxisLenght()/(m_MaxValue-m_MinValue);
		//double b=GetAxisLenght();
    }
 
    if (m_bIsHorizontal)
    {
	   if (!m_bIsInverted)
		  return (m_StartPos + Offset);
	   else
		   return (m_EndPos - Offset);
     }
     else
     {
    	if (!m_bIsInverted)
    		return (m_StartPos - Offset);
    	else
    		return (m_EndPos + Offset);
     }
}

double CChartAxis::ScreenToValue(long ScreenVal) const
{
    int AxisOffset=0;
	if (!m_bIsHorizontal)
	{
		if (m_bIsInverted)
			AxisOffset = ScreenVal - m_EndPos;      
		else
			AxisOffset = m_StartPos - ScreenVal;
        
	}
	else
	{
		if (!m_bIsInverted)
		   AxisOffset = ScreenVal - m_StartPos;
		else
			AxisOffset = m_EndPos - ScreenVal;
	}
	if (!m_bIsLogarithmic)
       return ( (AxisOffset * 1.0 / GetAxisLenght()*(m_MaxValue-m_MinValue)) + m_MinValue);
    else
        return (pow(10.0,(AxisOffset *1.0 / GetAxisLenght()*(log10(m_MaxValue)-log10(m_MinValue)) ) + log10(m_MinValue)) );

}

double CChartAxis::ScreenToValueRel(long Pixels) const
{
	if (!m_bIsLogarithmic)
       return (Pixels * 1.0 / GetAxisLenght()*(m_MaxValue-m_MinValue));
    else
        return 0;
}
void CChartAxis::PanAxis(long PanStart, long PanEnd)
{
	double StartVal = ScreenToValue(PanStart);
	double EndVal = ScreenToValue(PanEnd);

	if (!m_bIsLogarithmic)
	{
		double Shift = StartVal - EndVal;
		SetZoomMinMax(m_MinValue+Shift,m_MaxValue+Shift);
	}
	else
	{
		double Factor = StartVal/EndVal;
		SetZoomMinMax(m_MinValue*Factor,m_MaxValue*Factor);
	}
}

void CChartAxis::SetZoomMinMax(double Minimum, double Maximum)
{
	if (Minimum > Maximum)
	{
		TRACE("Maximum axis value must be > minimum axis value");
		return;
	}

	m_MinValue = Minimum;
	m_MaxValue = Maximum;
}

void CChartAxis::UndoZoom()
{
	SetMinMax(m_UnzoomMin,m_UnzoomMax);
}

void CChartAxis::RegisterSeries(CChartSerie* pSeries)
{
	// First check if the series is already present in the list
	SeriesList::iterator iter = m_pRelatedSeries.begin();
	for (iter; iter!=m_pRelatedSeries.end(); iter++)
	{
		if ( (*iter) == pSeries)
			return;
	}

	m_pRelatedSeries.push_back(pSeries);
}

void CChartAxis::UnregisterSeries(CChartSerie* pSeries)
{
	SeriesList::iterator iter = m_pRelatedSeries.begin();
	for (iter; iter!=m_pRelatedSeries.end(); iter++)
	{
		if ( (*iter) == pSeries)
		{
			m_pRelatedSeries.erase(iter);
			return;
		}
	}
}


void CChartAxis::RefreshAutoAxis()
{
	if (!m_bIsAutomatic)
		return;

	double SeriesMin = 1e300;
	double SeriesMax = -1e300;
	double TempMin = 1e300;
	double TempMax = -1e300;
	

	SeriesList::iterator iter = m_pRelatedSeries.begin();
	if (iter != m_pRelatedSeries.end())
	{
		if (m_bIsHorizontal)
			(*iter)->GetSerieXMinMax(SeriesMin,SeriesMax);
		else
			(*iter)->GetSerieYMinMax(SeriesMin,SeriesMax);
	}

	for (iter; iter!=m_pRelatedSeries.end(); iter++)
	{
		if (m_bIsHorizontal)
			(*iter)->GetSerieXMinMax(TempMin,TempMax);
		else
			(*iter)->GetSerieYMinMax(TempMin,TempMax);

		if (TempMin < SeriesMin)
			SeriesMin = TempMin;
		if (TempMax > SeriesMax)
			SeriesMax = TempMax;
	}

	if(m_bUseReduceLimits)
	{
		if(SeriesMin>m_ReduceLimitLower)
			SeriesMin=m_ReduceLimitLower;
		if(SeriesMax<m_ReduceLimitUpper)
			SeriesMax=m_ReduceLimitUpper;
	}

	if(!IsLogarithmic())
	{
		double LogicSpan=max(0,SeriesMax-SeriesMin);
		double RelExtensionH=(double)m_HigherOffset/GetAxisLenght();
		double RelExtensionL=(double)m_LowerOffset/GetAxisLenght();
		double HigherOffset=LogicSpan*RelExtensionH/max(1-RelExtensionH,0.01);
		double LowerOffset=LogicSpan*RelExtensionL/max(1-RelExtensionL,0.01);;

		SeriesMin-=LowerOffset;
		SeriesMax+=HigherOffset;
	}

	if(m_bExpandOnly)
	{
		if (m_MinValue < SeriesMin)
			SeriesMin = m_MinValue;
		if (m_MaxValue > SeriesMax)
			SeriesMax = m_MaxValue;		
	}

	if(m_ExpandOnLower>0)
		SeriesMin=floor(SeriesMin/m_ExpandOnLower)*m_ExpandOnLower;
	if(m_ExpandOnUpper>0)
		SeriesMax=ceil(SeriesMax/m_ExpandOnUpper)*m_ExpandOnUpper;
	if(SeriesMin>SeriesMax)
		SeriesMin=SeriesMax=0;
	if ( (SeriesMax!=m_MaxValue) || (SeriesMin!=m_MinValue) )
	{
		SetMinMax(SeriesMin,SeriesMax);
		m_pParent->RefreshCtrl();
	}

}

//if ExpandOnly==true, (Min,Max) range cannot become narrower; Default false.
//ExpandOn (if >0)means that (Min<=n*ExpandOn) && (Max>=m*ExpandOn), where n,m are integer
//#define EXPAND_ONLY_ON true
//#define EXPAND_ONLY_OFF false
void CChartAxis::SetAutomaticMode(bool ExpandOnly, double ExpandOnLower, double ExpandOnUpper)
{
	m_bExpandOnly=ExpandOnly;
	m_ExpandOnLower=ExpandOnLower;
	m_ExpandOnUpper=ExpandOnUpper;
}

void CChartAxis::SetAutomaticReduceLimits(bool UseLimits, double Lower, double Upper)
{
	m_bUseReduceLimits=UseLimits;
	m_ReduceLimitLower=Lower;
	m_ReduceLimitUpper=Upper;
}

//If EnableTimeFormat==true, can display time interpteting coordinate as seconds
//Format can be:
//1. "hh:mm:ss", "h:mm:ss", "m:ss", "h:mm", "m:ss.uu", "m:ss.uuu" etc.
//   If a letter is double this results to minimum 2 digits (single means minimum 1 digit)
//   If time unit overflow it will rollover and cannot be greater 23 or 59
//2. "*hh:mm:ss", "*h:mm:ss", "*m:ss.uu",...
//   The same as above, but if upper time unit is insufficient to contain 
//   all time it can become greater than 23 or 59
//3. "M:ss", "*M:ss", "SS", "s.U", "s.UUU"...
//   Upper letter instructs to extend format to minutes, hours if they are nonzero, beginning
//   with minutes or seconds. Capital U means displaying milliseconds if they are, but not 
//   longer then number of this letters. "*M:ss" means displaying more then 23 hours
void CChartAxis::SetTimeFormat(bool EnableTimeFormat, const char *Format)
{
	m_bIsTimeFormat=EnableTimeFormat;
	if(Format!=NULL)
		if(strlen(Format)>0)
		{
			m_strTimeFormat=Format;
			m_aTimeElementFormat.RemoveAll();
			m_vTimeElementType.clear();
			CString TmpFormat;
			int j;
			int Length=m_strTimeFormat.GetLength();
			m_eExpandStyle=expand_none;

			for(int i=0;i<Length;)
			{
				if(m_strTimeFormat[0]=='*' && i==0)
				{
					m_eExpandStyle=expand_numeric;
					i++;
				}
				else if(m_strTimeFormat[i]=='h')
				{
					for(j=1;i+j<Length;j++)
					{
						if(m_strTimeFormat[i+j]!='h')
							break;
					}
					TmpFormat.Format("%%.%ii",j);
					m_aTimeElementFormat.Add(TmpFormat);
					m_vTimeElementType.push_back(t_hours);
					i+=j;
				}
				else if(m_strTimeFormat[i]=='m' || m_strTimeFormat[i]=='M')
				{
					if(m_strTimeFormat[i]=='M')
						m_eExpandStyle=expand_timelike;
					for(j=1;i+j<Length;j++)
					{
						if(m_strTimeFormat[i+j]!='m')
							break;
					}
					TmpFormat.Format("%%.%ii",j);
					m_aTimeElementFormat.Add(TmpFormat);
					m_vTimeElementType.push_back(t_minutes);
					i+=j;
				}
				else if(m_strTimeFormat[i]=='s' || m_strTimeFormat[i]=='S')
				{
					if(m_strTimeFormat[i]=='S')
						m_eExpandStyle=expand_timelike;
					for(j=1;i+j<Length;j++)
					{
						if(m_strTimeFormat[i+j]!='s')
							break;
					}
					TmpFormat.Format("%%.%ii",j);
					m_aTimeElementFormat.Add(TmpFormat);
					m_vTimeElementType.push_back(t_seconds);
					i+=j;
				}
				else if(m_strTimeFormat[i]=='u'|| m_strTimeFormat[i]=='U')
				{
					if(m_strTimeFormat[i]=='u')
						m_bSupressTrailingMs=false;
					else
						m_bSupressTrailingMs=true;

					for(j=1;i+j<Length;j++)
					{
						if(m_strTimeFormat[i+j]!='u' && m_strTimeFormat[i+j]!='U')
							break;
					}
					TmpFormat.Format("%%.%if",j);

					m_aTimeElementFormat.Add(TmpFormat);
					m_vTimeElementType.push_back(t_milliseconds);
					i+=j;
				}
				else
				{
					j=1;
					for(j=1;i+j<Length;j++)
					{
						if((m_strTimeFormat[i+j]=='h') || (m_strTimeFormat[i+j]=='m') || 
							(m_strTimeFormat[i+j]=='s') || (m_strTimeFormat[i+j]=='u') ||
							(m_strTimeFormat[i+j]=='H') || (m_strTimeFormat[i+j]=='M') || 
							(m_strTimeFormat[i+j]=='S') || (m_strTimeFormat[i+j]=='U'))
							break;
					}
					TmpFormat=m_strTimeFormat.Mid(i,j);
					m_aTimeElementFormat.Add(TmpFormat);
					m_vTimeElementType.push_back(t_char);
					i+=j;
				}

			}
		}
}

CString CChartAxis::GetInTimeFormat(double Value)
{
	CString Time, str, TmpFormat;
	int Hours=abs(Value/3600);
	int Minutes=abs((int)(Value/60))%60;
	int Seconds=abs((int)Value)%60;
	double Milliseconds=fabs(Value)-floor(fabs(Value));
	int Sign=(Value>=0)?1:-1;

	char *Minus="-";
	bool WasFirstTimeVal=false;
	int i;
	for(i=0; i<m_aTimeElementFormat.GetSize(); i++)
	{
		TmpFormat=m_aTimeElementFormat[i];
		type t=(type)m_vTimeElementType[i];
		switch (t)
		{
		case t_hours:
			str.Format(TmpFormat,Hours);
			if(!WasFirstTimeVal && (Sign==-1))
				Time+=Minus;
			Time+=str;
			WasFirstTimeVal=true;
			break;
		case t_minutes:
			if(m_eExpandStyle==expand_none || WasFirstTimeVal)
				str.Format(TmpFormat,Minutes);
			else if(m_eExpandStyle==expand_numeric)
				str.Format(TmpFormat,WasFirstTimeVal?Minutes:Minutes+Hours*60);
			else if(m_eExpandStyle==expand_timelike)
			{
				if(Hours>0)
					str.Format("%i:%.2i",Hours,Minutes);
				else
					str.Format(TmpFormat,Minutes);
			}
			if(!WasFirstTimeVal && (Sign==-1))
				Time+=Minus;
			Time+=str;
			WasFirstTimeVal=true;
			break;
		case t_seconds:
			if(m_eExpandStyle==expand_none || WasFirstTimeVal)
				str.Format(TmpFormat,Seconds);
			else if(m_eExpandStyle==expand_numeric)
				str.Format(TmpFormat,WasFirstTimeVal?Seconds:Seconds+Minutes*60+Hours*3600);
			else if(m_eExpandStyle==expand_timelike)
			{
				if(Hours>0)
					str.Format("%i:%.2i:%.2i",Hours,Minutes,Seconds);
				else if(Minutes>0)
					str.Format("%i:%.2i",Minutes,Seconds);
				else
					str.Format(TmpFormat,Seconds);
			}
			if(!WasFirstTimeVal && (Sign==-1))
				Time+=Minus;
			Time+=str;
			WasFirstTimeVal=true;
			break;
		case t_milliseconds:
			str.Format(TmpFormat,Milliseconds);
			if(!WasFirstTimeVal && (Sign==-1))
				Time+=Minus;
			if(WasFirstTimeVal)
			{
				Time+=str.Mid(str.Find('.')+1);
			}
			else
				Time+=str;
			if(m_bSupressTrailingMs)
			{
				while(Time[Time.GetLength()-1]=='0')
					Time.Delete(Time.GetLength()-1);
				if(Time[Time.GetLength()-1]=='.')
					Time.Delete(Time.GetLength()-1);
			}

			WasFirstTimeVal=true;
			break;
		default:
			Time+=TmpFormat;
		}
	}
	return Time;
}

void CChartAxis::SetPixelOffsets(int Lower, int Higher)
{
	m_LowerOffset=Lower;
	m_HigherOffset=Higher;
}
