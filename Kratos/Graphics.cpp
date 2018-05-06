// Graphics.cpp : implementation file
//

#include "stdafx.h"
#include "Main.h"
#include "ProgNew.h"
#include "DomainLock.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CProgNewApp theApp;
/////////////////////////////////////////////////////////////////////////////
// CGraphics

extern CCriticalSection CrSecGraph;

CGraphics::CGraphics()
{
	sprintf(m_strCaption, "%s", "Region");
	m_Grid = TRUE;
	m_Font = NULL;
	m_pLogFont = new LOGFONT;
	ReadLOGFONTFromIniFile("TextFont", m_pLogFont);
	CWinApp* App = AfxGetApp();
	m_TextColor = App->GetProfileInt("TextFont", "TextColor", 0xff);
	m_Font = ::CreateFontIndirect(m_pLogFont);

	m_DataAllColor = (COLORREF) App->GetProfileInt("COLOR","1",0xff0000);
	m_DataShortColor = (COLORREF) App->GetProfileInt("COLOR","2",0xff0000);
	m_GridColor = (COLORREF) App->GetProfileInt("COLOR","3",0xff0000);

	m_Grid = (BOOL) App->GetProfileInt("SettingsGraph","Grid", 1);
	m_GuideLines = (BOOL) App->GetProfileInt("SettingsGraph","GuideLines", 1);

	m_LineAll = (BOOL) App->GetProfileInt("SettingsGraph","LineAll", 1);
	m_PointsAll  = (BOOL) App->GetProfileInt("SettingsGraph","PointsAll", 1);;
	m_SizePointsAll  = (int) App->GetProfileInt("SettingsGraph","SizePointsAll", 2);

	m_LineShort = (BOOL) App->GetProfileInt("SettingsGraph","LineShort", 1);
	m_PointsShort  = (BOOL) App->GetProfileInt("SettingsGraph","PointsShort", 1);;
	m_SizePointsShort  = (int) App->GetProfileInt("SettingsGraph","SizePointsShort", 2);

	m_pDataAll=NULL;
	m_pDataShort=NULL;

	WNDCLASS m_WC;
	if(!(::GetClassInfo(AfxGetInstanceHandle(), "WndCurve", &m_WC)))
		{	
		sprintf(m_cl_name_for_curve,"%s","WndCurve");
		memset(&m_WC, 0, sizeof(WNDCLASS));
		m_WC.lpszClassName = m_cl_name_for_curve;
		m_WC.lpfnWndProc = (WNDPROC) ::DefWindowProc;
		m_WC.style = CS_HREDRAW | CS_VREDRAW;
		m_WC.hInstance = AfxGetInstanceHandle();
		m_WC.hIcon = NULL;//::LoadIcon(NULL, IDI_APPLICATION);
		m_WC.hCursor = ::LoadCursor(NULL, IDC_CROSS);
		m_WC.hbrBackground = (HBRUSH) ::GetStockObject(BLACK_BRUSH);
		m_WC.lpszMenuName = NULL;
		::RegisterClass(&m_WC);
		}
	m_Viewer = FALSE;
}

CGraphics::~CGraphics()
{
delete m_pLogFont;
}

BEGIN_MESSAGE_MAP(CGraphics, CWnd)
	//{{AFX_MSG_MAP(CGraphics)
	ON_WM_LBUTTONDOWN()
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_MOUSEMOVE()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGraphics message handlers

void CGraphics::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CWnd::OnLButtonDown(nFlags, point);
}

void CGraphics::CalcLXaxisAndX0Pixels()
{
	RECT r;
	SIZE size;
	m_hDC = ::GetDC(this->m_hWnd);
	HFONT OldFont = (HFONT) ::SelectObject(m_hDC, m_Font);
	::GetClientRect(this->m_hWnd, &r);
	m_X0_pixels = 19 + m_LMaxTextY;
	char str[16];
	sprintf(str,"%lf", 1600.025);
	::GetTextExtentPoint32(m_hDC, str, strlen(str), &size);
	m_LXaxis =  (int) (r.right - (m_X0_pixels + size.cx/2. + 5) );
	::SelectObject(m_hDC, OldFont);
	::ReleaseDC(this->m_hWnd, m_hDC);
}

void CGraphics::CalcLYaxisAndY0Pixels()
{
	RECT r;
	SIZE size;
	char str[8];
	m_hDC = ::GetDC(this->m_hWnd);
	HFONT OldFont = (HFONT) ::SelectObject(m_hDC, m_Font);
	::GetClientRect(this->m_hWnd, &r);
	sprintf(str,"%c", '5');
	::GetTextExtentPoint32(m_hDC, str, strlen(str), &size);

	m_Y0_pixels = r.bottom - (13 + size.cy);
	m_LYaxis = (m_Y0_pixels - 2*size.cy);
	::SelectObject(m_hDC, OldFont);
	::ReleaseDC(this->m_hWnd, m_hDC);
}

