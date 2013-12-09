// MainFrame.cpp : implementation file
//
/*
#include "stdafx.h"

#include "ProgNew.h"
#include "BigClientWnd.h"
#include "HideWnd.h"
#include "Region.h"
#include "DialogParamRegion.h"
#include "ListRegionWnd.h"
#include "RegionWnd.h"
//#include "MainFrame.h"
#include "OpenSaveFun.h"
#include "MainFrame.h"
*/
#include "stdafx.h"
#include "DlgAbout.h"
#include "Main.h"
#include "MainFrame.h"
#include "SerialCounter\SerialCounterDlg.h"

//#include<stdio.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

extern UINT MY_WM_RESIZE;
extern CMutex MutexThread;
extern CProgNewApp theApp;
extern CString FileSaveOpenErrorDescription;
UINT wm_RegRemoteDxpsStartID=RegisterWindowMessage("RemoteDxpsStart");
UINT wm_CurrentTempMessage=RegisterWindowMessage("RemoteCurrentTemp");

#define POST_POST_CREATEWINDOW 10
#define TIMER_CHECK_DXPS_STATE 11
#define TIMER_CHECK_VALID_TEMPERATURE 12
//IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

CMainFrame::CMainFrame() : m_pHideWnd(0), m_ScreenDpi(96)
{
	if(!theApp.Ini.HighPressureMode.Value)
		strcpy(AppTitle, "KRATOS");
	else
		strcpy(AppTitle, "HP");
	sprintf(m_Doc.m_WindowCaption, "%s - %s", AppTitle, "[unsaved]");
	
	HDC dc = CreateDC("DISPLAY", NULL, NULL, NULL);
	m_ScreenDpi = GetDeviceCaps(dc, LOGPIXELSX);
	DeleteDC(dc);

	m_pRegion = NULL;
	m_pRegionWnd = new CRegionWnd;
	m_pRegionWnd->RegisterRegionWndClass();
	m_pRegionWnd->m_pMainFrame = this;
	m_Doc.fpPrj=NULL;
	m_Doc.m_NeedSave = m_Doc.NoNeed;
	m_Doc.m_ThrComm.pRegNow = NULL;
	m_Doc.m_ThrComm.pRegEdit = NULL;
	m_Doc.m_ThrComm.fp = NULL;
	m_Doc.m_ThrComm.RetardCalibration=theApp.Ini.RetardCalibration.Value;
//	/*
	CWinApp* App=AfxGetApp();
	m_Doc.m_ThrComm.NSigma = App->GetProfileInt("Measuring Options", "NSigma", 30)/10.;
	m_Doc.m_ThrComm.Attempts= App->GetProfileInt("Measuring Options", "Attempts", 3);
	m_Doc.m_ThrComm.SubMeasurings=App->GetProfileInt("Measuring Options", "SubMeasurings", 1);
	m_Doc.m_ThrComm.EnableRemeasure=App->GetProfileInt("Measuring Options", "EnableRemeasure", 1);
	m_Doc.m_ThrComm.RegionDelay=App->GetProfileInt("Measuring Options", "RegionDelay", 0);

//	*/
	//	m_Doc.m_ThrComm.pRegWnd = m_pRegionWnd;
	m_StartStop = Start;
	m_pDxpsDlg = new CDxpsDlg;
	m_bSynchronousResize=FALSE;

}

CMainFrame::~CMainFrame()
{
	if(m_pRegion) { while(m_pRegion = CRegion::GetFirst()) delete m_pRegion;}
	delete m_pRegionWnd;
	m_pDxpsDlg->DestroyWindow();
	delete m_pDxpsDlg;
	if( m_pHideWnd )
		delete m_pHideWnd;
	LogFile("--- Exit Program ---");
}


BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_COMMAND(ID_FILE_NEW_PROJECT, OnFileNew)
	ON_WM_SIZE()
	ON_WM_SIZING()
	ON_COMMAND(ID_PROGRAM_REGION_TABLE, OnRegions)
	ON_WM_DESTROY()
	ON_COMMAND(ID_FILE_EXIT, OnFileExit)
	ON_COMMAND(ID_FILE_SAVE_AS_EASYPLOT, OnFileSaveAsEasyPlot)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE_AS_EASYPLOT, OnUpdateFileSaveAsEasyPlot)
	ON_COMMAND(ID_FILE_SAVE_PROJECT_AS, OnFileSaveProjectAs)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE_PROJECT_AS, OnUpdateFileSaveProjectAs)
	ON_COMMAND(ID_PROGRAMM_START, OnProgrammStart)
	ON_UPDATE_COMMAND_UI(ID_PROGRAMM_START, OnUpdateProgrammStart)
	ON_COMMAND(ID_PROGRAMM_STOP, OnProgrammStop)
	ON_UPDATE_COMMAND_UI(ID_PROGRAMM_STOP, OnUpdateProgrammStop)
	ON_COMMAND(ID_SETTINGS_CAMAC_SETUP, OnSettingsCamacSetup)
	ON_COMMAND(ID_SETTINGS_HARDWARESETUP, OnSettingsHardwareSetup)
	ON_COMMAND(ID_SETTINGS_MEASURING_OPTIONS, OnSettingsMeasuringOptions)
	ON_COMMAND(ID_FILE_OPEN_PROJECT, OnFileOpenProject)
	ON_UPDATE_COMMAND_UI(ID_FILE_OPEN_PROJECT, OnUpdateFileOpenProject)
	ON_COMMAND(ID_GRAPHICS_FONT, OnGraphicsFont)
	ON_COMMAND(ID_FILE_SAVE_PROJECT, OnFileSaveProject)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE_PROJECT, OnUpdateFileSaveProject)
	ON_UPDATE_COMMAND_UI(ID_FILE_NEW_PROJECT, OnUpdateFileNewProject)
	ON_UPDATE_COMMAND_UI(ID_FILE_EXIT, OnUpdateFileExit)
	ON_WM_CLOSE()
	ON_COMMAND(ID_GRAPHICS_GRID, OnGraphicsGrid)
	ON_UPDATE_COMMAND_UI(ID_GRAPHICS_GRID, OnUpdateGraphicsGrid)
	ON_COMMAND(ID_GRAPHICS_SETTINGS_FOR_CURVERS, OnGraphicsSettingsForCurvers)
	ON_COMMAND(ID_GRAPHICS_SETTINGS_FOR_VIEWER, OnGraphicsSettingsForViewer)
	ON_UPDATE_COMMAND_UI(ID_GRAPHICS_SETTINGS_FOR_VIEWER, OnUpdateGraphicsSettingsForViewer)
	ON_COMMAND(ID_FILE_SAVE_AS_EASYPLOT_MC, OnFileSaveAsEasyplotMc)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE_AS_EASYPLOT_MC, OnUpdateFileSaveAsEasyplotMc)
	ON_COMMAND(ID_SETTINGS_FITABLE, OnSettingsFitable)
	ON_COMMAND(ID_FILE_SAVE_AS_ORIGIN, OnFileSaveAsOrigin)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE_AS_ORIGIN, OnUpdateFileSaveAsOrigin)
	ON_COMMAND(ID_PROGRAM_DXPS_TABLE, OnProgramDxpsTable)
	ON_UPDATE_COMMAND_UI(ID_PROGRAM_DXPS_TABLE, OnUpdateProgramDxpsTable)
	ON_UPDATE_COMMAND_UI(ID_PROGRAM_REGION_TABLE, OnUpdateProgramRegionTable)
	ON_COMMAND(ID_FILE_RECENTPROJECTS_FILE1, OnFileRecentProjectsFile1)
	ON_COMMAND(ID_FILE_RECENTPROJECTS_FILE2, OnFileRecentProjectsFile2)
	ON_COMMAND(ID_FILE_RECENTPROJECTS_FILE3, OnFileRecentProjectsFile3)
	ON_COMMAND(ID_FILE_RECENTPROJECTS_FILE4, OnFileRecentProjectsFile4)
	ON_COMMAND(ID_ABOUT, OnAbout)
	ON_WM_ERASEBKGND()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_POSTCREATEWINDOW, OnPostCreateWindow)
	ON_REGISTERED_MESSAGE(wm_RegRemoteDxpsStartID, OnRemoteDxpsStart)
	ON_REGISTERED_MESSAGE(wm_CurrentTempMessage, OnRemoteCurrentTempMessage)
	ON_COMMAND(ID_SETTINGS_COUNTERSETUP, OnSettingsCountersetup)
	ON_UPDATE_COMMAND_UI(ID_SETTINGS_COUNTERSETUP, &CMainFrame::OnUpdateSettingsCounterSetup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs) 
{
	// TODO: Add your specialized code here and/or call the base class
	//cs.lpszName = "Program";
	cs.lpszName = m_Doc.m_WindowCaption;
	cs.style |= WS_CLIPCHILDREN;  
	
	CWinApp* App = AfxGetApp();
	cs.x = App->GetProfileInt("MainFrameSize", "X", 0);
	cs.y = App->GetProfileInt("MainFrameSize", "Y", 0);
	cs.cx = App->GetProfileInt("MainFrameSize", "CX", GetSystemMetrics(SM_CXSCREEN));
	cs.cy = App->GetProfileInt("MainFrameSize", "CY", GetSystemMetrics(SM_CYSCREEN)/3);
      // Use the specific class name you established earlier.
    cs.lpszClass = _T("KratosWindowClass");

	//::SetClassLong(this->m_hWnd, GCL_HBRBACKGROUND, (LONG) GetStockObject(BLACK_BRUSH));
	return CFrameWnd::PreCreateWindow(cs);
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	//if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
	//	return -1;
	
	// TODO: Add your specialized creation code here
	if(GetExStyle() & WS_EX_CLIENTEDGE) 
		ModifyStyleEx(WS_EX_CLIENTEDGE,0,SWP_FRAMECHANGED);
	::SetClassLong(this->m_hWnd, GCL_HBRBACKGROUND, (LONG) GetStockObject(LTGRAY_BRUSH));
	m_bSynchronousResize = TRUE; // Проигнорируем настройки размеров окна DxpsDlg
	m_pDxpsDlg->Create();
	m_bSynchronousResize = FALSE;

	m_pRegionWnd->CreateEx(0, m_pRegionWnd->m_WC.lpszClassName, "Regions",
		WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
		m_pRegionWnd->m_rectWnd.left, m_pRegionWnd->m_rectWnd.top,
		m_pRegionWnd->m_rectWnd.right - m_pRegionWnd->m_rectWnd.left,
		m_pRegionWnd->m_rectWnd.bottom - m_pRegionWnd->m_rectWnd.top,
		NULL,NULL);

	if(this->m_StartStop == Stop) 
		{
		SetIconForReg(this->m_pRegionWnd->m_pListRegionWnd, m_Doc.m_ThrComm.pRegNow, 1);
		}
	
	CRegion* pReg;
	for(pReg=CRegion::GetFirst(); pReg!=NULL; pReg=CRegion::GetNext(pReg))
		{
		if(pReg->m_DataIn.Off == TRUE) SetIconForReg(m_pRegionWnd->m_pListRegionWnd, pReg, 2);
		}

	if(!CreateClient(lpCreateStruct)) return -1; //if(CreateClient(lpCreateStruct) == FALSE) return -1;
	PostMessage(WM_POSTCREATEWINDOW);
	SetRecentProjects();
	m_RegDxpsMessageID=RegisterWindowMessage("RemoteDxpsState");
	SetTimer(TIMER_CHECK_DXPS_STATE,1000,NULL);
	SetTimer(TIMER_CHECK_VALID_TEMPERATURE,1000,NULL);

	return 0;
}

