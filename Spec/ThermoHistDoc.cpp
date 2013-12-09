// ThermoData.cpp: implementation of the CThermoData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Spec.h"
#include "SpecView.h"
#include "ThermoHistDoc.h"
#include "MainFrame.h"
#include "Functions.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CThermoHistDoc::CThermoHistDoc()
{
	TheVeryFirstTime=GetTimeDouble();
}

CThermoHistDoc::~CThermoHistDoc()
{

}

CChartCtrl* CThermoHistDoc::GetChart()
{
	return m_pView->m_pChart;
}

BOOL CThermoHistDoc::IsThermoHistMode()
{
	return (m_pView->m_DisplayMode==m_pView->eThermoChart);
}

void CThermoHistDoc::FormatChartToThermoHist()
{
	CChartCtrl* pChart=GetChart();
	pChart->RemoveAllCursors();
	pChart->RemoveAllSeries();
	pChart->GetTitle()->SetFont(11,"Arial",true);
	pChart->GetTitle()->RemoveAll();
	pChart->GetTitle()->AddString(std::string("Temperature History"));
	pChart->GetBottomAxis()->SetMinMax(0,120);
	if(!RefTemp.empty())
		pChart->GetBottomAxis()->SetAutomaticReduceLimits(true,RefTemp.front().Time,RefTemp.front().Time+120.);
	else
		pChart->GetBottomAxis()->SetAutomaticReduceLimits(true,0,120);
	pChart->GetBottomAxis()->SetAutomaticMode(EXPAND_ONLY_OFF,0,30.);
	pChart->GetBottomAxis()->SetAutomatic(true);
	pChart->GetBottomAxis()->SetTimeFormat(true,"M:ss.UU");
	pChart->GetLeftAxis()->SetAutomatic(true);
	pChart->GetLeftAxis()->GetLabel()->SetFont(90,"Microsoft Sans Serif");
	pChart->GetLeftAxis()->GetLabel()->SetText("Temperature, °C");
	pChart->GetLeftAxis()->SetPixelOffsets(5,5);
	pChart->GetLegend()->SetVisible(true);
	pChart->GetLegend()->SetFrameVisible(false);
	pChart->GetLegend()->SetAutoPosition(false);
	pChart->GetLegend()->SetUserPosition(CPoint(60,40));
	pChart->GetLegend()->SetFont(100,"Microsoft Sans Serif");
	pChart->AddSerie(CChartSerie::stLinePointsSerie);
	pChart->GetSerie(0)->GetAsLinePoints()->m_bShowPoints=TRUE;
	pChart->GetSerie(0)->GetAsLinePoints()->SetPointSize(3,3);
	pChart->GetSerie(0)->GetAsLinePoints()->m_bShowLine=FALSE;
	pChart->GetSerie(0)->GetAsLinePoints()->m_PointsInnerColor=RGB(255,170,170);
	pChart->GetSerie(0)->SetName("Reference T");
	pChart->GetSerie(0)->m_bDrawLegendGalo=true;
	pChart->AddSerie(CChartSerie::stLinePointsSerie);
	pChart->GetSerie(1)->GetAsLinePoints()->m_bShowPoints=TRUE;
	pChart->GetSerie(1)->GetAsLinePoints()->SetPointSize(3,3);
	pChart->GetSerie(1)->GetAsLinePoints()->m_PointsInnerColor=RGB(0,225,0);
	pChart->GetSerie(1)->SetName("Measured T");
	pChart->GetSerie(1)->m_bDrawLegendGalo=true;
	if(theApp.Ini.ThermoHistShowPower)
	{
		pChart->GetRightAxis()->SetVisible(true);
		pChart->GetRightAxis()->SetAutomatic(true);
		pChart->GetRightAxis()->SetMinMax(0,5);
		pChart->GetRightAxis()->SetAutomaticReduceLimits(true,0,5);
		pChart->GetRightAxis()->GetLabel()->SetFont(90,"Microsoft Sans Serif");
		pChart->GetRightAxis()->GetLabel()->SetText("Power, W");
		pChart->GetRightAxis()->SetPixelOffsets(3,3);
		pChart->GetRightAxis()->GetGrid()->SetVisible(false);
		pChart->AddSerie(CChartSerie::stLinePointsSerie);
		pChart->GetSerie(2)->GetAsLinePoints()->m_bShowPoints=FALSE;
		pChart->GetSerie(2)->SetVerticalAxis(true);
		pChart->GetSerie(2)->SetColor(RGB(255,150,0));
		pChart->GetSerie(2)->SetName("Power");
		pChart->GetSerie(2)->m_bDrawLegendGalo=true;
	}
	else
		pChart->GetRightAxis()->SetVisible(false);
	if(pChart->GetZoomStatus())
		pChart->UndoZoom();
	pChart->RefreshCtrl();
}

