// ViewWnd.cpp : implementation file
//

#include "stdafx.h"
#include "Main.h"

//#include "ProgNew.h"
//#include "ViewWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CViewWnd

CViewWnd::CViewWnd()
{
	memset(&m_WC, 0, sizeof(WNDCLASS));
	m_WC.lpszClassName = "ViewWindow";
	m_WC.lpfnWndProc = (WNDPROC) ::DefWindowProc;
	//m_WC.lpfnWndProc = (WNDPROC) NULL;
	m_WC.style = CS_HREDRAW | CS_VREDRAW;
	m_WC.hInstance = AfxGetInstanceHandle();
	m_WC.hIcon = ::LoadIcon(NULL, IDI_APPLICATION);
	m_WC.hCursor = ::LoadCursor(NULL, IDC_ARROW);
	m_WC.hbrBackground = (HBRUSH) ::GetStockObject(BLACK_BRUSH);//WHITE_BRUSH
	m_WC.lpszMenuName = NULL;
	if( !::RegisterClass(&m_WC) ) {AfxMessageBox("No registered ViewWindowClass"); }



	
	//this->m_ViewGraph.m_pDataAll = NULL;
	//this->m_ViewGraph.m_pDataShort = NULL;
	
/* см  CRegionWnd::OnCommand (кнопка View)
	CWinApp* App = AfxGetApp();
	m_rectWnd.left = App->GetProfileInt("SizeOfViewWindow", "Left", 0);
	m_rectWnd.top = App->GetProfileInt("SizeOfViewWindow", "Top", 0);
	m_rectWnd.right = App->GetProfileInt("SizeOfViewWindow", "Right", CW_USEDEFAULT);
	m_rectWnd.bottom = App->GetProfileInt("SizeOfViewWindow", "Bottom", CW_USEDEFAULT);
*/

}

CViewWnd::~CViewWnd()
{
}


BEGIN_MESSAGE_MAP(CViewWnd, CWnd)
	//{{AFX_MSG_MAP(CViewWnd)
	ON_WM_CLOSE()
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_SIZING()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CViewWnd message handlers

void CViewWnd::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	RECT r;
	::GetWindowRect(this->m_hWnd, &r);
	
	//AfxMessageBox("CViewWnd::OnClose()");
	// Записываю вершины углов. Размеры cx, cy вычисляются при создании окна 
	// см. CRegionWnd::OnCommand (кнопка View)
	CWinApp* App = AfxGetApp();
	
	if(!::IsIconic(this->m_hWnd) && !::IsZoomed(this->m_hWnd))
		{
		App->WriteProfileInt("SizeOfViewWindow", "Left", r.left);
		App->WriteProfileInt("SizeOfViewWindow", "Top", r.top);
		App->WriteProfileInt("SizeOfViewWindow", "Right", r.right);
		App->WriteProfileInt("SizeOfViewWindow", "Bottom", r.bottom);
		}

	WriteLOGFONTToIniFile("ViewTextFont", this->m_ViewGraph.m_pLogFont);
	App->WriteProfileInt("ViewTextFont", "ColorFont", this->m_ViewGraph.m_ColorFont);
	App->WriteProfileInt("SettingsViewer", "ColorLine", this->m_ViewGraph.m_DataAllColor);
	App->WriteProfileInt("SettingsViewer", "ColorGrid", this->m_ViewGraph.m_GridColor);


	App->WriteProfileInt("SettingsViewer","Line", (UINT) this->m_ViewGraph.m_LineAll);
	App->WriteProfileInt("SettingsViewer","Points", (UINT) this->m_ViewGraph.m_PointsAll);
	App->WriteProfileInt("SettingsViewer","SizePoints", this->m_ViewGraph.m_SizePointsAll);
	App->WriteProfileInt("SettingsViewer","Grid", (UINT) this->m_ViewGraph.m_Grid);
	
	
	CWnd::OnClose();
}

int CViewWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	//LONG style = ::GetWindowLong(this->m_hWnd, GWL_STYLE);
	
	m_hStatusBar = ::CreateStatusWindow(WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS,
	NULL, m_hWnd, IDW_STATUS_BAR_VIEW);  // IDW_STATUS_BAR_VIEW -- идентификатор StatusBar
	if(m_hStatusBar==NULL) return -1;
	
	RECT rSB;
	::GetClientRect(m_hStatusBar, &rSB);
	int nParts = 2;
	int SizeParts[2] = {rSB.right/2, -1};
	::SendMessage(m_hStatusBar, SB_SETPARTS, (WPARAM) nParts, (LPARAM) SizeParts);

	RECT r;
	::GetClientRect(this->m_hWnd, &r);
	m_ViewGraph.CreateEx(0, this->m_pMainFrame->m_pHideWnd->m_pBigClientWnd->m_class_name, 
		NULL,
		WS_CHILD|WS_CLIPCHILDREN|WS_VISIBLE,//|WS_HSCROLL|WS_VSCROLL, 
		0, 0, r.right, r.bottom-rSB.bottom, this->m_hWnd, NULL, 0);

	if(m_ViewGraph.m_hWnd == NULL) { AfxMessageBox("m_ViewGraph.m_hWnd == NULL"); return -1;}
	
	return 0;
}

void CViewWnd::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	::SendMessage(m_hStatusBar, WM_SIZE, 0, 0);
	RECT rSB;
	::GetClientRect(m_hStatusBar, &rSB);
	int nParts = 2;
	int SizeParts[2] = {rSB.right/2, -1};
	::SendMessage(m_hStatusBar, SB_SETPARTS, (WPARAM) nParts, (LPARAM) SizeParts);

	RECT r;
	::GetClientRect(this->m_hWnd, &r);
	::MoveWindow(m_ViewGraph.m_hWnd, 0, 0, r.right, r.bottom-rSB.bottom, TRUE);
}

BOOL CViewWnd::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	// TODO: Add your specialized code here and/or call the base class
	char str[64];
	char FormatStr[64];
	DATA_OUT* pNewData;
	int nNewData;
	int i, k;
	NMHDR* hdr = (NMHDR*) lParam;
	//if(NotifyCoord->hdr.hwndFrom == m_ViewGraph.m_hWnd)
	if(hdr->hwndFrom == m_ViewGraph.m_hWnd)
		{
		if(hdr->code == WM_MOUSEMOVE)
			{
			NOTIFY_COORD* NotifyCoord = (NOTIFY_COORD*) lParam;
			sprintf(FormatStr, " X = %c%c%i%c%c  Y = %c%c%i%c%c",
				'%','.',NotifyCoord->n_digitsX,'l','f', '%','.',NotifyCoord->n_digitsY,'l','f');
			sprintf(str, FormatStr, NotifyCoord->x, NotifyCoord->y);
			::SendMessage(m_hStatusBar, SB_SETTEXT, 
					0, (LPARAM) (LPSTR) str);
			}
		
		else if(hdr->code == WM_LBUTTONUP)
			{//AfxMessageBox("WM_LBUTTONUP");
			NOTIFY_ZOOM* NotifyZoom = (NOTIFY_ZOOM*) lParam;
			double x, y;
		//	char s[256];
		//	sprintf(s, "\n xBeg=%lf\n\n xEnd=%lf\n\n yBeg=%lf\n\n yEnd=%lf\n\n Data[0].x=%lf",
		//		NotifyZoom->xBeg, NotifyZoom->xEnd, NotifyZoom->yBeg, NotifyZoom->yEnd,
		//		I2D(m_ViewGraph.m_pReg->m_pDataOut[0].x) );
		//	::MessageBox(m_hWnd, s, "AAA", MB_OK);
			//AfxMessageBox(s);
			
			for(i=0; i<this->m_ViewGraph.m_pReg->m_NDataOut; ++i)
				{
				x = I2D(m_ViewGraph.m_pReg->m_pDataOut[i].x);
				y = m_ViewGraph.m_pReg->m_pDataOut[i].y;
				if( x >= NotifyZoom->xBeg && x <= NotifyZoom->xEnd && y >= NotifyZoom->yBeg && y <= NotifyZoom->yEnd)
					{	
					//m_ViewGraph.m_pDataAll = (DATA_OUT*) &m_ViewGraph.m_pReg->m_pDataOut[i].x;
					//m_ViewGraph.m_pDataAll = &m_ViewGraph.m_pReg->m_pDataOut[i];
					m_ViewGraph.y1 = m_ViewGraph.y2 = m_ViewGraph.m_pReg->m_pDataOut[i].y;
					if(i>0) --i;
					pNewData = &m_ViewGraph.m_pReg->m_pDataOut[i];
					nNewData = i;  //Здесь просто как int	
					//m_ViewGraph.m_NDataAll = i; //Здесь просто как int	
					break;
					}
				}
			if(i == this->m_ViewGraph.m_pReg->m_NDataOut) ;
			//	::MessageBox(m_hWnd,"i == this->m_ViewGraph.m_pReg->m_NDataOut","Attention",MB_OK);
			else
				{
				k=0;
				for(k=i+1; k<this->m_ViewGraph.m_pReg->m_NDataOut; ++k)
					{	
					if( I2D(m_ViewGraph.m_pReg->m_pDataOut[k].x) >= NotifyZoom->xEnd ) break;
					if( m_ViewGraph.m_pReg->m_pDataOut[k].y >= NotifyZoom->yBeg && m_ViewGraph.m_pReg->m_pDataOut[k].y <= NotifyZoom->yEnd)
						{
						i=k;
						if(m_ViewGraph.m_pReg->m_pDataOut[k].y < m_ViewGraph.y1)
							m_ViewGraph.y1 = m_ViewGraph.m_pReg->m_pDataOut[k].y;
						else if(m_ViewGraph.m_pReg->m_pDataOut[k].y > m_ViewGraph.y2)
							m_ViewGraph.y2 = m_ViewGraph.m_pReg->m_pDataOut[k].y;
	
						}
					}
				if(k < m_ViewGraph.m_pReg->m_NDataOut-1) ++i;
				nNewData = i - nNewData + 1;
				if(nNewData >= 4)
					{
					m_ViewGraph.m_NDataAll = nNewData;	
					m_ViewGraph.m_pDataAll = pNewData;
					m_ViewGraph.ReDrawAll();	
					}
				}
			
			//m_ViewGraph.m_NDataAll = i - m_ViewGraph.m_NDataAll + 1;
			
			//m_ViewGraph.ReDrawAll();	
			}// end else if(NotifyCoord->hdr.code == WM_LBUTTONUP)

		else if(hdr->code == WM_RBUTTONDOWN)
			{
			if(m_ViewGraph.m_pDataAll != m_ViewGraph.m_pReg->m_pDataOut || m_ViewGraph.m_NDataAll != m_ViewGraph.m_pReg->m_NDataOut)
				{
				m_ViewGraph.m_NDataAll = m_ViewGraph.m_pReg->m_NDataOut;	
				m_ViewGraph.m_pDataAll = m_ViewGraph.m_pReg->m_pDataOut;
				m_ViewGraph.ReDrawAll();	
				}
			}

		}// end if(NotifyCoord->hdr.hwndFrom == m_pMainFrame->m_Doc.m_Graph.m_hWnd)
	
	
	
	return CWnd::OnNotify(wParam, lParam, pResult);
}

