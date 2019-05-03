// DxpsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "prognew.h"
#include "DxpsDlg.h"
#include "DxpsRegion.h"
#include "Main.h"
#include "resource.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
extern CProgNewApp theApp;
extern CMutex MutexThread;
/////////////////////////////////////////////////////////////////////////////
// CDxpsDlg dialog


CDxpsDlg::CDxpsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDxpsDlg::IDD, pParent), m_EditBkBrush( RGB(255, 255, 255) )
{
	//{{AFX_DATA_INIT(CDxpsDlg)
	m_EditTime = _T("");
	//}}AFX_DATA_INIT
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON_DXPS);
	Init=TRUE;
	m_ColorIndex[0]=RGB(255,255,255);
	m_ColorIndex[1]=RGB(0,255,255);
	m_ColorIndex[2]=RGB(255,0,255);
	m_ColorIndex[3]=RGB(0,0,255);
	m_ColorIndex[4]=RGB(255,255,0);
	m_ColorIndex[5]=RGB(0,255,0);
	m_ColorIndex[6]=RGB(255,0,0);
	m_ColorIndex[7]=RGB(128,128,128);
	m_ColorIndex[8]=RGB(192,192,192);
	m_ColorIndex[9]=RGB(0,128,128);
	m_ColorIndex[10]=RGB(128,0,128);
	m_ColorIndex[11]=RGB(0,0,128);
	m_ColorIndex[12]=RGB(128,128,0);
	m_ColorIndex[13]=RGB(0,128,0);
	m_ColorIndex[14]=RGB(128,0,0);
}


void CDxpsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDxpsDlg)
	DDX_Control(pDX, IDC_LIST_DXPS, m_ListDxps);
	DDX_Text(pDX, IDC_EDIT_DXPS_TIME, m_EditTime);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDxpsDlg, CDialog)
	//{{AFX_MSG_MAP(CDxpsDlg)
	ON_WM_SIZE()
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_DXPS, OnDblclkListDxps)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_DXPS, OnRclickListDxps)
	ON_BN_CLICKED(IDC_BUTTON_DXPS_EDIT, OnButtonDxpsEdit)
	ON_WM_MOVE()
	ON_WM_TIMER()
	ON_EN_KILLFOCUS(IDC_EDIT_DXPS_TIME, OnKillfocusEditDxpsTime)
	ON_BN_CLICKED(IDC_BUTTON_DXPS_ADDNEW, OnButtonDxpsAddNew)
	ON_COMMAND(ID_ROOT_ADDNEW, OnRootAddNew)
	ON_BN_CLICKED(IDC_BUTTON_DXPS_DELETE, OnButtonDxpsDelete)
	ON_COMMAND(ID_ROOT_DELETE, OnRootDelete)
	ON_COMMAND(ID_ROOT_EDIT, OnRootEdit)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_BUTTON_DXPS_ONOFF, OnButtonDxpsOnoff)
	ON_EN_CHANGE(IDC_EDIT_DXPS_TIME, OnChangeEditDxpsTime)
	ON_WM_CTLCOLOR()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_DXPS_CLOSE, OnButtonDxpsClose)
	ON_WM_GETMINMAXINFO()
	ON_COMMAND(ID_ROOT_ONOFF, OnButtonDxpsOnoff)
	ON_EN_SETFOCUS(IDC_EDIT_DXPS_TIME, OnSetFocusEditDxpsTime)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_CHECK_DXPS_HIDE_OFF, &CDxpsDlg::OnClickedCheckDxpsHideOff)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDxpsDlg message handlers