void CThermoHistDoc::RecieveNewMessage()
{
	////// Obsolete - Уже не используется ////////
	CTempMessage Msg;
	CChartCtrl* pChart=GetChart();
	CMainFrame *pMainFrame=(CMainFrame*)theApp.m_pMainWnd;

	while(ThermoPlotQueue.try_front_pop(Msg))
	{
		if(Msg.PointType==CTempMessage::eRefTemp)
		{
			RefTemp.push_back(Msg.Pt);
			LastRefTemp=Msg.Pt.Temp;
			if(VerifyGUIObject(pMainFrame))
				pMainFrame->SetStatusTRef(Msg.Pt.Temp);
			if(IsThermoHistMode() && !pChart->GetZoomStatus())
				pChart->GetSerie(0)->AddPoint(Msg.Pt.Time,Msg.Pt.Temp);

		}
		else
		{
			MeasTemp.push_back(Msg.Pt);
			LastMeasTemp=Msg.Pt.Temp;
			if(VerifyGUIObject(pMainFrame))
				pMainFrame->SetStatusTCur(Msg.Pt.Temp);
			if(IsThermoHistMode() && !pChart->GetZoomStatus())
				pChart->GetSerie(1)->AddPoint(Msg.Pt.Time,Msg.Pt.Temp);
		}
	}
}

void CThermoHistDoc::RedrawChart(bool Refresh)
{
	CChartCtrl* pChart=GetChart();
	if(!pChart->GetZoomStatus())
	{
		pChart->GetSerie(0)->ClearSerie(); //RefTemp
		pChart->GetSerie(1)->ClearSerie(); //MeasTemp
		double *TimeArray=new double[max(max(RefTemp.size(),MeasTemp.size()),Power.size())];
		double *TempArray=new double[max(max(RefTemp.size(),MeasTemp.size()),Power.size())];
		int i=0;
		
		for(std::deque<CTempPoint>::iterator iter=RefTemp.begin();iter!=RefTemp.end(); iter++)
		{
			TimeArray[i]=iter->Time;
			TempArray[i++]=iter->Temp;
		}
		pChart->GetSerie(0)->SetPoints(TimeArray,TempArray,i);
		
		i=0;
		for(std::deque<CTempPoint>::iterator iter=MeasTemp.begin();iter!=MeasTemp.end(); iter++)
		{
			TimeArray[i]=iter->Time;
			TempArray[i++]=iter->Temp;
		}
		pChart->GetSerie(1)->SetPoints(TimeArray,TempArray,i);

		if(theApp.Ini.ThermoHistShowPower)
		{
			i=0;
			std::deque<PairDbl>::iterator iter;
			for(iter=Power.begin();iter!=Power.end(); iter++)
			{
				TimeArray[i]=iter->first;
				TempArray[i++]=iter->second;
			}
			pChart->GetSerie(2)->SetPoints(TimeArray,TempArray,i);
		}
		delete[] TimeArray;
		delete[] TempArray;
		if(Refresh)
			pChart->RefreshCtrl();
	}
}