BOOL CMainFrame::CreateClient(LPCREATESTRUCT lpcs) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	if(!CreateStatusAndToolBar()) return FALSE; //if(CreateStatusAndToolBar() == FALSE) return FALSE;
	if(!CreateHideWnd()) return FALSE;
	RECT rect={50,50,200,200};
	const char* class_name = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW,
	AfxGetApp()->LoadStandardCursor(IDC_ARROW),
	(HBRUSH) GetStockObject(LTGRAY_BRUSH), 0);
	m_Graph.Create(class_name, NULL,
		WS_CHILD|WS_CLIPCHILDREN,//|WS_VISIBLE,//|WS_HSCROLL|WS_VSCROLL, 
		rect, this, NULL, 0);
	if(!IsWindow(m_Graph.m_hWnd))
	{
		MessageBox("Ошибка! Компонент \"NTGraph.ocx\" не установлен. Необходимо "
			"скопировать компонент NTGraph.ocx в \"%WINDIR%\\System32\" и из этого "
			"каталога выполнить \"regsvr32.exe ntgraph.ocx\".",
			"Kratos", MB_ICONSTOP);
		return TRUE;
	}
	m_Graph.SetPlotAreaColor(RGB(0,0,0));
    m_Graph.SetGridColor(RGB(0x55,0x77,0x55));
	m_Graph.SetShowGrid(TRUE);
	m_Graph.SetFrameStyle(0);

	ULONG R=((( (GetSysColor(COLOR_3DFACE)&0xff0000) + (GetSysColor(COLOR_3DHILIGHT)&0xff0000) )/2)&0xff0000);
	ULONG G=((((GetSysColor(COLOR_3DFACE)&0xff00)+(GetSysColor(COLOR_3DHILIGHT)&0xff00))/2) &0xff00);
	ULONG B=( ((GetSysColor(COLOR_3DFACE)&0xff) + (GetSysColor(COLOR_3DHILIGHT)&0xff)) /2);
	
    m_Graph.SetControlFrameColor(R+G+B);
	CDxpsRegion::ScanTime=atof(theApp.GetProfileString("DxpsDlg", "ScanTime", "30"));
	m_Graph.SetRange(0, 1/24./60./60.*CDxpsRegion::ScanTime, 0,100);
	m_Graph.SetYGridNumber(10);
	m_Graph.SetXTime(TRUE);
	m_Graph.SetFormatAxisBottom("%M:%S");
	m_Graph.SetXGridNumber(6);
	m_Graph.SetTrackMode(2); //Zoom
	return TRUE;
	//return CFrameWnd::OnCreateClient(lpcs, pContext);
}

BOOL CMainFrame::CreateStatusAndToolBar()
{
	::InitCommonControls();
	m_hStatusBar = ::CreateStatusWindow(WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS,
		NULL, m_hWnd, IDW_STATUS_BAR);  // IDW_STATUS_BAR -- идентификатор StatusBar
	if(m_hStatusBar==NULL) return FALSE;
	
	ResizeStatusBar();
	
	// Tool Bar
	
	TBBUTTON* TB = CreateStructTBBUTTON();
	if(TB==NULL) return FALSE;
	m_hToolBar = ::CreateToolbarEx(this->m_hWnd, 
		WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS|WS_CLIPCHILDREN|TBSTYLE_TOOLTIPS, 
		IDW_TOOL_BAR, 8,
		AfxGetInstanceHandle(), IDR_BITMAPTOOLBAR, TB, NumberToolbarsButton,
		16, 15, 16, 15, sizeof(TBBUTTON));
	delete[] TB;

	if(m_hToolBar==NULL) return FALSE;
	
	//::SendMessage(m_hToolBar, TB_PRESSBUTTON, (WPARAM) ID_PROGRAMM_STOP,(LPARAM) MAKELONG(TRUE, 0));
	::SendMessage(m_hToolBar, TB_CHECKBUTTON  , (WPARAM) ID_PROGRAMM_STOP,(LPARAM) MAKELONG(TRUE, 0));
	
	
	m_SAndTBarHWND.Sb = m_hStatusBar;
	m_SAndTBarHWND.Tb = m_hToolBar;

	//::SendMessage(m_hToolBar, TB_ENABLEBUTTON , (WPARAM) ID_FILE_SAVE_PROJECT,(LPARAM) MAKELONG(FALSE, 0));
	
	return TRUE;
}

void CMainFrame::ResizeStatusBar()
{
	RECT rSB;
	::GetClientRect(m_hStatusBar, &rSB);
	// Части статус-бара: координаты курсора, параметры измерения XPS, оставшееся время
	// "X = , Y = ", "Volts = %.3lf  F = %i  dF = %i", "%.2i:%.2i:%.2i"
	const int nParts = 4;
	int coordPart = min(rSB.right/3, 200*m_ScreenDpi/m_DefaultDpi);
	int timePart = min(rSB.right/7, 70*m_ScreenDpi/m_DefaultDpi);
	int tempPart = min(rSB.right/6, 105*m_ScreenDpi/m_DefaultDpi);
	int xpsPart = rSB.right - coordPart - timePart - tempPart;
	int SizeParts[nParts] = {coordPart, coordPart + xpsPart, coordPart + xpsPart + timePart, -1};
	::SendMessage(m_hStatusBar, SB_SETPARTS, (WPARAM) nParts, (LPARAM) SizeParts);
}

///<summary>Показывает температуру в статус-баре<summary>
///<param name="Temp">Температура в C<param>
///<param name="Defined">Определена (известна) ли температура<param> 
void CMainFrame::SetStatusTemperature(double Temp, bool Defined)
{
	CString str;
	if(Defined)
		str.Format("T Cur: %.1fC", Temp);
	else
		str = "T Cur: ?";
	::SendMessage(this->m_hStatusBar, SB_SETTEXT, StatusBarPartTemperature, (LPARAM)(LPCSTR)str);
}

///<summary>Показывает оставшееся время в статус-баре<summary>
///<param name="Seconds">Время в милисекундах<param>
///<param name="Defined">Определена (известна) ли температура; если false, то не отображается<param> 
void CMainFrame::SetStatusTime(int Seconds, bool Defined)
{
	char str[255];
	if(Defined)
		TIME2Str(Seconds, str);

	else
		sprintf(str, "\n");
	::SendMessage(this->m_hStatusBar, SB_SETTEXT, StatusBarPartTime, (LPARAM)str);
}

TBBUTTON* CMainFrame::CreateStructTBBUTTON()
{
	NumberToolbarsButton = 11; 
	TBBUTTON* tb = new TBBUTTON[11];
	if(tb==NULL) return NULL;
	int tbBitmap[] = {0,1,2,0,3,0,4,5,0,6,7};
	int tbCommand[] = { ID_FILE_NEW_PROJECT,
						ID_FILE_OPEN_PROJECT,
						ID_FILE_SAVE_PROJECT,
						0,
						ID_D,
						0,
						ID_PROGRAM_REGION_TABLE,
						ID_PROGRAM_DXPS_TABLE,
						0,
						ID_PROGRAMM_START,
						ID_PROGRAMM_STOP
						};
	BYTE tbState[] = { TBSTATE_ENABLED,
					   TBSTATE_ENABLED,	
					   TBSTATE_ENABLED, // См выше ()
					   0,
					   TBSTATE_INDETERMINATE,
						 //TBSTATE_ENABLED,
						 0,
						 TBSTATE_ENABLED,
						 TBSTATE_ENABLED,
						 0,
						 TBSTATE_ENABLED,
						 TBSTATE_ENABLED
					};
	BYTE tbStyle[] = {  TBSTYLE_BUTTON,
						TBSTYLE_BUTTON,
						TBSTYLE_BUTTON,
						TBSTYLE_SEP,
						TBSTYLE_BUTTON,
						TBSTYLE_SEP,
						TBSTYLE_BUTTON,
						TBSTYLE_BUTTON,
						TBSTYLE_SEP,
						TBSTYLE_BUTTON|TBSTYLE_CHECKGROUP,
						TBSTYLE_BUTTON|TBSTYLE_CHECKGROUP,
					};
	char*  str[11]={"New Project\0", "Open Project\0", "Save Project\0","\0","\0","\0","\0","\0","\0","\0","\0"};
	for(int i=0; i<NumberToolbarsButton; ++i)
		{
		tb[i].iBitmap = tbBitmap[i];
		tb[i].idCommand = tbCommand[i]; // Need replace
		tb[i].fsState = tbState[i] ;
		tb[i].fsStyle = tbStyle[i];
		tb[i].dwData = 0;
		tb[i].iString = ::SendMessage(m_hToolBar, TB_ADDSTRING, (WPARAM) 0,(LPARAM)(LPSTR) str[i]); 
		}
	
	return tb;
}

BOOL CMainFrame::CreateHideWnd()
{
	m_pHideWnd = new CHideWnd;
	m_pHideWnd->m_pMainFrame = this;
	m_pHideWnd->m_hWnd = NULL;
	m_pHideWnd->m_pBigClientWnd = NULL;
	const char* class_name = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW,
		//::LoadCursor(AfxGetInstanceHandle(), IDC_ARROW), 
		AfxGetApp()->LoadStandardCursor(IDC_ARROW),
		(HBRUSH) GetStockObject(LTGRAY_BRUSH), 0);
	RECT rc_sb, rc_tb;
	::GetWindowRect(m_hStatusBar,  &rc_sb);
	::GetWindowRect(m_hToolBar, &rc_tb);
	::ScreenToClient(this->m_hWnd, (LPPOINT) &rc_sb);
	::ScreenToClient(this->m_hWnd, (LPPOINT) &rc_sb.right);
	::ScreenToClient(this->m_hWnd, (LPPOINT) &rc_tb);
	::ScreenToClient(this->m_hWnd, (LPPOINT) &rc_tb.right);
	m_pHideWnd->CreateEx(WS_EX_CLIENTEDGE, class_name, NULL,
		WS_CHILD|WS_CLIPCHILDREN|WS_VISIBLE,//|WS_HSCROLL|WS_VSCROLL, 
		rc_tb.left, rc_tb.bottom, 
		rc_tb.right - rc_tb.left, rc_sb.top - rc_tb.bottom,
		this->m_hWnd, NULL, 0);
	
	if(m_pHideWnd->m_hWnd == NULL) return FALSE;
	
	
	return TRUE;
}

