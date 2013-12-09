/*
 *
 *	ChartAxis.h
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

#if !defined(AFX_CHARTAXIS_H__063D695C_43CF_4A46_8AA0_C7E00268E0D3__INCLUDED_)
#define AFX_CHARTAXIS_H__063D695C_43CF_4A46_8AA0_C7E00268E0D3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ChartObject.h"
#include <afx.h>

#include <string>
#include <list>

class CChartGrid;
class CChartSerie;
class CChartAxisLabel;
#define EXPAND_ONLY_ON true
#define EXPAND_ONLY_OFF false
    
class CChartAxis : public CChartObject  
{
	friend CChartCtrl;
	friend CChartGrid;
	friend CChartSerie;

public:
	void SetPixelOffsets(int Lower, int Higher);
	void SetTimeFormat(bool EnableTimeFormat, const char* Format=NULL);
	void SetAutomaticMode(bool ExpandOnly, double ExpandOnLower, double ExpandOnUpper);
	void SetAutomaticReduceLimits(bool UseLimits, double Lower=0, double Upper=0);

	void SetInverted(bool bNewValue);
	bool IsInverted() const  { return m_bIsInverted; }
	void SetLogarithmic(bool bNewValue) { m_bIsLogarithmic = bNewValue; }
	bool IsLogarithmic() const          { return m_bIsLogarithmic; }
	void SetAutomatic(bool bNewValue);  
	bool IsAutomatic()  const  { return m_bIsAutomatic; }

	void SetMinMax(double Minimum, double Maximum);
	void GetMinMax(double& Minimum, double& Maximum) const
	{
		Minimum = m_MinValue;
		Maximum = m_MaxValue;
	}

    long   ValueToScreen(double Value) const;
    double ScreenToValue(long ScreenVal) const;
	double ScreenToValueRel(long Pixels) const;
	void SetFont(int nPointSize,std::string  strFaceName)
	{
		m_nFontSize = nPointSize;
		m_strFontName = strFaceName;
	}

	void   SetTickIncrement(bool bAuto, double Increment);
	double GetTickIncrement() const  { return m_TickIncrement; }

	CChartAxisLabel* GetLabel() const  { return m_pAxisLabel; }
	CChartGrid*		 GetGrid()	const  { return m_pAxisGrid;  }

	CChartAxis(CChartCtrl* pParent,bool bHoriz);
	virtual ~CChartAxis();


private:
	void PanAxis(long PanStart, long PanEnd);
	void SetZoomMinMax(double Minimum, double Maximum);
	void UndoZoom();

	void SetDecimals(int NewValue)  { m_DecCount = NewValue; }
	bool IsHorizontal() const  { return m_bIsHorizontal; }

    int  GetAxisLenght() const;
    void CalculateTicksIncrement();
	void CalculateFirstTick();
	void SetSecondary(bool bNewVal)  { m_bIsSecondary = bNewVal; }

	void RefreshAutoAxis();
	void FormatValue(char* outBuffer, int precision, double value, double prevValue, bool havePrevValue) const;

	void SetAxisSize(CRect ControlRect,CRect MarginRect);
	int ClipMargin(CRect ControlRect,CRect& MarginRect,CDC* pDC) const;	// Allows to calculate the margin required to displayys ticks and text

	long GetTickScreenPos(int TickIndex) const;

	void Draw(CDC* pDC);

	// To register/Unregister series related to this axis
	void RegisterSeries(CChartSerie* pSeries);
	void UnregisterSeries(CChartSerie* pSeries);


	bool m_bIsHorizontal;	  // Indicates if this is an horizontal or vertical axis
	bool m_bIsInverted;		  // Indicates if the axis is inverted
	bool m_bIsAutomatic;      // Indicates if the axis is automatic
	bool m_bIsLogarithmic;    // Indicates if the axis is logarithmic
	bool m_bIsTimeFormat;	// If EnableTimeFormat==true, can display time interpteting coordinate as seconds
	bool m_bIsSecondary;	// If the axis is secondary, it will be positioned to 
                            // the right (vertical) or to the top (horizontal)
	double m_MaxValue;		// Maximum value on the axis
	double m_MinValue;		
	double m_UnzoomMin;		// Min and max values of the axis before it has been zoomed
	double m_UnzoomMax;		// (used when we unzoom the chart -> go back to previous state)

	bool   m_bAutoTicks;		// Specify if the tick increment is manual or automatic
	bool   m_bExpandOnly;		// If ExpandOnly==true, (Min,Max) range cannot become narrower; Default false.
	double m_ExpandOnLower;			// ExpandOn (if >0) means that (Min<=n*ExpandOn) && (Max>=m*ExpandOn), where n,m are integer
	double m_ExpandOnUpper;
	double m_TickIncrement;		// Indicates the space between ticks (in axis value) or for log axis the mult base between two ticks
	double m_FirstTickVal;
	CString m_strTimeFormat;		//Format can be "hh:mm:ss", "h:mm:ss", "m:ss", "h:mm" etc.
	unsigned int m_DecCount;	// Number of decimals to display
	int m_StartPos;			    // Start position of the axis 
	int m_EndPos;
	int m_LowerOffset;
	int m_HigherOffset;

	int  m_nFontSize;			
	std::string m_strFontName;

	enum type {t_hours, t_minutes, t_seconds, t_milliseconds, t_char};
	std::vector<int> m_vTimeElementType;
	CStringArray m_aTimeElementFormat;
	enum expand_style {expand_none, expand_numeric, expand_timelike};
	expand_style m_eExpandStyle;
	bool m_bSupressTrailingMs;
	CChartGrid*			m_pAxisGrid;
	CChartAxisLabel*	m_pAxisLabel;

	bool m_bUseReduceLimits;
	double m_ReduceLimitLower;
	double m_ReduceLimitUpper;

	typedef std::list<CChartSerie*> SeriesList;
	SeriesList m_pRelatedSeries;		// List containing pointers to series related to this axis
protected:
	CString GetInTimeFormat(double Value);
};

#endif // !defined(AFX_CHARTAXIS_H__063D695C_43CF_4A46_8AA0_C7E00268E0D3__INCLUDED_)
