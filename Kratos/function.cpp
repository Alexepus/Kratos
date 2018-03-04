// File FUNCTION.CPP
/*
#include "stdafx.h"
//#include<windows.h>
#include <string.h>
#include <stdio.h>
#include "ProgNew.h"
#include "Region.h"
#include "DialogParamRegion.h"
#include "ListRegionWnd.h"
#include "function.h"
*/
#include "stdafx.h"
#include "Main.h"
#include "hardware.h"

extern CMutex MutexThread;
extern CProgNewApp theApp;
BOOL ReadLOGFONTFromIniFile(LPCTSTR lpszSection, LOGFONT* LogFont)
{
	CString string;
	//memset(LogFont, 0, sizeof(LOGFONT));
	CWinApp* App = AfxGetApp();
	LogFont->lfHeight = (LONG) App->GetProfileInt(lpszSection, "lfHeight", 20);
	LogFont->lfWidth = (LONG) App->GetProfileInt(lpszSection, "lfWidth", 0);
	LogFont->lfEscapement = (LONG) App->GetProfileInt(lpszSection, "lfEscapement", 0);
	LogFont->lfOrientation = (LONG) App->GetProfileInt(lpszSection, "lfOrientation", 0);
	LogFont->lfWeight = (LONG) App->GetProfileInt(lpszSection, "lfWeight", FW_NORMAL);
	LogFont->lfItalic = (BYTE) App->GetProfileInt(lpszSection, "lfItalic", 0);
	LogFont->lfUnderline = (BYTE) App->GetProfileInt(lpszSection, "lfUnderline", 0);
	LogFont->lfStrikeOut = (BYTE) App->GetProfileInt(lpszSection, "lfStrikeOut", 0);
	LogFont->lfCharSet = (BYTE) App->GetProfileInt(lpszSection, "lfCharSet", ANSI_CHARSET);
	LogFont->lfOutPrecision = (BYTE) App->GetProfileInt(lpszSection, "lfOutPrecision", OUT_DEFAULT_PRECIS);
	LogFont->lfClipPrecision = (BYTE) App->GetProfileInt(lpszSection, "lfClipPrecision", CLIP_DEFAULT_PRECIS);
	LogFont->lfQuality = (BYTE) App->GetProfileInt(lpszSection, "lfQuality", DEFAULT_QUALITY);
	LogFont->lfPitchAndFamily = (BYTE) App->GetProfileInt(lpszSection, "lfPitchAndFamily", (DEFAULT_PITCH|FF_MODERN));
	//LogFont->lfFaceName = (TCHAR) App->GetProfileString(lpszSection, "lfFaceName", "System");
	string = App->GetProfileString(lpszSection, "lfFaceName", "Times New Romanm");
	sprintf(LogFont->lfFaceName, "%s", (LPCSTR)string);
	return TRUE;
}

BOOL WriteLOGFONTToIniFile(LPCTSTR lpszSection, LOGFONT* LogFont)
{
	CString string;
	//memset(LogFont, 0, sizeof(LOGFONT));
	CWinApp* App = AfxGetApp();
	App->WriteProfileInt(lpszSection, "lfHeight", LogFont->lfHeight);
	App->WriteProfileInt(lpszSection, "lfWidth", LogFont->lfWidth);
	App->WriteProfileInt(lpszSection, "lfEscapement", LogFont->lfEscapement);
	App->WriteProfileInt(lpszSection, "lfOrientation", LogFont->lfOrientation);
	App->WriteProfileInt(lpszSection, "lfWeight", LogFont->lfWeight);
	App->WriteProfileInt(lpszSection, "lfItalic", LogFont->lfItalic);
	App->WriteProfileInt(lpszSection, "lfUnderline", LogFont->lfUnderline);
	App->WriteProfileInt(lpszSection, "lfStrikeOut", LogFont->lfStrikeOut);
	App->WriteProfileInt(lpszSection, "lfCharSet", LogFont->lfCharSet);
	App->WriteProfileInt(lpszSection, "lfOutPrecision", LogFont->lfOutPrecision);
	App->WriteProfileInt(lpszSection, "lfClipPrecision", LogFont->lfClipPrecision);
	App->WriteProfileInt(lpszSection, "lfQuality", LogFont->lfQuality);
	App->WriteProfileInt(lpszSection, "lfPitchAndFamily", LogFont->lfPitchAndFamily);
	//LogFont->lfFaceName = (TCHAR) App->GetProfileString(lpszSection, "lfFaceName", "System");
	string = LogFont->lfFaceName;
//	sprintf(string, "%s", LogFont->lfFaceName);
	App->WriteProfileString(lpszSection, "lfFaceName", string);
	return TRUE;
}