void CMainFrame::OnSize(UINT nType, int cx, int cy) 
{
	::SendMessage(m_hStatusBar, WM_SIZE, 0, 0);
	ResizeStatusBar();

	::SendMessage(m_hToolBar, WM_SIZE, 0, 0);
	if(m_pHideWnd) ::SendMessage(m_pHideWnd->m_hWnd, MY_WM_RESIZE, 
										(WPARAM) this->m_hWnd, (LPARAM) &m_SAndTBarHWND);

	RECT rc_sb, rc_tb;
	::GetWindowRect(m_SAndTBarHWND.Sb,  &rc_sb);
	::GetWindowRect(m_SAndTBarHWND.Tb, &rc_tb);
	::ScreenToClient(m_hWnd, (LPPOINT) &rc_sb); ::ScreenToClient(m_hWnd, (LPPOINT) &rc_sb.right);
	::ScreenToClient(m_hWnd, (LPPOINT) &rc_tb); ::ScreenToClient(m_hWnd, (LPPOINT) &rc_tb.right);
	
	if(::IsWindow(m_Graph.GetSafeHwnd()))
	{
		m_Graph.MoveWindow (rc_tb.left, rc_tb.bottom, rc_tb.right - rc_tb.left, rc_sb.top - rc_tb.bottom);
	//LogFileFormat("%i, %i - %i, %i", rc_tb.left, rc_tb.bottom, rc_tb.right - rc_tb.left, rc_sb.top - rc_tb.bottom);
	}
	//else
		//LogFileFormat("Invalid hWnd %i, %i - %i, %i", rc_tb.left, rc_tb.bottom, rc_tb.right - rc_tb.left, rc_sb.top - rc_tb.bottom);

//*/
}


void CMainFrame::OnSizing(UINT fwSide, LPRECT pRect) 
{
	CFrameWnd::OnSizing(fwSide, pRect);
	
	// Х.з. что тут делалось, но возникает бага с изменением размера главного окна: оно то раскрывается на 2 монитора, то исчезает
	/*int cx = 200;
	int cy = 150;
	int CY = 0;
	SIZE size;
	RECT r;
	::GetWindowRect(m_hStatusBar, &r);
	CY += (r.bottom - r.top);
	::GetWindowRect(m_hToolBar, &r);
	CY += (r.bottom - r.top);
	if(::IsWindow(m_Doc.m_Graph.m_hWnd))
		{
		HDC dc = ::GetDC(m_Doc.m_Graph.m_hWnd);
		HFONT OldFont = (HFONT) ::SelectObject(dc, m_Doc.m_Graph.m_Font);
		::GetTextExtentPoint32(dc, str, strlen(str), &size);
		CY += (10*size.cy);
		::SelectObject(dc, OldFont);
		::ReleaseDC(m_Doc.m_Graph.m_hWnd, dc);
		}
	else CY += 50;
	
	if((2*m_Doc.m_Graph.m_LMaxTextY) > 200) cx = 2*m_Doc.m_Graph.m_LMaxTextY;
	if( (pRect->right - pRect->left) < cx)
		{
		if( (fwSide==WMSZ_BOTTOMRIGHT) || (fwSide==WMSZ_TOPRIGHT) || (fwSide==WMSZ_RIGHT)) 
			pRect->right = pRect->left + cx;
		else pRect->left = pRect->right - cx;
		}
	
	if(CY > cy) cy = CY;
	if( (pRect->bottom - pRect->top) < cy)
		{
		if( (fwSide==WMSZ_BOTTOM) || (fwSide==WMSZ_BOTTOMRIGHT) || (fwSide==WMSZ_BOTTOMLEFT)) 
			pRect->bottom = pRect->top + cy;
		else pRect->top = pRect->bottom - cy;
		}*/
}

void CMainFrame::RecalcClient() // Need del
{
	::SendMessage(m_hStatusBar, WM_SIZE, 0, 0);
	::SendMessage(m_hToolBar, WM_SIZE, 0, 0);
	RECT rc_sb, rc_tb;
	::GetWindowRect(m_hStatusBar,  &rc_sb);
	::GetWindowRect(m_hToolBar, &rc_tb);
	::ScreenToClient(this->m_hWnd, (LPPOINT) &rc_sb);
	::ScreenToClient(this->m_hWnd, (LPPOINT) &rc_sb.right);
	::ScreenToClient(this->m_hWnd, (LPPOINT) &rc_tb);
	::ScreenToClient(this->m_hWnd, (LPPOINT) &rc_tb.right);
	
	//if(m_pHideWnd && m_pHideWnd->m_hWnd) 
	//		::SendMessage(m_pHideWnd->m_hWnd, WM)
	
	//if(m_pHideWnd==NULL) AfxMessageBox("m_pHideWnd == NULL");
	//else AfxMessageBox("m_pHideWnd != NULL");
	//if(!::IsWindow(m_pHideWnd->m_hWnd)) AfxMessageBox("m_pHideWnd->m_hWnd == NULL");
	//
	/*
	if(m_pHideWnd)
		::MoveWindow(m_pHideWnd->m_hWnd,
					rc_tb.left, rc_tb.bottom, 
					rc_tb.right - rc_tb.left, rc_sb.top - rc_tb.bottom,
					FALSE);
	*/
}

void CMainFrame::OnFileNew() 
{
CSingleLock sLock(&MutexThread);	
	// TODO: Add your command handler code here

//	HMENU	SysMenu = ::GetSystemMenu(this->m_hWnd, FALSE);
//	::EnableMenuItem(SysMenu, SC_CLOSE, MF_GRAYED);
	//AfxMessageBox("MF_GRAYED");
	//::EnableMenuItem(SysMenu, SC_CLOSE, MF_ENABLED);

int YesNoCancel; 

THRI_LOCK();
//Дополнительное сохранение
if(m_Doc.CheckDocType()!=m_Doc.NoDoc)
	m_Doc.m_NeedSave = m_Doc.Need;
if(m_Doc.m_NeedSave == m_Doc.Need)
	{
	YesNoCancel = AfxMessageBox("The progect has changed.\n\nDo you want save the changes?", 
																MB_YESNOCANCEL);
	if(YesNoCancel == IDCANCEL) { THRI_UNLOCK(); return;}
	else if(YesNoCancel == IDYES)
		{
		if(m_Doc.fpPrj) SaveBinaryFile(m_Doc.fpPrj);
		else // if(m_Doc.fpPrj == NULL)
			{	
			m_Doc.m_TypeFile = m_Doc.Project; 
			m_Doc.m_SaveAsOpen = m_Doc.SaveAs; 
			WindowSaveAsOpen(this);			
			}
		//return;
		}
	}// end if(m_Doc.m_NeedSave == m_Doc.Need)
if(m_Doc.fpPrj) fclose(m_Doc.fpPrj);
m_Doc.fpPrj=NULL;
m_Doc.m_ThrComm.pRegNow = NULL;
m_Doc.m_ProjectFile.FullPath[0] = '\0';


m_Doc.m_Graph.m_pDataAll=NULL;
m_Doc.m_Graph.m_pDataShort=NULL;
m_Doc.m_Graph.ReDrawAll();


if(::IsWindow(m_Doc.m_ViewWnd.m_hWnd))
	{
	m_Doc.m_ViewWnd.m_ViewGraph.m_pDataAll = NULL;
	m_Doc.m_ViewWnd.m_ViewGraph.ReDrawAll();
	::SendMessage(m_Doc.m_ViewWnd.m_hWnd, WM_CLOSE, 0,0);
	m_Doc.m_ViewWnd.DestroyWindow();
	}
EmptyAllData();
m_Doc.m_NeedSave = m_Doc.NoNeed;
m_Doc.CheckDocType();

if( !(::IsWindow(m_pRegionWnd->m_pListRegionWnd->m_hWnd)) ) 
		OnRegions();
else
	{	m_pRegionWnd->ShowWindow(SW_SHOWNORMAL);
		::SetFocus(m_pRegionWnd->m_hWnd);
	}

if( (::IsWindow(this->m_pRegionWnd->m_pListRegionWnd->m_CommentsWnd.m_hWnd) ))
	{
	char str[32];
	sprintf(str, "No Comments");
	::SendMessage(this->m_pRegionWnd->m_pListRegionWnd->m_CommentsWnd.m_hWnd, WM_SETTEXT, 
								0, (LPARAM) str);
	::SendMessage(this->m_pRegionWnd->m_pListRegionWnd->m_CommentsWnd.m_hWndEdit, WM_SETTEXT, 
								0, (LPARAM) str);
	}	


SetNewTIME(&m_Doc.m_ThrComm.TIME);
SetStatusTime(m_Doc.m_ThrComm.TIME);

THRI_UNLOCK();

//Clear status
char *strn="";
::SendMessage(theApp.m_pMainFrame->m_hStatusBar, SB_SETTEXT, 0, (LPARAM) (LPSTR) strn);
::SendMessage(theApp.m_pMainFrame->m_hStatusBar, SB_SETTEXT, 1, (LPARAM) (LPSTR) strn);

sprintf(m_Doc.m_WindowCaption, "%s - %s", AppTitle, "[no data]");
SetWindowText(m_Doc.m_WindowCaption);
}


void CMainFrame::OnRegions() 
{
	// TODO: Add your command handler code here
	if(::IsWindow(m_pRegionWnd->m_hWnd))
		{	m_pRegionWnd->ShowWindow(SW_SHOWNORMAL);
			::SetFocus(m_pRegionWnd->m_hWnd);
		}
	else
		{
		m_pRegionWnd->CreateEx(0, m_pRegionWnd->m_WC.lpszClassName, "Regions",
			WS_OVERLAPPEDWINDOW | WS_VISIBLE | WS_CLIPCHILDREN,
			m_pRegionWnd->m_rectWnd.left, m_pRegionWnd->m_rectWnd.top,
			m_pRegionWnd->m_rectWnd.right - m_pRegionWnd->m_rectWnd.left,
			m_pRegionWnd->m_rectWnd.bottom - m_pRegionWnd->m_rectWnd.top,
			//0,0,0,0,
			NULL,NULL);
	}
	
}

