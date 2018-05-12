/*
#include "stdafx.h"
#include <string.h>
#include "ProgNew.h"
#include "DialogParamRegion.h"
#include "RegionWnd.h"
#include "MainFrame.h"
//#include "RegionWnd.h"
#include "OpenSaveFun.h"
//#include "MainFrame.h"
*/
#include "stdafx.h"
#include <limits>
#include "Main.h"
#include "Time.h"

extern CMutex MutexThread;
extern CProgNewApp theApp;
CString FileSaveOpenErrorDescription;
//___________________________________________________________________________
BOOL WindowSaveAsOpen(CMainFrame* pMainFrame)
{
CSingleLock sLock(&MutexThread);
if(::IsWindow(pMainFrame->m_pRegionWnd->m_hWnd)) 
	::EnableWindow(pMainFrame->m_pRegionWnd->m_hWnd, FALSE);

char fullpath[260]; char filename[260]; char dir[256]; char ext[8];
FILE_NAME* FN;
FILE* fp;
//sprintf(fullpath,"FileName"); sprintf(filename,""); sprintf(dir,"");

OPENFILENAME ofn;
memset(&ofn, 0, sizeof(OPENFILENAME));
ofn.lStructSize = sizeof(OPENFILENAME);
ofn.hwndOwner = pMainFrame->m_hWnd; //NULL; //???????????
ofn.hInstance = AfxGetInstanceHandle();
if(pMainFrame->m_Doc.m_TypeFile == pMainFrame->m_Doc.EasyPlot) 
	{FN = &(pMainFrame->m_Doc.m_EasyPlotFile);
	ofn.lpstrFilter = "EasyPlot files (*.epx)\0*.epx\0All files (*.*)\0*.*\0";
	sprintf(ext, "epx");
	//if(FN->FileName[0]=='\0') 
		//strcpy(FN->FileName,(LPCSTR)*theApp.Ini.EasyPlotFile.Value);
	//ofn.lpstrDefExt = ext;
	//AfxMessageBox(pMainFrame->m_Doc.m_EasyPlotFile.FileName);
	}
else if(pMainFrame->m_Doc.m_TypeFile == pMainFrame->m_Doc.EasyPlotMC) 
	{FN = &(pMainFrame->m_Doc.m_EasyPlotFile);
	ofn.lpstrFilter = "MultiColomn EasyPlot files (*.epx)\0*.epx\0All files (*.*)\0*.*\0";
	sprintf(ext, "epx");
	//if(FN->FileName[0]=='\0') 
		//strcpy(FN->FileName,(LPCSTR)*theApp.Ini.EasyPlotFile.Value);
	}
else if(pMainFrame->m_Doc.m_TypeFile == pMainFrame->m_Doc.Origin) 
	{FN = &(pMainFrame->m_Doc.m_OriginFile);
	ofn.lpstrFilter = "Origin files (*.dat)\0*.dat\0All files (*.*)\0*.*\0";
	sprintf(ext, "epx");
	//if(FN->FileName[0]=='\0') 
		//strcpy(FN->FileName,(LPCSTR)*theApp.Ini.OriginFile.Value);
	}
else // По умолчанию: if(pMainFrame->m_Doc.m_TypeFile == pMainFrame->m_Doc.Project) 
	{FN = &(pMainFrame->m_Doc.m_ProjectFile);	
	ofn.lpstrFilter = "Project files (*.prj)\0*.prj\0All files (*.*)\0*.*\0";
	sprintf(ext, "prj");
	if(FN->FileName[0]=='\0' && !theApp.Ini.ProjectFile[0].Value.IsEmpty()) 
		strcpy(FN->FileName,(LPCSTR)theApp.Ini.ProjectFile[0].Value);
	}
//CString cstr=FN->FileName;
//int EndPath=cstr.ReverseFind('\\');
//cstr.Delete(EndPath+1,cstr.GetLength()-EndPath-1);
//if(FN->Dir[0]=='\0') 
		//strcpy(FN->Dir,(LPCSTR)cstr);
//else {AfxMessageBox("Error WindowSaveAs 2"); return FALSE;}
sprintf(fullpath,"%s",FN->FileName);
sprintf(filename,"%s",FN->FileName);
sprintf(dir,"%s",FN->Dir);
//ofn.lpstrFilter = "Text files (*.txt)\0*.txt\0All files (*.*)\0*.*\0";
ofn.lpstrDefExt = ext;
ofn.nFilterIndex = 1;
ofn.lpstrFile = fullpath; //filename;
ofn.nMaxFile = sizeof(fullpath);
ofn.lpstrFileTitle = filename;
ofn.nMaxFileTitle = sizeof(filename);
ofn.lpstrInitialDir = dir;
if(pMainFrame->m_Doc.m_SaveAsOpen == pMainFrame->m_Doc.SaveAs) 
	ofn.lpstrTitle = "Save as...";
else ofn.lpstrTitle = "Open";	
ofn.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT | OFN_HIDEREADONLY | OFN_EXPLORER;


int result;
if(pMainFrame->m_Doc.m_SaveAsOpen == pMainFrame->m_Doc.SaveAs) 
				result = ::GetSaveFileName(&ofn);
else		result = ::GetOpenFileName(&ofn);
//sprintf(dir, "%s", fullpath); 
//dir[ofn.nFileOffset-1]='\0';
::EnableWindow(pMainFrame->m_pRegionWnd->m_hWnd, TRUE);
if(result)
	{
	if(pMainFrame->m_Doc.m_TypeFile == pMainFrame->m_Doc.Project)
		theApp.m_pMainFrame->InsertProjectToRecent(fullpath);
	else if(pMainFrame->m_Doc.m_TypeFile == pMainFrame->m_Doc.EasyPlot
		||pMainFrame->m_Doc.m_TypeFile == pMainFrame->m_Doc.EasyPlotMC)
		theApp.Ini.EasyPlotFile.Value=fullpath;
	else if(pMainFrame->m_Doc.m_TypeFile == pMainFrame->m_Doc.Origin)
		theApp.Ini.OriginFile.Value=(LPCTSTR)fullpath;

//	sprintf(dir, "%s", fullpath); 
//	dir[ofn.nFileOffset-1]='\0';
//	sprintf(FN->FullPath,"%s",fullpath);
//	sprintf(FN->FileName,"%s",filename);
//	sprintf(FN->Dir,"%s",dir);
	
	if(pMainFrame->m_Doc.m_SaveAsOpen == pMainFrame->m_Doc.SaveAs) 
		{
		if(pMainFrame->m_Doc.m_TypeFile == pMainFrame->m_Doc.EasyPlot)
			{
			fp=fopen(fullpath, "w");
			if(!fp) {AfxMessageBox("Can`t save file"); return FALSE;}
			SaveEasyPlotFile(fp, filename);
			fclose(fp);
			theApp.Ini.EasyPlotFile.Value=fullpath;
			}
		else if(pMainFrame->m_Doc.m_TypeFile == pMainFrame->m_Doc.EasyPlotMC) 
			{
			fp=fopen(fullpath, "w");
			if(!fp) {AfxMessageBox("Can`t save file"); return FALSE;}
			SaveEasyPlotMC(fp, filename, theApp.m_pMainFrame->m_Doc.m_DocType);
			fclose(fp);
			theApp.Ini.EasyPlotFile.Value=fullpath;
			}
		else if(pMainFrame->m_Doc.m_TypeFile == pMainFrame->m_Doc.Origin) 
			{
			fp=fopen(fullpath, "w");
			if(!fp) {AfxMessageBox("Can`t save file"); return FALSE;}
			SaveOrigin(fp, filename);
			fclose(fp);
			theApp.Ini.OriginFile.Value=fullpath;
			}
		else if(pMainFrame->m_Doc.m_TypeFile == pMainFrame->m_Doc.Project)
			{
			fp=fopen(fullpath, "wb+");
			if(!fp) 
				{ AfxMessageBox("Can`t save file"); 
					return FALSE; 
				}
			THRI_LOCK();
			if(pMainFrame->m_Doc.fpPrj) fclose(pMainFrame->m_Doc.fpPrj); 
			SaveBinaryFile(fp);
			pMainFrame->m_Doc.m_NeedSave = pMainFrame->m_Doc.NoNeed;
			//if(pMainFrame->m_Doc.fpPrj) fclose(pMainFrame->m_Doc.fpPrj); 
			pMainFrame->m_Doc.fpPrj = fp;
			pMainFrame->m_Doc.m_ThrComm.fp = fp;
			THRI_UNLOCK();
			}
		} // end if(pMainFrame->m_Doc.m_SaveAsOpen == pMainFrame->m_Doc.SaveAs) 
	else if(pMainFrame->m_Doc.m_SaveAsOpen == pMainFrame->m_Doc.Open)
		{
		// Вход в MutexThread происходит при вызове этой функции из OnFileOpenProject
RetryRead:	fp=fopen(fullpath, "rb+");
		if(!fp) {AfxMessageBox("Can`t open file"); return FALSE;}
		if(!ReadBinaryFile(fp) )
			{
			fclose(fp);
			CString str="Project file is corrupted. Continuing may crash the application.\n"+ FileSaveOpenErrorDescription+ "\n\nDo you want to abort opening this document?";
			int result=pMainFrame->MessageBox(str,"Open project error",MB_ABORTRETRYIGNORE|MB_ICONSTOP); 
			if(result==IDRETRY)
			{
				EmptyAllData();
				goto RetryRead;
			}
			if(result==IDABORT)
			{
				pMainFrame->m_Doc.fpPrj = pMainFrame->m_Doc.m_ThrComm.fp = NULL;
				return FALSE;
			}
			if(result==IDIGNORE)
				fp=fopen(fullpath, "rb+");
			}
		pMainFrame->m_Doc.fpPrj = fp;
		pMainFrame->m_Doc.m_ThrComm.fp = fp;
		} //end else //if(pMainFrame->m_Doc.m_SaveAsOpen == pMainFrame->m_Doc.Open)
	
	sprintf(dir, "%s", fullpath); 
	dir[ofn.nFileOffset-1]='\0';
	sprintf(FN->FullPath,"%s",fullpath);
	sprintf(FN->FileName,"%s",filename);
	sprintf(FN->Dir,"%s",dir);
	
	}//end if(result) 
else
	{
	if(pMainFrame->m_Doc.m_SaveAsOpen == pMainFrame->m_Doc.Open)
		pMainFrame->m_Doc.fpPrj = pMainFrame->m_Doc.m_ThrComm.fp = NULL;
	return FALSE;
	}

return TRUE;
}

