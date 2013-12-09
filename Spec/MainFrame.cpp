// MainFrame.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "Spec.h"
#include "SpecView.h"
#include "HSChart\\ChartCtrl.h"
#include "MainFrame.h"
#include "HardSetupDlg.h"
#include "resource.h"
#include "Threads.h"
#include "ProtectionSetupDlg.h"
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
UINT wm_RegDxpsMessageID=RegisterWindowMessage("RemoteDxpsState");
// Формат сообщения:
// hWnd = FindWindow(_T("SpecWindowClass"),NULL);
// wParam = 0;
// lParam = DxpsDocReadyState;

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_COMMAND(ID_SETTINGS_HARDWARE, OnSettingsHardware)
	ON_WM_CLOSE()
	ON_COMMAND(ID_TR_TRSETTINGS, OnTrTrsettings)
	//}}AFX_MSG_MAP
	ON_WM_TIMER()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()

	ON_UPDATE_COMMAND_UI(ID_INDICATOR_REMAIN_TIME, OnUpdateStatusRemainTime)
	ON_REGISTERED_MESSAGE(wm_RegDxpsMessageID, OnRemoteDxpsState)
	//ON_UPDATE_COMMAND_UI(ID_INDICATOR_REF_TEMP, OnUpdateStatusRefTemp)
	//ON_UPDATE_COMMAND_UI(ID_INDICATOR_CUR_TEMP, OnUpdateStatusCurTemp)
	ON_COMMAND(ID_POPUP_RESETPROTECTION, &CMainFrame::OnPopupResetProtection)
	ON_COMMAND(ID_POPUP_PROTECTIONPARAMETERS, &CMainFrame::OnPopupProtectionParameters)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