void CMainFrame::OnDestroy() 
{
//	CFrameWnd::OnDestroy();
	
	// TODO: Add your message handler code here
	//AfxMessageBox("Destroy Main Frame");
	if(m_Doc.fpPrj != NULL)
		{
		fclose(m_Doc.fpPrj);
		m_Doc.fpPrj = NULL;
		}
	
	if(::IsWindow(m_Doc.m_ViewWnd.m_hWnd)) 
		{
		//m_Doc.m_ViewWnd.OnClose();
		::SendMessage(m_Doc.m_ViewWnd.m_hWnd, WM_CLOSE, 0, 0);
		m_Doc.m_ViewWnd.DestroyWindow();
		}

	if(m_pRegionWnd->m_pDlgParamReg)
		{
		if(::IsWindow(m_pRegionWnd->m_pDlgParamReg->m_hWnd)) 
			{
			//m_pRegionWnd->m_pDlgParamReg->EndDialog(IDCANCEL);
			//::EndDialog(m_pRegionWnd->m_pDlgParamReg->m_hWnd, IDCANCEL);
			m_pRegionWnd->m_pDlgParamReg->DestroyWindow();
			delete m_pRegionWnd->m_pDlgParamReg;
			m_pRegionWnd->m_pDlgParamReg = NULL;
			}
		}
	
	CRegion* pReg;
	while((pReg=CRegion::GetFirst()) != NULL) {delete pReg;}
	
	m_pRegionWnd->DestroyWindow();
	HWND hWnd=::FindWindow(_T("SpecWindowClass"),NULL);
		if (hWnd)
			::SendMessage(hWnd,m_RegDxpsMessageID,0,0);
	
CFrameWnd::OnDestroy();
}
//=========
void CMainFrame::OnFileExit() 
{
	// TODO: Add your command handler code here
	::SendMessage(this->m_hWnd, WM_CLOSE, 0, 0);
	//DestroyWindow();
}
//=========

void CMainFrame::OnFileOpenProject() 
{

int YesNoCancel; 
CRegion* pReg;
CSingleLock sLock(&MutexThread);
THRI_LOCK();
//Дополнительное сохранение
if(m_Doc.CheckDocType()!=m_Doc.NoDoc)
	m_Doc.m_NeedSave = m_Doc.Need;

if(m_Doc.m_NeedSave == m_Doc.Need)
	{
	YesNoCancel = AfxMessageBox("The project has changed.\n\nDo you want save the changes?", 
																MB_YESNOCANCEL);
	if(YesNoCancel == IDCANCEL) { THRI_UNLOCK(); return;}
	else if(YesNoCancel == IDYES)
		{
		if(m_Doc.fpPrj) SaveBinaryFile(m_Doc.fpPrj);
		else // if(m_Doc.fpPrj == NULL)
			{	
			m_Doc.m_TypeFile = m_Doc.Project; 
			m_Doc.m_SaveAsOpen = m_Doc.SaveAs; 
			WindowSaveAsOpen(this);			
			}
		//return;
		}
	}// end if(m_Doc.m_NeedSave == m_Doc.Need)
if(m_Doc.fpPrj) fclose(m_Doc.fpPrj);
m_Doc.m_ThrComm.pRegNow = NULL;

m_Doc.m_Graph.m_pDataAll=NULL;
m_Doc.m_Graph.m_pDataShort=NULL;
m_Doc.m_Graph.ReDrawAll();
m_pHideWnd->ShowWindow(SW_SHOW);
m_Graph.ShowWindow(SW_HIDE);


if(::IsWindow(m_Doc.m_ViewWnd.m_hWnd))
	{
	m_Doc.m_ViewWnd.m_ViewGraph.m_pDataAll = NULL;
//	m_Doc.m_ViewWnd.m_ViewGraph.ReDrawAll();
	::SendMessage(m_Doc.m_ViewWnd.m_hWnd, WM_CLOSE, 0,0);
	m_Doc.m_ViewWnd.DestroyWindow();
	}

EmptyAllData();
m_Doc.m_NeedSave = m_Doc.NoNeed;
m_Doc.CheckDocType();

sprintf(m_Doc.m_WindowCaption, "%s - %s", AppTitle, "[no data]");

SetWindowText(m_Doc.m_WindowCaption);

//Clear status
char *strn="";
::SendMessage(theApp.m_pMainFrame->m_hStatusBar, SB_SETTEXT, 0, (LPARAM) (LPSTR) strn);
::SendMessage(theApp.m_pMainFrame->m_hStatusBar, SB_SETTEXT, 1, (LPARAM) (LPSTR) strn);

m_Doc.m_TypeFile = m_Doc.Project; 
m_Doc.m_SaveAsOpen = m_Doc.Open; 
if( !WindowSaveAsOpen(this) )
	{
	THRI_UNLOCK();
	EmptyAllData();
	return;
	}
sprintf(m_Doc.m_WindowCaption, "%s - %s", AppTitle, m_Doc.m_ProjectFile.FileName);
SetWindowText(m_Doc.m_WindowCaption);
m_Doc.CheckDocType();
if(m_Doc.m_DocType==m_Doc.XPS)
	{
	//if( !(::IsWindow(m_pRegionWnd->m_pListRegionWnd->m_hWnd)) ) 

	if(::IsWindow(m_pRegionWnd->m_pListRegionWnd->m_hWnd))
	//	{
	//else
		{	
		for(pReg=CRegion::GetFirst(); pReg!=NULL; pReg=CRegion::GetNext(pReg))
			{ 
			SetNewRegionItemForListView(m_pRegionWnd->m_pListRegionWnd, pReg);
			if(pReg->m_DataIn.Off == TRUE) SetIconForReg(m_pRegionWnd->m_pListRegionWnd, pReg, 2);
			}
		}

	if( (::IsWindow(this->m_pRegionWnd->m_pListRegionWnd->m_CommentsWnd.m_hWnd) ))
		{
		char str[32];
		sprintf(str, "No Comments");
		::SendMessage(this->m_pRegionWnd->m_pListRegionWnd->m_CommentsWnd.m_hWnd, WM_SETTEXT, 
									0, (LPARAM) str);
		::SendMessage(this->m_pRegionWnd->m_pListRegionWnd->m_CommentsWnd.m_hWndEdit, WM_SETTEXT, 
									0, (LPARAM) str);
		}	


	//	}// end if(::IsWindow(m_pRegionWnd->m_pListRegionWnd->m_hWnd))

	// С какого региона начинать
	///*
	for(pReg=CRegion::GetFirst(); pReg!=NULL; pReg=CRegion::GetNext(pReg))
		{
		if(pReg->m_NDataOutCurr != 0) { m_Doc.m_ThrComm.pRegNow = pReg;	break;}
		}
	//*/
	OnRegions();

	SetNewTIME(&m_Doc.m_ThrComm.TIME);

	SetStatusTime(m_Doc.m_ThrComm.TIME);
	}
else if(m_Doc.m_DocType==m_Doc.DXPS)
	{
	CString str;
	m_pDxpsDlg->FillTable();
	str.Format("%g",CDxpsRegion::ScanTime);
	m_pDxpsDlg->GetDlgItem(IDC_EDIT_DXPS_TIME)->SetWindowText((LPCSTR)str);
		OnProgramDxpsTable();
		if(CDxpsRegion::PassedNumberOfPoints>0)
		{
			m_pHideWnd->ShowWindow(SW_HIDE);
			m_Graph.ShowWindow(SW_SHOW);
			m_Graph.PlotData();
		}
	}

THRI_UNLOCK();

}
//=========

void CMainFrame::OnUpdateFileOpenProject(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if(m_StartStop == Start) pCmdUI->Enable(TRUE);
	else pCmdUI->Enable(FALSE);	
}
//=========
void CMainFrame::OnFileSaveAsEasyPlot() 
{
	// TODO: Add your command handler code here
	m_Doc.m_TypeFile = m_Doc.EasyPlot; 
	m_Doc.m_SaveAsOpen = m_Doc.SaveAs; 
	WindowSaveAsOpen(this);
}

void CMainFrame::OnFileSaveProjectAs() 
{
	// TODO: Add your command handler code here
	m_Doc.m_TypeFile = m_Doc.Project; 
	m_Doc.m_SaveAsOpen = m_Doc.SaveAs; 
	WindowSaveAsOpen(this);
}

void CMainFrame::OnUpdateFileSaveAsEasyPlot(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if(CRegion::m_NReg > 0) pCmdUI->Enable(TRUE);
	else pCmdUI->Enable(FALSE);
}

void CMainFrame::OnUpdateFileSaveProjectAs(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if(m_Doc.CheckDocType()!=-1) pCmdUI->Enable(TRUE);
	else pCmdUI->Enable(FALSE);
}

void CMainFrame::OnProgrammStart() 
{
	m_Doc.CheckDocType();
	if(m_Doc.m_DocType==m_Doc.NoDoc)
	{
		::SendMessage(m_hToolBar, TB_CHECKBUTTON  , 
							(WPARAM) ID_PROGRAMM_START, (LPARAM) MAKELONG(FALSE, 0));
		Msg("There is no data to measure.     ");
		return;
	}
	if(m_StartStop!=Start)
	{
		Msg("Last measure has still not stopped. Wait for a while.   ");
		return;
	}

	m_Doc.m_ThrComm.hWnd = m_pHideWnd->m_pBigClientWnd->m_hWnd; // DELETE
	m_Doc.m_ThrComm.StopContinue = m_Doc.m_ThrComm.Continue;
	m_Doc.m_ThrComm.pMainFrame = this;

	if(!m_Doc.fpPrj)
		{
		m_Doc.m_TypeFile = m_Doc.Project; 
		m_Doc.m_SaveAsOpen = m_Doc.SaveAs; 
		if(!WindowSaveAsOpen(this))
			{	AfxMessageBox("You must save the project file."); 
				::SendMessage(this->m_hToolBar, TB_CHECKBUTTON  , 
							(WPARAM) ID_PROGRAMM_START, (LPARAM) MAKELONG(FALSE, 0));
				return;
			}
		}
	sprintf(m_Doc.m_WindowCaption, "%s - %s", AppTitle, m_Doc.m_ProjectFile.FileName);
	SetWindowText(m_Doc.m_WindowCaption);

	::SendMessage(m_hToolBar, TB_CHECKBUTTON  , (WPARAM) ID_PROGRAMM_START, 
																							(LPARAM) MAKELONG(TRUE, 0));
	::SendMessage(m_hToolBar, TB_ENABLEBUTTON  , (WPARAM) ID_FILE_OPEN_PROJECT, 
																							(LPARAM) MAKELONG(FALSE, 0));
	::SendMessage(m_hToolBar, TB_ENABLEBUTTON  , (WPARAM) ID_FILE_NEW_PROJECT, 
																							(LPARAM) MAKELONG(FALSE, 0));
	
	HMENU	SysMenu = ::GetSystemMenu(this->m_hWnd, FALSE);
	::EnableMenuItem(SysMenu, SC_CLOSE, MF_GRAYED);

	//m_Doc.m_pMeasThread = NULL;
	if(m_Doc.m_DocType==m_Doc.XPS)
		m_Doc.m_pMeasThread = AfxBeginThread( (AFX_THREADPROC) MeasuringThread, (LPVOID) (&m_Doc.m_ThrComm));
	if(m_Doc.m_DocType==m_Doc.DXPS)
		m_Doc.m_pMeasThread = AfxBeginThread( (AFX_THREADPROC) DxpsThread, (LPVOID) (&m_Doc.m_ThrComm));
	m_StartStop = Stop;
	//AfxMessageBox("THE END");
	
}