BOOL CDxpsDlg::OnInitDialog()
{
	CWnd::SetIcon(m_hIcon, TRUE);			// Set big icon

	CDialog::OnInitDialog();
	CWinApp* App=AfxGetApp();
	EditScanTimeActualized=TRUE;
	m_SetFocusEditScanTime=TRUE;
	::SendMessage(m_ListDxps.m_hWnd, LVM_SETEXTENDEDLISTVIEWSTYLE, 0,LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);
	long style = GetWindowLong(m_ListDxps.m_hWnd,GWL_STYLE);
	SetWindowLong(m_ListDxps.m_hWnd,GWL_STYLE, (style& ~LVS_TYPEMASK)|LVS_REPORT );

// Добавили колонки
	m_ListDxps.InsertColumn(0,_T("Region"),LVCFMT_LEFT,App->GetProfileInt("DxpsDlg", "ListColumn0", 72));
	m_ListDxps.InsertColumn(1,_T("Type"),LVCFMT_CENTER,App->GetProfileInt("DxpsDlg", "ListColumn1", 95));
	m_ListDxps.InsertColumn(2,_T("BE [eV]"),LVCFMT_CENTER,App->GetProfileInt("DxpsDlg", "ListColumn2", 95));
	m_ListDxps.InsertColumn(3,_T("HV [eV]"),LVCFMT_CENTER,App->GetProfileInt("DxpsDlg", "ListColumn3", 95));
	m_ListDxps.InsertColumn(4,_T("Anode"),LVCFMT_CENTER,App->GetProfileInt("DxpsDlg", "ListColumn4", 95));
	m_ListDxps.InsertColumn(5,_T("Delay [s]"),LVCFMT_CENTER,App->GetProfileInt("DxpsDlg", "ListColumn5", 95));
	m_ListDxps.InsertColumn(6,_T("Dwell [s]"),LVCFMT_CENTER,App->GetProfileInt("DxpsDlg", "ListColumn6", 95));
	m_ListDxps.InsertColumn(7,_T("Comments"),LVCFMT_LEFT,App->GetProfileInt("DxpsDlg", "ListColumn7", 125));

	LOGFONT lgfnt;
	CFont *pfont;
	pfont=m_ListDxps.GetFont();	
	pfont->GetLogFont(&lgfnt);

	strcpy( lgfnt.lfFaceName, "MS Sans Serif" );

	m_Font.DeleteObject();
	m_Font.CreateFontIndirect( &lgfnt );
	m_ListDxps.SetFont( &m_Font, TRUE );

	int ScreenSizeX=GetSystemMetrics(SM_CXSCREEN);
	int ScreenSizeY=GetSystemMetrics(SM_CYSCREEN);
	int MinWindowSizeX=GetSystemMetrics(SM_CXMIN);
	int MinWindowSizeY=GetSystemMetrics(SM_CYMIN);

	int left=App->GetProfileInt("SizeOfRegionWindow", "Left", 0);
	int top=App->GetProfileInt("SizeOfRegionWindow", "Top", 0);
	int right=App->GetProfileInt("SizeOfRegionWindow", "Right", 340);
	int bottom=App->GetProfileInt("SizeOfRegionWindow", "Bottom", 203);
	if(top<-10) top=-10;
	if(right<50) right=100;
	if(bottom<20) bottom=20;
	if(right-left<MinWindowSizeX) left=right-MinWindowSizeX;
	if(bottom-top<MinWindowSizeY) top=bottom-MinWindowSizeY;
	if(right-left>ScreenSizeX) left=right-ScreenSizeX;
	if(bottom-top>ScreenSizeY) top=bottom-ScreenSizeY;

	MoveWindow(left,top,right-left,	bottom-top);
	CString str;
	str.Format("%.3g", CDxpsRegion::ScanTime);
	GetDlgItem(IDC_EDIT_DXPS_TIME)->SetWindowText((LPCSTR)str);
	ImLst.Create(36,16,TRUE,1,1);

	CBitmap bm;
	bm.LoadBitmap(IDB_BITMAP_DEF);
	ImLst.Add(&bm, RGB(255, 255, 255));
	bm.DeleteObject();
	bm.LoadBitmap(IDB_BITMAP_CUR);
	ImLst.Add(&bm, RGB(255, 255, 255));
	bm.DeleteObject();
	bm.LoadBitmap(IDB_BITMAP_OFF);
	ImLst.Add(&bm, RGB(255, 255, 255));
	bm.DeleteObject();
	m_ListDxps.SetImageList(&ImLst, LVSIL_SMALL);	
	m_EditTime.Format("%g", CDxpsRegion::ScanTime);
	LoadRegionPreferences();
	Init=FALSE;

	MoveControls();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDxpsDlg::MoveRegionWindow()
{
	if(!theApp.m_pMainFrame->m_bSynchronousResize)
	{
		RECT rect;
		GetWindowRect(&rect);
		theApp.m_pMainFrame->m_bSynchronousResize=TRUE;
		if(theApp.m_pMainFrame->m_pRegionWnd->GetSafeHwnd())
			theApp.m_pMainFrame->m_pRegionWnd->MoveWindow(&rect,FALSE);
		theApp.m_pMainFrame->m_bSynchronousResize=FALSE;
	}
}

void CDxpsDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	if(!Init)
	{
		MoveControls();

		MoveRegionWindow();
	}
}

void CDxpsDlg::OnDblclkListDxps(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LPNMITEMACTIVATE lpnmitem = (LPNMITEMACTIVATE) pNMHDR;
	if(lpnmitem->iItem<0)
		OnButtonDxpsAddNew();
	else
		{	
		OnButtonDxpsEdit();
		}
	
	*pResult = 0;
}

