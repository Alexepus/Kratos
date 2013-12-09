// SpecView.cpp : implementation of the CSpecView class
//

#include "stdafx.h"
#include "Spec.h"
#include "HSChart\\ChartCtrl.h"
#include "ReportCtrl.h"
#include "MassSpecDoc.h"
#include "SpecView.h"
#include "MainFrame.h"
#include "LogToFile.h"
#include "Approx.h"
#include "AppearSetupDlg.h"
#include "HeaterSetupDlg.h"
#include "MeasuringOptDlg.h"
#include "HardSetupDlg.h"
#include "Threads.h"
#include "AdcDacDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CSpecApp theApp;
extern UINT wm_RegDxpsMessageID;

using namespace std;
/////////////////////////////////////////////////////////////////////////////
// CSpecView

IMPLEMENT_DYNCREATE(CSpecView, CView)

BEGIN_MESSAGE_MAP(CSpecView, CView)
	//{{AFX_MSG_MAP(CSpecView)
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_COMMAND(ID_VIEW_MASSES, OnViewMasses)
	ON_UPDATE_COMMAND_UI(ID_VIEW_MASSES, OnUpdateViewMasses)
	ON_COMMAND(ID_SETTINGS_CALIBRATION, OnSettingsCalibration)
	ON_WM_TIMER()
	ON_COMMAND(ID_BUTTON_START, OnToolBarStart)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_START, OnUpdateToolBarButtonStart)
	ON_COMMAND(ID_BUTTON_STOP, OnToolBarStop)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_STOP, OnUpdateToolBarButtonStop)
	ON_WM_DESTROY()
	ON_COMMAND(ID_SETTINGS_APPEARANCE, OnSettingsAppearance)
	ON_COMMAND(ID_VIEW_MASSCHART, OnViewMassChart)
	ON_UPDATE_COMMAND_UI(ID_VIEW_MASSCHART, OnUpdateViewMassChart)
	ON_COMMAND(ID_VIEW_TEMPERATURECHART, OnViewTemperatureChart)
	ON_UPDATE_COMMAND_UI(ID_VIEW_TEMPERATURECHART, OnUpdateViewTemperatureChart)
	ON_COMMAND(ID_TR_START, OnTrStart)
	ON_COMMAND(ID_TR_WAIT, OnTrWait)
	ON_COMMAND(ID_TR_WAIT_CONT, OnTrWaitCont)
	ON_COMMAND(ID_TR_CONTINUE, OnTrContinue)
	ON_COMMAND(ID_TR_MAINOFF, OnTrMainOff)
	ON_COMMAND(ID_TR_PRIME, OnTrPrime)
	ON_UPDATE_COMMAND_UI(ID_TR_MAINOFF, OnUpdateTrMainoff)
	ON_UPDATE_COMMAND_UI(ID_TR_PRIME, OnUpdateTrPrime)
	ON_UPDATE_COMMAND_UI(ID_TR_START, OnUpdateTrStart)
	ON_UPDATE_COMMAND_UI(ID_TR_WAIT, OnUpdateTrWait)
	ON_UPDATE_COMMAND_UI(ID_TR_WAIT_CONT, OnUpdateTrWaitCont)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CUT, OnUpdateEditCut)
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_UPDATE_COMMAND_UI(ID_SETTINGS_CALIBRATION, OnUpdateSettingsCalibration)
	ON_COMMAND(ID_SETTINGS_HEATER_SETUP, OnSettingsHeaterSetup)
	ON_UPDATE_COMMAND_UI(ID_SETTINGS_HEATER_SETUP, OnUpdateSettingsHeaterSetup)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, OnUpdateEditCopy)
	ON_COMMAND(IDM_FILE_SAVEAS_EASYPLOT, OnFileSaveasEasyplot)
	ON_UPDATE_COMMAND_UI(ID_TR_SYNC_DXPS, OnUpdateTrSyncDxps)
	ON_UPDATE_COMMAND_UI(ID_TR_SYNC_MSP, OnUpdateTrSyncMsp)
	ON_COMMAND(ID_TR_SYNC_DXPS, OnTrSyncDxps)
	ON_COMMAND(ID_TR_SYNC_MSP, OnTrSyncMsp)
	ON_COMMAND(ID_SETTINGS_MEASURINGOPTIONS, OnSettingsMeasuringOptions)
	ON_COMMAND(ID_SETTINGS_HARDWARESETUP, OnSettingsHardwareSetup)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
	ON_MESSAGE(WM_ON_CHKBOX, OnListCheckBox)
	ON_MESSAGE(WM_ITEM_SORTED, OnListItemSorted)
	ON_MESSAGE(WM_EDIT_COMMITTED, OnListEditCommited)
	ON_MESSAGE(WM_COM_EVENT, OnSerialEvent)
	ON_MESSAGE(WM_MEAS_THREAD_DATA, OnMeasThreadMessage)
	ON_REGISTERED_MESSAGE(wm_RegDxpsMessageID, OnRemoteDxpsState)

	ON_COMMAND(ID_SETTINGS_ADCSDACS, &CSpecView::OnSettingsAdcsdacs)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSpecView construction/destruction
