// CalibDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Spec.h"
#include "CalibDlg.h"
#include "Approx.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#include "SpecView.h"
#include "HSChart\\ChartCtrl.h"
extern CSpecApp theApp;
#include "Threads.h"
#include "InspectDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CCalibDlg dialog


CCalibDlg::CCalibDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCalibDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCalibDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_bMeasureStart=false;
}


void CCalibDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCalibDlg)
	DDX_Control(pDX, IDC_BUTTON_CALIB_INSPECT, m_ButtonInspect);
	DDX_Control(pDX, IDC_BUTTON_CALIB_CANCEL, m_ButtonCancel);
	DDX_Control(pDX, IDC_BUTTON_CALIB_OK, m_ButtonOK);
	DDX_Control(pDX, IDC_LIST_CALIB, m_List);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCalibDlg, CDialog)
	//{{AFX_MSG_MAP(CCalibDlg)
	ON_BN_CLICKED(IDC_BUTTON_CALIB_OK, OnButtonCalibOk)
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_NOTIFY(NM_CLICK, IDC_LIST_CALIB, OnClickListCalib)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_BUTTON_CALIB_CANCEL, OnButtonCancel)
	ON_NOTIFY(LVN_KEYDOWN, IDC_LIST_CALIB, OnKeydownListCalib)
	ON_BN_CLICKED(IDC_BUTTON_CALIB_INSPECT, OnButtonCalibInspect)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_ON_CHKBOX, OnListCheckBox)
	ON_MESSAGE(WM_NEW_CALIB_MSG, OnNewMsg)
	ON_MESSAGE(WM_EDIT_COMMITTED, OnListEditCommited)

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCalibDlg message handlers

void CCalibDlg::OnButtonCalibOk() 
{
	CMassCalibDoc *pDoc=theApp.m_pMassCalibDoc;
	pDoc->m_MassTable.clear();
	char EntryContents[256];
	double Mass[200], Voltage[200];
	int CalibPointsCount=0;
	CMassTable Row;
	memset(Row.Comments,0,sizeof(Row.Comments));
	for(int i=0; i<m_List.GetItemCount(); i++)
	{
		m_List.GetItemText(i,0,EntryContents,255);
		Row.Mass=atof(EntryContents);
		if(Row.Mass<=0)
			continue;
		Row.Checked=(bool)(m_List.GetItemStates(i)&RC_ITEM_CHECKED);
		pDoc->m_MassTable.push_back(Row);
		CChartCursor* pCursor=(CChartCursor*)m_List.GetItemData(i);
		if(Row.Checked && pCursor)
		{
			Mass[CalibPointsCount]=Row.Mass;
			Voltage[CalibPointsCount]=pDoc->VoltageByMass(pCursor->GetCoord());
			CalibPointsCount++;
		}
	}
	double Divisor;
	switch(CalibPointsCount)
	{
	case 0:
	case 1:
		MessageBox("Too few points for calibration.          ", "Error",MB_OK|MB_ICONEXCLAMATION);
		break;
	case 2:
		Divisor=Mass[0]-Mass[1];
		if(Divisor==0)
		{
			MessageBox("Calibration impossible due to duplicate masses.       ", "Error",MB_OK|MB_ICONEXCLAMATION);
			break;
		}
		pDoc->c=0;
		pDoc->a=(Mass[0]*Voltage[1]-Mass[1]*Voltage[0])/Divisor;
		pDoc->b=(Voltage[0]-Voltage[1])/Divisor;
		break;
	default:
		ApproxParabolaMean(Mass,Voltage,CalibPointsCount,pDoc->a,pDoc->b,pDoc->c);
	}
	FillTable();
	pDoc->SaveTable();
	//theApp.m_pMassCalibDoc->Calibrate();
	//ShowWindow(SW_HIDE);
	//m_pSpecView->OnButtonCalibOK();
}