void CDxpsDlg::OnRclickListDxps(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LPNMITEMACTIVATE lpnmitem = (LPNMITEMACTIVATE) pNMHDR;

	CPoint point = lpnmitem->ptAction;
	DWORD EnableItem = (m_ListDxps.HitTest(point)>=0)? MF_ENABLED:MF_GRAYED;
	RECT ListRect;
	m_ListDxps.GetWindowRect(&ListRect);
	point.x+=ListRect.left;
	point.y+=ListRect.top;

    CMenu menu;
    if (menu.LoadMenu(IDR_MENU_DXPS_POPUP))
    {
        CMenu* pPopup = menu.GetSubMenu(0);
        ASSERT(pPopup != NULL);
		pPopup->EnableMenuItem(ID_ROOT_EDIT,MF_BYCOMMAND|EnableItem);
		pPopup->EnableMenuItem(ID_ROOT_DELETE,MF_BYCOMMAND|EnableItem);
		pPopup->EnableMenuItem(ID_ROOT_VIEW,MF_BYCOMMAND|EnableItem);
		pPopup->EnableMenuItem(ID_ROOT_ONOFF,MF_BYCOMMAND|EnableItem);

		pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON,
        point.x, point.y,
        this); // use main window for cmds
    }	
	*pResult = 0;
}

BOOL CDxpsDlg::Create()
{
	return CDialog::Create(CDxpsDlg::IDD);
}

BOOL CDxpsDlg::PreCreateWindow(CREATESTRUCT& cs) 
{
	CWinApp* App=AfxGetApp();

	cs.x=App->GetProfileInt("DxpsDlg", "X", 0);
	cs.y=App->GetProfileInt("DxpsDlg", "Y", 0);
	cs.cx=App->GetProfileInt("DxpsDlg", "CX", 340);
	cs.cy=App->GetProfileInt("DxpsDlg", "CY", 203);
	
	return CDialog::PreCreateWindow(cs);
}

void CDxpsDlg::OnMove(int x, int y) 
{
	CDialog::OnMove(x, y);
	if(!Init)
	{
		MoveRegionWindow();
	}
}