void CViewWnd::OnSizing(UINT fwSide, LPRECT pRect) 
{
	CWnd::OnSizing(fwSide, pRect);
	
	// TODO: Add your message handler code here
	int cx = 200;
	int cy = 150;
	int CY = 0;
	SIZE size;
	RECT r;
	char* str = "A";
	::GetWindowRect(m_hStatusBar, &r);
	CY += (r.bottom - r.top);
	if(::IsWindow(this->m_ViewGraph.m_hWnd))
		{
		HDC dc = ::GetDC(this->m_ViewGraph.m_hWnd);
		HFONT OldFont = (HFONT) ::SelectObject(dc, this->m_ViewGraph.m_Font);
		::GetTextExtentPoint32(dc, str, strlen(str), &size);
		CY += (10*size.cy);
		::SelectObject(dc, OldFont);
		::ReleaseDC(this->m_ViewGraph.m_hWnd, dc);
		}
	else CY += 50;
	
	if((2*this->m_ViewGraph.m_LMaxTextY) > 200) cx = 2*this->m_ViewGraph.m_LMaxTextY;
	if( (pRect->right - pRect->left) < cx)
		{
		if( (fwSide==WMSZ_BOTTOMRIGHT) || (fwSide==WMSZ_TOPRIGHT) || (fwSide==WMSZ_RIGHT)) 
			pRect->right = pRect->left + cx;//AfxMessageBox("<200");
		else pRect->left = pRect->right - cx;
		}
	
	if(CY > cy) cy = CY;
	if( (pRect->bottom - pRect->top) < cy)
		{
		if( (fwSide==WMSZ_BOTTOM) || (fwSide==WMSZ_BOTTOMRIGHT) || (fwSide==WMSZ_BOTTOMLEFT)) 
			pRect->bottom = pRect->top + cy;//AfxMessageBox("<200");
		else pRect->top = pRect->bottom - cy;
		}
	
}
