/*
 *
 *	ChartCtrl.h
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


#if !defined(AFX_CHARTCTRL_H__5DDBAE42_77C3_4344_A207_3F39B623343B__INCLUDED_)
#define AFX_CHARTCTRL_H__5DDBAE42_77C3_4344_A207_3F39B623343B__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "ChartSerie.h"
#include "ChartLineSerie.h"
#include "ChartPointsSerie.h"
#include "ChartLinePointsSerie.h"
#include "ChartTrapeziumSerie.h"
#include "ChartCursor.h"
#include "ChartAxis.h"
#include "ChartGrid.h"
#include "ChartLegend.h"
#include "ChartTitle.h"
#include "ChartAxisLabel.h"
#include "..\\LogToFile.h"
#include <afxmt.h>
	
#include <vector>
#pragma warning (disable:4786)

#define WM_ON_DRAGCURSOR	(WM_USER + 10)


/////////////////////////////////////////////////////////////////////////////
// CChartCtrl window

class CChartCtrl : public CWnd
{
friend class CChartCursor;
friend class CChartSerie;

public:
	CCriticalSection m_CritSec;
	CBitmap m_memBitmap;
	CDC* GetDC();
	CDC* GetScreenDC(){return CWnd::GetDC();}
	void ReleaseDC(CDC* pDC);
	void ReleaseScreenDC(CDC* pDC){CWnd::ReleaseDC(pDC);};
	CRect GetPlottingRect()  const { return m_PlottingRect; }

	CChartLegend* GetLegend() const  { return m_pLegend; }
	CChartTitle*  GetTitle()  const  { return m_pTitles; }

	CChartAxis* GetBottomAxis() const;
	CChartAxis* GetLeftAxis() const;
	CChartAxis* GetTopAxis() const;
	CChartAxis* GetRightAxis() const;

	UINT GetEdgeType() const        { return EdgeType;    }
	void SetEdgeType(UINT NewEdge)  { EdgeType = NewEdge; }

	COLORREF GetBackColor() const       { return BackColor;   }
	void SetBackColor(COLORREF NewCol)  { BackColor = NewCol; }

	CChartSerie* AddSerie(int Type);
	CChartSerie* GetSerie(size_t Index) const;
	void RemoveSerie(size_t Index);
	void RemoveAllSeries();
	size_t GetSeriesCount() const;

	CChartCursor* AddCursor();
	CChartCursor* GetCursor(size_t Index) const;
	void RemoveCursor(size_t Index); 
	void RemoveCursorByPointer(CChartCursor* p);
	void RemoveAllCursors();
	size_t GetCursorsCount() const;


	void SetPanEnabled(bool bEnabled)  { m_bPanEnabled = bEnabled;  }
	bool GetPanEnabled() const		   { return m_bPanEnabled;	    }
	void SetZoomEnabled(bool bEnabled) { m_bZoomEnabled = bEnabled; }
	bool GetZoomEnabled() const		   { return m_bZoomEnabled;	    }
	bool GetZoomStatus() const		   { return m_bZoomStatus;	    }
	void UndoZoom();

	void RefreshCtrl();
	int Create(CWnd* pParentWnd, const RECT& rect, UINT nID, DWORD dwStyle=WS_VISIBLE);
	
    CChartCtrl();	
    virtual ~CChartCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CChartCtrl)
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	
	COLORREF BackColor;		
private:
	HCURSOR	m_hDragCursor;
	bool m_bDragStarted;		//Cursor dragging was started
	CPoint m_CurDragAnchor;
	int m_iCurID;
	bool RegisterWindowClass();

	CDC m_BackgroundDC;		// Contains the background of chart (axes, grid ...)
	bool m_bMemDCCreated;

	UINT EdgeType;		

	CRect m_PlottingRect;	// Zone in wich the series will be plotted

	std::vector<CChartSerie*> m_pSeriesList;		// Table containing all the series (dynamic)
	std::vector<CChartAxis*>  m_pAxisList;			// Table containing all the axes (dynamic)
	std::vector<CChartCursor*> m_pCursorList;		// Table containing all the cursors (dynamic)

	CChartLegend* m_pLegend;	// Chart legend
	CChartTitle*  m_pTitles;	// Chart titles

	// Support for mouse panning
	bool m_bPanEnabled;
	bool m_bRMouseDown;		// If the right mouse button is pressed
	CPoint m_PanAnchor;

	// Support for manual zoom
	bool  m_bZoomEnabled;
	bool m_bZoomStatus;
	bool  m_bLMouseDown;	// If the left mouse button is pressed
	CRect m_rectZoomArea;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHARTCTRL_H__5DDBAE42_77C3_4344_A207_3F39B623343B__INCLUDED_)
