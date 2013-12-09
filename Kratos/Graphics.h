#if !defined(AFX_GRAPHICS_H__9B0EBD9B_3E6F_4DEB_87F9_C806E97BA8A7__INCLUDED_)
#define AFX_GRAPHICS_H__9B0EBD9B_3E6F_4DEB_87F9_C806E97BA8A7__INCLUDED_

#include "WndForCurve.h"	// Added by ClassView
#include "NotifyStruct.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Graphics.h : header file
//


class CMainFrame;
/////////////////////////////////////////////////////////////////////////////
// CGraphics window

class CGraphics : public CWnd
{
public:
	CMainFrame* m_pMainFrame;
	COLORREF m_TextColor;
	COLORREF m_DataAllColor;
	COLORREF m_DataShortColor;
	COLORREF m_GridColor;

	BOOL m_LineAll;
	BOOL m_PointsAll;
	int  m_SizePointsAll;

	BOOL m_LineShort;
	BOOL m_PointsShort;
	int  m_SizePointsShort;
	
	BOOL m_Grid;
	BOOL m_GuideLines;//Рисовать курсор длинными линиями
//	char m_FontStyle[LF_FACESIZE];
//	char* m_lpszSection;
	LOGFONT* m_pLogFont;
	HFONT m_Font;
	NOTIFY_COORD m_NotifyCoord;


// Construction
public:
	CGraphics();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGraphics)
	//}}AFX_VIRTUAL

// Implementation
public:
	RECT m_LastUpdateRect;
	BOOL m_WasOnPaint;
	void DrawKRATOS();
	double y2;
	double y1;
	CRegion* m_pReg;
	NOTIFY_ZOOM m_NotifyZoom;
	BOOL m_Viewer;
	void ParamForStatusBar();
	
	void DrawGrid();
	char m_strCaption[64];
	void ReDrawAll();
	void DrawLineDataShort(int n1, int n2);
	void DrawGraphics();
	char m_cl_name_for_curve[16];
	CWndForCurve m_WndForCurve;
	//void DrawXaxis();
	BOOL LimitsX();
//	int m_nY;
//	int m_VesY;
	void DrawAxis();
	void CalcLXaxisAndX0Pixels();
	BOOL LimitsY();
	void CalcLYaxisAndY0Pixels();
	double ScaleYToScreen(int scrBottom, double logicMin, double logicMax, double val);

	HDC m_hDC;
	DATA_OUT* m_pDataAll;
	DATA_OUT* m_pDataShort;
	int m_NDataAll;
	int m_NDataShort;
	int m_NDataShortCurr;

	int m_nStepX; //Количество интервалов сетки по оси Х
	int m_nX;
	int m_VesX;
	double m_X1;
	double m_X2;

	int m_nStepY; //Количество интервалов сетки по оси Y
	int m_nY;
	int m_VesY;
	double m_Y1;
	double m_Y2;
	int m_LMaxTextY;
//	BOOL LimitsY();
//	void CalcLYaxisAndY0Pixels();
	
	int m_X0_pixels;
	int m_Y0_pixels;
	int m_LXaxis;
	//int m_Y0_pixels;
	int m_LYaxis;
	
	
	virtual ~CGraphics();

	// Generated message map functions
protected:
	//{{AFX_MSG(CGraphics)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GRAPHICS_H__9B0EBD9B_3E6F_4DEB_87F9_C806E97BA8A7__INCLUDED_)