void CMainFrame::OnUpdateProgrammStart(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if(m_StartStop == Start) pCmdUI->Enable(TRUE);
	else	pCmdUI->Enable(FALSE);
}

void CMainFrame::OnProgrammStop() 
{
	m_Doc.m_ThrComm.StopContinue = m_Doc.m_ThrComm.Stop;
	//m_StartStop = Start;
	::SendMessage(m_hToolBar, TB_CHECKBUTTON  , (WPARAM) ID_PROGRAMM_STOP, 
																							(LPARAM) MAKELONG(TRUE, 0));
	::SendMessage(m_hToolBar, TB_ENABLEBUTTON  , (WPARAM) ID_FILE_OPEN_PROJECT, 
																							(LPARAM) MAKELONG(TRUE, 0));
	::SendMessage(m_hToolBar, TB_ENABLEBUTTON  , (WPARAM) ID_FILE_NEW_PROJECT, 
																							(LPARAM) MAKELONG(TRUE, 0));
	char str[2];
	str[0] = '\0';
	::SendMessage(m_hStatusBar, SB_SETTEXT, 1, (LPARAM) (LPSTR) str);
}

void CMainFrame::OnUpdateProgrammStop(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if(m_StartStop == Stop) pCmdUI->Enable(TRUE);
	else	pCmdUI->Enable(FALSE);
}

void CMainFrame::OnSettingsCamacSetup() 
{
	if(theApp.Ini.HighPressureMode.Value)
	{
		CCamSetupHPDlg dlg(AfxGetApp(), this);
		dlg.DoModal();
	}
	else
	{
		CCamSetupDlg dlg(AfxGetApp(), this);
		dlg.DoModal();
	}

}

void CMainFrame::OnSettingsHardwareSetup() 
{
	CHardwareSetupDlg dlg;
	dlg.m_pThreadCom=&m_Doc.m_ThrComm;
	dlg.DoModal();
}


void CMainFrame::OnSettingsMeasuringOptions() 
{
	CMeasOptDlg dlg;
	dlg.m_pThreadCom=&m_Doc.m_ThrComm;
	dlg.DoModal();
}


//==========


void CMainFrame::OnGraphicsFont() 
{
	// TODO: Add your command handler code here
LOGFONT NewLogFont;
memmove((void*) &NewLogFont, (void*) m_Doc.m_Graph.m_pLogFont, sizeof(LOGFONT));	
if(ChooseNewFont(this->m_hWnd, &NewLogFont, &m_Doc.m_Graph.m_TextColor))	
	{
	HDC DC = ::GetDC(m_Doc.m_Graph.m_hWnd);
	HFONT NewFont = ::CreateFontIndirect(&NewLogFont);
	HFONT OldFont = (HFONT) ::SelectObject(DC, NewFont);
	RECT r; 
	::GetClientRect(m_Doc.m_Graph.m_hWnd, &r);
	int cy = r.bottom - r.top;
	char* str = "A";
	SIZE size;
	::GetTextExtentPoint32(DC, str, strlen(str), &size);
	::SelectObject(DC, OldFont);
	
	if(size.cy < cy)
		{
		WriteLOGFONTToIniFile("TextFont", &NewLogFont);
		CWinApp* App = AfxGetApp();
		App->WriteProfileInt("TextFont", "TextColor", m_Doc.m_Graph.m_TextColor);
		m_Doc.m_ViewWnd.m_ViewGraph.m_TextColor = m_Doc.m_Graph.m_TextColor;
		if(m_Doc.m_Graph.m_Font) ::DeleteObject( (HGDIOBJ) m_Doc.m_Graph.m_Font);
		::DeleteObject( (HGDIOBJ) NewFont);
		memmove( (void*) m_Doc.m_Graph.m_pLogFont, (void*) &NewLogFont, sizeof(LOGFONT));	
		m_Doc.m_Graph.m_Font = ::CreateFontIndirect(m_Doc.m_Graph.m_pLogFont);
		m_Doc.m_ViewWnd.m_ViewGraph.m_Font = m_Doc.m_Graph.m_Font;
		if(::IsWindow(m_Doc.m_ViewWnd.m_hWnd)) m_Doc.m_ViewWnd.m_ViewGraph.ReDrawAll();
		m_Doc.m_Graph.ReDrawAll();
		}
	else AfxMessageBox("ERROR !\n\n Can't create font");
	::ReleaseDC(m_Doc.m_Graph.m_hWnd, DC);
	}

/*
if(ChooseNewFont(this->m_hWnd, m_Doc.m_Graph.m_pLogFont, &m_Doc.m_Graph.m_TextColor))	
	{
	//AfxMessageBox("ChooseNewFont OK");
	WriteLOGFONTToIniFile("TextFont", m_Doc.m_Graph.m_pLogFont);
	CWinApp* App = AfxGetApp();
	App->WriteProfileInt("TextFont", "TextColor", m_Doc.m_Graph.m_TextColor);
	if(m_Doc.m_Graph.m_Font) ::DeleteObject( (HGDIOBJ) m_Doc.m_Graph.m_Font);
	m_Doc.m_Graph.m_Font = ::CreateFontIndirect(m_Doc.m_Graph.m_pLogFont);
	m_Doc.m_Graph.ReDrawAll();
	}
*/	


}
/*
void CMainFrame::OnGraphicsDrawGraph() 
{
	// TODO: Add your command handler code here
	CRegion* pReg;
	pReg=CRegion::GetFirst();
	if(!pReg) {AfxMessageBox("pReg==NULL"); return;}
	m_Doc.m_Graph.m_pDataAll = pReg->m_pDataOut;
	m_Doc.m_Graph.m_NDataAll = pReg->m_NDataOut;
	
	pReg=CRegion::GetNext(pReg);
	if(pReg)
		{
		m_Doc.m_Graph.m_pDataShort = pReg->m_pDataOut;
		m_Doc.m_Graph.m_NDataShort = pReg->m_NDataOut;
		}

	::SendMessage(m_Doc.m_Graph.m_hWnd, WM_PAINT, 0,0);
//	m_Doc.m_Graph.DrawAxis();
//	m_Doc.m_Graph.DrawGraphics();

//	::MoveWindow(m_Doc.m_Graph.m_WndForCurve.m_hWnd,0,0,100,100,NULL);
}
*/
/*
void CMainFrame::OnGraphicsColor() 
{
	// TODO: Add your command handler code here
		COLORREF rgb, CustColor[16];
	CHOOSECOLOR cc;
	CWinApp* App = AfxGetApp();
	rgb = (COLORREF) App->GetProfileInt("COLOR","FirstColor",0xff0000);
	char* str[] = {"1","2","3","4","5","6","7","8","9","10","11","12","13","14","15","16"};
	//rgb=RGB(0,0,255);
	int i;
	for(i=0; i<16; ++i) 
		CustColor[i]=(COLORREF) App->GetProfileInt("COLOR",str[i],0xffffff);
	memset(&cc, 0, sizeof(CHOOSECOLOR));
	cc.lStructSize=sizeof(CHOOSECOLOR);
	cc.hwndOwner=m_hWnd;
//	cc.hInstance=AfxGetInstanceHandle();
	cc.lpCustColors=CustColor;
	cc.Flags=CC_RGBINIT|CC_FULLOPEN;
	cc.rgbResult=rgb;
	if(::ChooseColor(&cc)) rgb=cc.rgbResult;

	App->WriteProfileInt("COLOR","FirstColor",(int) rgb);
	for(i=0; i<16; ++i) 
		App->WriteProfileInt("COLOR",str[i],cc.lpCustColors[i]);

	m_Doc.m_Graph.m_DataAllColor = (COLORREF) App->GetProfileInt("COLOR","1",0xff0000);
	m_Doc.m_Graph.m_DataShortColor = (COLORREF) App->GetProfileInt("COLOR","2",0xff0000);
	m_Doc.m_Graph.m_GridColor = (COLORREF) App->GetProfileInt("COLOR","3",0xff0000);
//REDRAW
	m_Doc.m_Graph.ReDrawAll();
}
*/

void CMainFrame::OnFileSaveProject() 
{
	CSingleLock sLock(&MutexThread);
	if(!m_Doc.fpPrj) 
		return;
	// TODO: Add your command handler code here
	THRI_LOCK();
	//if(m_Doc.fpPrj) SaveBinaryFile(m_Doc.fpPrj);
	//AfxMessageBox("OnFileSaveProject()");
	//if(m_Doc.fpPrj!=m_Doc.m_ThrComm.fp) AfxMessageBox("m_Doc.fpPrj != m_Doc.m_ThrComm.fp");
	//else AfxMessageBox("m_Doc.fpPrj == m_Doc.m_ThrComm.fp");
	//if(m_Doc.fpPrj) SaveBinaryFile(m_Doc.fpPrj);
	
	if(m_Doc.fpPrj) fclose(m_Doc.fpPrj);
	m_Doc.fpPrj = fopen(m_Doc.m_ProjectFile.FullPath, "wb+");
	m_Doc.m_ThrComm.fp = m_Doc.fpPrj;
	SaveBinaryFile(m_Doc.fpPrj);
	sprintf(m_Doc.m_WindowCaption, "%s - %s", AppTitle, m_Doc.m_ProjectFile.FileName);
	SetWindowText(m_Doc.m_WindowCaption);
	m_Doc.m_NeedSave=m_Doc.NoNeed;
	THRI_UNLOCK();
}