BOOL CGraphics::LimitsY()
{

m_hDC = ::GetDC(this->m_hWnd);
HFONT OldFont = (HFONT) ::SelectObject(m_hDC, m_Font);
SIZE size;
m_LMaxTextY=0;
double TextCoef = 0.5;
	int i,j,k;
int Circle=TRUE;
char str[256];
char strFormat[16];
int n=0;
int ves[] = {1,2,4,5};

	if(m_Viewer==TRUE)
	{
	if(m_pReg->m_pDataOut == m_pDataAll && m_pReg->m_NDataOut == m_NDataAll)
		{	
		if(m_pDataAll)
			{
			y1=y2=m_pDataAll[0].y;
			for(i=0; i<m_NDataAll; ++i)
				{
				if(m_pDataAll[i].y < y1) y1 = m_pDataAll[i].y;
				if(m_pDataAll[i].y > y2) y2 = m_pDataAll[i].y;
				}		
			}
		}	//end if(m_pReg->m_pDataOut == m_pDataAll && m_pReg->m_NDataOut == m_NDataAll)
	
	}// end if(m_Viewer==TRUE)

if(m_Viewer==FALSE)
	{
	if(m_pDataAll)
		{
		y1=y2=m_pDataAll[0].y;
		for(i=0; i<m_NDataAll; ++i)
			{
			if(m_pDataAll[i].y < y1) y1 = m_pDataAll[i].y;
			if(m_pDataAll[i].y > y2) y2 = m_pDataAll[i].y;
			}		
		}
	}

if(m_pDataShort)
	{
	if(!m_pDataAll) y1=y2=m_pDataShort[0].y;
	
	for(i=0; i<m_NDataShortCurr; ++i)
		{
		if(m_pDataShort[i].y < y1) y1 = m_pDataShort[i].y;
		if(m_pDataShort[i].y > y2) y2 = m_pDataShort[i].y;
		}		
	}

if(!m_pDataAll && !m_pDataShort) return FALSE;

double Delta = y2-y1;
if(Delta == 0) {m_nStepY=1; goto Met_End;}

n=0;
int intY1 = (int) y1;
if(y1 < intY1) --intY1;
m_Y1 = intY1;
m_Y2 = m_Y1 + 1;
m_nStepY = (int) (m_Y2 - m_Y1);
while(m_Y2 < y2) { m_Y2 += 1; ++m_nStepY;}

int LStep = m_LYaxis/m_nStepY;
size.cx=0;
sprintf(strFormat,"%c%c%i%c%c",'%','.',n,'l','f');
for(i=0,j=0; i<=10 && j<=m_nStepY; ++i,++j)
	{
	sprintf(str,strFormat, (m_Y1+i*m_nStepY) );
	::GetTextExtentPoint32(m_hDC, str, strlen(str), &size);
	if(size.cx > m_LMaxTextY) m_LMaxTextY=size.cx;
	sprintf(str,strFormat, (m_Y2 - i*m_nStepY ) );
	::GetTextExtentPoint32(m_hDC, str, strlen(str), &size);
	if(size.cx > m_LMaxTextY) m_LMaxTextY=size.cx;
	}

m_Y1=m_Y2=0;
m_LMaxTextY=0;
if(size.cy > (TextCoef*LStep))
	{
	n=-1;
	while(Circle)
		{
		++n;
		for(k=0; k<4; ++k)
			{
			m_nStepY = (int) ( Delta/(ves[k]*pow(10.,n)) );
			if(m_nStepY <= 1) {m_nStepY = 1; Circle = FALSE; break;}
			intY1 = (int) ( y1/pow(10.,n) );
			if(y1 < intY1) --intY1;
			if(ves[k]==5) { while(intY1%5) {intY1 -= 1;} }
			else if( (ves[k]==2 || ves[k]==4) && (intY1%2) ) intY1 -= 1;
			m_Y1 = intY1*pow(10.,n);
			m_Y2 = m_Y1 + ves[k]*pow(10.,n)*m_nStepY;
			while(m_Y2 < y2) { m_Y2 += ves[k]*pow(10.,n); ++m_nStepY;}

			LStep=m_LYaxis/(m_nStepY+1);
			sprintf(strFormat,"%c%c%c%c%c",'%','.','0','l','f');
			size.cx = 0;
			for(i=0,j=0; i<=10 && j<=m_nStepY; ++i,++j)
				{
				sprintf(str,strFormat, (m_Y1+i*m_nStepY) );
				::GetTextExtentPoint32(m_hDC, str, strlen(str), &size);
				if(size.cx > m_LMaxTextY) m_LMaxTextY=size.cx;
				sprintf(str,strFormat, (m_Y2 - i*m_nStepY ) );
				::GetTextExtentPoint32(m_hDC, str, strlen(str), &size);
				if(size.cx > m_LMaxTextY) m_LMaxTextY=size.cx;
				} // end for(i=0,j=0; i<=10 && j<=nStep; ++i,++j)
			if(size.cy < TextCoef*LStep) { Circle = FALSE; break; }
			} // end for(k=0; k<4; ++k)

		} // end while(Circle)
	} // end if(LText > (LStep))
else //if(m_pLogFont->lfHeight < TextCoef*LStep)
	{
	n=0;
	Circle = TRUE;
	while(Circle)
		{
		--n;
		for(k=3; k>=0; --k)
			{
			m_nStepY = (int) (Delta/(ves[k]*pow(10.,n)));
			if(m_nStepY == 0) ++m_nStepY;
			intY1 = (int) (y1/pow(10.,n));
			if(y1 < intY1) --intY1;
			if(ves[k]==5) { while(intY1%5) {--intY1;} }
			else if( (ves[k]==2 || ves[k]==4) && (intY1%2) ) intY1 -= 1;
			//if(intX1 < 0) intX1=0;
			m_Y1 = intY1*pow(10.,n);
			m_Y2 = m_Y1 + ves[k]*pow(10.,n)*m_nStepY;
			while(m_Y2 < y2) { m_Y2 += (ves[k]*pow(10.,n)); ++m_nStepY;}
			sprintf(strFormat,"%c%c%i%c%c", '%', '.', abs(n), 'l', 'f');
			size.cx = 0;
			m_LMaxTextY = 0;
			LStep=m_LYaxis/(m_nStepY+1);
			for(i=0,j=0; i<=10 && j<=m_nStepY; ++i,++j)
				{
				sprintf(str,strFormat, (m_Y1+i*m_nStepY) );
				::GetTextExtentPoint32(m_hDC, str, strlen(str), &size);
				if(size.cx > m_LMaxTextY) m_LMaxTextY=size.cx;
				sprintf(str,strFormat, (m_Y2 - i*m_nStepY ) );
				::GetTextExtentPoint32(m_hDC, str, strlen(str), &size);
				if(size.cx > m_LMaxTextY) m_LMaxTextY=size.cx;
				} // end for(i=0,j=0; i<=10 && j<=nStep; ++i,++j)
			if(size.cy > TextCoef*LStep)
				{
				Circle = FALSE;
				if(ves[k]==5) { ++n; k=0;}
				else ++k;
				m_nStepY = (int) (Delta/(ves[k]*pow(10.,n)));
				intY1 = (int) (y1/pow(10.,n));
				if(y1 < intY1) --intY1;
				if(ves[k]==5) { while(intY1%5) {--intY1;} }
				else if( (ves[k]==2 || ves[k]==4) && (intY1%2) ) intY1 -= 1;
				//if(intX1 < 0) intX1=0;
				m_Y1 = intY1*pow(10.,n);
				m_Y2 = m_Y1 + ves[k]*pow(10.,n)*m_nStepY;
				while(m_Y2 < y2) { m_Y2 += (ves[k]*pow(10.,n)); ++m_nStepY;}
				break;
				}//end if(LText < TextCoef*LStep)
			} // end for(k=3; k>=0; --k)
		} // end 	while(Circle)2
	} // else if(LText < (LStep))

Met_End:
if(m_nStepY <= 1)
	{ m_Y1 = y1; m_Y2 = y2; m_nStepY=1;
		sprintf(str,"%lf", m_Y2);
		::GetTextExtentPoint32(m_hDC, str, strlen(str), &size);
		m_LMaxTextY = size.cx;
	}
else { m_VesY = ves[k]; m_nY = n;}

::SelectObject(m_hDC, OldFont);
::ReleaseDC(this->m_hWnd, m_hDC);
return TRUE;
}

