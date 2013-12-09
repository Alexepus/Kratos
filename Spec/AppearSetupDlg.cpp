// AppearSetupDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Spec.h"
#include "Functions.h"
#include "MainFrame.h"
#include "SpecView.h"
#include "ThermoHistDoc.h"
#include "ThermoReg.h"
#include "AppearSetupDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAppearSetupDlg dialog


CAppearSetupDlg::CAppearSetupDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAppearSetupDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAppearSetupDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CAppearSetupDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAppearSetupDlg)
	DDX_Control(pDX, IDC_APPEAR_TREE, m_Tree);
	DDX_Control(pDX, IDC_DATETIMEPICKER_KEEP, m_KeepTime);
	DDX_Control(pDX, IDC_CHECK_SHOW_POWER, m_CheckShowPower);
	DDX_Control(pDX, IDC_BUTTON_SERIES1_COLOR, m_ButtonSeries1Color);
	DDX_Control(pDX, IDC_STATIC_SIZE, m_StaticSize1);
	DDX_Control(pDX, IDC_SLIDER_SERIES1_SIZE, m_SliderSeries1Size);
	DDX_Control(pDX, IDC_COMBO_SERIES1_TYPE, m_ComboSeries1Type);
	DDX_Control(pDX, IDC_COMBO_SERIES1_ELEMENT, m_ComboSeries1Element);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_APPEARANCE_CALIB_CHART, m_Chart);
}


BEGIN_MESSAGE_MAP(CAppearSetupDlg, CDialog)
	//{{AFX_MSG_MAP(CAppearSetupDlg)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_SERIES1_SIZE, OnCustomdrawSliderSeries1Size)
	ON_CBN_CLOSEUP(IDC_COMBO_SERIES1_ELEMENT, OnCloseupComboSeries1Element)
	ON_CBN_CLOSEUP(IDC_COMBO_SERIES1_TYPE, OnCloseupComboSeries1Type)
	ON_NOTIFY(TVN_SELCHANGED, IDC_APPEAR_TREE, OnSelchangedAppearTree)
	//}}AFX_MSG_MAP
	ON_MESSAGE(CPN_SELCHANGE, OnButtonSeries1Color)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAppearSetupDlg message handlers

BOOL CAppearSetupDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	m_Chart.SetEdgeType(BDR_RAISEDINNER);
	m_Chart.SetZoomEnabled(false);
	m_Chart.SetPanEnabled(false);

	m_Series1Type=1;
	m_Series1Width=2;
	m_Series1DotSize=5;
	m_Series1Color=RGB(255,0,0);
	m_Series1DotColor=RGB(255,0,0);
	m_Series2Type=3;
	m_Series2Width=1;
	m_Series2DotSize=5;
	m_Series2Color=RGB(0,0,255);
	m_Series2DotColor=RGB(0,0,255);

	double X[11]= {  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10};
	double Y1[11]={  1,1.5,1.8,  4,  5,  3,2.5,1.8,2.3,0.9,1.3};
	double Y2[11]={1.5,  1,  2,  5,4.5,  2,2.5,  1,1.3,0.2,1.8};
	m_Chart.AddSerie(CChartSerie::stTrapeziumSerie)->SetPoints(X,Y2,11);
	m_Chart.AddSerie(CChartSerie::stLinePointsSerie)->SetPoints(X,Y1,11);;
	m_SliderSeries1Size.SetRange(1,8,1);
	m_ComboSeries1Type.SetCurSel(1);

	HTREEITEM hItem=m_Tree.InsertItem("Test (Calibration)");
	HTREEITEM hFirstItem=hItem;
	m_Tree.InsertItem("Series New",hItem);
	m_Tree.InsertItem("Series Last",hItem);
	hItem=m_Tree.InsertItem("MSP Measure");
	m_Tree.InsertItem("All Series",hItem);
	hItem=m_Tree.InsertItem("Temperature");
	m_Tree.InsertItem("Ref. Temp",hItem);
	m_Tree.InsertItem("Meas. Temp",hItem);
	m_Tree.InsertItem("Power",hItem);
	m_Tree.SelectItem(hFirstItem);
	NM_TREEVIEW nmTree;
	nmTree.itemNew.hItem=hFirstItem;
	OnSelchangedAppearTree((NMHDR*)&nmTree,0);
	OnCloseupComboSeries1Type();
	
	int Sec,Min,Hour;
	Sec=theApp.Ini.ThermoHistKeepTime%60;
	Min=theApp.Ini.ThermoHistKeepTime/60%60;
	Hour=theApp.Ini.ThermoHistKeepTime/3600%24;
	m_KeepTime.SetTime(&CTime(2000,1,1,Hour,Min,Sec,0));
	m_CheckShowPower.SetCheck(theApp.Ini.ThermoHistShowPower);

	return TRUE;
}