void CDxpsDlg::MoveControls()
{
	RECT rect={1, 1, 2, 2}, rectCtrl={1, 1, 2, 2}, OriginDlgRect={1, 1, 339, 203};
	GetClientRect(&rect);
	MapDialogRect(&OriginDlgRect);
	//Элементы справа от ListCtrl. При resize постоянным сохраняется
	//расстояние от правого верхнего угла
	//IDC_STATIC_SCAN_T,297,2,41,8
	rectCtrl.left=297;rectCtrl.top=2;
	MapDialogRect(&rectCtrl);
	GetDlgItem(IDC_STATIC_SCAN_T)->SetWindowPos(NULL, rect.right-OriginDlgRect.right+rectCtrl.left, rectCtrl.top, 0, 0, SWP_NOSIZE|SWP_NOZORDER);

	//IDC_EDIT_DXPS_TIME,297,12,40,12
	rectCtrl.left=297;rectCtrl.top=12;
	MapDialogRect(&rectCtrl);
	GetDlgItem(IDC_EDIT_DXPS_TIME)->SetWindowPos(NULL, rect.right-OriginDlgRect.right+rectCtrl.left, rectCtrl.top, 0, 0, SWP_NOSIZE|SWP_NOZORDER);

	//IDC_BUTTON_DXPS_ADDNEW,297,27,42,12
	rectCtrl.left=297;rectCtrl.top=27;rectCtrl.bottom=rectCtrl.top+12;
	MapDialogRect(&rectCtrl);
	GetDlgItem(IDC_BUTTON_DXPS_ADDNEW)->SetWindowPos(NULL, rect.right-OriginDlgRect.right+rectCtrl.left, rectCtrl.top, 0, 0, SWP_NOSIZE|SWP_NOZORDER);
	if(rect.bottom<(rectCtrl.top+rectCtrl.bottom)/2) 
		GetDlgItem(IDC_BUTTON_DXPS_ADDNEW)->ShowWindow(SW_HIDE);
	else
		GetDlgItem(IDC_BUTTON_DXPS_ADDNEW)->ShowWindow(SW_SHOW);

	//IDC_BUTTON_DXPS_EDIT,297,41,42,12
	rectCtrl.left=297;rectCtrl.top=41;rectCtrl.bottom=rectCtrl.top+12;
	MapDialogRect(&rectCtrl);
	GetDlgItem(IDC_BUTTON_DXPS_EDIT)->SetWindowPos(NULL, rect.right-OriginDlgRect.right+rectCtrl.left, rectCtrl.top, 0, 0, SWP_NOSIZE|SWP_NOZORDER);
	if(rect.bottom<(rectCtrl.top+rectCtrl.bottom)/2) 
		GetDlgItem(IDC_BUTTON_DXPS_EDIT)->ShowWindow(SW_HIDE);
	else
		GetDlgItem(IDC_BUTTON_DXPS_EDIT)->ShowWindow(SW_SHOW);

	//IDC_BUTTON_DXPS_DELETE,297,55,42,12
	rectCtrl.left=297;rectCtrl.top=55;rectCtrl.bottom=rectCtrl.top+12;
	MapDialogRect(&rectCtrl);
	GetDlgItem(IDC_BUTTON_DXPS_DELETE)->SetWindowPos(NULL, rect.right-OriginDlgRect.right+rectCtrl.left, rectCtrl.top, 0, 0, SWP_NOSIZE|SWP_NOZORDER);
	if(rect.bottom<(rectCtrl.top+rectCtrl.bottom)/2) 
		GetDlgItem(IDC_BUTTON_DXPS_DELETE)->ShowWindow(SW_HIDE);
	else
		GetDlgItem(IDC_BUTTON_DXPS_DELETE)->ShowWindow(SW_SHOW);

	//IDC_BUTTON_DXPS_VIEW,297,72,42,12
	//rectCtrl.left=297;rectCtrl.top=72;
	//MapDialogRect(&rectCtrl);
	//GetDlgItem(IDC_BUTTON_DXPS_VIEW)->SetWindowPos(NULL, rect.right-OriginDlgRect.right+rectCtrl.left, rectCtrl.top, 0, 0, SWP_NOSIZE|SWP_NOZORDER);

	//IDC_BUTTON_DXPS_ONOFF,297,69,42,12
	rectCtrl.left=297;rectCtrl.top=69; rectCtrl.bottom=rectCtrl.top+12;
	MapDialogRect(&rectCtrl);
	GetDlgItem(IDC_BUTTON_DXPS_ONOFF)->SetWindowPos(NULL, rect.right-OriginDlgRect.right+rectCtrl.left, rectCtrl.top, 0, 0, SWP_NOSIZE|SWP_NOZORDER);
	if(rect.bottom<(rectCtrl.top+rectCtrl.bottom)/2) 
		GetDlgItem(IDC_BUTTON_DXPS_ONOFF)->ShowWindow(SW_HIDE);
	else
		GetDlgItem(IDC_BUTTON_DXPS_ONOFF)->ShowWindow(SW_SHOW);

	//IDC_BUTTON_DXPS_CLOSE,297,83,42,12
	rectCtrl.left=297;rectCtrl.top=83; rectCtrl.bottom=rectCtrl.top+12;
	MapDialogRect(&rectCtrl);
	GetDlgItem(IDC_BUTTON_DXPS_CLOSE)->SetWindowPos(NULL, rect.right-OriginDlgRect.right+rectCtrl.left, rectCtrl.top, 0, 0, SWP_NOSIZE|SWP_NOZORDER);
	if(rect.bottom<(rectCtrl.top+rectCtrl.bottom)/2) 
		GetDlgItem(IDC_BUTTON_DXPS_CLOSE)->ShowWindow(SW_HIDE);
	else
		GetDlgItem(IDC_BUTTON_DXPS_CLOSE)->ShowWindow(SW_SHOW);

	//IDC_CHECK_DXPS_HIDE_OFF,298,96,39,10
	rectCtrl.left=298;rectCtrl.top=96; rectCtrl.bottom=rectCtrl.top+10;
	MapDialogRect(&rectCtrl);
	GetDlgItem(IDC_CHECK_DXPS_HIDE_OFF)->SetWindowPos(NULL, rect.right-OriginDlgRect.right+rectCtrl.left, rectCtrl.top, 0, 0, SWP_NOSIZE|SWP_NOZORDER);
	if(rect.bottom<(rectCtrl.top+rectCtrl.bottom)/2) 
		GetDlgItem(IDC_CHECK_DXPS_HIDE_OFF)->ShowWindow(SW_HIDE);
	//else
	//	GetDlgItem(IDC_CHECK_DXPS_HIDE_OFF)->ShowWindow(SW_SHOW);

	//ListCtrl. При resize постоянным сохраняется
	//расстояние от всех углов
	//IDC_LIST_DXPS, 0,0,293,203
	rectCtrl.left=0;rectCtrl.top=0;rectCtrl.right=293;rectCtrl.bottom=203;
	MapDialogRect(&rectCtrl);
	GetDlgItem(IDC_LIST_DXPS)->SetWindowPos(NULL, rectCtrl.left, rectCtrl.top, rect.right-OriginDlgRect.right+rectCtrl.right, rect.bottom-OriginDlgRect.bottom+rectCtrl.bottom, SWP_NOMOVE|SWP_NOZORDER);

	AdjustListLastColomn();
}