BOOL ChooseNewFont(HWND hWnd, LOGFONT* LogFont, COLORREF* Color)
{
	CHOOSEFONT cf;
	memset(&cf, 0, sizeof(CHOOSEFONT));
	cf.lStructSize = sizeof(CHOOSEFONT);
	cf.hwndOwner = hWnd;
	cf.lpLogFont = LogFont;
	cf.Flags = CF_SCREENFONTS | CF_EFFECTS | CF_INITTOLOGFONTSTRUCT |CF_LIMITSIZE;
	//cf.Flags = CF_SCREENFONTS | CF_INITTOLOGFONTSTRUCT |CF_LIMITSIZE;
	cf.rgbColors = *Color;
	cf.lpszStyle = (LPSTR) LogFont->lfFaceName;
	cf.nFontType = SCREEN_FONTTYPE;
	cf.nSizeMin = 5;
	cf.nSizeMax = 30;
	if(::ChooseFont(&cf)) 
	{
		*Color = cf.rgbColors; 
		sprintf(LogFont->lfFaceName, "%s", cf.lpszStyle);
		return TRUE;}
	return FALSE;
}


/**
 * \brief ѕримен€ет параметры региона, заданные пользователем, в модели региона
 * \param pReg ќбъект региона, в котором нужно изменить параметры
 * \param pDlgParamReg ƒиалог, в котором пользователь помен€л параметры
 */