void CGraphics::DrawAxis()
{
	int i;
	double x,y, LStep;
	char str[32];
	char FormatStr[32];
	SIZE size;
	CalcLYaxisAndY0Pixels();
	if(!LimitsY()) { AfxMessageBox("LimitsY == FALSE"); return;}
	CalcLXaxisAndX0Pixels();
	if(!LimitsX()) { AfxMessageBox("LimitsX == FALSE"); return;}
	
	m_hDC = ::GetDC(this->m_hWnd);
	HFONT OldFont = (HFONT) ::SelectObject(m_hDC, m_Font);
	sprintf(str,"%c", '5');
	::GetTextExtentPoint32(m_hDC, str, strlen(str), &size);

	LOGBRUSH LogBrush = {BS_SOLID, m_TextColor,0};
	HPEN NewPen = ::ExtCreatePen(	PS_COSMETIC | PS_SOLID, 1, &LogBrush,0,0);
	HPEN OldPen = (HPEN) ::SelectObject(m_hDC, NewPen);
	::SetTextColor(m_hDC, m_TextColor);
	::SetTextAlign(m_hDC, TA_RIGHT);
	::SetBkColor(m_hDC, (COLORREF) RGB(0,0,0));	
	
	::MoveWindow(m_WndForCurve.m_hWnd, m_X0_pixels+1, m_Y0_pixels-m_LYaxis+1,
								m_LXaxis-2, m_LYaxis-2, NULL);
	// Bar - Ограничивающий прямоугольник
	::MoveToEx(m_hDC, m_X0_pixels, m_Y0_pixels, NULL);
	::LineTo(m_hDC, m_X0_pixels, m_Y0_pixels-m_LYaxis);
	::LineTo(m_hDC, m_X0_pixels+m_LXaxis, m_Y0_pixels-m_LYaxis);
	::LineTo(m_hDC, m_X0_pixels+m_LXaxis, m_Y0_pixels);
	::LineTo(m_hDC, m_X0_pixels, m_Y0_pixels);
	
	// Y axis
	LStep = ((double) m_LYaxis)/m_nStepY;
	if(m_nY >= 0) sprintf(FormatStr, "%c%c%c%c%c", '%', '.', '0', 'l', 'f');
	else sprintf(FormatStr, "%c%c%i%c%c", '%', '.', abs(m_nY), 'l', 'f');
	for(i=0; i<=m_nStepY; ++i)
		{
		y = m_Y0_pixels - i*LStep;
		// Горизонтальные линии сетки:
		::MoveToEx(m_hDC, m_X0_pixels, (int) y, NULL);
		::LineTo(m_hDC, m_X0_pixels-5, (int) y);
		
		if(m_nStepY > 1) sprintf(str, FormatStr, (m_Y1 + i*m_VesY*pow(10.,m_nY)));
		else
			{
			if(i==0) sprintf(str,"%.3lf",m_Y1);
			else if(i==1) sprintf(str,"%.3lf",m_Y2);
			}
		::TextOut(m_hDC, m_X0_pixels-10, (int) (y - size.cy/2), str, strlen(str));
		}

	// X axis
	::SetTextAlign(m_hDC, TA_CENTER);
	
	LStep = ((double) m_LXaxis)/m_nStepX;
	if(m_nX >= 0) sprintf(FormatStr, "%c%c%c%c%c", '%', '.', '0', 'l', 'f');
	else sprintf(FormatStr, "%c%c%i%c%c", '%', '.', abs(m_nX), 'l', 'f');
	for(i=0; i<=m_nStepX; ++i)
		{
		x = m_X0_pixels + i*LStep;
		// Вертикальные линии сетки:
		::MoveToEx(m_hDC, (int) x, m_Y0_pixels, NULL);
		::LineTo(m_hDC, (int) x, m_Y0_pixels+5);
		
		if(m_nStepX > 1) sprintf(str, FormatStr, (m_X1 + i*m_VesX*pow(10.,m_nX)));
		else
			{
			if(i==0) sprintf(str,"%.3lf",m_X1);
			else if(i==1) sprintf(str,"%.3lf",m_X2);
			}
		::TextOut(m_hDC, (int) x, m_Y0_pixels+8, str, strlen(str));
		}
	
	::SetTextAlign(m_hDC, TA_LEFT);
	::TextOut(m_hDC, m_X0_pixels, size.cy/2, m_strCaption, strlen(m_strCaption));
	
	::SelectObject(m_hDC, OldFont);
	::SelectObject(m_hDC, OldPen);
	::DeleteObject(NewPen);
	::ReleaseDC(this->m_hWnd, m_hDC);

	if(m_Grid == TRUE) 
		DrawGrid();	
}