//CSpecView::
CSpecView::CSpecView()
{
	m_bShowList.Describe(TRUE,"View","ShowList");
	m_iListWidth.Describe(100,"View","ListWidth");
	m_iColumn0Width.Describe(60,"View","Column0Width");
	m_iColumn1Width.Describe(60,"View","Column1Width");
	m_bWasLButtonDown=FALSE;
	m_OldDispMode=m_DisplayMode=eMassSpec;
	m_bMeasureStart=FALSE;
	m_hCursorDivSize=AfxGetApp()->LoadCursor(IDC_CURSOR_DIVSIZE);
	m_pHeaterSetupDlg=new CHeaterSetupDlg;
	theApp.m_pSpecView=this;
}

CSpecView::~CSpecView()
{
	delete m_pChart;
	delete m_pList;
	delete m_pHeaterSetupDlg;
}

BOOL CSpecView::PreCreateWindow(CREATESTRUCT& cs)
{
	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CSpecView drawing

void CSpecView::OnDraw(CDC* pDC)
{
	CMassSpecDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	DrawDivider(pDC);
}

/////////////////////////////////////////////////////////////////////////////
// CSpecView printing

BOOL CSpecView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CSpecView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
}

void CSpecView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
}

/////////////////////////////////////////////////////////////////////////////
// CSpecView diagnostics

#ifdef _DEBUG
void CSpecView::AssertValid() const
{
	CView::AssertValid();
}

void CSpecView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMassSpecDoc* CSpecView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMassSpecDoc)));
	return (CMassSpecDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CSpecView message handlers

BOOL CSpecView::OnEraseBkgnd(CDC* pDC) 
{
	return 0;//CView::OnEraseBkgnd(pDC);
}

void CSpecView::OnSize(UINT nType, int cx, int cy) 
{
	if(nType!=SIZE_RESTORED)
		return;
	CView::OnSize(nType, cx, cy);
	ResizeClient(cx,cy);
}

int CSpecView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	//SetTimer(1,1000,NULL);
	RECT rect={50,50,100,100};
	GetClientRect(&rect);
	m_pChart=new CChartCtrl;
	m_pChart->Create(this,rect,ID_CHART,
		WS_CHILD|WS_CLIPCHILDREN|WS_VISIBLE);//|WS_HSCROLL|WS_VSCROLL,
	m_pChart->SetEdgeType(0);//BDR_RAISEDOUTER
	FormatChartToMassHist();

	m_pList=new CReportCtrl;
	m_pList->Create(this, ID_LIST, &rect);
	m_pList->ModifyStyle(0,LVS_SHOWSELALWAYS | WS_BORDER | WS_TABSTOP);
	m_pList->SetExtendedStyle(LVS_EX_GRIDLINES);
	m_pList->SetCheckboxeStyle();
	m_pList->InsertColumn(0,"Mass", LVCFMT_LEFT,m_iColumn0Width);
	m_pList->SetEditable(TRUE,-1,0);
	m_pList->InsertColumn(1,"Comments", LVCFMT_LEFT,m_iColumn1Width);
	m_pList->SetEditable(TRUE,-1,1);

	m_pCalibDlg=new CCalibDlg;
	m_pCalibDlg->Create(IDD_CALIB_DLG, this);
	m_pCalibDlg->m_pSpecView=this;
	
	m_ThermoReg.Init(this);

	return 0;
}

/*void CSpecView::OnLButtonClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CRect rect=m_rDivider;
	rect.InflateRect(1,0);
	if(rect.PtInRect(point))
	{
		m_bShowList=FALSE;
		RECT rect={50,50,200,200};
		GetClientRect(&rect);
		OnSize(SIZE_RESTORED,rect.right, rect.bottom);
		UpdateWindow();

	}
	CView::OnLButtonDblClk(nFlags, point);
}*/

void CSpecView::OnMouseMove(UINT nFlags, CPoint point) 
{
	if(m_bWasLButtonDown)
	{
		CRect rect;
		GetClientRect(&rect);
		m_iListWidth=m_iListWidth.iVal+m_LastPoint.x-point.x;
		m_LastPoint=point;
		if(m_iListWidth.iVal<0)
		{
			m_iListWidth.iVal=0;
			m_LastPoint.x=rect.Width()-3;
		}
		if(m_iListWidth.iVal>rect.Width()-5)
		{
			m_iListWidth.iVal=rect.Width()-5;
			m_LastPoint.x=3;
		}

		ResizeClient();
		UpdateWindow();
	}
	else
	{
		if(m_rDivider.PtInRect(point))
			SetCursor(m_hCursorDivSize);
		//else
			////SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
	}
	CView::OnMouseMove(nFlags, point);
}

void CSpecView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	m_ClickPoint=point;
	m_LastPoint=point;

	if(m_rDivider.PtInRect(point))
	{
		SetCapture();
		SetCursor(m_hCursorDivSize);
		m_bWasLButtonDown=TRUE;
	}
	CView::OnLButtonUp(nFlags, point);
}