bool SetRegionParametersFromDialog(CRegion* pReg, CDialogParamRegion* pDlgParamReg)
{
int i,k;
int Step = D2I(pDlgParamReg->m_Step);
int N_Step = ( D2I(pDlgParamReg->m_KE_End)  - D2I(pDlgParamReg->m_KE_Start)) / Step;
if(pReg->m_NewOrEdit==pReg->New)
	{
	pReg->m_NDataOut = N_Step+1;
	pReg->m_pDataOut = (DATA_OUT*) malloc(pReg->m_NDataOut*sizeof(DATA_OUT));
	if (pReg->m_pDataOut == NULL) 
		return false;
	for(i=0; i<pReg->m_NDataOut; ++i) 
		{pReg->m_pDataOut[i].x = D2I(pDlgParamReg->m_KE_Start) + i*Step; //??????
		 pReg->m_pDataOut[i].y = 0;}
	}
else if(pReg->m_NewOrEdit==pReg->Edit)
	{
	int NNewDataOut = N_Step+1;
	int KE_Start = D2I(pDlgParamReg->m_KE_Start);
	int KE_End = D2I(pDlgParamReg->m_KE_End);
	int del_KE_Start = abs(pReg->m_DataIn.KE_Start - KE_Start);
	int del_KE_End = abs(pReg->m_DataIn.KE_End - KE_End);
	int NewArrayBegin = 0;
	int OldArrayBegin = 0;
	int NBytes = NNewDataOut * sizeof(DATA_OUT);
	auto pNewDataOut = (DATA_OUT*)malloc(NBytes);
	if (pNewDataOut == NULL) return false;
	memset(pNewDataOut, 0, NBytes);
	for (i = 0; i<NNewDataOut; ++i) 
		pNewDataOut[i].x = KE_Start + i*Step;
	if(pReg->m_DataIn.Curr_N>0 || pReg->m_NDataOutCurr > 0)
		{
		if(pReg->m_DataIn.KE_Start < KE_Start) 
			{
			OldArrayBegin = del_KE_Start/Step;
			pReg->m_NDataOutCurr -= OldArrayBegin;
			if(pReg->m_NDataOutCurr < 0) pReg->m_NDataOutCurr = 0;
			if(KE_Start > pReg->m_DataIn.KE_End) 
				{
				OldArrayBegin = pReg->m_NDataOut;
				pReg->m_NDataOutCurr = 0;
				}
			} // end if(pReg->m_DataIn.KE_Start < KE_Start) 
		else if(pReg->m_DataIn.KE_Start > KE_Start) 
			{
			NewArrayBegin = del_KE_Start/Step;
			pReg->m_NDataOutCurr += NewArrayBegin;
			if(pReg->m_NDataOutCurr >= NNewDataOut) pReg->m_NDataOutCurr = 0;
			if(KE_End < pReg->m_DataIn.KE_Start)
				{NewArrayBegin = NNewDataOut;
				 pReg->m_NDataOutCurr = 0;
				}
			} // end else if(pReg->m_DataIn.KE_Start > KE_Start) 
		
		if(pReg->m_NDataOutCurr > NNewDataOut-1)
			{ pReg->m_NDataOutCurr = 0;
				++pReg->m_DataIn.Curr_N;
				if(pReg->m_DataIn.Curr_N > pReg->m_DataIn.N_) 
					pReg->m_DataIn.Curr_N = pReg->m_DataIn.N_;
				if(pDlgParamReg->m_pMainFrame->m_Doc.m_ThrComm.pRegEdit==pDlgParamReg->m_pMainFrame->m_Doc.m_ThrComm.pRegNow)	
					pDlgParamReg->m_pMainFrame->m_Doc.m_ThrComm.pRegNow = CRegion::GetNext(pDlgParamReg->m_pMainFrame->m_Doc.m_ThrComm.pRegNow);
			}
		
		for(i=OldArrayBegin, k=NewArrayBegin; i<pReg->m_NDataOut && k<NNewDataOut; ++i, ++k)
			{
			pNewDataOut[k].x = pReg->m_pDataOut[i].x;
			pNewDataOut[k].y = pReg->m_pDataOut[i].y;
			}
		} // end if(pReg->m_DataIn.Curr_N>0)
	else pReg->m_NDataOutCurr=0;  // else if(pReg->m_DataIn.Curr_N==0)
	
	free(pReg->m_pDataOut);
	pReg->m_pDataOut = pNewDataOut;
	pReg->m_NDataOut = NNewDataOut;
	} // end else if(pReg->m_NewOrEdit==pReg->Edit)
	
pReg->m_DataIn.HV = D2I(pDlgParamReg->m_HV);
pReg->m_DataIn.KE_Start = D2I(pDlgParamReg->m_KE_Start);
pReg->m_DataIn.KE_End = D2I(pDlgParamReg->m_KE_End);

pReg->m_DataIn.Step = D2I(pDlgParamReg->m_Step);
pReg->m_DataIn.N_ = pDlgParamReg->m_N;
pReg->m_DataIn.Time = D2I(pDlgParamReg->m_Time);
sprintf(pReg->m_DataIn.Comments, "%s", (LPCSTR)pDlgParamReg->m_Comments);
pReg->m_DataIn.Comments[255] = '\0';
pReg->m_DataIn.Off = pDlgParamReg->m_Off;
if(pDlgParamReg->m_KE_BE == DATA_IN::EnergyType::KE)
	{ pReg->m_DataIn.KE_BE = DATA_IN::EnergyType::KE;
		pReg->m_DataIn.DeltaVolts = D2I((double) pDlgParamReg->m_pMainFrame->m_Doc.m_ThrComm.FiTable.GetFiByHV((int) I2D(pReg->m_DataIn.HV))) 
				+ D2I(100.0);	
	}
else 
	{pReg->m_DataIn.KE_BE = DATA_IN::EnergyType::BE;
	 pReg->m_DataIn.N_h_nu = pDlgParamReg->m_Anode;
	 pReg->m_DataIn.DeltaVolts = D2I((double) pDlgParamReg->m_pMainFrame->m_Doc.m_ThrComm.FiTable.GetFiByHV((int) I2D(pReg->m_DataIn.HV))) 
																+D2I(pReg->h_nu_Info.Value_h_nu[pReg->m_DataIn.N_h_nu])
																+ D2I(100.0);
	}

pReg->UpdateStrValues();
return true;
}