void CCalibDlg::OnSize(UINT nType, int cx, int cy) 
{
	//CRect OriginDlgRect(0, 0, 97, 260);
	//CRect OriginListRect(0, 0, 97, 205);
	CRect OriginSubListRect(1, 205, 97, 260);
	CRect OriginButtonOKRect(23, 208, 23+50, 208+14);
	CRect OriginButtonInspectRect(30, 225, 30+37, 225+14);
	CRect OriginButtonCancelRect(30, 242, 31+37, 242+14);

	MapDialogRect(&OriginSubListRect);
	MapDialogRect(&OriginButtonOKRect);
	MapDialogRect(&OriginButtonInspectRect);
	MapDialogRect(&OriginButtonCancelRect);


	m_ListRect.left=0; m_ListRect.top=0; m_ListRect.right=cx;
	m_ListRect.bottom=cy-OriginSubListRect.Height();
	if(::IsWindow(m_List.GetSafeHwnd()))
		m_List.MoveWindow(m_ListRect);

	m_ButtonOKRect.left=cx/2-OriginButtonOKRect.Width()/2;
	m_ButtonOKRect.top=cy-(OriginSubListRect.bottom-OriginButtonOKRect.top);
	
	if(::IsWindow(m_ButtonOK.GetSafeHwnd()))
		m_ButtonOK.SetWindowPos(NULL, m_ButtonOKRect.left, m_ButtonOKRect.top, 0, 0, SWP_NOSIZE|SWP_NOZORDER);

	if(::IsWindow(m_ButtonInspect.GetSafeHwnd()))
		m_ButtonInspect.SetWindowPos(NULL, cx/2-OriginButtonInspectRect.Width()/2, 
			cy-(OriginSubListRect.bottom-OriginButtonInspectRect.top), 0, 0, SWP_NOSIZE|SWP_NOZORDER);

	if(::IsWindow(m_ButtonCancel.GetSafeHwnd()))
		m_ButtonCancel.SetWindowPos(NULL, cx/2-OriginButtonCancelRect.Width()/2, 
			cy-(OriginSubListRect.bottom-OriginButtonCancelRect.top), 0, 0, SWP_NOSIZE|SWP_NOZORDER);
	UpdateWindow();
	
}


BOOL CCalibDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	m_List.SetExtendedStyle(LVS_EX_GRIDLINES);
	m_List.SetCheckboxeStyle();
	m_List.InsertColumn(0,"Mass", LVCFMT_LEFT, 100);
	//m_List.InsertItem(0,"1");
	//m_List.InsertItem(1,"28");
	//m_List.InsertItem(2,"40");
	m_List.SetEditable(TRUE,-1,0);

	m_List.ModifyStyle(0,WS_BORDER);
	m_List.ModifyStyleEx(WS_EX_STATICEDGE|WS_EX_CLIENTEDGE,0);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CCalibDlg::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	//if(m_bCalibration)
		//DrawDlgArea(m_CalibDlgRect, pDC);
	pDC->ExcludeClipRect(m_ListRect);
	return CDialog::OnEraseBkgnd(pDC);
}



void CCalibDlg::OnClickListCalib(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LPNMITEMACTIVATE lpnmitem = (LPNMITEMACTIVATE) pNMHDR;
	//MessageBeep(MB_ICONQUESTION);
	if(lpnmitem->iItem<0 && lpnmitem->iSubItem>=0)
	{	int Items=m_List.GetItemCount();
		m_List.InsertItem(Items,"");
		m_List.SetItemState(Items, LVIS_FOCUSED|LVIS_SELECTED, LVIS_FOCUSED|LVIS_SELECTED); 
	}

	*pResult = 0;
}

void CCalibDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	if(bShow)
	{
		FormatChartToCalib();
		FillTable();

		CDialog::OnShowWindow(bShow, nStatus);
	}
	else
	{
		CDialog::OnShowWindow(bShow, nStatus);
		m_pSpecView->m_pChart->RemoveAllCursors();
		m_List.DeleteAllItems();
	}
	//m_List.SetFocus();
	
}

void CCalibDlg::FillTable()
{
	CMassCalibDoc *pDoc=theApp.m_pMassCalibDoc;
	m_List.DeleteAllItems();
	m_pSpecView->m_pChart->RemoveAllCursors();
	for(int i=0; i<pDoc->m_MassTable.size(); i++)
	{
		m_List.InsertItem(i,PrintfFTruncate(pDoc->m_MassTable[i].Mass,2));
		m_List.SetCheck(i, pDoc->m_MassTable[i].Checked);
		if(pDoc->m_MassTable[i].Checked)
		{
			CChartCursor *pCursor=m_pSpecView->m_pChart->AddCursor();
			if(pCursor->GetColor()==RGB(255,0,0))
				pCursor->SetColor(RGB(150,0,0));
			if(pCursor->GetColor()==RGB(0,150,0))
				pCursor->SetColor(RGB(0,220,0));
			pCursor->SetCoord(pDoc->m_MassTable[i].Mass);
			m_List.SetItemData(i,(DWORD)pCursor);
			m_List.SetItemTextColor(i,0,pCursor->GetColor(),TRUE);
		}
		else
			m_List.SetItemData(i,0);
	}
	m_pSpecView->m_pChart->RefreshCtrl();

}