//========
void SaveEasyPlotMC(FILE* fp, char* FileName, int DocType)
{
if(DocType==CDoc::XPS)
{
	CRegion* pReg;
	int MaxPoints=0;
	unsigned int *MaxTextLength;
	MaxTextLength = new unsigned int[CRegion::m_NReg];
	memset((void*)MaxTextLength, 0, sizeof(int)*CRegion::m_NReg);
	char str[256];
	char Spaces[256];
	memset((void*)&Spaces, ' ', sizeof(Spaces));
	int i;
	char Value[16];
	int RegionsOff=0;
	for (pReg=CRegion::GetFirst(); pReg!=NULL; pReg=CRegion::GetNext(pReg))
		if(pReg->m_DataIn.Off) RegionsOff++;
	CTime time = CTime::GetCurrentTime();

	fprintf(fp, ";File \"%s\", saved by %s in %s\n", FileName, theApp.m_pMainFrame->AppTitle, (LPCSTR)time.Format("%b %d, %Y at %H:%M:%S"));
	if(RegionsOff==0)
		fprintf(fp, ";Number of regions: %i\n", CRegion::m_NReg);
	else 
		fprintf(fp, ";Number of regions: %i (%i of them %s off)\n", CRegion::m_NReg, RegionsOff,(RegionsOff==1)?"is":"are");
	fprintf(fp, ";|Reg |KE/BE|Anode| HV |  Start  |  Finish | Step |  N |  n | Time |    Start Time   |     End Time    |\n");
	for (pReg=CRegion::GetFirst(); pReg!=NULL; pReg=CRegion::GetNext(pReg))
	{
		if(pReg->m_DataIn.Off)
			continue;
		sprintf(Value,"R%i",pReg->ID+1);
		fprintf(fp,";|%4s|",Value);
		if(pReg->m_DataIn.KE_BE == DATA_IN::EnergyType::KE)
			{sprintf(Value," %s", "KE");
			 fprintf(fp,"%5s|",Value);
			 //sprintf(Value,"%s", "None");
			 sprintf(Value,"%s", pReg->h_nu_Info.strName_h_nu[pReg->m_DataIn.N_h_nu]);
			 fprintf(fp,"%5s|",Value);
			}
		else
			{sprintf(Value," %s", "BE");
			 fprintf(fp,"%5s|",Value);
			 sprintf(Value,"%s", pReg->h_nu_Info.strName_h_nu[pReg->m_DataIn.N_h_nu]);
			 fprintf(fp,"%5s|",Value);
			}
		sprintf(Value,"%.0lf", I2D(pReg->m_DataIn.HV) );
		fprintf(fp,"%4s|",Value);
		sprintf(Value,"%.3lf", I2D(pReg->m_DataIn.KE_Start) );
		fprintf(fp,"%9s|",Value);
		sprintf(Value,"%.3lf", I2D(pReg->m_DataIn.KE_End) );
		fprintf(fp,"%9s|",Value);
		sprintf(Value,"%.3lf", I2D(pReg->m_DataIn.Step));
		fprintf(fp,"%6s|",Value);
		sprintf(Value,"%i", pReg->m_DataIn.N_);
		fprintf(fp,"%4s|",Value);
		sprintf(Value,"%i", pReg->m_DataIn.Curr_N);
		fprintf(fp,"%4s|",Value);
		sprintf(Value,"%.3lf", I2D(pReg->m_DataIn.Time));
		fprintf(fp,"%6s|",Value);
		fprintf(fp, "%17s|", FormatTime(pReg->m_BeginTime, "%d.%m.%Y %H:%M").GetString());
		fprintf(fp, "%17s|", FormatTime(pReg->m_EndTime, "%d.%m.%Y %H:%M").GetString());
		fprintf(fp,"\n"); 
		fprintf(fp,";%s\n",pReg->m_DataIn.Comments); 

	}

	//Добавление легенды = Comments
	int Count=2;
	for (pReg=CRegion::GetFirst(); pReg!=NULL; pReg=CRegion::GetNext(pReg))
	{
		if(pReg->m_DataIn.Off)
			continue;
		fprintf(fp,"/sa l \"%s\" %i\n",pReg->m_DataIn.Comments, Count);
		Count+=2;
	}

	fprintf(fp,"/td \"");
	for (pReg=CRegion::GetFirst(), i=0; pReg!=NULL; pReg=CRegion::GetNext(pReg), i++)
	{	
		if(pReg->m_DataIn.Off)
			continue;

		for (int Point=0;Point<pReg->m_NDataOut; Point++) 
		{
			sprintf(str, "%.3lf %.3f ", I2D(pReg->m_pDataOut[Point].x), pReg->m_pDataOut[Point].y);
			if(strlen(str) > MaxTextLength[i]) MaxTextLength[i]=strlen(str);
		}
		fprintf(fp,"xy");
		if (pReg->m_NDataOut>MaxPoints) MaxPoints=pReg->m_NDataOut;
	}
	fprintf(fp,"\"\n");

	for (int Point=0;Point<MaxPoints; Point++)
	{
		for (pReg=CRegion::GetFirst(), i=0; pReg!=NULL; pReg=CRegion::GetNext(pReg), i++)
		{
			if(pReg->m_DataIn.Off)
				continue;

			if(Point<pReg->m_NDataOut) 
				sprintf(str,"%.3lf %.3f ", I2D(pReg->m_pDataOut[Point].x), pReg->m_pDataOut[Point].y);
			else
				sprintf(str,"//m   //m    ");
			strncat(str,Spaces, MaxTextLength[i]-strlen(str)+1);
			fprintf(fp,"%s",str);
		}
		fprintf(fp,"\n");
	}
	delete[] MaxTextLength;
}

else if(DocType==CDoc::DXPS)
{
	CDxpsRegion *pReg;
	char Value[16];
	int RegionsOff=0;
	for (pReg=CDxpsRegion::GetFirst(); pReg!=NULL; pReg=CDxpsRegion::GetNext(pReg))
		if(pReg->Parameters.Off) RegionsOff++;
	char *anodes[]={"Al", "Mg", "He I", "He II"};
	CTime time = CTime::GetCurrentTime();

	fprintf(fp, ";File \"%s\", saved by %s in %s\n", FileName, theApp.m_pMainFrame->AppTitle, (LPCSTR)time.Format("%b %d, %Y at %H:%M:%S"));
	fprintf(fp, ";Measurement started in \"%s\"\n", (LPCSTR)COleDateTime(CDxpsRegion::ScanStartDateTime).Format("%b %d, %Y at %H:%M:%S"));
	fprintf(fp, ";Total scan time: %.2i:%.2i:%.2i. ",  ((int)CDxpsRegion::PassedCommonTime)/60/60,(((int)CDxpsRegion::PassedCommonTime)/60)%60, ((int)CDxpsRegion::PassedCommonTime)%60);
	if(RegionsOff==0)
		fprintf(fp, "Number of regions: %i\n", CDxpsRegion::GetRegNumber());
	else 
		fprintf(fp, ";Number of regions: %i (%i of them %s off)\n", CRegion::m_NReg, RegionsOff,(RegionsOff==1)?"is":"are");
	fprintf(fp, ";| Reg| Type |    BE|   HV|Anode| Delay| Dwell| Comments \n");
	for (pReg=CDxpsRegion::GetFirst(); pReg!=NULL; pReg=CDxpsRegion::GetNext(pReg))
	{
		if(pReg->Parameters.Off)
			continue;
		sprintf(Value,"D%i",pReg->ID+1);
		fprintf(fp,";|%4s|",Value);
		if(pReg->Parameters.Type == TYPE_BE)
			{sprintf(Value, "  BE  ");
			 fprintf(fp,"%6s|",Value);
			 sprintf(Value,"%g", pReg->Parameters.BE);
			 fprintf(fp,"%6s|",Value);
			 sprintf(Value,"%g", pReg->Parameters.HV);
			 fprintf(fp,"%5s|",Value);
			 sprintf(Value,"%s", anodes[pReg->Parameters.Anode]);
			 fprintf(fp,"%5s|",Value);
			 sprintf(Value,"%g", pReg->Parameters.Delay);
			 fprintf(fp,"%6s|",Value);
			 sprintf(Value,"%g", pReg->Parameters.Dwell);
			 fprintf(fp,"%6s|",Value);
			}
		else
			{sprintf(Value,"D%i/D%i", pReg->Parameters.Divident,pReg->Parameters.Divisor );
			 fprintf(fp,"%6s|",Value);
			 sprintf(Value,"%s", "  -  ");
			 fprintf(fp,"%6s|",Value);
			 sprintf(Value,"%s", "  -  ");
			 fprintf(fp,"%5s|",Value);
			 sprintf(Value,"%s", "  -  ");
			 fprintf(fp,"%5s|",Value);
			 sprintf(Value,"%s", "  -  ");
			 fprintf(fp,"%6s|",Value);			
 			 sprintf(Value,"%s", "  -  ");
			 fprintf(fp,"%6s|",Value);
		}
		fprintf(fp,"%s\n",pReg->Parameters.Comments); 
	}
	//Добавление легенды = Comments
	int Count=2;
	fprintf(fp,"/sa l \"%s\" %i\n","T Ref.", Count++);
	fprintf(fp,"/sa l \"%s\" %i\n", "T Meas." , Count++);

	for (pReg=CDxpsRegion::GetFirst(); pReg!=NULL; pReg=CDxpsRegion::GetNext(pReg))
	{
		if(pReg->Parameters.Off)
			continue;
		fprintf(fp,"/sa l \"%s\" %i\n",pReg->Parameters.Comments, Count);
		Count++;
	}

	fprintf(fp,"/td \"xpp");
	int i;
	for (pReg=CDxpsRegion::GetFirst(), i=0; pReg!=NULL; pReg=CDxpsRegion::GetNext(pReg), i++)
	{	
		if(pReg->Parameters.Off)
			continue;

		/*for (int Point=0;Point<pReg->m_NDataOut; Point++) 
		{
			sprintf(str, "%.3lf ", pReg->m_pDataOut[Point].y);
			if(strlen(str) > MaxTextLength[i]) MaxTextLength[i]=strlen(str);
		}*/
		fprintf(fp,"y");
	}
	fprintf(fp,"\"\n");
	double Time=-1;
	DxpsOutList::iterator j;
	DxpsOutData data;
	for (j = CDxpsRegion::OutData.begin(); j != CDxpsRegion::OutData.end();j++)
	{
		if(CDxpsRegion::GetRegByN(j->RegionN)->Parameters.Off)
			continue;

		data=*j;
		if(j->Time>Time)
		{
			if(j->Time>0)
				fprintf(fp,"\n");
			fprintf(fp,"%.1f ", j->Time);
			volatile double a = j->Tref;
			if(j->Tref == a ) // !NaN
				fprintf(fp,"%.1f ", j->Tref);
			else
				fprintf(fp,"//m ");
			
			a = j->Tcur;
			if(j->Tcur == a) // !NaN
				fprintf(fp,"%.1f ", j->Tcur);
			else
				fprintf(fp,"//m ");
			Time=j->Time;
		}
		fprintf(fp,"%.3f ", j->Freq);
	}
}
}