void CSpecView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(m_rDivider.PtInRect(point)&&m_rDivider.PtInRect(m_ClickPoint) && 
		(abs(m_ClickPoint.x-point.x)<=1) && (abs(m_ClickPoint.y-point.y)<=1))
	{
		m_bShowList=FALSE;
		ResizeClient();
		UpdateWindow();
	}
	m_bWasLButtonDown=FALSE;
	ReleaseCapture();
	CView::OnLButtonDown(nFlags, point);
}

void CSpecView::OnViewMasses() 
{
	m_bShowList=!(BOOL)m_bShowList;
	RECT rect={50,50,200,200};
	ResizeClient();
	UpdateWindow();

}

void CSpecView::OnUpdateViewMasses(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_bShowList);
}

void CSpecView::ResizeClient(int x, int y)
{
	//Взять размеры клиентской области автоматически
	if(x==-1 && y==-1)
	{
		RECT rect;
		GetClientRect(&rect);
		x=rect.right;
		y=rect.bottom;
	}
	if(m_DisplayMode==eThermoChart)
	{
		m_pCalibDlg->ShowWindow(SW_HIDE);
		m_pList->ShowWindow(SW_HIDE);
		m_rDivider.right=x;
		m_rDivider.left=m_rDivider.right;
	}
	else if(m_DisplayMode==eMassCalib)
	{
		m_rDivider.right=x-m_iListWidth.iVal;
		m_rDivider.left=m_rDivider.right-5;
		m_rDivider.top=0;
		m_rDivider.bottom=y;
		m_pList->ShowWindow(SW_HIDE);
		m_CalibDlgRect.left=m_rDivider.right;
		m_CalibDlgRect.top=0;
		m_CalibDlgRect.right=x;
		m_CalibDlgRect.bottom=y;
		m_pCalibDlg->MoveWindow(m_CalibDlgRect);
		m_pCalibDlg->ShowWindow(SW_SHOW);
		DrawDivider();
		UpdateWindow();
	}
	else if((BOOL)m_bShowList)
	{
		m_pCalibDlg->ShowWindow(SW_HIDE);
		m_rDivider.right=x-m_iListWidth.iVal;
		m_rDivider.left=m_rDivider.right-5;
		m_rDivider.top=0;
		m_rDivider.bottom=y;
		m_pList->ShowWindow(SW_SHOW);
		//m_pList->SetFocus();
		m_pList->MoveWindow(m_rDivider.right,0,x-m_rDivider.right,y);
		DrawDivider();
	}
	else
	{
		m_pCalibDlg->ShowWindow(SW_HIDE);
		m_pList->ShowWindow(SW_HIDE);
		m_rDivider.right=x;
		m_rDivider.left=m_rDivider.right;
		//m_pButtonOK->ShowWindow(SW_HIDE);
	}
	m_pChart->MoveWindow(0,0,m_rDivider.left,y);
}

void CSpecView::DrawDivider(CDC *pDC)
{
	if(pDC==NULL)
		pDC=GetDC();
	if(m_rDivider.Width()>0)
	{
		CBrush NewBrush(GetSysColor(COLOR_BTNFACE));
		CPen NewPen(PS_SOLID,1,GetSysColor(COLOR_BTNFACE));
		CBrush *pOldBrush=pDC->SelectObject(&NewBrush);
		CPen *pOldPen=pDC->SelectObject(&NewPen);

		pDC->Rectangle(m_rDivider);
		m_rDivider.bottom+=1;m_rDivider.top-=1;
		pDC->Draw3dRect(m_rDivider.left,m_rDivider.top-1,m_rDivider.Width(),m_rDivider.Height()+2,GetSysColor(COLOR_3DHILIGHT),GetSysColor(COLOR_BTNSHADOW));
		pDC->SelectObject(pOldBrush);
		pDC->SelectObject(pOldPen);
	}

}

BOOL CSpecView::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	int wNotifyCode = HIWORD(wParam); // notification code 
	int wID = LOWORD(wParam);         // item, control, or accelerator identifier 
	HWND hwndCtl = (HWND) lParam;      // handle of control
	int a=0;
	switch (wID)
	{
	case ID_CHART:
		break;
	case ID_LIST:
		break;
	}
//MessageBeep(MB_ICONQUESTION);

	return CView::OnCommand(wParam, lParam);
}

void CSpecView::OnSettingsCalibration() 
{
	if(m_DisplayMode!=eMassCalib)
	{
		m_OldDispMode=m_DisplayMode;
		m_DisplayMode=eMassCalib;
		m_pCalibDlg->FormatChartToCalib();
		m_pCalibDlg->RedrawChart();
		ResizeClient();
	}
	else
	{
		OnButtonCalibOK();
	}
}

void CSpecView::OnButtonCalibOK()
{
	if(m_OldDispMode!=eMassCalib)
		m_DisplayMode=m_OldDispMode;
	else
		m_DisplayMode=eMassSpec;
	ResizeClient();
	if(m_DisplayMode==eMassSpec)
	{
		FormatChartToMassHist();
		RedrawChart();
	}
	else
	{
		m_ThermoReg.m_Hist.FormatChartToThermoHist();
		m_ThermoReg.m_Hist.RedrawChart();
	}
}