//	ID_INDICATOR_CAPS,
//	ID_INDICATOR_NUM,
	ID_INDICATOR_PROTECTION,
	ID_INDICATOR_POWER,
	ID_INDICATOR_REF_TEMP,
	ID_INDICATOR_CUR_TEMP,
	ID_INDICATOR_REMAIN_TIME
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame():m_dRemainTime(0)
{
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC|CBRS_FLOATING) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
	CPoint point(50,50);
	if (!m_wndToolBarTR.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC|CBRS_FLOATING) ||
		!m_wndToolBarTR.LoadToolBar(IDR_TOOLBAR_TR))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
	//FloatControlBar(&m_wndToolBar, point, CBRS_ALIGN_RIGHT  );
	if (!m_wndStatusBar.CreateStatusBar(this, indicators, sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}
	m_wndStatusBar.SetStyle(0, SBPS_NOBORDERS | SBPS_STRETCH);

	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndToolBarTR.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	RECT rect1={theApp.Ini.MainToolBarRect.left, theApp.Ini.MainToolBarRect.top, theApp.Ini.MainToolBarRect.right, theApp.Ini.MainToolBarRect.bottom};
	ClientToScreen(&rect1);
	DockControlBar(&m_wndToolBar,AFX_IDW_DOCKBAR_TOP|AFX_IDW_DOCKBAR_LEFT,&rect1);
	
	RECT rect2={theApp.Ini.TRToolBarRect.left, theApp.Ini.TRToolBarRect.top, theApp.Ini.TRToolBarRect.right, theApp.Ini.TRToolBarRect.bottom};
	ClientToScreen(&rect2);
	DockControlBar(&m_wndToolBarTR,AFX_IDW_DOCKBAR_TOP|AFX_IDW_DOCKBAR_LEFT,&rect2);
	//FloatControlBar(&m_wndToolBarTR,point);
	SetStatusTCur(0, false);
	SetStatusProtection(ProtectionStatus::Unknown);

	m_PopupMenuProtection.LoadMenu(IDR_MENU_PROTECTION);
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style=cs.style&~FWS_ADDTOTITLE;
	cs.lpszClass = WINDOW_CLASS_NAME;
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers


BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{
return	CFrameWnd::OnCreateClient(lpcs, pContext); 
}

void CMainFrame::OnSettingsHardware() 
{
	CHardSetupDlg dlg;
	dlg.DoModal();
}

void CMainFrame::OnClose() 
{
	m_wndToolBar.GetWindowRect(&theApp.Ini.MainToolBarRect);
	ScreenToClient(&theApp.Ini.MainToolBarRect);
	m_wndToolBarTR.GetWindowRect(&theApp.Ini.TRToolBarRect);
	ScreenToClient(&theApp.Ini.TRToolBarRect);
	DWORD ExitCode;
	SetEvent(theApp.m_EventThreadExit);
	if(theApp.m_pThread)
	{
		GetExitCodeThread(theApp.m_pThread->m_hThread,&ExitCode);
		if(ExitCode==STILL_ACTIVE)
			TerminateThread(theApp.m_pThread->m_hThread,1);
	}
	if(WaitForSingleObject(theApp.m_hThermoRegThread,2000)==WAIT_TIMEOUT)
			MessageBox("Поток терморегулятора не самозавершился. При принудительном\n"
			"завершении возможно, что нагреватель останется включеным.","Spec - Ошибка", 
				MB_ICONERROR|MB_OK);

	CFrameWnd::OnClose();
}

afx_msg void CMainFrame::OnLButtonDown(UINT nFlags, CPoint point)
{
	if(m_wndStatusBar.GetPaneAtPosition(point) == Indicators::Protection)
	{
		OnPopupResetProtection();
	}
}

afx_msg void CMainFrame::OnRButtonDown(UINT nFlags, CPoint point)
{
	if(m_wndStatusBar.GetPaneAtPosition(point) == Indicators::Protection)
	{
		RECT rect;
		m_wndStatusBar.GetItemRect(Indicators::Protection, &rect);
		CPoint screenPoint(rect.left, rect.top);
		m_wndStatusBar.ClientToScreen(&screenPoint);
		m_PopupMenuProtection.GetSubMenu(0)->TrackPopupMenu(0, screenPoint.x, screenPoint.y, this, 0);
	}
}

void CMainFrame::OnUpdateStatusRefTemp(CCmdUI *pCmdUI)
{

}

void CMainFrame::OnUpdateStatusCurTemp(CCmdUI *pCmdUI)
{

}

void CMainFrame::OnUpdateStatusRemainTime(CCmdUI *pCmdUI)
{
    pCmdUI->Enable(); 
    CString strTime;
	int Hours=abs(m_dRemainTime/3600);
	int Minutes=abs((int)(m_dRemainTime/60))%60;
	int Seconds=abs((int)m_dRemainTime)%60;
	double Milliseconds=fabs(m_dRemainTime)-floor(fabs(m_dRemainTime));
	char Sign[2];
	strcpy(Sign,(m_dRemainTime>=0)?" ":"-");
    strTime.Format( "%s%.2i:%.2i:%.2i", Sign,Hours,Minutes,Seconds);  
    pCmdUI->SetText(strTime);
}

void CMainFrame::SetStatusRemainTime(double Time)
{
	m_dRemainTime=Time;
    CString strTime;
	int Hours=abs(m_dRemainTime/3600);
	int Minutes=abs((int)(m_dRemainTime/60))%60;
	int Seconds=abs((int)m_dRemainTime)%60;
	double Milliseconds=fabs(m_dRemainTime)-floor(fabs(m_dRemainTime));
	char Sign[2];
	strcpy(Sign,(m_dRemainTime>=0)?" ":"-");
    strTime.Format( "%s%.2i:%.2i:%.2i", Sign,Hours,Minutes,Seconds);
	//::SendMessage(m_wndStatusBar.GetSafeHwnd(), SB_SETTEXT,	Indicators::RemainTime, (LPARAM) (LPCSTR) strTime);
	m_wndStatusBar.SetText(Indicators::RemainTime, (LPCSTR) strTime, (LPCSTR) strTime);

}

void CMainFrame::OnTrTrsettings() 
{
	CThermoSettingsDlg dlg;
	dlg.DoModal();
}

void CMainFrame::SetStatusTRef(double TRef, bool On)
{
	m_dTRef=TRef;
    CString str;
	if(On)
		str.Format("T Ref: %.1f",m_dTRef);
	else
		str="T Ref: Off";

	//::SendMessage(m_wndStatusBar.GetSafeHwnd(), SB_SETTEXT,	Indicators::RefTemp, (LPARAM) (LPCSTR) str);
	m_wndStatusBar.SetText(Indicators::RefTemp, (LPCSTR) str, (LPCSTR) str);
}

void CMainFrame::SetStatusTCur(double TCur, bool Known)
{
	m_dTCur=TCur;
    CString str;
	if(Known)
		str.Format("T Cur: %.1f",m_dTCur);
	else
		str="T Cur: ???";
	//::SendMessage(m_wndStatusBar.GetSafeHwnd(), SB_SETTEXT,	Indicators::CurTemp, (LPARAM) (LPCSTR) str);
	m_wndStatusBar.SetText(Indicators::CurTemp, (LPCSTR) str, (LPCSTR) str);
}

void CMainFrame::SetStatusPower(double Power, bool On)
{
    CString str;
	if(On)
	{
		if(fabs(Power)<10)
			str.Format("Power: %.1f W",Power);
		else
			str.Format("Power: %.0f W",Power);
	}
	else
		str="Power: Off";

	//::SendMessage(m_wndStatusBar.GetSafeHwnd(), SB_SETTEXT,	Indicators::Power, (LPARAM) (LPCSTR) str);
	m_wndStatusBar.SetText(Indicators::Power, (LPCSTR) str, (LPCSTR) str);
}

void CMainFrame::SetStatusProtection(int state)
{
    CString str;
	if(state & ProtectionStatus::OK)
	{
		str = "Protection: OK";
	}
	else if(state & ProtectionStatus::Unknown)
	{
		str = "Protection: ?";
	}
	else if( (state & ProtectionStatus::LimitationI) 
		&& !(state & ProtectionStatus::OverloadI) 
		&& !(state & ProtectionStatus::OverloadU)
		&& !(state & ProtectionStatus::OverloadP))
	{
		str = "Limitation: I";
	}
	else
	{
		str = " Overload: ";
		if(state & ProtectionStatus::OverloadI)
			str += "I,";
		if(state & ProtectionStatus::OverloadU)
			str += "U,";
		if(state & ProtectionStatus::OverloadP)
			str += "P,";
		str.Truncate(str.GetLength() - 1);
	}
	
	if((state & ProtectionStatus::OK) || (state & ProtectionStatus::Unknown))
	{
		ProtectionSignalingCount = 0;
		SetStatusProtectionHighlight();
		m_wndStatusBar.SetFgColor(Indicators::Protection, 0x000000, 0x000000);
	}
	else if((state & ProtectionStatus::OverloadI) || (state & ProtectionStatus::OverloadU) || (state & ProtectionStatus::OverloadP))
	{
		ProtectionSignalingCount = 1;
		SetStatusProtectionHighlight();
		m_wndStatusBar.SetFgColor(Indicators::Protection, 0x0000FF, 0x0000FF);
	}
	else
	{
		ProtectionSignalingCount = 0;
		SetStatusProtectionHighlight();
		m_wndStatusBar.SetFgColor(Indicators::Protection, 0xFF0000, 0xFF0000);
	}
	//::SendMessage(m_wndStatusBar.GetSafeHwnd(), SB_SETTEXT,	Indicators::Protection, (LPARAM) (LPCSTR) str);
	m_wndStatusBar.SetText(Indicators::Protection, (LPCSTR) str, (LPCSTR) str);
}

afx_msg void CMainFrame::OnTimer(UINT_PTR nIDEvent)
{
	if(nIDEvent == TimerIdStatusProtection)
	{
		SetStatusProtectionHighlight();
		ProtectionSignalingCount++;
	}
}

void CMainFrame::SetStatusProtectionHighlight()
{
	m_wndStatusBar.SetPaneStyle(Indicators::Protection, (ProtectionSignalingCount%2 == 0)? SBPS_NORMAL : SBPS_POPOUT );
}

LRESULT CMainFrame::OnRemoteDxpsState(WPARAM WParam, LPARAM LParam)
{
	return ::SendMessage(GetActiveView()->GetSafeHwnd(), wm_RegDxpsMessageID, WParam, LParam);
}

void CMainFrame::OnPopupResetProtection()
{
	CThermoReg::ResetProtection();
}

void CMainFrame::OnPopupProtectionParameters()
{
	CProtectionSetupDlg dlg;
	dlg.DoModal();
}