//========
void LeaveCrSecAndEndThread(CMainFrame* pMainFrame, CRegion* pReg, int Image, CSingleLock &tsLock, BOOL UserStop)
{
	CListRegionWnd* pList=pMainFrame->m_pRegionWnd->m_pListRegionWnd;
	if(pMainFrame->m_Doc.m_Graph.m_pDataShort) 
	{
		delete[] pMainFrame->m_Doc.m_Graph.m_pDataShort; 
		pMainFrame->m_Doc.m_Graph.m_pDataShort = NULL; 
	}

	int CrateN=theApp.Ini.CrateNumber.Value;
	int RegisterN=theApp.Ini.RegisterPosition.Value;
	int RegisterRetardN=theApp.Ini.RegisterRetardPosition.Value;
	int RegisterHVN=theApp.Ini.RegisterHVPosition.Value;

	if(!theApp.Ini.CamacSimulation.Value) 	//Hardware
	{
		//Writing HV (younger HV bit = 1 V) - зануление HV перед выходом
		if(!theApp.Ini.HighPressureMode.Value)
			Camac_WriteLong(CrateN, RegisterN, REGISTER_HV_A, REGISTER_POT_WRITE_F, 0L);
								//Writing HV; (younger HV bit = 1 V)
		else
		{
			if(!pReg)
				pReg=CRegion::GetFirst();
			int Retard =pReg->m_DataIn.DeltaVolts -0;
			if(Retard <0) Retard=0;
			Camac_WriteLong(CrateN, RegisterRetardN, REGISTER_5P850_A, REGISTER_5P850_WRITE_F, (long)(Retard/pMainFrame->m_Doc.m_ThrComm.RetardCalibration/HP_RETARD_GUAGE));
									//Writing BE=0; younger Retard bit = 50 mV (49.8 mV - точнее)

			Camac_WriteLong(CrateN, RegisterHVN, REGISTER_5P850_A, REGISTER_5P850_WRITE_F, GetRegisterHVCodeFromHV(2.));
								//Writing HV 
		}
		#ifdef IPS6
		Camac_Close(CrateN);
		#endif
	}

	if(tsLock.IsLocked())
	{ 
		tsLock.Unlock();
	}
	if(!UserStop)
	{
		char *str="Abnormal thread stop.\nSave current \"Kratos.log\" file in other location and show it to the developer.";
		LogFile(str);
		AfxMessageBox(str);
	}
	pList->SetIconForReg(pReg, 0);
	::SendMessage(pMainFrame->m_hToolBar, TB_CHECKBUTTON  , 
								(WPARAM) ID_PROGRAMM_START, (LPARAM) MAKELONG(FALSE, 0));
	::SendMessage(pMainFrame->m_hToolBar, TB_ENABLEBUTTON  , 
								(WPARAM) ID_FILE_OPEN_PROJECT, (LPARAM) MAKELONG(TRUE, 0));
	::SendMessage(pMainFrame->m_hToolBar, TB_ENABLEBUTTON  , 
								(WPARAM) ID_FILE_NEW_PROJECT, (LPARAM) MAKELONG(TRUE, 0));

	HMENU	SysMenu = ::GetSystemMenu(pMainFrame->m_hWnd, FALSE);
	::EnableMenuItem(SysMenu, SC_CLOSE, MF_ENABLED);

	if(pMainFrame->m_Doc.m_Graph.m_pDataAll == NULL)
		{	
		if(pReg->m_NDataOutCurr > 0)
			{
			pMainFrame->m_Doc.m_Graph.m_pDataAll = pReg->m_pDataOut;
			pMainFrame->m_Doc.m_Graph.m_NDataAll = pReg->m_NDataOutCurr;
			}
		}
	pMainFrame->m_Doc.m_Graph.ReDrawAll();

	pMainFrame->m_StartStop = pMainFrame->Start;
	AfxEndThread(0);
}
//=========