void CCalibDlg::OnButtonCancel() 
{
	StopCalib();
	ShowWindow(SW_HIDE);
	m_pSpecView->OnButtonCalibOK();
}

LRESULT CCalibDlg::OnListCheckBox(WPARAM WParam, LPARAM LParam)
{
	int nIndex=WParam, nMsg=LParam;
	//MessageBeep(MB_ICONQUESTION);
	if(m_List.GetItemStates(nIndex)&RC_ITEM_CHECKED)
	{
		char str[64];
		m_List.GetItemText(nIndex, 0, str, 63);
		if(atof(str)<=0)
			 return (LRESULT)0;
		CChartCursor *pCursor=m_pSpecView->m_pChart->AddCursor();
		if(pCursor->GetColor()==RGB(255,0,0))
			pCursor->SetColor(RGB(150,0,0));
		if(pCursor->GetColor()==RGB(0,150,0))
			pCursor->SetColor(RGB(0,220,0));
		pCursor->SetCoord(atof(str));
		m_List.SetItemData(nIndex,(DWORD)pCursor);
		m_List.SetItemTextColor(nIndex,0,pCursor->GetColor(),TRUE);

	}
	else
	{
		CChartCursor *pCursor=(CChartCursor *)m_List.GetItemData(nIndex);
		if(pCursor)
			m_pSpecView->m_pChart->RemoveCursorByPointer(pCursor);
		m_List.SetItemData(nIndex,0);
		m_List.SetItemTextColor(nIndex,0,RGB(0,0,0));
	}
	m_pSpecView->m_pChart->RefreshCtrl();
	return (LRESULT)0;
} 

void CCalibDlg::OnKeydownListCalib(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_KEYDOWN* pLVKeyDown = (LV_KEYDOWN*)pNMHDR;

	if((pLVKeyDown->wVKey==VK_DELETE) && !m_List.IsUnderEdit())
	{
		for(int i=0; i<m_List.GetItemCount(); i++)
			if((m_List.GetItemStates(i)&(RC_ITEM_FOCUSED|RC_ITEM_SELECTED))==(RC_ITEM_FOCUSED|RC_ITEM_SELECTED))
			{
				CChartCursor *pCursor=(CChartCursor *)m_List.GetItemData(i);
				if(pCursor)
					m_pSpecView->m_pChart->RemoveCursorByPointer(pCursor);

				m_List.DeleteItem(i);
				break;
			}
	}
	if((pLVKeyDown->wVKey==VK_INSERT) && !m_List.IsUnderEdit())
	{
		for(int i=0; i<m_List.GetItemCount(); i++)
			if((m_List.GetItemStates(i)&(RC_ITEM_FOCUSED|RC_ITEM_SELECTED))==(RC_ITEM_FOCUSED|RC_ITEM_SELECTED))
			{
				m_List.InsertItem(i,"");
				m_List.SetItemStates(i+1,RC_ITEM_UNSELECTED);
				m_List.SetItemStates(i,RC_ITEM_FOCUSED|RC_ITEM_SELECTED);
				break;
			}
	}

	*pResult = 0;
}

void CCalibDlg::OnOK()
{
	if(!m_List.IsUnderEdit())
		for(int i=0; i<m_List.GetItemCount(); i++)
			if((m_List.GetItemStates(i)&(RC_ITEM_FOCUSED|RC_ITEM_SELECTED))==(RC_ITEM_FOCUSED|RC_ITEM_SELECTED))
			{
				m_List.StartEdit(i,0);
				break;
			}
}

void CCalibDlg::OnCancel()
{
}

void CCalibDlg::StartCalib()
{
	m_vPointsCurrentPass.clear();
	m_vPointsLastPass.clear();
	if(m_pSpecView->m_DisplayMode==m_pSpecView->eMassCalib)
		RedrawChart();
	m_bMeasureStart=true;
	theApp.m_pThread=AfxBeginThread(CalibThread,this);
}