BOOL CSpecView::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	NMHDR *pnmh = (LPNMHDR) lParam;
	if(pnmh->code==NM_CLICK)//||pnmh->code==NM_SETFOCUS
	{
		LPNMITEMACTIVATE lpnmitem = (LPNMITEMACTIVATE) pnmh;
		if(lpnmitem->iItem<0 && lpnmitem->iSubItem>=0)
		{	int Items=m_pList->GetItemCount();
			m_pList->InsertItem(Items,"");
			m_pList->SetItemState(Items, LVIS_FOCUSED|LVIS_SELECTED, LVIS_FOCUSED|LVIS_SELECTED); 
			{
				CMassTable EmptyRow;
				EmptyRow.Checked=false;
				EmptyRow.Mass=0;
				memset(EmptyRow.Comments,0,sizeof(EmptyRow.Comments));
				GetDocument()->m_MassTable.insert(GetDocument()->m_MassTable.begin()+Items,EmptyRow);
			}

		
		}
	}
	if(pnmh->code==LVN_KEYDOWN)
	{
		LV_KEYDOWN* pLVKeyDown = (LV_KEYDOWN*)lParam;

		if((pLVKeyDown->wVKey==VK_DELETE) && !m_pList->IsUnderEdit() &&m_pList->GetEnableInput())
		{
			for(int i=0; i<m_pList->GetItemCount(); i++)
				if((m_pList->GetItemStates(i)&(RC_ITEM_FOCUSED|RC_ITEM_SELECTED))==(RC_ITEM_FOCUSED|RC_ITEM_SELECTED))
				{
					char EntryContents[256];
					m_pList->GetItemText(i,0,EntryContents,255);
					if(atoi(EntryContents)>0)
						GetDocument()->SetModifiedFlag();
					m_pList->DeleteItem(i);
					GetDocument()->m_MassTable.erase(GetDocument()->m_MassTable.begin()+i);
					break;
				}
		}

		if((pLVKeyDown->wVKey==VK_INSERT) && !m_pList->IsUnderEdit() &&m_pList->GetEnableInput())
		{
			for(int i=0; i<m_pList->GetItemCount(); i++)
				if((m_pList->GetItemStates(i)&(RC_ITEM_FOCUSED|RC_ITEM_SELECTED))==(RC_ITEM_FOCUSED|RC_ITEM_SELECTED))
				{
					m_pList->InsertItem(i,"");
					m_pList->SetItemStates(i+1,RC_ITEM_UNSELECTED);
					m_pList->SetItemStates(i,RC_ITEM_FOCUSED|RC_ITEM_SELECTED);
					{
						CMassTable EmptyRow;
						EmptyRow.Checked=false;
						EmptyRow.Mass=0;
						memset(EmptyRow.Comments,0,sizeof(EmptyRow.Comments));
						int size=GetDocument()->m_MassTable.size();
						GetDocument()->m_MassTable.insert(GetDocument()->m_MassTable.begin()+i,EmptyRow);
					}
					break;
				}
		}
		*pResult = 0;
	}
	if(pnmh->code==HDN_ENDTRACK)
	{
		LVCOLUMN col;
		col.mask = LVCF_WIDTH;
		if (m_pList->GetColumn(0, &col))
		   m_iColumn0Width=max(5,col.cx);
		if (m_pList->GetColumn(1, &col))
		   m_iColumn1Width=max(5,col.cx);
	}
//LogFileFormat("hwndFrom:0x%X, idFrom:%i, code:%i", pnmh->hwndFrom, pnmh->idFrom, pnmh->code);
	return CView::OnNotify(wParam, lParam, pResult);
}

LRESULT CSpecView::OnListCheckBox(WPARAM WParam, LPARAM LParam)
{
	int ItemIndex=WParam;
	char EntryContents[256];
	CMassTable Row=GetDocument()->m_MassTable[ItemIndex];
	if(Row.Mass<=0)
		return (LRESULT)0;
	m_pList->GetItemText(ItemIndex,0,EntryContents,255);
	ASSERT((int)Row.Mass==(int)atof(EntryContents)); // Должны быть равны
	Row.Checked=m_pList->GetItemStates(ItemIndex)&RC_ITEM_CHECKED;
	GetDocument()->m_MassTable[ItemIndex]=Row;
	GetDocument()->SetModifiedFlag();
	return (LRESULT)0;
}
 
LRESULT CSpecView::OnListItemSorted(WPARAM WParam, LPARAM LParam)
{
//MessageBeep(MB_ICONQUESTION);
 return (LRESULT)0;
}
 