BOOL CGraphics::LimitsX()
{
	m_hDC = ::GetDC(this->m_hWnd);
	HFONT OldFont = (HFONT) ::SelectObject(m_hDC, m_Font);
	SIZE size;
	double TextCoef = 0.7;
	int i,j,k;
	int Circle=TRUE;
	char str[256];
	char strFormat[16];
	int ves[] = {1,2,4,5};
	double Delta;     // Длина оси х в логических коондинатах (эВ)
	double x1=0,x2=0; // Начальная и конечная точки по оси х в логических коондинатах (эВ)

	if(m_pDataAll && m_pDataShort) 
	{ x1 = I2D(m_pDataAll[0].x); 
	x2 = I2D(m_pDataAll[m_NDataAll-1].x);
	if( I2D(m_pDataShort[0].x) < x1) x1 = I2D(m_pDataShort[0].x); 
	if( I2D(m_pDataShort[m_NDataShort-1].x) > x2) x2 = I2D(m_pDataShort[m_NDataShort-1].x);
	}
	else if(m_pDataAll && !m_pDataShort) 
	{ x1 = I2D(m_pDataAll[0].x); 
	x2 = I2D(m_pDataAll[m_NDataAll-1].x);
	}
	else if(!m_pDataAll && m_pDataShort) 
	{ x1 = I2D(m_pDataShort[0].x); 
	x2 = I2D(m_pDataShort[m_NDataShort-1].x);
	}

	if(!m_pDataAll && !m_pDataShort) return FALSE;

	Delta = x2-x1; 
	if(Delta <0)
	{
		MsgLog("Регион содержит некорректные данные по оси Х. Рекомендуется сбросить данные региона.");
		return FALSE;
	}
	int n = 0;

	int intX1 = (int) x1;
	if(x1 < intX1) --intX1;
	m_X1 = intX1;
	m_X2 = m_X1 + 1;
	m_nStepX = (int) (m_X2 - m_X1);
	while(m_X2 < x2) { m_X2 += 1; ++m_nStepX;}

	int LStep = m_LXaxis/(m_nStepX); // Длина деления в пикселах
	int m_LMaxTextX = 0;
	sprintf(strFormat,"%c%c%i%c%c",'%','.',n,'l','f');
	for(i=0,j=0; i<=10 && j<m_nStepX; ++i,++j)
	{
		sprintf(str,strFormat, (m_X1+i*m_nStepX) );
		::GetTextExtentPoint32(m_hDC, str, strlen(str), &size);
		if(size.cx > m_LMaxTextX) m_LMaxTextX=size.cx;
		if(m_LMaxTextX > (TextCoef*LStep)) break;
		sprintf(str,strFormat, (m_X2 - i*m_nStepX ) );
		::GetTextExtentPoint32(m_hDC, str, strlen(str), &size);
		if(size.cx > m_LMaxTextX) m_LMaxTextX=size.cx;
		if(m_LMaxTextX > (TextCoef*LStep)) break;
	}

	m_X1=m_X2=0;
	if(m_LMaxTextX > (TextCoef*LStep))
	{
		n=-1;
		while(Circle)
		{
			++n;
			for(k=0; k<4; ++k)
			{
				m_nStepX = (int) ( Delta/(ves[k]*pow(10.,n)) );
				if(m_nStepX == 1) {Circle = FALSE; break;}
				intX1 = (int) ( x1/pow(10.,n) );
				if(x1 < intX1) --intX1;
				if(ves[k]==5) { while(intX1%5) {intX1 -= 1;} }
				else if( (ves[k]==2 || ves[k]==4) && (intX1%2) ) intX1 -= 1;
				m_X1 = intX1*pow(10.,n);
				m_X2 = m_X1 + ves[k]*pow(10.,n)*m_nStepX;
				while(m_X2 < x2) { m_X2 += ves[k]*pow(10.,n); ++m_nStepX;}

				LStep=m_LXaxis/(m_nStepX);
				sprintf(strFormat,"%c%c%c%c%c",'%','.','0','l','f');
				m_LMaxTextX=0;
				for(i=0,j=0; i<=10 && j<m_nStepX; ++i,++j)
				{
					sprintf(str,strFormat, (m_X1 + i*ves[k]*pow(10.,n)) );
					::GetTextExtentPoint32(m_hDC, str, strlen(str), &size);
					if(size.cx > m_LMaxTextX) m_LMaxTextX=size.cx;
					if(m_LMaxTextX > (TextCoef*LStep)) break;
					sprintf(str,strFormat, (m_X2 - i*ves[k]*pow(10.,n)) );
					::GetTextExtentPoint32(m_hDC, str, strlen(str), &size);
					if(size.cx > m_LMaxTextX) m_LMaxTextX=size.cx;
					if(m_LMaxTextX > (TextCoef*LStep)) break;
				} // end for(i=0,j=0; i<=10 && j<=nStep; ++i,++j)
				if(m_LMaxTextX < TextCoef*LStep) { Circle = FALSE; break; }
			} // end for(k=0; k<4; ++k)

		} // end while(Circle)
	} // end if(LText > (LStep))
	else //if(LText < (TextCoef*LStep))
	{
		n=0;
		Circle = TRUE;
		while(Circle)
		{
			--n;
			for(k=3; k>=0; --k)
			{
				double p10n = pow(10.,n);
				m_nStepX = (int) (Delta/(ves[k]*p10n));
				if(m_nStepX == 0) ++m_nStepX;
				intX1 = (int) (x1/p10n);
				if(x1 < intX1) --intX1;
				if(ves[k]==5) { while(intX1%5) {--intX1;} }
				else if( (ves[k]==2 || ves[k]==4) && (intX1%2) ) intX1 -= 1;
				//if(intX1 < 0) intX1=0;
				m_X1 = intX1*p10n;
				m_X2 = m_X1 + ves[k]*p10n*m_nStepX;
				while(m_X2 < x2) { m_X2 += (ves[k]*p10n); ++m_nStepX;}
				sprintf(strFormat,"%c%c%i%c%c", '%', '.', abs(n), 'l', 'f');
				m_LMaxTextX=0;
				LStep=m_LXaxis/(m_nStepX+1);
				for(i=0,j=0; i<=10 && j<m_nStepX; ++i,++j)
				{
					sprintf(str,strFormat, (m_X1 + i*ves[k]*p10n) );
					::GetTextExtentPoint32(m_hDC, str, strlen(str), &size);
					if(size.cx > m_LMaxTextX) m_LMaxTextX=size.cx;
					if(m_LMaxTextX > (TextCoef*LStep)) break;
					sprintf(str,strFormat, (m_X2 - i*ves[k]*p10n) );
					::GetTextExtentPoint32(m_hDC, str, strlen(str), &size);
					if(size.cx > m_LMaxTextX) m_LMaxTextX=size.cx;
					if(m_LMaxTextX > (TextCoef*LStep)) break;
				} // end for(i=0,j=0; i<=10 && j<=nStep; ++i,++j)
				if(m_LMaxTextX > TextCoef*LStep)
				{
					Circle = FALSE;
					if(ves[k]==5) 
					{
						++n; k=0;
						p10n = pow(10.,n);
					}
					else ++k;
					m_nStepX = (int) (Delta/(ves[k]*p10n));
					intX1 = (int) (x1/p10n);
					if(x1 < intX1) --intX1;
					if(ves[k]==5) { while(intX1%5) {--intX1;} }
					else if( (ves[k]==2 || ves[k]==4) && (intX1%2) ) intX1 -= 1;
					//if(intX1 < 0) intX1=0;
					m_X1 = intX1*p10n;
					m_X2 = m_X1 + ves[k]*p10n*m_nStepX;
					while(m_X2 < x2) { m_X2 += (ves[k]*p10n); ++m_nStepX;}
					break;
				}//end if(LText < TextCoef*LStep)
			} // end for(k=3; k>=0; --k)
		} // end 	while(Circle)2
	} // else if(LText < (LStep))

	if(m_nStepX <= 1)
	{ m_X1 = x1; m_X2 = x2; m_nStepX=1;

	}
	else { m_VesX = ves[k]; m_nX = n;}

	::SelectObject(m_hDC, OldFont);
	::ReleaseDC(this->m_hWnd, m_hDC);
	return TRUE;
}




