// File Doc.cpp
#include "stdafx.h"
#include "Main.h"

extern CProgNewApp theApp;
CDoc::CDoc()
{
//m_EasyPlotFile.FullPath[0]='\0';
sprintf(m_EasyPlotFile.FileName,"");
sprintf(m_EasyPlotFile.FullPath,"");
sprintf(m_EasyPlotFile.Dir,"");

m_OriginFile.FullPath[0]='\0';
m_OriginFile.FileName[0]='\0';
m_OriginFile.Dir[0]='\0';

//sprintf(m_EasyPlotFile.FileName,"FileBreb.epx");
//m_EasyPlotFile.FileName[0]='A';
//m_EasyPlotFile.FileName[0]='\0';
//m_EasyPlotFile.Dir[0]='\0';
//m_EasyPlotFile.TypeFile = EasyPlot;
m_EasyPlotFile.NeedSave = Need;

m_ProjectFile.FullPath[0]='\0';
m_ProjectFile.FileName[0]='\0';
m_ProjectFile.Dir[0]='\0';
//m_ProjectFile.TypeFile = Project;
m_ProjectFile.NeedSave = Need;
}

int CDoc::CheckDocType()
{
HMENU hMenu=::GetMenu(theApp.m_pMainFrame->m_hWnd);
if(CRegion::m_NReg>0)
	m_DocType=XPS;
else 
	{
	if(CDxpsRegion::GetRegNumber()>0)
		m_DocType=DXPS;
	else
		m_DocType=NoDoc;
	}

if(m_DocType==XPS)
	{
	theApp.m_pMainFrame->m_pDxpsDlg->ShowWindow(SW_HIDE);
	::SendMessage(theApp.m_pMainFrame->m_hToolBar, TB_ENABLEBUTTON, 
							(WPARAM) ID_PROGRAM_DXPS_TABLE, (LPARAM) MAKELONG(FALSE, 0));
	::SendMessage(theApp.m_pMainFrame->m_hToolBar, TB_ENABLEBUTTON, 
							(WPARAM) ID_PROGRAM_REGION_TABLE, (LPARAM) MAKELONG(TRUE, 0));
	theApp.m_pMainFrame->m_pHideWnd->ShowWindow(SW_SHOW);
	theApp.m_pMainFrame->m_Graph.ShowWindow(SW_HIDE);

	}

if(m_DocType==DXPS)
	{
		if(::IsWindow(theApp.m_pMainFrame->m_pRegionWnd->m_hWnd))
			theApp.m_pMainFrame->m_pRegionWnd->ShowWindow(SW_HIDE);
		::SendMessage(theApp.m_pMainFrame->m_hToolBar, TB_ENABLEBUTTON, 
								(WPARAM) ID_PROGRAM_REGION_TABLE, (LPARAM) MAKELONG(FALSE, 0));
		::SendMessage(theApp.m_pMainFrame->m_hToolBar, TB_ENABLEBUTTON, 
								(WPARAM) ID_PROGRAM_DXPS_TABLE, (LPARAM) MAKELONG(TRUE, 0));
		if(CDxpsRegion::PassedNumberOfPoints>0)
		{
			theApp.m_pMainFrame->m_pHideWnd->ShowWindow(SW_HIDE);
			theApp.m_pMainFrame->m_Graph.ShowWindow(SW_SHOW);
			theApp.m_pMainFrame->m_Graph.PlotData();
		}
		else
		{
			theApp.m_pMainFrame->m_pHideWnd->ShowWindow(SW_SHOW);
			theApp.m_pMainFrame->m_Graph.ShowWindow(SW_HIDE);
		}
	}
if(m_DocType==NoDoc)
	{
	::SendMessage(theApp.m_pMainFrame->m_hToolBar, TB_ENABLEBUTTON, 
							(WPARAM) ID_PROGRAM_REGION_TABLE, (LPARAM) MAKELONG(TRUE, 0));
	::SendMessage(theApp.m_pMainFrame->m_hToolBar, TB_ENABLEBUTTON, 
							(WPARAM) ID_PROGRAM_DXPS_TABLE, (LPARAM) MAKELONG(TRUE, 0));
	theApp.m_pMainFrame->m_pHideWnd->ShowWindow(SW_SHOW);
	if(IsWindow(theApp.m_pMainFrame->m_Graph.m_hWnd))
		theApp.m_pMainFrame->m_Graph.ShowWindow(SW_HIDE);
	sprintf(m_WindowCaption, "%s - %s", theApp.m_pMainFrame->AppTitle, "[no data]");
	theApp.m_pMainFrame->SetWindowText(m_WindowCaption);
	}
else
	{
	if(fpPrj)
		{
		sprintf(m_WindowCaption, "%s - %s", theApp.m_pMainFrame->AppTitle, m_ProjectFile.FileName);
		theApp.m_pMainFrame->SetWindowText(m_WindowCaption);
		}
	else
		{
		sprintf(m_WindowCaption, "%s - %s", theApp.m_pMainFrame->AppTitle, "[unsaved]");
		theApp.m_pMainFrame->SetWindowText(m_WindowCaption);
		}

	}
return m_DocType;
}