void CThermoHistDoc::AddRefTempPoint(double RefT)
{
	CTempPoint Pt;
	Pt.Temp=RefT;
	Pt.Time=GetTimeDouble()-TheVeryFirstTime;
	RefTemp.push_back(Pt);
	LastRefTemp=RefT;
	if(IsThermoHistMode())
		GetChart()->GetSerie(0)->AddPoint(Pt.Time,Pt.Temp);
	DeleteExpiredData();
}

void CThermoHistDoc::AddMeasTempPoint(double MeasT)
{
	CTempPoint Pt;
	Pt.Temp=MeasT;
	Pt.Time=GetTimeDouble()-TheVeryFirstTime;
	MeasTemp.push_back(Pt);
	LastMeasTemp=MeasT;
	if(IsThermoHistMode())
		GetChart()->GetSerie(1)->AddPoint(Pt.Time,Pt.Temp);
	DeleteExpiredData();
}

void CThermoHistDoc::AddPowerPoint(double Pwr)
{
	std::pair<double,double> Pt;
	Pt.second=Pwr;
	Pt.first=GetTimeDouble()-TheVeryFirstTime;
	Power.push_back(Pt);
	if(IsThermoHistMode() && theApp.Ini.ThermoHistShowPower)
		GetChart()->GetSerie(2)->AddPoint(Pt.first,Pt.second);
	DeleteExpiredData();
}

void CThermoHistDoc::OnClearHistory()
{
	CChartCtrl* pChart=GetChart();
	double Min,Max;
	if(pChart->GetZoomStatus())
	{
		pChart->GetBottomAxis()->GetMinMax(Min,Max);
		while(!RefTemp.empty())
			if(RefTemp.front().Time<Max)
				RefTemp.pop_front();
			else
				break;
		while(!MeasTemp.empty())
			if(MeasTemp.front().Time<Max)
				MeasTemp.pop_front();
			else
				break;
		while(!Power.empty())
			if(Power.front().first<Max)
				Power.pop_front();
			else
				break;
		pChart->UndoZoom();
		pChart->GetBottomAxis()->SetAutomaticReduceLimits(true,Max,Max+120);
		RedrawChart();

	}
	else
	{
		TheVeryFirstTime=GetTimeDouble();
		RefTemp.clear();
		MeasTemp.clear();
		Power.clear();
		RedrawChart(false);
		FormatChartToThermoHist();
	}
}

void CThermoHistDoc::DeleteExpiredData()
{
	double Now=GetTimeDouble()-TheVeryFirstTime;
	double Limit=max(theApp.Ini.ThermoHistKeepTime*1.1,theApp.Ini.ThermoHistKeepTime+30);
	double A;
	bool Expired=false;
	if(!MeasTemp.empty())
	{
		A=Now-MeasTemp.front().Time;
		if(Now-MeasTemp.front().Time>Limit)
			Expired=true;
	}
	if(!RefTemp.empty())
	{
		A=Now-RefTemp.front().Time;
		if(Now-RefTemp.front().Time>Limit)
			Expired=true;
	}
	if(!Power.empty())
	{
		A=Now-Power.front().first;
		if(Now-Power.front().first>Limit)
			Expired=true;
	}
	if(Expired)
	{
		double Min=Now-theApp.Ini.ThermoHistKeepTime;
		while(!RefTemp.empty())
			if(RefTemp.front().Time<Min)
				RefTemp.pop_front();
			else
				break;

		while(!MeasTemp.empty())
			if(MeasTemp.front().Time<Min)
				MeasTemp.pop_front();
			else
				break;
		while(!Power.empty())
			if(Power.front().first<Min)
				Power.pop_front();
			else
				break;
		if(IsThermoHistMode())
		{
			GetChart()->GetBottomAxis()->SetAutomaticReduceLimits(true,Min,Min+120.);
			RedrawChart();
		}
	}
}

void CThermoHistDoc::CopyToClipboard()
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