void CAppearSetupDlg::OnOK() 
{
	theApp.Ini.ThermoHistShowPower=m_CheckShowPower.GetCheck();
	CTime time;
	m_KeepTime.GetTime(time);
	theApp.Ini.ThermoHistKeepTime=time.GetSecond()+time.GetMinute()*60+time.GetHour()*3600;
	if(VerifyGUIObject(theApp.m_pSpecView))
	{
		if(theApp.m_pSpecView->m_DisplayMode==CSpecView::eThermoChart)
		{
			theApp.m_pSpecView->m_ThermoReg.m_Hist.FormatChartToThermoHist();
			theApp.m_pSpecView->m_ThermoReg.m_Hist.RedrawChart();
		}
	}
	theApp.Ini.Save();
	CDialog::OnOK();
}

void CAppearSetupDlg::OnCustomdrawSliderSeries1Size(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CString str;
	m_Series1Width=m_SliderSeries1Size.GetPos();
	str.Format("%i", m_Series1Width);
	m_StaticSize1.SetWindowText((LPCSTR)str);
	m_Chart.GetSerie(1)->GetAsLinePoints()->SetWidth(m_Series1Width);
	m_Chart.RefreshCtrl();
	*pResult = 0;
}

void CAppearSetupDlg::OnCloseupComboSeries1Element() 
{
	// TODO: Add your control notification handler code here
	
}

void CAppearSetupDlg::OnCloseupComboSeries1Type() 
{
	m_Series1Type=m_ComboSeries1Type.GetCurSel();
	m_ComboSeries1Element.ResetContent();
	BOOL Enabled=m_ComboSeries1Type.IsWindowEnabled(); 
	switch(m_Series1Type)
	{
	case SERIES_TYPE_LINE_DOTS:
		m_ComboSeries1Element.InsertString(0,"Line");
		m_ComboSeries1Element.InsertString(1,"Dot");
		m_ComboSeries1Element.EnableWindow(Enabled);
		break;
	case SERIES_TYPE_LINE:
		m_ComboSeries1Element.InsertString(0,"Line");
		m_ComboSeries1Element.EnableWindow(FALSE);
		break;
	case SERIES_TYPE_DOTS:
		m_ComboSeries1Element.InsertString(0,"Dot");
		m_ComboSeries1Element.EnableWindow(FALSE);
		break;
	case SERIES_TYPE_TRAPEZIUM:
		m_ComboSeries1Element.InsertString(0,"Trapezium");
		m_ComboSeries1Element.EnableWindow(FALSE);
		break;

	}
	m_ComboSeries1Element.SetCurSel(0);
}

void CAppearSetupDlg::OnSelchangedAppearTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	HTREEITEM hParentItem=m_Tree.GetParentItem(pNMTreeView->itemNew.hItem);
	CString ItemText=m_Tree.GetItemText(pNMTreeView->itemNew.hItem);
	CString ParentItemText;
	if(hParentItem)
		ParentItemText=m_Tree.GetItemText(pNMTreeView->itemNew.hItem);
	if(ItemText == "Test (Calibration)")
	{
		EnableSeriesControls(FALSE);
	}
	else if(ItemText == "Series New")
	{
		EnableSeriesControls();
		LoadFromStruct(&theApp.Ini.CalibSeriesNew);
	}
	else if(ItemText == "Series Last")
	{
		EnableSeriesControls();
		LoadFromStruct(&theApp.Ini.CalibSeriesLast);
	}
	else if(ItemText == "MSP Measure")
	{
		EnableSeriesControls(FALSE);
	}
	else if(ItemText == "All Series")
	{
		EnableSeriesControls();
		LoadFromStruct(&theApp.Ini.MspSeriesAll);
	}
	else if(ItemText == "Temperature")
	{
		EnableSeriesControls(FALSE);
	}
	else if(ItemText == "Ref. Temp")
	{
		EnableSeriesControls();
	}
	else if(ItemText == "Meas. Temp")
	{
		EnableSeriesControls();
	}
	else if(ItemText == "Power")
	{
		EnableSeriesControls();
	}
	
}

void CAppearSetupDlg::EnableSeriesControls(BOOL Enable)
{
	m_ButtonSeries1Color.EnableWindow(Enable);
	m_StaticSize1.EnableWindow(Enable);
	m_SliderSeries1Size.EnableWindow(Enable);
	m_ComboSeries1Type.EnableWindow(Enable);
	m_ComboSeries1Element.EnableWindow(Enable);

}

LRESULT CAppearSetupDlg::OnButtonSeries1Color(WPARAM Color, LPARAM DlgId) 
{
	m_Chart.GetSerie(0)->SetColor(Color);
	return 0;
}

void CAppearSetupDlg::LoadFromStruct(CSeriesSetup *Opt)
{
	m_ComboSeries1Element.SetCurSel(Opt->PlotType);
	m_ButtonSeries1Color.SetColour(Opt->Color);
	m_SliderSeries1Size.SetPos(Opt->Size);
	OnCloseupComboSeries1Type();
}