LRESULT CSpecView::OnListEditCommited(WPARAM WParam, LPARAM LParam)
{
	int ItemIndex=WParam;
	int ColumnIndex=LParam;

	char EntryContents[256];
	if(ColumnIndex>0)
		return (LRESULT)0;

	while(GetDocument()->m_MassTable.size()<=ItemIndex)
	{
		CMassTable EmptyRow;
		EmptyRow.Checked=false;
		EmptyRow.Mass=0;
		memset(EmptyRow.Comments,0,sizeof(EmptyRow.Comments));
		GetDocument()->m_MassTable.push_back(EmptyRow);
	}
	GetDocument()->m_MassTable[ItemIndex].Checked=m_pList->GetItemStates(ItemIndex)&RC_ITEM_CHECKED;
	m_pList->GetItemText(ItemIndex,0,EntryContents,255);
	GetDocument()->m_MassTable[ItemIndex].Mass=atof(EntryContents);
	m_pList->GetItemText(ItemIndex,1,GetDocument()->m_MassTable[ItemIndex].Comments,63);
	m_pList->SetItemText(ItemIndex, ColumnIndex, PrintfFTruncate(GetDocument()->
		m_MassTable[ItemIndex].Mass,2));

	GetDocument()->SetModifiedFlag();
	return (LRESULT)0;
} 

void CSpecView::OnTimer(UINT nIDEvent) 
{
	m_ThermoReg.OnTimer(nIDEvent);
	CMassSpecDoc *pDoc = GetDocument();
	bool bIsValidReg=false;
	if(pDoc->IsEmpty())
	{
		size_t RegIndex; 
		for(RegIndex=0; RegIndex<pDoc->m_MassTable.size(); RegIndex++)
		{
			if(!pDoc->m_MassTable[RegIndex].Checked || pDoc->m_MassTable[RegIndex].Mass<=0)
				continue;
			bIsValidReg=true;
			break;
		}
	}
	((CMainFrame*)theApp.m_pMainWnd)->m_wndToolBarTR.GetToolBarCtrl().MarkButton(
		ID_TR_SYNC_MSP, !bIsValidReg);
	
	CView::OnTimer(nIDEvent);
}

void CSpecView::OnInitialUpdate() 
{
	CView::OnInitialUpdate();
	if(IsWindow(theApp.m_pMainWnd->GetSafeHwnd()))
		GetDocument()->SetTitle(NULL);

	FillTable();
	m_OldDispMode=m_DisplayMode=eMassSpec;
	ResizeClient();
	FormatChartToMassHist();
	RedrawChart();
}

void CSpecView::OnToolBarStart() 
{
	if(!m_bMeasureStart)
	{
		if(m_DisplayMode==eMassCalib)
		{
			m_pCalibDlg->StartCalib();
			m_bMeasureStart=TRUE;
		}
		else //if(m_DisplayMode==eMassSpec)

		{
			CMassSpecDoc *pDoc = GetDocument();
			if(pDoc->IsModified() && !pDoc->IsEmpty())
			{
				pDoc->SaveDefData();
				theApp.m_pMainWnd->SendMessage(WM_COMMAND,ID_FILE_NEW,NULL);
			}
			if(pDoc->IsEmpty())
			{
				bool bIsValidReg=false;
				size_t RegIndex; 
				for(RegIndex=0; RegIndex<pDoc->m_MassTable.size(); RegIndex++)
				{
					if(!pDoc->m_MassTable[RegIndex].Checked || pDoc->m_MassTable[RegIndex].Mass<=0)
						continue;
					bIsValidReg=true;
					break;
				}
				if(bIsValidReg)
				{
					m_pList->EnableInput(false);
					GetDocument()->m_bMeasurementInProgress=true;
					theApp.m_pThread=AfxBeginThread(MeasuringThread,this);
					m_bMeasureStart=TRUE;
				}
				else
					MessageBox("There is no Mass-Spectrometer data to measure.      ", "Spec", MB_OK|MB_ICONINFORMATION);
			}
		}
	}
}

void CSpecView::OnUpdateToolBarButtonStart(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_bMeasureStart);

	
}

void CSpecView::OnToolBarStop() 
{
	m_bMeasureStart=FALSE;
	m_pCalibDlg->StopCalib();
	m_pList->EnableInput(true);
}

void CSpecView::OnUpdateToolBarButtonStop(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(!m_bMeasureStart);
	
}

void CSpecView::FillTable()
{
	CMassSpecDoc *pDoc=GetDocument();
	m_pList->DeleteAllItems();
	for(int i=0; i<pDoc->m_MassTable.size(); i++)
	{
		m_pList->InsertItem(i,PrintfFTruncate(pDoc->m_MassTable[i].Mass,2));
		m_pList->SetCheck(i, pDoc->m_MassTable[i].Checked);
		m_pList->SetItemText(i,1,pDoc->m_MassTable[i].Comments);
	}

}

void CSpecView::OnDestroy() 
{
	CView::OnDestroy();

}

void CSpecView::OnSettingsAppearance() 
{
	CAppearSetupDlg dlg;
	if(dlg.DoModal() == IDOK)
		switch(m_DisplayMode)
		{
		case eMassSpec:
			FormatChartToMassHist();
			RedrawChart();
			break;
		case eMassCalib:
			m_pCalibDlg->FormatChartToCalib();
			m_pCalibDlg->RedrawChart();
			break;
		case eThermoChart:
			m_ThermoReg.m_Hist.FormatChartToThermoHist();
			m_ThermoReg.m_Hist.RedrawChart();
		}
}