CString CThermoHistDoc::PrintEasyPlotData()
{
	CString sData, str;
	//Добавление легенды
	sData = "/sa l \"T Ref, °C\" 2\n/sa l \"T Cur, °C\" 3\r\n";
	if(theApp.Ini.ThermoHistShowPower)
		sData += "/sa l \"Power, W\" 4\r\n";
	sData+="/td \"xyy";
	if(theApp.Ini.ThermoHistShowPower)
		sData += "y";
	sData += "\"\r\n";

	CChartCtrl* pChart=GetChart();
	double Min,Max;
	pChart->GetBottomAxis()->GetMinMax(Min,Max);
	std::deque<CTempPoint>::iterator iRef;
	std::deque<CTempPoint>::iterator iCur;
	std::deque<PairDbl>::iterator iPow;
	for(iRef=RefTemp.begin(); iRef!=RefTemp.end(); ++iRef)
		if(iRef->Time>=Min)
			break;
	for(iCur=MeasTemp.begin(); iCur!=MeasTemp.end(); ++iCur)
		if(iCur->Time>=Min)
			break;

	for(iPow=Power.begin(); iPow!=Power.end() && theApp.Ini.ThermoHistShowPower; ++iPow)
		if(iPow->first>=Min)
			break;
	if(theApp.Ini.ThermoHistShowPower)
		while (iRef!=RefTemp.end() && iCur!=MeasTemp.end() && iPow!=Power.end())
		{
			if(iRef->Time>Max && iCur->Time>Max && iPow->first>Max)
				break;
			double Time, refTime, curTime, powTime;
			double refVal, curVal, powVal;
			if(iRef!=RefTemp.end())
			{
				refTime = iRef->Time;
				refVal = iRef->Temp;
			}
			if(iCur!=MeasTemp.end())
			{
				curTime = iCur->Time;
				curVal = iCur->Temp;
			}
			if(iPow!=Power.end())
			{
				powTime = iPow->first;
				powVal = iPow->second;
			}


			if(iRef->Time<=iCur->Time && iRef->Time<=iPow->first)
				Time=Round(iRef->Time, 0.1);
			else if(iCur->Time<=iRef->Time && iCur->Time<=iPow->first)
				Time=Round(iCur->Time, 0.1);
			else
				Time=Round(iPow->first, 0.1);
			
			str.Format("%.1f, ", Time);
			sData += str;
			if(Round(iRef->Time, 0.1) == Time)
			{
				str.Format("%.1f, ", iRef->Temp);
				sData += str;
				++iRef;
			}
			else
				sData += "//m, ";

			if(Round(iCur->Time, 0.1) == Time)
			{
				str.Format("%.1f, ", iCur->Temp);
				sData += str;
				++iCur;
			}
			else
				sData += "//m, ";

			if(Round(iPow->first, 0.1) == Time)
			{
				str.Format("%.1f\r\n", iPow->second);
				sData += str;
				++iPow;
			}
			else
				sData += "//m\r\n";
		}
	else //!theApp.Ini.ThermoHistShowPower
		while (iRef!=RefTemp.end() && iCur!=MeasTemp.end())
		{
			if(iRef->Time>Max && iCur->Time>Max)
				break;
			double Time;
			if(iRef->Time<=iCur->Time && iRef->Time<=iPow->first)
				Time=Round(iRef->Time, 0.1);
			else
				Time=Round(iCur->Time, 0.1);

			str.Format("%.1f, ", Time);
			sData += str;
			if(Round(iRef->Time, 0.1) == Time)
			{
				str.Format("%.1f, ", iRef->Temp);
				sData += str;
				++iRef;
			}
			else
				sData += "//m, ";

			if(Round(iCur->Time, 0.1) == Time)
			{
				str.Format("%.1f\n", iCur->Temp);
				sData += str;
				++iCur;
			}
			else
				sData += "//m\n";

		}
	return sData;
}