void CDxpsDlg::OnTimer(UINT nIDEvent) 
{
	switch(nIDEvent)
	{
	case DXPS_TIMER_INIT:
		KillTimer(nIDEvent);
		//MoveControls();
		break;
	}
	
	CDialog::OnTimer(nIDEvent);
}

BOOL CDxpsDlg::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{ int a=HDN_ENDTRACK;
	LPNMHDR pnmHDR=(LPNMHDR)lParam;

	//A list colomn has beed resized
	if((pnmHDR->hwndFrom==m_ListDxps.GetHeaderCtrl()->m_hWnd) && (pnmHDR->code==HDN_ENDTRACK))
	{	
		AdjustListLastColomn();
		LVCOLUMN col;

		col.mask=LVCF_WIDTH;
		col.iOrder=0;
		CString str;
		CWinApp* App=AfxGetApp();
		for (int i=0; i<=7;i++)
		{
			m_ListDxps.GetColumn(i,&col);
			str.Format("ListColumn%i",i);
			if((int)App->GetProfileInt("DxpsDlg", str, -1)!=col.cx)
				App->WriteProfileInt("DxpsDlg", str, col.cx);

		}
	}
	return CDialog::OnNotify(wParam, lParam, pResult);
}

void CDxpsDlg::OnKillfocusEditDxpsTime() 
{
	TryWithMessageBox([&](){
		char str[25];
		GetDlgItem(IDC_EDIT_DXPS_TIME)->GetWindowText(str, 24);
		double NewVal = atof(str);
		if (CDxpsRegion::ScanTime == NewVal)
		{
			EditScanTimeActualized = TRUE;
			GetDlgItem(IDC_EDIT_DXPS_TIME)->Invalidate();
		}
		else if ((CDxpsRegion::OutData.size() == 0) ||
			((CDxpsRegion::OutData.size() > 0) && (NewVal > CDxpsRegion::PassedCommonTime)))
		{
			CDxpsRegion::ScanTime = NewVal;
			m_EditTime.Format("%g", CDxpsRegion::ScanTime);
			GetDlgItem(IDC_EDIT_DXPS_TIME)->SetWindowText((LPCSTR)m_EditTime);
			theApp.Ini.DxpsLastScanTime.Value = (int)NewVal;
			theApp.Ini.DxpsLastScanTime.Write();

			if(theApp.m_pMainFrame->m_Doc.IsFileOpen())
				theApp.m_pMainFrame->m_Doc.DxpsProject.WriteDxpsRegionsParam();

			EditScanTimeActualized = TRUE;
			GetDlgItem(IDC_EDIT_DXPS_TIME)->Invalidate();
			theApp.m_pMainFrame->m_Graph.SetRange(0, 1 / 24. / 60. / 60.*CDxpsRegion::ScanTime, 0, 100);
		}
		m_SetFocusEditScanTime = FALSE;
	}, this);
}