void LeaveCrSecAndEndDxpsThread(CMainFrame* pMainFrame, CDxpsRegion* pReg, CSingleLock &tsLock, BOOL UserStop)
{
	int CrateN=theApp.Ini.CrateNumber.Value;
	int RegisterN=theApp.Ini.RegisterPosition.Value;
	int RegisterRetardN=theApp.Ini.RegisterRetardPosition.Value;
	int RegisterHVN=theApp.Ini.RegisterHVPosition.Value;

	if(!theApp.Ini.CamacSimulation.Value) 	//Hardware
	{
		//Writing HV (younger HV bit = 1 V) - зануление HV перед выходом
		if(!theApp.Ini.HighPressureMode.Value)
			Camac_WriteLong(CrateN, RegisterN, REGISTER_HV_A, REGISTER_POT_WRITE_F, 0L);
								//Writing HV; (younger HV bit = 1 V)
		else
		{
			if(!pReg)
				pReg=CDxpsRegion::GetFirst();
			C_h_nu_Info h_nu_info;
			int Retard = D2I(pMainFrame->m_Doc.m_ThrComm.FiTable.GetFiByHV((int)pReg->Parameters.HV)+ 
										h_nu_info.Value_h_nu[pReg->Parameters.Anode]);
			if(Retard <0) Retard=0;
			Camac_WriteLong(CrateN, RegisterRetardN, REGISTER_5P850_A, REGISTER_5P850_WRITE_F, (long)(Retard/pMainFrame->m_Doc.m_ThrComm.RetardCalibration/HP_RETARD_GUAGE));
									//Writing BE=0; younger Retard bit = 50 mV (49.8 mV - точнее)

			Camac_WriteLong(CrateN, RegisterHVN, REGISTER_5P850_A, REGISTER_5P850_WRITE_F, GetRegisterHVCodeFromHV(2.));
								//Writing HV 
		}
		#ifdef IPS6
		Camac_Close(CrateN);
		#endif
	}
if(tsLock.IsLocked())
{ 
	tsLock.Unlock();
}

::SendMessage(pMainFrame->m_hToolBar, TB_CHECKBUTTON  , 
							(WPARAM) ID_PROGRAMM_START, (LPARAM) MAKELONG(FALSE, 0));
::SendMessage(pMainFrame->m_hToolBar, TB_ENABLEBUTTON  , 
							(WPARAM) ID_FILE_OPEN_PROJECT, (LPARAM) MAKELONG(TRUE, 0));
::SendMessage(pMainFrame->m_hToolBar, TB_ENABLEBUTTON  , 
							(WPARAM) ID_FILE_NEW_PROJECT, (LPARAM) MAKELONG(TRUE, 0));

HMENU	SysMenu = ::GetSystemMenu(pMainFrame->m_hWnd, FALSE);
::EnableMenuItem(SysMenu, SC_CLOSE, MF_ENABLED);

pMainFrame->m_Doc.m_Graph.ReDrawAll();

pMainFrame->m_StartStop = pMainFrame->Start;
AfxEndThread(0);
}