void CSpecView::RedrawChart()
{	
	CMassSpecDoc* pDoc=GetDocument();
	m_pChart->RemoveAllSeries();
	for(size_t RegIndex=0; RegIndex<pDoc->m_MassTable.size(); RegIndex++)
		pDoc->m_MassTable[RegIndex].bIntensityMaxChanged=true;
	FinishDrawMassChart();
}

void CSpecView::FormatChartToMassHist()
{
	m_pChart->RemoveAllCursors();
	m_pChart->RemoveAllSeries();
	m_pChart->GetTitle()->RemoveAll();
	m_pChart->GetBottomAxis()->SetMinMax(0,30);
	m_pChart->GetBottomAxis()->SetAutomaticMode(EXPAND_ONLY_ON,0,10.);
	m_pChart->GetBottomAxis()->SetAutomaticReduceLimits(false);
	m_pChart->GetBottomAxis()->SetAutomatic(true);
	m_pChart->GetBottomAxis()->SetTimeFormat(true,"M:ss.U");
	m_pChart->GetLeftAxis()->SetAutomatic(false);
	m_pChart->GetLeftAxis()->SetMinMax(0,100.);
	m_pChart->GetLeftAxis()->SetAutomaticReduceLimits(false);
	m_pChart->GetLeftAxis()->GetLabel()->SetText("");
	m_pChart->GetLegend()->SetVisible(true);
	m_pChart->GetLegend()->SetFrameVisible(false);
	m_pChart->GetLegend()->SetAutoPosition(false);
	m_pChart->GetLegend()->SetUserPosition(CPoint(40,20));
	m_pChart->GetLegend()->SetFont(100,"Microsoft Sans Serif");
	m_pChart->GetLeftAxis()->GetLabel()->SetText("");
	m_pChart->GetRightAxis()->SetVisible(false);
	m_pChart->RefreshCtrl();

}

void CSpecView::OnViewMassChart() 
{
	if(m_DisplayMode==eThermoChart)
	{
		DispMode Old=m_OldDispMode;
		m_OldDispMode=m_DisplayMode;
		m_DisplayMode=Old;
		if(m_DisplayMode==eThermoChart)
			m_DisplayMode=eMassSpec;
		ResizeClient();
		if(m_DisplayMode==eMassSpec)
		{
			FormatChartToMassHist();
			RedrawChart();
		}
		else if(m_DisplayMode==eMassCalib)
		{
			m_pCalibDlg->FormatChartToCalib();
			m_pCalibDlg->RedrawChart();
		}

	}
}

void CSpecView::OnUpdateViewMassChart(CCmdUI* pCmdUI) 
{	pCmdUI->SetCheck(m_DisplayMode!=eThermoChart);}

void CSpecView::OnViewTemperatureChart() 
{
	m_OldDispMode=m_DisplayMode;
	m_DisplayMode=eThermoChart;
	ResizeClient();
	m_ThermoReg.m_Hist.FormatChartToThermoHist();
	m_ThermoReg.m_Hist.RedrawChart();
}

void CSpecView::OnUpdateViewTemperatureChart(CCmdUI* pCmdUI) 
{	pCmdUI->SetCheck(m_DisplayMode==eThermoChart);}


LRESULT CSpecView::OnMeasThreadMessage(WPARAM WParam, LPARAM LParam)
{
	WORD DataType = HIWORD(WParam);
	WORD RegIndex = LOWORD(WParam);
	CMassSpecDoc *pDoc=GetDocument();
	static double LastTime=0;
	switch(DataType)
	{
	case MTHR_REGION_DATA:
		{
		double Voltage=LParam*4.096/0x3FFF;
		if(RegIndex>=pDoc->m_MassTable.size())
			return (LRESULT)0;
		if(pDoc->m_MassTable[RegIndex].vIntensity.empty())
		{
			pDoc->m_MassTable[RegIndex].IntensityMax=Voltage*2;
			pDoc->m_MassTable[RegIndex].bIntensityMaxChanged=true;
		}
		else if(Voltage>pDoc->m_MassTable[RegIndex].IntensityMax)
		{
			pDoc->m_MassTable[RegIndex].IntensityMax=Voltage;
			pDoc->m_MassTable[RegIndex].bIntensityMaxChanged=true;
		}
		pDoc->m_MassTable[RegIndex].vIntensity.push_back(Voltage);
		pDoc->m_MassTable[RegIndex].vTime.push_back(LastTime);
		pDoc->SetModifiedFlag();
		UpdateView();
		break;
		}
	case MTHR_REGIONLIST_BEGIN:
		pDoc->m_PassedCommonTime = LParam;
		LastTime = LParam*0.001;
		pDoc->m_vRefTemp.push_back(m_ThermoReg.m_Hist.LastRefTemp);
		pDoc->m_vMeasTemp.push_back(m_ThermoReg.m_Hist.LastMeasTemp);
		pDoc->SetModifiedFlag();
		((CMainFrame*)theApp.m_pMainWnd)->SetStatusRemainTime((pDoc->m_ScanTime-pDoc->m_PassedCommonTime)*0.001);
		break;
	case MTHR_REGIONLIST_END:
		break;
	case MTHR_THREAD_END:
		m_pList->EnableInput();
		((CMainFrame*)theApp.m_pMainWnd)->SetStatusRemainTime(0);
		break;

	}

	return (LRESULT)0;
}