void CGraphics::OnPaint() 
{
	GetUpdateRect(&m_LastUpdateRect);
	CString str;
	str.Format("(%i,%i)-(%i,%i)", m_LastUpdateRect.left, m_LastUpdateRect.top,m_LastUpdateRect.right,m_LastUpdateRect.bottom);
	CPaintDC dc(this); // device context for painting
	m_WasOnPaint=TRUE;

	if(theApp.m_pMainFrame->m_Doc.CheckDocType()==CDoc::XPS || TRUE)
	{
		if(m_pDataAll || m_pDataShort) 
		{ 
			try
			{
				DomainLock lock(CrSecGraph);
				DrawAxis(); 
				ParamForStatusBar();
				DrawGraphics();			
			}
			catch(...)
			{
				LogFile("Ошибка при отрисовке графика - вероятно, Access Violation", __FILE__, __LINE__);
			}
		}
		else DrawKRATOS();
	}
	else if(theApp.m_pMainFrame->m_Doc.CheckDocType()==CDoc::DXPS)
	{
	}
	else DrawKRATOS();
}



int CGraphics::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	SetWindowText("CGraphics");

	RECT r;
	::GetWindowRect(this->m_hWnd, &r);
	m_WndForCurve.CreateEx(0, m_cl_name_for_curve, NULL,
		WS_CHILD|WS_CLIPCHILDREN|WS_VISIBLE,
		0, 0, r.right, r.bottom, this->m_hWnd, NULL, 0);
	if(m_WndForCurve.m_hWnd == NULL) {AfxMessageBox("m_WndForCurve.m_hWnd == NULL"); return -1;}
	m_WndForCurve.m_pGraph = this;
	m_NotifyCoord.hdr.hwndFrom = this->m_hWnd;
	m_NotifyCoord.hdr.idFrom = ID_GRAPHICS_WND;
	m_NotifyCoord.hdr.code = WM_MOUSEMOVE;

	m_NotifyZoom.hdr.hwndFrom = this->m_hWnd;
	m_NotifyZoom.hdr.idFrom = ID_GRAPHICS_WND;
	m_NotifyZoom.hdr.code = WM_LBUTTONUP;

	m_NotifyCoord.x = 0;
	m_NotifyCoord.y = 0;
	m_NotifyCoord.n_digitsX = 0;
	m_NotifyCoord.n_digitsY = 0;
	
	return 0;
}