void CCalibDlg::FormatChartToCalib()
{
	m_pSpecView->m_pChart->RemoveAllSeries();
	m_pSpecView->m_pChart->GetTitle()->RemoveAll();
	m_pSpecView->m_pChart->GetTitle()->SetFont(11,"Arial",true);
	m_pSpecView->m_pChart->GetTitle()->AddString("MassSpectrometer Test");
	m_pSpecView->m_pChart->GetBottomAxis()->SetTimeFormat(false);
	m_pSpecView->m_pChart->GetBottomAxis()->SetAutomaticReduceLimits(false);
	m_pSpecView->m_pChart->GetBottomAxis()->SetAutomatic(false);
	m_pSpecView->m_pChart->GetBottomAxis()->SetMinMax(theApp.Ini.CalibMassStart,theApp.Ini.CalibMassStop);
	m_pSpecView->m_pChart->GetLeftAxis()->SetAutomatic(true);
	m_pSpecView->m_pChart->GetLeftAxis()->SetMinMax(0,1);
	m_pSpecView->m_pChart->GetLeftAxis()->SetAutomaticMode(EXPAND_ONLY_ON,-1,-1);
	m_pSpecView->m_pChart->GetLeftAxis()->GetLabel()->SetText("");
	m_pTSerie=(CChartTrapeziumSerie*)m_pSpecView->m_pChart->AddSerie(CChartSerie::stTrapeziumSerie);
	m_pLPSerie=(CChartLinePointsSerie*)m_pSpecView->m_pChart->AddSerie(CChartSerie::stLinePointsSerie);
	m_pLPSerie->m_bShowPoints=true;
	m_pLPSerie->SetPointSize(3,3);
	m_pSpecView->m_pChart->GetRightAxis()->SetVisible(false);

}

void CCalibDlg::StopCalib()
{
	m_bMeasureStart=false;
}

LRESULT CCalibDlg::OnNewMsg(WPARAM WParam, LPARAM LParam)
{
	CMassCalibMsg CalibMsg;
	if(WParam==1)
	{
		while(m_CalibMsgQueue.try_front_pop(CalibMsg))
		{
			if(CalibMsg.Action==CMassCalibMsg::eNewPoint)
			{
				CChartSerie::SChartPoint Pt={CalibMsg.Mass, CalibMsg.Voltage};
				m_vPointsCurrentPass.push_back(Pt);
				if(m_pSpecView->m_DisplayMode==m_pSpecView->eMassCalib)
				{
					m_pLPSerie->AddPoint(CalibMsg.Mass, CalibMsg.Voltage);
				}
			}
			else if(CalibMsg.Action==CMassCalibMsg::eEndSweep)
			{
				if(m_pSpecView->m_DisplayMode==m_pSpecView->eMassCalib)
				{
					m_pLPSerie->ClearSerie();
					m_pTSerie->SetPoints(m_vPointsCurrentPass);
					m_vPointsLastPass=m_vPointsCurrentPass;
					double Sum=0;
					double Min=1e300, Max=-1e300, Point;
					for(int i=0; i<m_vPointsCurrentPass.size(); i++)
					{
						Point=m_vPointsCurrentPass[i].Y;
						Sum+=Point;
						if(Point<Min)
							Min=Point;
						if(Point>Max)
							Max=Point;

					}
					if(m_vPointsCurrentPass.size()!=0)
					{
						Sum/=m_vPointsCurrentPass.size();
						double CurMin, CurMax;
						m_pSpecView->m_pChart->GetLeftAxis()->GetMinMax(CurMin, CurMax);
						if(((Sum*3.2<CurMax) || (2.8*Sum>CurMax)) && Sum*3>Max)
						{
							m_pSpecView->m_pChart->GetLeftAxis()->SetMinMax(Min,Sum*3);
						}
						else if((Max*1.2<CurMax) || (0.8*Min>CurMax))
							m_pSpecView->m_pChart->GetLeftAxis()->SetMinMax(Min,Max);

					}
					m_vPointsCurrentPass.clear();
					m_pSpecView->m_pChart->RefreshCtrl();
				}
			}
		}
	}
	else
	{
		m_pSpecView->m_bMeasureStart=false;
		m_bMeasureStart=false;
	}
	return 0;
}