void CDxpsDlg::OnButtonDxpsAddNew() 
{
	TryWithMessageBox([&]() {
		CDxpsRegion* pReg;
		int i;
		BOOL NeedBreak = FALSE;
		int ExistingColors[15];
		ZeroMemory(&ExistingColors, 15 * sizeof(int));
		for (pReg = CDxpsRegion::GetFirst(); pReg != 0; pReg = CDxpsRegion::GetNext(pReg))
		{
			ExistingColors[pReg->Parameters.ColorIndex] += 1;
		}

		while (!NeedBreak)
			for (i = 0; i < 15; i++)
			{
				if (ExistingColors[i]-- <= 0)
				{
					NeedBreak = TRUE;
					break;
				}
			}
		DxpsRegPar Parameters;
		memcpy(&Parameters, &PreferredParams, sizeof(DxpsRegPar));
		Parameters.Type = TYPE_BE;
		Parameters.Divident = 1;
		Parameters.Divisor = 2;
		Parameters.ColorIndex = i;
		Parameters.Comments[0] = '\0';
		CDxpsParamDlg dlg;
		dlg.pParameters = &Parameters;
		::EnableWindow(theApp.m_pMainFrame->m_hWnd, FALSE);
		dlg.m_pParentWindow = this;
		dlg.m_NewRegion = TRUE;
		if (dlg.DoModal() == IDOK)
		{
			CSingleLock sLock(&MutexThread);
			THRI_LOCK();
			CDxpsRegion::CreateNewRegion();
			memcpy(&CDxpsRegion::GetLast()->Parameters, &Parameters, sizeof(DxpsRegPar));
			FillTableRow(CDxpsRegion::GetLast()->ID, &CDxpsRegion::GetLast()->Parameters);
			theApp.m_pMainFrame->m_Doc.CheckDocType();
			memcpy(&PreferredParams, &CDxpsRegion::GetLast()->Parameters, sizeof(DxpsRegPar));
			SaveRegionPreferences();
			if (Parameters.Type == TYPE_DIV)
				CDxpsRegion::CalcAllMissingDivRegions([](DxpsOutData outData) { theApp.m_pMainFrame->m_Graph.PlotNewSegment(outData); });
			//Если проект был сохранен (или открыт), удаляем его и переписываем заново
			if (theApp.m_pMainFrame->m_Doc.IsFileOpen())
				theApp.m_pMainFrame->m_Doc.SaveProjectFile();
			else
				theApp.m_pMainFrame->m_Doc.m_NeedSave = theApp.m_pMainFrame->m_Doc.Need;
			THRI_UNLOCK();
		}
		::SetFocus(m_hWnd);
		::EnableWindow(theApp.m_pMainFrame->m_hWnd, TRUE);
		m_ListDxps.SetFocus();
	}, this);
}

void CDxpsDlg::OnButtonDxpsEdit() 
{
	TryWithMessageBox([&]() {
		int Selection=FindSelectedItem();
		if(Selection<0)
			{
			MessageBox("Please select DXPS region.", "No selection",MB_OK|MB_ICONINFORMATION);
			return;
			}
		CDxpsRegion* reg=CDxpsRegion::GetRegByN(m_ListDxps.GetSelectionMark());
		if(reg==NULL)
		{
			Msg("Error: reg==NULL"); 
			return;
		}
		CDxpsParamDlg dlg;
		dlg.pParameters=&reg->Parameters;
		dlg.m_RegionID=reg->ID;
		dlg.m_pReg=reg;
		dlg.m_pParentWindow=this;
		::EnableWindow(theApp.m_pMainFrame->m_hWnd, FALSE);

		if(dlg.DoModal()==IDOK)
		{
			CSingleLock sLock(&MutexThread);
			THRI_LOCK();

			memcpy(&PreferredParams,&reg->Parameters,sizeof(DxpsRegPar));

			//Если проект был сохранен (или открыт), переписываем параметры регионов
			if(theApp.m_pMainFrame->m_Doc.IsFileOpen())
				theApp.m_pMainFrame->m_Doc.DxpsProject.WriteDxpsRegionsParam();
			else
				theApp.m_pMainFrame->m_Doc.m_NeedSave=theApp.m_pMainFrame->m_Doc.Need;
			THRI_UNLOCK();
		}
		::EnableWindow(theApp.m_pMainFrame->m_hWnd, TRUE);
		FillTableRow(reg->ID,&reg->Parameters);
		m_ListDxps.SetFocus();
	}, this);
}

// Обновляет значения или вобавляет в ListBox строку с индексом Row
void CDxpsDlg::FillTableRow(int Row, DxpsRegPar *Param)
{
	if(!::IsWindow(m_ListDxps.m_hWnd))
		return;
	int i;
	CString str;
	int Items;
	Items=m_ListDxps.GetItemCount();
	for(i=Items; i<=Row; i++)
	{
		str.Format("D%i", i+1); 
		m_ListDxps.InsertItem( i, (LPCTSTR) str );
	}

	if (Param->Type==0)
	{
		str="BE";
		m_ListDxps.SetItemText(Row, 1, (LPCTSTR) str );
		str.Format("%g", Param->BE);
		m_ListDxps.SetItemText(Row, 2, (LPCTSTR) str );
		str.Format("%g", Param->HV);
		m_ListDxps.SetItemText(Row, 3, (LPCTSTR) str );
		char *anodes[]={"Al", "Mg", "He I", "He II"};
		m_ListDxps.SetItemText(Row, 4, anodes[Param->Anode]);
		str.Format("%g", Param->Delay);
		m_ListDxps.SetItemText(Row, 5, (LPCTSTR) str );
		str.Format("%g", Param->Dwell);
		m_ListDxps.SetItemText(Row, 6, (LPCTSTR) str );
	}
	else
	{
		str.Format("Div %i/%i", Param->Divident,Param->Divisor); 
		m_ListDxps.SetItemText(Row, 1, (LPCTSTR) str );
		m_ListDxps.SetItemText(Row, 2, "-" );
		m_ListDxps.SetItemText(Row, 3, "-" );
		m_ListDxps.SetItemText(Row, 4, "-" );
		m_ListDxps.SetItemText(Row, 5, "-" );
		m_ListDxps.SetItemText(Row, 6, "-" );
	}
	m_ListDxps.SetItemText(Row, 7, Param->Comments );
	SetIcon(Row,Param->ColorIndex,Param->Off*2);

}