void CGraphics::DrawGraphics()
{
	int i;
	double x_scr,y_scr;

	RECT r;
	::GetClientRect(m_WndForCurve.m_hWnd, &r);
	int X2_scr = m_LXaxis-1;
	int Y1_scr = r.bottom-1;

	m_hDC = ::GetDC(m_WndForCurve.m_hWnd);
	LOGBRUSH LogBrush = {BS_SOLID, m_DataAllColor,0};
	HPEN NewPen = ::ExtCreatePen(	PS_GEOMETRIC | PS_SOLID, 1, &LogBrush,0,0);
	HPEN OldPen = (HPEN) ::SelectObject(m_hDC, NewPen);

	HBRUSH NewBrush = ::CreateBrushIndirect(&LogBrush);
	HBRUSH OldBrush = (HBRUSH) ::SelectObject(m_hDC, NewBrush);

	if(m_pDataAll)
		{
		x_scr = ((double) X2_scr)*( ( I2D(m_pDataAll[0].x) - m_X1) / (m_X2-m_X1) );
		y_scr = ScaleYToScreen(Y1_scr, m_Y1, m_Y2, m_pDataAll[0].y);

		::MoveToEx(m_hDC, (int) x_scr, (int) y_scr, NULL);
		for(i=1; i<m_NDataAll; ++i) 
			{
			if(m_LineAll) ::LineTo(m_hDC, (int) x_scr, (int) y_scr);
			if(m_PointsAll) 	::Ellipse(m_hDC, (int)(x_scr-m_SizePointsAll/2), 
																				 (int)(y_scr-m_SizePointsAll/2),
																				 (int)(x_scr+m_SizePointsAll/2.+0.5), 
																				 (int)(y_scr+m_SizePointsAll/2.+0.5) );
			x_scr = ((double) X2_scr)*( ( I2D(m_pDataAll[i].x) - m_X1) / (m_X2-m_X1) );
			y_scr = ScaleYToScreen(Y1_scr, m_Y1, m_Y2, m_pDataAll[i].y);;
			//y_scr =  - Y1_scr*( ( m_pDataAll[0].y - m_X1) / (m_Y2-m_Y1) );
			}
		if(m_LineAll) ::LineTo(m_hDC, (int) x_scr, (int) y_scr);
		if(m_PointsAll) 	::Ellipse(m_hDC, (int)(x_scr-m_SizePointsAll/2), 
																			 (int)(y_scr-m_SizePointsAll/2),
																			 (int)(x_scr+m_SizePointsAll/2.+0.5), 
																			 (int)(y_scr+m_SizePointsAll/2.+0.5) );
		}// end if(m_pDataAll)

	::SelectObject(m_hDC, OldBrush);
	::SelectObject(m_hDC, OldPen);
	::DeleteObject(NewBrush);
	::DeleteObject(NewPen);

	LogBrush.lbColor = m_DataShortColor;
	NewPen = ::ExtCreatePen(	PS_COSMETIC | PS_SOLID, 1, &LogBrush,0,0);
	OldPen = (HPEN) ::SelectObject(m_hDC, NewPen);

	NewBrush = ::CreateBrushIndirect(&LogBrush);
	OldBrush = (HBRUSH) ::SelectObject(m_hDC, NewBrush);

	if(m_pDataShort)
	{

		x_scr = ((double) X2_scr)*( ( I2D(m_pDataShort[0].x) - m_X1) / (m_X2-m_X1) );
		y_scr = ScaleYToScreen(Y1_scr, m_Y1, m_Y2, m_pDataShort[0].y);

		::MoveToEx(m_hDC, (int) x_scr, (int) y_scr, NULL);
		for(i=1; (i<m_NDataShortCurr); ++i) 
		{
			if(m_LineShort) ::LineTo(m_hDC, (int) x_scr, (int) y_scr);
			if(m_PointsShort) 	::Ellipse(m_hDC, (int)(x_scr-m_SizePointsShort/2), 
													(int)(y_scr-m_SizePointsShort/2),
													(int)(x_scr+m_SizePointsShort/2.+0.5), 
													(int)(y_scr+m_SizePointsShort/2.+0.5) );
			x_scr = ((double) X2_scr)*( ( I2D(m_pDataShort[i].x) - m_X1) / (m_X2-m_X1) );
			y_scr = ScaleYToScreen(Y1_scr, m_Y1, m_Y2, m_pDataShort[i].y);
		}
		if(m_LineShort) ::LineTo(m_hDC, (int) x_scr, (int) y_scr);
		if(m_PointsShort) 	::Ellipse(m_hDC, (int)(x_scr-m_SizePointsShort/2), 
													(int)(y_scr-m_SizePointsShort/2),
													(int)(x_scr+m_SizePointsShort/2.+0.5), 
													(int)(y_scr+m_SizePointsShort/2.+0.5) );
	}

	::SelectObject(m_hDC, OldBrush);
	::SelectObject(m_hDC, OldPen);
	::DeleteObject(NewBrush);
	::DeleteObject(NewPen);
	::ReleaseDC(m_WndForCurve.m_hWnd, m_hDC);
}