void CCalibDlg::RedrawChart()
{
	m_pLPSerie->SetPoints(m_vPointsCurrentPass);
	m_pTSerie->SetPoints(m_vPointsLastPass);
	m_pSpecView->m_pChart->RefreshCtrl();

}

void CCalibDlg::CopyToClipboard()
{
    LPTSTR  lptstrCopy; 
    HGLOBAL hglbCopy; 
	CString sData=PrintEasyPlotData();

    if (!::OpenClipboard(theApp.m_pMainWnd->GetSafeHwnd())) 
        return; 
    EmptyClipboard();

	 // Allocate a global memory object for the text. 

    hglbCopy = GlobalAlloc(GMEM_MOVEABLE, 
        sData.GetLength()+1); 
    if (hglbCopy == NULL) 
    { 
        CloseClipboard(); 
        return; 
    } 

    // Lock the handle and copy the text to the buffer. 
    lptstrCopy = (char*)GlobalLock(hglbCopy); 
    memcpy(lptstrCopy, (LPCSTR)sData, 
       sData.GetLength()+1); 
    lptstrCopy[sData.GetLength()] = (TCHAR) 0;    // null character 
    GlobalUnlock(hglbCopy); 

    // Place the handle on the clipboard. 
    SetClipboardData(CF_TEXT, hglbCopy); 
    ::CloseClipboard(); 
}

CString CCalibDlg::PrintEasyPlotData()
{
	CString sData, str;
	//Добавление легенды
	//str.Format("/sa l \"%s\" %i\n",m_MassTable[i].Comments, Count);
	//sData+=str;

	str.Format("/td \"xy");
	sData+=str;

	str.Format("\"\n");
	sData+=str;
	std::vector<CChartSerie::SChartPoint>* pSeries;
	if(!m_vPointsLastPass.empty())
		pSeries = &m_vPointsLastPass;
	else
		pSeries = &m_vPointsCurrentPass;

	for (size_t i=0; i<pSeries->size(); i++)
	{
		str.Format("%.1f %.3f\n", (*pSeries)[i].X,(*pSeries)[i].Y);
		sData+=str;
	}
	return sData;
}

void CCalibDlg::SaveEasyPlot(char *FileName)
{
	FILE *fp=fopen(FileName,"w");
	CTime time = CTime::GetCurrentTime();
	fprintf(fp, ";File \"%s\", saved by \'Spec\' in %s\n", FileName, time.Format("%b %d, %Y at %H:%M:%S"));
	fprintf(fp, ";Mass sweep: %g - %g, deltaMass: %g, Setup delay: %g sec\n", theApp.Ini.CalibMassStart, 
		theApp.Ini.CalibMassStop, theApp.Ini.CalibMassStep, theApp.Ini.MassSetupDelay);
	fprintf(fp,(LPCSTR)PrintEasyPlotData());
	fclose(fp);

}

void CCalibDlg::OnButtonCalibInspect() 
{
	CInspectDlg dlg;
	dlg.DoModal();
}

LRESULT CCalibDlg::OnListEditCommited(WPARAM WParam, LPARAM LParam)
{
	int ItemIndex=WParam;
	int ColumnIndex=LParam;
	CMassCalibDoc *pDoc=theApp.m_pMassCalibDoc;

	char EntryContents[256];
	while(pDoc->m_MassTable.size()<=ItemIndex)
	{
		CMassTable EmptyRow;
		EmptyRow.Checked=false;
		EmptyRow.Mass=0;
		pDoc->m_MassTable.push_back(EmptyRow);
	}
	pDoc->m_MassTable[ItemIndex].Checked=m_List.GetItemStates(ItemIndex)&RC_ITEM_CHECKED;
	m_List.GetItemText(ItemIndex,0,EntryContents,255);
	pDoc->m_MassTable[ItemIndex].Mass=atof(EntryContents);
	m_List.SetItemText(ItemIndex, ColumnIndex, PrintfFTruncate(pDoc->m_MassTable[ItemIndex].Mass,2));
	CChartCursor *pCursor=(CChartCursor *)m_List.GetItemData(ItemIndex);
	if(pCursor)
	{
		pCursor->SetCoord(pDoc->m_MassTable[ItemIndex].Mass);
		m_pSpecView->m_pChart->RefreshCtrl();
	}

	return (LPARAM) 0;
}