void CSpecView::OnTrStart() 
{m_ThermoReg.m_Commands.CommandStart();}

void CSpecView::OnTrWait() 
{m_ThermoReg.m_Commands.CommandWait();}

void CSpecView::OnTrWaitCont() 
{m_ThermoReg.m_Commands.CommandWaitContinue();}

void CSpecView::OnTrContinue() 
{m_ThermoReg.m_Commands.CommandContinue();}

void CSpecView::OnTrMainOff() 
{m_ThermoReg.m_Commands.CommandMainOff();}

void CSpecView::OnTrPrime() 
{m_ThermoReg.m_Commands.CommandPrime();}

void CSpecView::OnUpdateTrMainoff(CCmdUI* pCmdUI) 
{pCmdUI->SetCheck(m_ThermoReg.m_Commands.State==CmdMainOff);}

void CSpecView::OnUpdateTrPrime(CCmdUI* pCmdUI) 
{pCmdUI->SetCheck(m_ThermoReg.m_Commands.State==CmdPrime);}

void CSpecView::OnUpdateTrStart(CCmdUI* pCmdUI) 
{pCmdUI->SetCheck(m_ThermoReg.m_Commands.State==CmdStart);}

void CSpecView::OnUpdateTrWait(CCmdUI* pCmdUI) 
{pCmdUI->SetCheck(m_ThermoReg.m_Commands.WaitState);}

void CSpecView::OnUpdateTrWaitCont(CCmdUI* pCmdUI) 
{pCmdUI->SetCheck(m_ThermoReg.m_Commands.WaitState);}

LRESULT CSpecView::OnSerialEvent(WPARAM WParam, LPARAM LParam)
{return m_ThermoReg.OnSerialEvent(WParam, LParam);}


void CSpecView::OnUpdateEditCut(CCmdUI* pCmdUI) 
{pCmdUI->Enable(m_DisplayMode==eThermoChart);
}

void CSpecView::OnEditCut() 
{m_ThermoReg.m_Hist.OnClearHistory();}

void CSpecView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	UpdateView();
}

void CSpecView::FinishDrawMassChart()
{
	CMassSpecDoc* pDoc=GetDocument();
	int ChartIndex=0;
	for(int RegIndex=0; RegIndex<pDoc->m_MassTable.size();RegIndex++)
	{
		if(pDoc->m_MassTable[RegIndex].vTime.size()==0 || 
			!pDoc->m_MassTable[RegIndex].Checked)
			continue;
		char str[50];
		CChartSerie* pSerie;
		if(ChartIndex<m_pChart->GetSeriesCount())
			pSerie=m_pChart->GetSerie(ChartIndex);
		else
		{
			pSerie=m_pChart->AddSerie(CChartSerie::stLinePointsSerie);
			pSerie->m_bDrawLegendGalo=true;
		}
		if(pDoc->m_MassTable[RegIndex].bIntensityMaxChanged)
			pSerie->ClearSerie();
		int DrawnCount=pSerie->GetPointsCount();
		if((int)pDoc->m_MassTable[RegIndex].vTime.size()-DrawnCount>0 && (int)pDoc->m_MassTable[RegIndex].vIntensity.size()-DrawnCount>0)
		{
			double *Intensity=new double[pDoc->m_MassTable[RegIndex].vIntensity.size()-DrawnCount];
			for(size_t i=0;i<pDoc->m_MassTable[RegIndex].vTime.size()-DrawnCount;i++)
			{
				Intensity[i]=pDoc->m_MassTable[RegIndex].vIntensity[i+DrawnCount]*100./pDoc->m_MassTable[RegIndex].IntensityMax;
			}
			int s=pDoc->m_MassTable[RegIndex].vTime.size();
			if(pDoc->m_MassTable[RegIndex].vTime.size()-DrawnCount==1)
				pSerie->AddPoint(pDoc->m_MassTable[RegIndex].vTime.back(),*Intensity);
			else
				pSerie->AddPoints(&pDoc->m_MassTable[RegIndex].vTime[DrawnCount],Intensity,pDoc->m_MassTable[RegIndex].vTime.size()-DrawnCount);
			delete[] Intensity;
			if(pDoc->m_MassTable[RegIndex].bIntensityMaxChanged && pDoc->m_MassTable[RegIndex].Checked)
			{
				sprintf(str,"Mass %s, 100%%=%.2f", PrintfFTruncate(
					pDoc->m_MassTable[RegIndex].Mass,2),pDoc->m_MassTable[RegIndex].IntensityMax);
				pSerie->SetName(str);
				pDoc->m_MassTable[RegIndex].bIntensityMaxChanged=false;
			}
		}
		ChartIndex++;
	}
	m_pChart->RefreshCtrl();
}