void CMainFrame::OnUpdateFileSaveProject(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if(m_Doc.fpPrj) pCmdUI->Enable(TRUE);
	else pCmdUI->Enable(FALSE);
}

void CMainFrame::OnUpdateFileNewProject(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if(m_StartStop == Start) pCmdUI->Enable(TRUE);
	else pCmdUI->Enable(FALSE);	
}

void CMainFrame::OnUpdateFileExit(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if(m_StartStop == Start) pCmdUI->Enable(TRUE);
	else	pCmdUI->Enable(FALSE);	
}

void CMainFrame::OnClose() 
{
CSingleLock sLock(&MutexThread);	
int YesNoCancel;
THRI_LOCK();
//Дополнительное сохранение
if(m_Doc.CheckDocType()!=m_Doc.NoDoc)
	m_Doc.m_NeedSave = m_Doc.Need;
if(m_Doc.m_NeedSave == m_Doc.Need)
	{
	char str[200];
	sprintf(str,"Exit %s", AppTitle);
	YesNoCancel = MessageBox("The progect has changed.\n\nDo you want save the changes?", str,	MB_YESNOCANCEL);
	if(YesNoCancel == IDCANCEL) { THRI_UNLOCK(); return;}
	else if(YesNoCancel == IDYES)
		{
		if(m_Doc.fpPrj) SaveBinaryFile(m_Doc.fpPrj);
		else // if(m_Doc.fpPrj == NULL)
			{	
			m_Doc.m_TypeFile = m_Doc.Project; 
			m_Doc.m_SaveAsOpen = m_Doc.SaveAs; 
			WindowSaveAsOpen(this);			
			}
		//return;
		}
	}// end if(m_Doc.m_NeedSave == m_Doc.Need)
if(m_Doc.fpPrj) fclose(m_Doc.fpPrj);
m_Doc.fpPrj=NULL;
m_Doc.m_ThrComm.pRegNow = NULL;
m_Doc.m_ProjectFile.FullPath[0] = '\0';


m_Doc.m_Graph.m_pDataAll=NULL;
m_Doc.m_Graph.m_pDataShort=NULL;
m_Doc.m_Graph.ReDrawAll();


if(::IsWindow(m_Doc.m_ViewWnd.m_hWnd))
	{
	m_Doc.m_ViewWnd.m_ViewGraph.m_pDataAll = NULL;
	m_Doc.m_ViewWnd.m_ViewGraph.ReDrawAll();
	::SendMessage(m_Doc.m_ViewWnd.m_hWnd, WM_CLOSE, 0,0);
	m_Doc.m_ViewWnd.DestroyWindow();
	}
EmptyAllData();


	RECT r;
	::GetWindowRect(this->m_hWnd, &r);
		CWinApp* App = AfxGetApp();
	
	if(!::IsIconic(this->m_hWnd) && !::IsZoomed(this->m_hWnd))
	{
		const int maxWidth = 3200;
		const int maxHeight = 1600;
		int cx = abs(r.right - r.left);
		int cy = abs(r.bottom - r.top);
		if(cx > maxWidth || cx < 50)
			cx = 800;
		if(cy > maxHeight || cy < 50)
			cy = 600;
		if(r.left + cx < 0 || r.left > maxWidth)
			r.left = 0;
		if(r.top < - 10 || r.top > maxHeight)
			r.top = 0;
		App->WriteProfileInt("MainFrameSize", "X", r.left);
		App->WriteProfileInt("MainFrameSize", "Y", r.top);
		App->WriteProfileInt("MainFrameSize", "CX", cx);
		App->WriteProfileInt("MainFrameSize", "CY", cy);
	}

		if(::IsWindowVisible(m_pRegionWnd->GetSafeHwnd()))
			App->WriteProfileInt("Common", "ShowSupplementaryWindows", 1);
		else if(::IsWindowVisible(m_pDxpsDlg->GetSafeHwnd()))
			App->WriteProfileInt("Common", "ShowSupplementaryWindows", 2);
		else
			App->WriteProfileInt("Common", "ShowSupplementaryWindows", 0);

	if(::IsWindow(this->m_pRegionWnd->m_hWnd))
		::SendMessage(this->m_pRegionWnd->m_hWnd, WM_CLOSE, 0, 0);
	
	CFrameWnd::OnClose();
}

void CMainFrame::OnGraphicsGrid() 
{
	// TODO: Add your command handler code here
	if(m_Doc.m_Graph.m_Grid == TRUE) {m_Doc.m_Graph.m_Grid = FALSE;}// AfxMessageBox("TRUE->FALSE");}
	else {m_Doc.m_Graph.m_Grid = TRUE;}//  AfxMessageBox("FALSE->TRUE");}
	m_Doc.m_Graph.ReDrawAll();
}

void CMainFrame::OnUpdateGraphicsGrid(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if(m_Doc.m_Graph.m_Grid == TRUE) pCmdUI->SetCheck();
	else pCmdUI->SetCheck(0);
}

void CMainFrame::OnGraphicsSettingsForCurvers() 
{
	// TODO: Add your command handler code here
	CDlgSettingsForCurvers Dlg;
	Dlg.m_pMainFrame = this;
	if(::IsWindow(m_pRegionWnd->m_hWnd))
		::EnableWindow(m_pRegionWnd->m_hWnd, FALSE);
	if(::IsWindow(m_Doc.m_ViewWnd.m_hWnd))
		::EnableWindow(m_Doc.m_ViewWnd.m_hWnd, FALSE);	
	int Res = Dlg.DoModal();
	if(Res == IDOK)
		{
		m_Doc.m_Graph.m_DataShortColor = Dlg.m_ColorCurr;
		m_Doc.m_Graph.m_LineShort = Dlg.m_CheckCurrLine;
		m_Doc.m_Graph.m_PointsShort = Dlg.m_CheckCurrPoints;
		m_Doc.m_Graph.m_SizePointsShort = Dlg.m_SizePointsCurr;

		m_Doc.m_Graph.m_DataAllColor = Dlg.m_ColorRes;
		m_Doc.m_Graph.m_LineAll = Dlg.m_CheckResLine;
		m_Doc.m_Graph.m_PointsAll = Dlg.m_CheckResPoints;
		m_Doc.m_Graph.m_SizePointsAll = Dlg.m_SizePointsRes;
		
		m_Doc.m_Graph.m_GridColor = Dlg.m_ColorGrid;
		m_Doc.m_Graph.m_Grid = Dlg.m_CheckGrid;
		m_Doc.m_Graph.m_GuideLines = Dlg.m_CheckGuideLines;
		if(::IsWindow(m_Doc.m_ViewWnd.m_hWnd))
			m_Doc.m_ViewWnd.m_ViewGraph.m_GuideLines = Dlg.m_CheckGuideLines;



		/*
		m_Doc.m_ViewWnd.m_ViewGraph.m_DataAllColor = Dlg.m_ColorRes;
		m_Doc.m_ViewWnd.m_ViewGraph.m_LineAll = Dlg.m_CheckResLine;
		m_Doc.m_ViewWnd.m_ViewGraph.m_PointsAll = Dlg.m_CheckResPoints;
		m_Doc.m_ViewWnd.m_ViewGraph.m_SizePointsAll = Dlg.m_SizePointsRes;
		
		m_Doc.m_ViewWnd.m_ViewGraph.m_GridColor = Dlg.m_ColorGrid;
		m_Doc.m_ViewWnd.m_ViewGraph.m_Grid = Dlg.m_CheckGrid;

		if(::IsWindow(m_Doc.m_ViewWnd.m_hWnd)) m_Doc.m_ViewWnd.m_ViewGraph.ReDrawAll();
		*/
		
		CWinApp* App = AfxGetApp();
		App->WriteProfileInt("COLOR","1",(UINT) m_Doc.m_Graph.m_DataAllColor);
		App->WriteProfileInt("COLOR","2",(UINT) m_Doc.m_Graph.m_DataShortColor);
		App->WriteProfileInt("COLOR","3",(UINT) m_Doc.m_Graph.m_GridColor);
		
		App->WriteProfileInt("SettingsGraph","LineAll", (UINT) m_Doc.m_Graph.m_LineAll);
		App->WriteProfileInt("SettingsGraph","PointsAll", (UINT) m_Doc.m_Graph.m_PointsAll);
		App->WriteProfileInt("SettingsGraph","SizePointsAll", (UINT) m_Doc.m_Graph.m_SizePointsAll);

		App->WriteProfileInt("SettingsGraph","LineShort", (UINT) m_Doc.m_Graph.m_LineShort);
		App->WriteProfileInt("SettingsGraph","PointsShort", (UINT) m_Doc.m_Graph.m_PointsShort);
		App->WriteProfileInt("SettingsGraph","SizePointsShort", (UINT) m_Doc.m_Graph.m_SizePointsShort);
		
		App->WriteProfileInt("SettingsGraph","Grid", (UINT) m_Doc.m_Graph.m_Grid);
		App->WriteProfileInt("SettingsGraph","GuideLines", (UINT) m_Doc.m_Graph.m_GuideLines);
		
		m_Doc.m_Graph.ReDrawAll();

		}// end IDOK
	
	else 
		{
		//AfxMessageBox("IDCANCEL");
		}

	::EnableWindow(m_Doc.m_ViewWnd.m_hWnd, TRUE);
	::EnableWindow(m_pRegionWnd->m_hWnd, TRUE);
}