// Рисует линию (точку) для текущего измерения между индексами n1 и n2
void CGraphics::DrawLineDataShort(int n1, int n2)
{
	double x, y;
	int X2_scr, Y1_scr;
	RECT r;

	if(n2 > m_NDataShortCurr)
		m_NDataShortCurr = n2;
	y = (double) m_pDataShort[n2].y;

	if(y < m_Y1 || y > m_Y2) 
		ReDrawAll();
	else
	{
		DomainLock lock(CrSecGraph);

		::GetClientRect(m_WndForCurve.m_hWnd, &r);
		X2_scr = m_LXaxis-1;
		Y1_scr=r.bottom;

		if(!m_pDataShort) return;
		m_hDC = ::GetDC(m_WndForCurve.m_hWnd);
		LOGBRUSH LogBrush = {BS_SOLID, m_DataShortColor,0};
		HPEN NewPen = ::ExtCreatePen(	PS_COSMETIC | PS_SOLID, 1, &LogBrush,0,0);
		HPEN OldPen = (HPEN) ::SelectObject(m_hDC, NewPen);
		
		HBRUSH NewBrush = ::CreateBrushIndirect(&LogBrush);
		HBRUSH OldBrush = (HBRUSH) ::SelectObject(m_hDC, NewBrush);
			
		if(m_PointsShort) 	
		{
			x = ((double) X2_scr)*( ( I2D(m_pDataShort[n2].x) - m_X1) / (m_X2-m_X1) );
			y = ScaleYToScreen(Y1_scr, m_Y1, m_Y2, m_pDataShort[n2].y);
			::Ellipse(m_hDC, (int)(x - m_SizePointsShort/2), 
															(int)(y - m_SizePointsShort/2),
															(int)(x + m_SizePointsShort/2.+0.5), 
															(int)(y + m_SizePointsShort/2.+0.5) );
		}
		if(n1 < 0) 
			return;
		if(m_LineShort)
		{
			x = ((double) X2_scr)*( ( I2D(m_pDataShort[n1].x) - m_X1) / (m_X2-m_X1) );
			y = ScaleYToScreen(Y1_scr, m_Y1, m_Y2, m_pDataShort[n1].y);
			::MoveToEx(m_hDC, (int) x, (int) y, NULL);
			x = ((double) X2_scr)*( ( I2D(m_pDataShort[n2].x) - m_X1) / (m_X2-m_X1) );
			y = ScaleYToScreen(Y1_scr, m_Y1, m_Y2, m_pDataShort[n2].y);
			::LineTo(m_hDC, (int) x, (int) y);
		}
		
		::SelectObject(m_hDC, OldBrush);
		::SelectObject(m_hDC, OldPen);
		::DeleteObject(NewBrush);
		::DeleteObject(NewPen);
		::ReleaseDC(m_WndForCurve.m_hWnd, m_hDC);
	}
}

double CGraphics::ScaleYToScreen(int scrBottom, double logicMin, double logicMax, double val)
{
	if(logicMin == logicMax)
		return scrBottom/2;
	return ((double) scrBottom) - scrBottom*( ( val - logicMin) / (logicMax-logicMin) );
}