void CSpecView::UpdateView()
{
	if(m_DisplayMode==eMassSpec)
	{
		FinishDrawMassChart();
	}	
}

void CSpecView::OnUpdateSettingsCalibration(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!GetDocument()->m_bMeasurementInProgress);
	pCmdUI->SetCheck(m_DisplayMode==eMassCalib);
}

void CSpecView::OnSettingsHeaterSetup() 
{
	m_pHeaterSetupDlg->Create(CHeaterSetupDlg::IDD);
	m_pHeaterSetupDlg->ShowWindow(SW_SHOWNORMAL);	
}

void CSpecView::OnUpdateSettingsHeaterSetup(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!m_pHeaterSetupDlg->m_bExist);
	
}

void CSpecView::OnEditCopy() 
{
	if(m_DisplayMode == eMassSpec)
		GetDocument()->CopyToClipboard();
	else if(m_DisplayMode == eMassCalib)
		m_pCalibDlg->CopyToClipboard();
	else if(m_DisplayMode == eThermoChart)
		m_ThermoReg.m_Hist.CopyToClipboard();
}

void CSpecView::OnUpdateEditCopy(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
}

void CSpecView::OnFileSaveasEasyplot() 
{
	if(m_DisplayMode != eMassSpec && m_DisplayMode != eMassCalib)
		return;
	char fullpath[260]="*.epx"; char dir[256]; char ext[4];
	const char * const strMassSpecTitle="Save as...";
	const char * const strMassCalibTitle="Save Test Chart as...";
	sprintf(dir, (LPCSTR)theApp.Ini.EasyPlotDir);
	OPENFILENAME ofn;
	memset(&ofn, 0, sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = theApp.m_pMainWnd->m_hWnd;
	ofn.hInstance = AfxGetInstanceHandle();
	ofn.lpstrFilter = "EasyPlot files (*.epx)\0*.epx\0All files (*.*)\0*.*\0";
	sprintf(ext, "epx");
	ofn.lpstrDefExt = ext;
	ofn.nFilterIndex = 1;
	ofn.lpstrFile = fullpath; //filename;
	ofn.nMaxFile = sizeof(fullpath);
	ofn.lpstrFileTitle = NULL; //out: file.ext
	//ofn.nMaxFileTitle = sizeof(filename);
	ofn.lpstrInitialDir = dir;
	ofn.lpstrTitle = (m_DisplayMode == eMassSpec)?strMassSpecTitle:strMassCalibTitle;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT | OFN_HIDEREADONLY | OFN_EXPLORER;

	int Result;
	Result = ::GetSaveFileName(&ofn);
	if(Result)
	{
		CString Path=ofn.lpstrFile;
		theApp.Ini.EasyPlotDir=Path.Left(Path.ReverseFind('\\'));
		if(m_DisplayMode == eMassSpec)
			GetDocument()->SaveEasyPlot(ofn.lpstrFile);
		else if (m_DisplayMode == eMassCalib)
			m_pCalibDlg->SaveEasyPlot(ofn.lpstrFile);
	}
}

void CSpecView::OnUpdateTrSyncDxps(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(theApp.Ini.SyncStartDxps);	
}

void CSpecView::OnUpdateTrSyncMsp(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(theApp.Ini.SyncStartMsp);	
}

// Нажатие кнопки на панели инструментов "Sync with DXPS"
void CSpecView::OnTrSyncDxps() 
{
	theApp.Ini.SyncStartDxps = !theApp.Ini.SyncStartDxps;
}

// Нажатие кнопки на панели инструментов "Sync with MassSpectrometer"
void CSpecView::OnTrSyncMsp() 
{
	theApp.Ini.SyncStartMsp = !theApp.Ini.SyncStartMsp;	
}

// Прием сообщения о состоянии готовности DXPS из Kratos (через CMainFrame)
LRESULT CSpecView::OnRemoteDxpsState(WPARAM WParam, LPARAM LParam)
{return m_ThermoReg.OnRemoteDxpsState(WParam, LParam);}

void CSpecView::OnSettingsMeasuringOptions() 
{
	CMeasuringOptDlg dlg;
	if(dlg.DoModal() == IDOK)
	{
		if(m_DisplayMode == eMassCalib)
		{
			m_pChart->GetBottomAxis()->SetMinMax(theApp.Ini.CalibMassStart,theApp.Ini.CalibMassStop);
			m_pChart->RefreshCtrl();
		}
		else if(m_DisplayMode == eMassSpec)
		{
			FormatChartToMassHist();
			UpdateView();
		}

	}
}

void CSpecView::OnSettingsHardwareSetup() 
{
	CHardSetupDlg dlg;
	dlg.DoModal();	
}

void CSpecView::OnSettingsAdcsdacs()
{
	CAdcDacDlg dlg;
	dlg.DoModal();
}