void CMainFrame::OnGraphicsSettingsForViewer() 
{
	// TODO: Add your command handler code here
	CDlgSetViewer Dlg;
	Dlg.m_pMainFrame = this;
	if(::IsWindow(m_pRegionWnd->m_hWnd))
		::EnableWindow(m_pRegionWnd->m_hWnd, FALSE);
	if(::IsWindow(m_Doc.m_ViewWnd.m_hWnd))
		::EnableWindow(m_Doc.m_ViewWnd.m_hWnd, FALSE);	
	int Res = Dlg.DoModal();
	if(Res == IDOK)
		{
		m_Doc.m_ViewWnd.m_ViewGraph.m_LineAll = Dlg.m_CheckLine;
		m_Doc.m_ViewWnd.m_ViewGraph.m_PointsAll = Dlg.m_CheckPoints;
		m_Doc.m_ViewWnd.m_ViewGraph.m_Grid = Dlg.m_CheckGrid;
		m_Doc.m_ViewWnd.m_ViewGraph.m_SizePointsAll = Dlg.m_SizePoints;
		
		m_Doc.m_ViewWnd.m_ViewGraph.m_DataAllColor = Dlg.m_ColorCurve;
		m_Doc.m_ViewWnd.m_ViewGraph.m_GridColor = Dlg.m_ColorGrid;
		m_Doc.m_ViewWnd.m_ViewGraph.m_ColorFont = Dlg.m_ColorFont;
		m_Doc.m_ViewWnd.m_ViewGraph.m_TextColor = Dlg.m_ColorFont;
		
		if(Dlg.m_Font) ::DeleteObject((HGDIOBJ) Dlg.m_Font);
		//m_Doc.m_ViewWnd.m_ViewGraph.m_Font = Dlg.m_Font;
		memmove((void*) m_Doc.m_ViewWnd.m_ViewGraph.m_pLogFont,
						(void*) &Dlg.m_LogFont, sizeof(LOGFONT));
		if(m_Doc.m_ViewWnd.m_ViewGraph.m_Font) 
			::DeleteObject((HGDIOBJ) m_Doc.m_ViewWnd.m_ViewGraph.m_Font);
		m_Doc.m_ViewWnd.m_ViewGraph.m_Font = (HFONT) ::CreateFontIndirect(&Dlg.m_LogFont);

		//if(!m_Doc.m_ViewWnd.m_ViewGraph.m_Font) AfxMessageBox("m_Doc.m_ViewWnd.m_ViewGraph.m_Font==NULL");
		//else AfxMessageBox("m_Doc.m_ViewWnd.m_ViewGraph.m_Font!=NULL");
		
		m_Doc.m_ViewWnd.m_ViewGraph.ReDrawAll();
		
		
		
		/*
		m_Doc.m_Graph.m_DataShortColor = Dlg.m_ColorCurr;
		m_Doc.m_Graph.m_LineShort = Dlg.m_CheckCurrLine;
		m_Doc.m_Graph.m_PointsShort = Dlg.m_CheckCurrPoints;
		m_Doc.m_Graph.m_SizePointsShort = Dlg.m_SizePointsCurr;

		m_Doc.m_Graph.m_DataAllColor = Dlg.m_ColorRes;
		m_Doc.m_Graph.m_LineAll = Dlg.m_CheckResLine;
		m_Doc.m_Graph.m_PointsAll = Dlg.m_CheckResPoints;
		m_Doc.m_Graph.m_SizePointsAll = Dlg.m_SizePointsRes;
		
		m_Doc.m_Graph.m_GridColor = Dlg.m_ColorGrid;
		m_Doc.m_Graph.m_Grid = Dlg.m_CheckGrid;

		
		m_Doc.m_ViewWnd.m_ViewGraph.m_DataAllColor = Dlg.m_ColorRes;
		m_Doc.m_ViewWnd.m_ViewGraph.m_LineAll = Dlg.m_CheckResLine;
		m_Doc.m_ViewWnd.m_ViewGraph.m_PointsAll = Dlg.m_CheckResPoints;
		m_Doc.m_ViewWnd.m_ViewGraph.m_SizePointsAll = Dlg.m_SizePointsRes;
		
		m_Doc.m_ViewWnd.m_ViewGraph.m_GridColor = Dlg.m_ColorGrid;
		m_Doc.m_ViewWnd.m_ViewGraph.m_Grid = Dlg.m_CheckGrid;

		if(::IsWindow(m_Doc.m_ViewWnd.m_hWnd)) m_Doc.m_ViewWnd.m_ViewGraph.ReDrawAll();
		
		
		CWinApp* App = AfxGetApp();
		App->WriteProfileInt("COLOR","1",(UINT) m_Doc.m_Graph.m_DataAllColor);
		App->WriteProfileInt("COLOR","2",(UINT) m_Doc.m_Graph.m_DataShortColor);
		App->WriteProfileInt("COLOR","3",(UINT) m_Doc.m_Graph.m_GridColor);
		
		App->WriteProfileInt("SettingsGraph","LineAll", (UINT) m_Doc.m_Graph.m_LineAll);
		App->WriteProfileInt("SettingsGraph","PointsAll", (UINT) m_Doc.m_Graph.m_PointsAll);
		App->WriteProfileInt("SettingsGraph","SizePointsAll", (UINT) m_Doc.m_Graph.m_SizePointsAll);

		App->WriteProfileInt("SettingsGraph","LineShort", (UINT) m_Doc.m_Graph.m_LineShort);
		App->WriteProfileInt("SettingsGraph","PointsShort", (UINT) m_Doc.m_Graph.m_PointsShort);
		App->WriteProfileInt("SettingsGraph","SizePointsShort", (UINT) m_Doc.m_Graph.m_SizePointsShort);
		
		App->WriteProfileInt("SettingsGraph","Grid", (UINT) m_Doc.m_Graph.m_Grid);
		
		m_Doc.m_Graph.ReDrawAll();
		*/
		}// end IDOK
	
	else 
		{
		//AfxMessageBox("IDCANCEL");
		}

	::EnableWindow(m_Doc.m_ViewWnd.m_hWnd, TRUE);
	::EnableWindow(m_pRegionWnd->m_hWnd, TRUE);
	
}

void CMainFrame::OnUpdateGraphicsSettingsForViewer(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if(::IsWindow(m_Doc.m_ViewWnd.m_hWnd)) pCmdUI->Enable(TRUE);
	else pCmdUI->Enable(FALSE);	
}

void CMainFrame::OnFileSaveAsEasyplotMc() 
{
	// TODO: Add your command handler code here
	m_Doc.m_TypeFile = m_Doc.EasyPlotMC; 
	m_Doc.m_SaveAsOpen = m_Doc.SaveAs; 
	WindowSaveAsOpen(this);		
}

void CMainFrame::OnUpdateFileSaveAsEasyplotMc(CCmdUI* pCmdUI) 
{
	if(m_Doc.m_DocType==m_Doc.XPS||m_Doc.m_DocType==m_Doc.DXPS) pCmdUI->Enable(TRUE);
	else pCmdUI->Enable(FALSE);	
}

void CMainFrame::OnSettingsFitable() 
{
	// TODO: Add your command handler code here
	CFiTableDlg dlg((CWnd*) this);
		dlg.m_pFiTable=&m_Doc.m_ThrComm.FiTable;
	dlg.DoModal();	
}

void CMainFrame::OnFileSaveAsOrigin() 
{
	// TODO: Add your command handler code here
	m_Doc.m_TypeFile = m_Doc.Origin; 
	m_Doc.m_SaveAsOpen = m_Doc.SaveAs; 
	WindowSaveAsOpen(this);	
}

void CMainFrame::OnUpdateFileSaveAsOrigin(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if(CRegion::m_NReg > 0) pCmdUI->Enable(TRUE);
	else pCmdUI->Enable(FALSE);	
}

void CMainFrame::OnProgramDxpsTable() 
{
//CDxpsDlg dlg;
//dlg.DoModal();
	// TODO: Add your command handler code here
	if(::IsWindow(m_pDxpsDlg->m_hWnd))
		{	m_pDxpsDlg->ShowWindow(SW_SHOWNORMAL);
			::SetFocus(m_pDxpsDlg->m_hWnd);
		}

}

void CMainFrame::OnUpdateProgramDxpsTable(CCmdUI* pCmdUI) 
{
	if(m_Doc.m_DocType==m_Doc.DXPS||m_Doc.m_DocType==m_Doc.NoDoc) pCmdUI->Enable(TRUE);
	else pCmdUI->Enable(FALSE);	
}

void CMainFrame::OnUpdateProgramRegionTable(CCmdUI* pCmdUI) 
{
	if(m_Doc.m_DocType==m_Doc.XPS||m_Doc.m_DocType==m_Doc.NoDoc) pCmdUI->Enable(TRUE);
	else pCmdUI->Enable(FALSE);	
}

LRESULT CMainFrame::OnPostCreateWindow(WPARAM WParam, LPARAM LParam)
{
	int ShowSupplementaryWindows=theApp.GetProfileInt("Common", "ShowSupplementaryWindows", 0);
	if(ShowSupplementaryWindows==1)
		OnRegions();
	if(ShowSupplementaryWindows==2)
		OnProgramDxpsTable();
	
	SetTimer(POST_POST_CREATEWINDOW, 0, NULL);
	return 0;
}

void CMainFrame::OnAbout() 
{
CDlgAbout dlg;
dlg.DoModal();	
}

BOOL CMainFrame::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	
	return 0;//CFrameWnd::OnEraseBkgnd(pDC);
}

void CMainFrame::OnTimer(UINT nIDEvent) 
{
	if(nIDEvent==POST_POST_CREATEWINDOW)
	{
	RECT rect;
	GetWindowRect( &rect );
	rect.right-=1;
	MoveWindow( &rect);
	GetWindowRect( &rect );
	rect.right+=1;
	MoveWindow( &rect);
	KillTimer(nIDEvent);
	}
	else if(nIDEvent==TIMER_CHECK_DXPS_STATE)
	{
		HWND hWnd=::FindWindow(_T("SpecWindowClass"),NULL);
		if (hWnd)
		{
			m_Doc.CheckDocType();
			::SendMessage(hWnd,m_RegDxpsMessageID,0,
				(m_Doc.m_DocType==CDoc::DXPS && CDxpsRegion::GetRegNumber()>0 
				&& CDxpsRegion::PassedNumberOfPoints==0 && strlen(m_Doc.m_ProjectFile.FullPath)>0)?1:0);
		}
	}
	else if(nIDEvent==TIMER_CHECK_VALID_TEMPERATURE)
	{
		COleDateTimeSpan timeFromTempReception = COleDateTime::GetCurrentTime() - COleDateTime(m_Doc.m_ThrComm.LastTemperatureTime);
		if( fabs(timeFromTempReception.GetTotalSeconds()) > 1.5 ) // > 1.5 sec
			SetStatusTemperature(0, false);
	}
	CFrameWnd::OnTimer(nIDEvent);
}