void CGraphics::ReDrawAll()
{
RECT r;
::GetClientRect(m_WndForCurve.m_hWnd, &r);
::InvalidateRect(m_WndForCurve.m_hWnd, &r, TRUE);
::UpdateWindow(m_WndForCurve.m_hWnd);

::GetClientRect(this->m_hWnd, &r);
::InvalidateRect(this->m_hWnd, &r, TRUE);
::UpdateWindow(this->m_hWnd);
}

void CGraphics::DrawGrid()
{
	double LStep,x,y;
	int i;
	RECT r;

	::GetClientRect(this->m_WndForCurve.m_hWnd, &r);
	m_hDC = ::GetDC(this->m_WndForCurve.m_hWnd);
	LOGBRUSH LogBrush = {BS_SOLID, m_GridColor,0};
	HPEN NewPen = ::ExtCreatePen(	PS_GEOMETRIC | PS_DOT, 1, &LogBrush,0,0);
	HPEN OldPen = (HPEN) ::SelectObject(m_hDC, NewPen);
	::SetBkColor(m_hDC, (COLORREF) RGB(0,0,0));

	LStep = ((double) m_LYaxis)/m_nStepY;
	for(i=1; i<=m_nStepY-1; ++i)
		{
		y = r.bottom - i*LStep +1;
		::MoveToEx(m_hDC, 0, (int) y, NULL);
		::LineTo(m_hDC, r.right, (int) y);
		}
	
	LStep = ((double) m_LXaxis)/m_nStepX;
	for(i=1; i<=m_nStepX-1; ++i)
		{
		x = i*LStep - 1;
		::MoveToEx(m_hDC, (int) x, 0, NULL);
		::LineTo(m_hDC, (int) x, r.bottom);
		}	

	::SelectObject(m_hDC, OldPen);
	::DeleteObject(NewPen);
	::ReleaseDC(this->m_WndForCurve.m_hWnd, m_hDC);
}

void CGraphics::OnMouseMove(UINT nFlags, CPoint point) 
{
	CWnd::OnMouseMove(nFlags, point);
}

void CGraphics::ParamForStatusBar()
{
RECT r;
::GetClientRect(m_WndForCurve.m_hWnd, &r);
double X2_scr = m_LXaxis-1;
double Y1_scr=r.bottom-1;

// X axis
double LogUnitInPixel= (m_X2-m_X1)/((double) m_LXaxis) ;
int n=0;
m_NotifyCoord.n_digitsX = 0;
if(LogUnitInPixel >= 1.0) m_NotifyCoord.n_digitsX = 0;
else if(LogUnitInPixel != 0.0)
	{ while((LogUnitInPixel *= pow(10.,n)) < 1.0) {++m_NotifyCoord.n_digitsX; ++n;} 
		++m_NotifyCoord.n_digitsX;
	}
else m_NotifyCoord.n_digitsX = 2;
m_WndForCurve.m_Coord_X = (m_X2 - m_X1)/((double) (X2_scr));

// Y axis
LogUnitInPixel= ((double) (m_VesY))*pow(10.,m_nY)*m_nStepY/m_LYaxis ;
n=0;
m_NotifyCoord.n_digitsY = 0;
if(LogUnitInPixel >= 1.0) m_NotifyCoord.n_digitsY = 0;
else if(LogUnitInPixel != 0.0)
	{ while((LogUnitInPixel *= pow(10.,n)) < 1.0) {++m_NotifyCoord.n_digitsY; ++n;} 
		++m_NotifyCoord.n_digitsY;
	}
else m_NotifyCoord.n_digitsY = 2;
m_WndForCurve.m_Coord_Y = (m_Y2 - m_Y1)/((double) (Y1_scr));
}

void CGraphics::DrawKRATOS()
{
char strK[30];
if(!theApp.Ini.HighPressureMode.Value)
	strcpy(strK,"KRATOS");
else
	strcpy(strK,"High Pressure");
HWND hWnd = this->m_WndForCurve.m_hWnd;
LOGFONT LogFont;
COLORREF Color = RGB(255,0,100);
RECT r;
SIZE size;

::GetClientRect(hWnd, &r);
memmove((void*) &LogFont, (void*) m_pLogFont, sizeof(LOGFONT));
LogFont.lfHeight = -r.bottom/2;
LogFont.lfWidth = r.right/(strlen(strK)+7);
HFONT Font = ::CreateFontIndirect(&LogFont);
HDC DC = ::GetDC(hWnd);
HFONT OldFont = (HFONT) ::SelectObject(DC, (HGDIOBJ) Font);
::GetTextExtentPoint32(DC, strK, strlen(strK), &size);
int x = (r.right - size.cx)/2;
int y = (r.bottom - size.cy)/2;

::SetTextColor(DC, Color);
::SetBkColor(DC, (COLORREF) RGB(0,0,0));
::TextOut(DC, x, y, strK, strlen(strK));

::SelectObject(DC, (HGDIOBJ) OldFont);
::DeleteObject((HGDIOBJ) Font);
::ReleaseDC(hWnd, DC);
}

BOOL CGraphics::OnEraseBkgnd(CDC* pDC) 
{
	return CWnd::OnEraseBkgnd(pDC);
}