void CDxpsDlg::OnRootAddNew()
{
	OnButtonDxpsAddNew();
}

void CDxpsDlg::OnButtonDxpsDelete() 
{
	TryWithMessageBox([&]() {
		int Selection = FindSelectedItem();
		if (Selection < 0)
		{
			MessageBox("Please select DXPS region.", "No selection", MB_OK | MB_ICONINFORMATION);
			return;
		}

		CDxpsRegion* reg = CDxpsRegion::GetRegByN(Selection);
		if (reg == NULL)
		{
			Msg("Error: reg==NULL");
			return;
		}
		int OkCancel = IDOK;
		if (reg->HasData())
		{		//There is any data in this region
			CString str;
			str.Format("Region %i contains data.\n\nAre you sure you want to delete it?", reg->ID);
			OkCancel = MessageBox((LPCSTR)str, "Warning", MB_ICONEXCLAMATION | MB_OKCANCEL);
		}
		if (OkCancel != IDOK)
		{
			m_ListDxps.SetFocus();
			return;
		}
		CSingleLock sLock(&MutexThread);
		THRI_LOCK();

		delete reg;
		//Если проект был сохранен (или открыт), удаляем его и переписываем заново
		if (theApp.m_pMainFrame->m_Doc.IsFileOpen())
			theApp.m_pMainFrame->m_Doc.SaveProjectFile();
		else
			theApp.m_pMainFrame->m_Doc.m_NeedSave = theApp.m_pMainFrame->m_Doc.Need;
		THRI_UNLOCK();
		theApp.m_pMainFrame->m_Doc.CheckDocType();
		FillTable();
		m_ListDxps.SetFocus();
	}, this);
}

void CDxpsDlg::OnRootDelete() 
{OnButtonDxpsDelete();}

void CDxpsDlg::OnRootEdit() 
{OnButtonDxpsEdit();}

void CDxpsDlg::FillTable()
{
	m_ListDxps.DeleteAllItems();
	CDxpsRegion *reg;
	for(reg=CDxpsRegion::GetFirst();reg!=NULL; reg=CDxpsRegion::GetNext(reg))
		FillTableRow(reg->ID,&reg->Parameters);
}

int CDxpsDlg::FindSelectedItem()
{
	UINT N_Item = m_ListDxps.GetItemCount();
	UINT mask = (LVIS_SELECTED | LVIS_FOCUSED);
	UINT state;
	for(UINT i=0; i<N_Item; ++i)
	{
		state =  m_ListDxps.GetItemState(i, mask);	
		if( (state & LVIS_SELECTED) && (state & LVIS_FOCUSED)) return ((int) i);
	}
	return -1;
}

void CDxpsDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	if(::IsWindow(theApp.m_pMainFrame->m_pRegionWnd->GetSafeHwnd()) && (bShow==SW_SHOW||bShow==SW_SHOWNORMAL))
		theApp.m_pMainFrame->m_pRegionWnd->ShowWindow(SW_HIDE);
	
}

void CDxpsDlg::OnButtonDxpsOnoff() 
{
	TryWithMessageBox([&]() {
		int Selection = FindSelectedItem();
		if (Selection < 0)
		{
			MessageBox("Please select DXPS region.", "No selection", MB_OK | MB_ICONINFORMATION);
			return;
		}
		CDxpsRegion* reg = CDxpsRegion::GetRegByN(Selection);
		CSingleLock sLock(&MutexThread);
		THRI_LOCK();

		reg->Parameters.Off = !reg->Parameters.Off;
		SetIcon(Selection, reg->Parameters.ColorIndex, reg->Parameters.Off * 2);

		//Если проект был сохранен (или открыт), переписываем параметры регионов
		if (theApp.m_pMainFrame->m_Doc.IsFileOpen())
			theApp.m_pMainFrame->m_Doc.DxpsProject.WriteDxpsRegionsParam();
		else
			theApp.m_pMainFrame->m_Doc.m_NeedSave = theApp.m_pMainFrame->m_Doc.Need;
		THRI_UNLOCK();
	}, this);
}