void CMainFrame::SetRecentProjects() 
{
   // Get the menu from the application window.
   CMenu* mmenu = GetMenu();

   // Look for "File" menu.
   int pos = FindMenuItem(mmenu, "&File");
   if (pos == -1)
      return;

   // Remove "New" menu item from the File menu.
   CMenu* FileSubMenu = mmenu->GetSubMenu(pos);
   pos = FindMenuItem(FileSubMenu, "&Recent Projects");
   if (pos == -1)
	   return;
   CMenu* FileRecentProjectsSubMenu = FileSubMenu->GetSubMenu(pos);
   FileRecentProjectsSubMenu->RemoveMenu(0, MF_BYPOSITION);
   FileRecentProjectsSubMenu->RemoveMenu(0, MF_BYPOSITION);
   FileRecentProjectsSubMenu->RemoveMenu(0, MF_BYPOSITION);
   FileRecentProjectsSubMenu->RemoveMenu(0, MF_BYPOSITION);
	int i;
	for(i=0; i<4; i++)
	{
		if(theApp.Ini.ProjectFile[i].Value.GetLength()<2)
			break;
		FileRecentProjectsSubMenu->InsertMenu(i, MF_BYPOSITION, ID_FILE_RECENTPROJECTS_FILE1+i, (LPCSTR)theApp.Ini.ProjectFile[i].Value);
	}

	if(i==0)
		FileRecentProjectsSubMenu->InsertMenu(0, MF_BYPOSITION, ID_FILE_RECENTPROJECTS_NOFILES, "No Files");



}

// FindMenuItem() will find a menu item string from the specified
// popup menu and returns its position (0-based) in the specified 
// popup menu. It returns -1 if no such menu item string is found.
int CMainFrame::FindMenuItem(CMenu* Menu, LPCTSTR MenuString)
{
   ASSERT(Menu);
   ASSERT(::IsMenu(Menu->GetSafeHmenu()));

   int count = Menu->GetMenuItemCount();
   for (int i = 0; i < count; i++)
   {
      CString str;
      if (Menu->GetMenuString(i, str, MF_BYPOSITION) &&
         (strcmp(str, MenuString) == 0))
         return i;
   }

   return -1;
}

void CMainFrame::InsertProjectToRecent(LPCTSTR ProjectFullPath)
{
	int i;
	for(i=0; i<4; i++)
	{
		if(theApp.Ini.ProjectFile[i].Value==ProjectFullPath || i==3)
			break;
	}
	//Ячейки "до" смещаются вверх (в индекс +1)
	for(int j=i; j>0; j--)
		theApp.Ini.ProjectFile[j].Value=theApp.Ini.ProjectFile[j-1].Value;
	theApp.Ini.ProjectFile[0].Value=ProjectFullPath;
	SetRecentProjects();
}

void CMainFrame::OnFileRecentProjectsFile1()
{
OnFileRecentProjects(0);
}

void CMainFrame::OnFileRecentProjectsFile2()
{
OnFileRecentProjects(1);
}

void CMainFrame::OnFileRecentProjectsFile3()
{
OnFileRecentProjects(2);
}

void CMainFrame::OnFileRecentProjectsFile4()
{
OnFileRecentProjects(3);
}

void CMainFrame::OnFileRecentProjects(int Index)
{

int YesNoCancel; 
CSingleLock sLock(&MutexThread);
THRI_LOCK();
//Дополнительное сохранение
if(m_Doc.CheckDocType()!=m_Doc.NoDoc)
	m_Doc.m_NeedSave = m_Doc.Need;

if(m_Doc.m_NeedSave == m_Doc.Need)
	{
	YesNoCancel = AfxMessageBox("The project has changed.\n\nDo you want save the changes?", 
																MB_YESNOCANCEL);
	if(YesNoCancel == IDCANCEL) { THRI_UNLOCK(); return;}
	else if(YesNoCancel == IDYES)
		{
		if(m_Doc.fpPrj) SaveBinaryFile(m_Doc.fpPrj);
		else // if(m_Doc.fpPrj == NULL)
			{	
			m_Doc.m_TypeFile = m_Doc.Project; 
			m_Doc.m_SaveAsOpen = m_Doc.SaveAs; 
			WindowSaveAsOpen(this);			
			}
		//return;
		}
	}// end if(m_Doc.m_NeedSave == m_Doc.Need)
if(m_Doc.fpPrj) fclose(m_Doc.fpPrj);
m_Doc.m_ThrComm.pRegNow = NULL;

m_Doc.m_Graph.m_pDataAll=NULL;
m_Doc.m_Graph.m_pDataShort=NULL;
m_Doc.m_Graph.ReDrawAll();
m_pHideWnd->ShowWindow(SW_SHOW);
m_Graph.ShowWindow(SW_HIDE);


if(::IsWindow(m_Doc.m_ViewWnd.m_hWnd))
	{
	m_Doc.m_ViewWnd.m_ViewGraph.m_pDataAll = NULL;
//	m_Doc.m_ViewWnd.m_ViewGraph.ReDrawAll();
	::SendMessage(m_Doc.m_ViewWnd.m_hWnd, WM_CLOSE, 0,0);
	m_Doc.m_ViewWnd.DestroyWindow();
	}

EmptyAllData();
m_Doc.m_NeedSave = m_Doc.NoNeed;
m_Doc.CheckDocType();

sprintf(m_Doc.m_WindowCaption, "%s - %s", AppTitle, "[no data]");

SetWindowText(m_Doc.m_WindowCaption);

//Clear status
char *strn="";
::SendMessage(theApp.m_pMainFrame->m_hStatusBar, SB_SETTEXT, 0, (LPARAM) (LPSTR) strn);
::SendMessage(theApp.m_pMainFrame->m_hStatusBar, SB_SETTEXT, 1, (LPARAM) (LPSTR) strn);

FILE *fp;
RetryRead:
	fp=fopen((LPCSTR)theApp.Ini.ProjectFile[Index].Value, "rb+");
	if(!fp) {THRI_UNLOCK(); MessageBox("Cannot open project.   ", "File open failed", MB_OK|MB_ICONSTOP); return;}
	if(!ReadBinaryFile(fp) )
	{
		fclose(fp);
		CString str="Project file is corrupted. Continuing may crash the application.\n"+ FileSaveOpenErrorDescription+ "\n\nDo you want to abort opening this document?";
		int result=MessageBox(str,"Open project error",MB_ABORTRETRYIGNORE|MB_ICONSTOP); 
		if(result==IDRETRY)
		{
			EmptyAllData();
			goto RetryRead;
		}
		if(result==IDABORT)
		{
			m_Doc.fpPrj = m_Doc.m_ThrComm.fp = NULL;
			EmptyAllData();
			THRI_UNLOCK();
			return;
		}
		if(result==IDIGNORE)
			fp=fopen((LPCSTR)theApp.Ini.ProjectFile[Index].Value, "rb+");
	}
	m_Doc.fpPrj = fp;
	m_Doc.m_ThrComm.fp = fp;
	strcpy(m_Doc.m_ProjectFile.FullPath,(LPCSTR)theApp.Ini.ProjectFile[Index].Value);
	strcpy(m_Doc.m_ProjectFile.FileName,m_Doc.m_ProjectFile.FullPath+theApp.Ini.ProjectFile[Index].Value.ReverseFind('\\')+1);

InsertProjectToRecent(m_Doc.m_ProjectFile.FullPath);
sprintf(m_Doc.m_WindowCaption, "%s - %s", AppTitle, m_Doc.m_ProjectFile.FileName);
SetWindowText(m_Doc.m_WindowCaption);
m_Doc.CheckDocType();
if(m_Doc.m_DocType==m_Doc.XPS)
	{
	//if( !(::IsWindow(m_pRegionWnd->m_pListRegionWnd->m_hWnd)) ) 
	CRegion *pReg;
	if(::IsWindow(m_pRegionWnd->m_pListRegionWnd->m_hWnd))
	//	{
	//else
		{	
		for(pReg=CRegion::GetFirst(); pReg!=NULL; pReg=CRegion::GetNext(pReg))
			{ 
			SetNewRegionItemForListView(m_pRegionWnd->m_pListRegionWnd, pReg);
			if(pReg->m_DataIn.Off == TRUE) SetIconForReg(m_pRegionWnd->m_pListRegionWnd, pReg, 2);
			}
		}

	if( (::IsWindow(this->m_pRegionWnd->m_pListRegionWnd->m_CommentsWnd.m_hWnd) ))
		{
		char str[32];
		sprintf(str, "No Comments");
		::SendMessage(this->m_pRegionWnd->m_pListRegionWnd->m_CommentsWnd.m_hWnd, WM_SETTEXT, 
									0, (LPARAM) str);
		::SendMessage(this->m_pRegionWnd->m_pListRegionWnd->m_CommentsWnd.m_hWndEdit, WM_SETTEXT, 
									0, (LPARAM) str);
		}	


	//	}// end if(::IsWindow(m_pRegionWnd->m_pListRegionWnd->m_hWnd))

	// С какого региона начинать
	///*
	for(pReg=CRegion::GetFirst(); pReg!=NULL; pReg=CRegion::GetNext(pReg))
		{
		if(pReg->m_NDataOutCurr != 0) { m_Doc.m_ThrComm.pRegNow = pReg;	break;}
		}
	//*/
	OnRegions();

	SetNewTIME(&m_Doc.m_ThrComm.TIME);

	SetStatusTime(m_Doc.m_ThrComm.TIME);
	}
else if(m_Doc.m_DocType==m_Doc.DXPS)
	{
	CString str;
	m_pDxpsDlg->FillTable();
	str.Format("%g",CDxpsRegion::ScanTime);
	m_pDxpsDlg->GetDlgItem(IDC_EDIT_DXPS_TIME)->SetWindowText((LPCSTR)str);
		OnProgramDxpsTable();
		if(CDxpsRegion::PassedNumberOfPoints>0)
		{
			m_pHideWnd->ShowWindow(SW_HIDE);
			m_Graph.ShowWindow(SW_SHOW);
			m_Graph.PlotData();
		}
	}

THRI_UNLOCK();

}

// Удаленный запуск из приложения Spec
LRESULT CMainFrame::OnRemoteDxpsStart(WPARAM WParam, LPARAM LParam)
{
	if(m_Doc.m_DocType==m_Doc.DXPS)
		OnProgrammStart();
	return (LRESULT) TRUE;
}

// Прием текущей температуры из приложения Spec
LRESULT CMainFrame::OnRemoteCurrentTempMessage(WPARAM WParam, LPARAM LParam)
{
	m_Doc.m_ThrComm.LastCurTemperature = ((int)LParam)/256.;
	m_Doc.m_ThrComm.LastRefTemperature = ((int)WParam)/256.;
	m_Doc.m_ThrComm.LastTemperatureTime = (double)COleDateTime::GetCurrentTime();
	SetStatusTemperature(m_Doc.m_ThrComm.LastCurTemperature);
	return (LRESULT) TRUE;
}

void CMainFrame::OnSettingsCountersetup()
{
	CSerialCounterDlg dlg;
	dlg.DoModal();
}

void CMainFrame::OnUpdateSettingsCounterSetup(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(theApp.Ini.UseUsbCounter.Value);
}