//========
void SaveMeasuringData(CMainFrame* pMainFrame, DATA_OUT* NewData, int NNewData)
{ //NewData - массив точек в недосн€том скане, NNewData - число точек

	int YesNo;
	int i;
	if(::IsWindow(pMainFrame->m_pRegionWnd->m_hWnd) ) 
		::EnableWindow(pMainFrame->m_pRegionWnd->m_hWnd, FALSE);
	YesNo = ::MessageBox(pMainFrame->m_hWnd, "Do you want to save measured data ?", 
												"Attention", MB_YESNO);
	if(YesNo == IDYES)
		{// «аписать в pReg измеренные данные
		// 
		--pMainFrame->m_Doc.m_ThrComm.pRegNow->m_NDataOutCurr;
		SaveDataInToFile(pMainFrame->m_Doc.m_ThrComm.fp, pMainFrame->m_Doc.m_ThrComm.pRegNow);
		for(i=0; i<NNewData; ++i)
			{
			pMainFrame->m_Doc.m_ThrComm.pRegNow->m_pDataOut[i].y = NewData[i].y;
			}
		}
	else //if(YesNo == IDNO)
		{// «аписать в файл старые данные
		pMainFrame->m_Doc.m_ThrComm.pRegNow->m_NDataOutCurr = 0;
		SaveDataInToFile(pMainFrame->m_Doc.m_ThrComm.fp, pMainFrame->m_Doc.m_ThrComm.pRegNow);
		for(i=0; i<=NNewData; ++i)
			{
			SaveDataToFile(pMainFrame->m_Doc.m_ThrComm.fp, pMainFrame->m_Doc.m_ThrComm.pRegNow, 
				i, &pMainFrame->m_Doc.m_ThrComm.pRegNow->m_pDataOut[i]);
			}

		}
	::EnableWindow(pMainFrame->m_pRegionWnd->m_hWnd, TRUE);

char TimeStr[64];
SetNewTIME(&pMainFrame->m_Doc.m_ThrComm.TIME);
TIME2Str(pMainFrame->m_Doc.m_ThrComm.TIME, TimeStr);

::PostMessage(pMainFrame->m_hStatusBar, SB_SETTEXT, 
							2, (LPARAM) (LPSTR) TimeStr);

}

//==========

int D2I(double d) {return ( (int) (d*1000.0) );}
double I2D(int i) {return ( ( (double) i)/1000.0 );}

// –ассчитывает оставшеес€ врем€, необходимое дл€ прохода недосн€тых регионов XPS
void SetNewTIME(int* TIME)
{
int Time=0, TimeReg, NumberOfUnstartedPassages=0,DeltaMeasurings;
CRegion* pReg;
for(pReg=CRegion::GetFirst(); pReg!=NULL; pReg=CRegion::GetNext(pReg))
	{
		if(!pReg->m_DataIn.Off)
		{
			NumberOfUnstartedPassages=pReg->m_DataIn.N_ - pReg->m_DataIn.Curr_N;
			if(pReg->m_NDataOutCurr > 0) NumberOfUnstartedPassages--;
			if(NumberOfUnstartedPassages<0) NumberOfUnstartedPassages=0;
			DeltaMeasurings=pReg->m_NDataOut - pReg->m_NDataOutCurr;
			if(pReg->m_NDataOutCurr==0) DeltaMeasurings=0;
			TimeReg=DeltaMeasurings*pReg->m_DataIn.Time+pReg->m_NDataOut*pReg->m_DataIn.Time*NumberOfUnstartedPassages+NumberOfUnstartedPassages*3000;
			if(pReg->m_DataIn.N_ == pReg->m_DataIn.Curr_N) TimeReg=0;
			Time +=TimeReg;
		}
	}
*TIME = Time;
}
void TIME2Str(int TIME, char* str) //TIME - в мсек
{
int hh,mm,ss,Time, tmp;
Time = abs(TIME)/1000;
hh = Time/3600;
tmp = (Time - hh*3600);
mm = tmp/60;
ss = tmp - mm*60;

if(TIME>=0)
	sprintf(str, "%.2i:%.2i:%.2i",hh,mm,ss);
else
	sprintf(str, "-%.2i:%.2i:%.2i",hh,mm,ss);
}

long GetRegisterHVCodeFromHV(double HV)
{
	double HVTable[HP_MAX_HV_CODE+1]={20., 200., 10., 100., 5., 50., 2.};
	int i;
	for(i=0; i<=HP_MAX_HV_CODE;i++)
		if(HV==HVTable[i])
			return i;
	return 14L; //=2.0 V
}