void CDxpsDlg::SetIcon(int Row, int ColorIndex, int Status)
{
	LV_ITEM item;
	memset(&item, 0, sizeof(LV_ITEM));
	item.iItem = Row;
	item.iSubItem=0;
	item.iImage=ColorIndex+15*Status;
	item.mask = LVIF_IMAGE;
	m_ListDxps.SetItem(&item);
}

void CDxpsDlg::SaveRegionPreferences()
{
char Buffer[2*sizeof(PreferredParams)+1];
int byte;
for(int i=0; i<sizeof(PreferredParams)-sizeof(PreferredParams.Comments);i++)
{
	byte=((BYTE*)(&PreferredParams))[i];
	sprintf(&Buffer[2*i],"%.2X",byte);
}
theApp.WriteProfileString("DxpsDlg","PreferredParams",&Buffer[0]);
}

void CDxpsDlg::LoadRegionPreferences()
{
	CString str;
	char buf[3];
	BYTE byte;
	str=theApp.GetProfileString("DxpsDlg","PreferredParams","");
	if(str.GetLength()!=2*sizeof(PreferredParams)-2*sizeof(PreferredParams.Comments))
	{
		memset(&PreferredParams, 0, sizeof(DxpsRegPar));
		PreferredParams.HV = 1000.;
		PreferredParams.Dwell = 10.;
		PreferredParams.Off = FALSE;
		PreferredParams.Type=TYPE_BE;
		return;
	}

	for(int i=0; i<sizeof(PreferredParams)-sizeof(PreferredParams.Comments);i++)
	{
		strcpy(&buf[0],(LPCSTR)str.Mid(2*i,2));
		byte=(BYTE)strtol(&buf[0],NULL,16);
		((BYTE*)(&PreferredParams))[i]=byte;
	}
	PreferredParams.Type=TYPE_BE;

	memset(PreferredParams.Comments,0, sizeof(PreferredParams.Comments));
}

BOOL CDxpsDlg::AdjustListLastColomn() //возвращает TRUE, если была передвижка
{
	int Sum=0, LastWidth, ListWidth=0;
	RECT Rect;
	LVCOLUMN col;
	col.mask=LVCF_WIDTH;
	col.iOrder=0;
	CString str;
	CWinApp* App=AfxGetApp();
	for (int i=0; i<=6;i++)
	{
		m_ListDxps.GetColumn(i,&col);
		Sum+=col.cx;
	}
	int lastIndex = 7;
	m_ListDxps.GetColumn(lastIndex,&col);
	LastWidth=col.cx;
	m_ListDxps.GetWindowRect( &Rect );
	ListWidth=Rect.right-Rect.left;
	m_ListDxps.GetClientRect( &Rect );
	ListWidth=Rect.right-Rect.left;
	if(ListWidth-Sum!=LastWidth)
	{
		LastWidth=ListWidth-Sum;
		col.cx=LastWidth;
		m_ListDxps.SetColumn(lastIndex,&col);
		return TRUE;
	}
	else
		return FALSE;

}

void CDxpsDlg::OnChangeEditDxpsTime() 
{
EditScanTimeActualized=FALSE;
GetDlgItem(IDC_EDIT_DXPS_TIME)->Invalidate();
}

HBRUSH CDxpsDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
 	if(nCtlColor==CTLCOLOR_EDIT && 
		pWnd->GetSafeHwnd()==GetDlgItem(IDC_EDIT_DXPS_TIME)->GetSafeHwnd()
		&& !EditScanTimeActualized)
	{
		pDC->SetTextColor(RGB(255, 0, 0));
		pDC->SetBkColor(RGB(255, 255, 255));
		return (HBRUSH)(m_EditBkBrush.GetSafeHandle());
	}
    else
        return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
}


void CDxpsDlg::OnButtonDxpsClose() 
{
	ShowWindow(SW_HIDE);	
}

void CDxpsDlg::OnOK()
{
	if(m_SetFocusEditScanTime)
	{
		OnKillfocusEditDxpsTime();
		m_SetFocusEditScanTime=TRUE;
	}
}

void CDxpsDlg::OnSetFocusEditDxpsTime() 
{
	m_SetFocusEditScanTime=TRUE;
}


void CDxpsDlg::OnClickedCheckDxpsHideOff()
{
	// TODO: Add your control notification handler code here
}
