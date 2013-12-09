// MassSpecDoc.cpp : implementation of the CMassSpecDoc class
//

#include "stdafx.h"
#include "Spec.h"
#include "SpecView.h"

#include "MassSpecDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CSpecApp theApp;
/////////////////////////////////////////////////////////////////////////////
// CMassSpecDoc

IMPLEMENT_DYNCREATE(CMassSpecDoc, CDocument)

BEGIN_MESSAGE_MAP(CMassSpecDoc, CDocument)
	//{{AFX_MSG_MAP(CMassSpecDoc)
	ON_COMMAND(ID_FILE_SAVE_AS, OnFileSaveAs)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMassSpecDoc construction/destruction

CMassSpecDoc::CMassSpecDoc()
{
}

CMassSpecDoc::~CMassSpecDoc()
{
	SaveDefData();
}

BOOL CMassSpecDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	InitDoc();

	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CMassSpecDoc serialization

void CMassSpecDoc::Serialize(CArchive& ar)
{
	m_DocModifyCS.Lock();
	CString str;
	char Key[]="msp";
	if (ar.IsStoring()) // Сохранение в файл
	{
		char Ver[]="01\r\n";
		ar.WriteString(Key);
		ar.WriteString(Ver);
		str.Format("Regions: %i\r\n",m_MassTable.size());
		ar.WriteString(str);
		int Count=0;
		for(int Reg=0;Reg<m_MassTable.size();Reg++)
		{
			CMassTable *pRow=&m_MassTable[Reg];
			if(pRow->Mass<=0)
				continue;
			Count++;
			str.Format("Region %i\r\n",Count);
			ar.WriteString(str);
			str.Format("%i,%.2f,%s\r\n", (int)pRow->Checked, pRow->Mass, pRow->Comments);
			ar.WriteString(str);
			str.Format("Length %i\r\n",pRow->vTime.size());
			ar.WriteString(str);
			for(int j=0;j<pRow->vTime.size();j++)
			{
				str.Format("%.3f, %.3f\r\n",pRow->vTime[j], pRow->vIntensity[j]);
				ar.WriteString(str);			
			}
		}
		ar.WriteString("TRef,TMeas\r\n");
		for(int i=0; i<m_vRefTemp.size() && i<m_vMeasTemp.size(); i++)
		{
			str.Format("%.2f, %.2f\r\n",m_vRefTemp[i], m_vMeasTemp[i]);
			ar.WriteString(str);
		}

		SetModifiedFlag(false);
	}
	else // Чтение из файла
	{
		CString MsgStr;
		ar.ReadString(str);
		if(str.Left(3)!=Key)
		{
			theApp.m_pMainWnd->MessageBox("This is not a Mass-Spectrometer file.   ","File Read Error",MB_ICONEXCLAMATION|MB_OK);
			AfxThrowArchiveException(CArchiveException::badIndex,0);
		}
		int Ver=0;
		sscanf((LPCSTR)str, "msp %x", &Ver);
		if(Ver==0)
		{
			MsgStr.Format("This is an incorrect Mass-Spectrometer file.   ");
			theApp.m_pMainWnd->MessageBox(MsgStr,"File Read Error",MB_ICONEXCLAMATION|MB_OK);
			AfxThrowArchiveException(CArchiveException::badIndex,0);
		}
		else if(Ver>1)
		{
			MsgStr.Format("This is a newer file version (v.%i) that is not supported by this program version.   ",Ver);
			theApp.m_pMainWnd->MessageBox(MsgStr,"File Read Error",MB_ICONEXCLAMATION|MB_OK);
			AfxThrowArchiveException(CArchiveException::badIndex,0);
		}
		m_MassTable.clear();
		if(!ar.ReadString(str)) //Regions: 
			AfxThrowArchiveException(CArchiveException::badIndex,0);
		double Time,Intensity, TRef, TMeas;
		enum {eUnkData, eRegionData, eTempData} DataType;
		DataType = eUnkData;
		while(ar.ReadString(str))
		{
			if(str.Left(7)=="Region ")
			{
				CMassTable Row;
				memset(Row.Comments,0,sizeof(Row.Comments));
				BOOL Checked;
				if(!ar.ReadString(str))
				{
					theApp.m_pMainWnd->MessageBox("Error in file format.   ","File Read Error",MB_ICONEXCLAMATION|MB_OK);
					goto EndRead;
				}
				sscanf((LPCSTR)str, "%i,%lf,%63c", &Checked, &Row.Mass, Row.Comments);
				Row.Checked=Checked;
				Row.IntensityMax=1e-100;
				Row.bIntensityMaxChanged=true;
				m_MassTable.push_back(Row);
				if(!ar.ReadString(str)) //Length: 
				{
					theApp.m_pMainWnd->MessageBox("Error in file format.   ","File Read Error",MB_ICONEXCLAMATION|MB_OK);
					goto EndRead;
				}
				DataType = eRegionData;
				continue;
			}
			else if(str.Left(10)=="TRef,TMeas")
			{
				DataType = eTempData;
				continue;
			}
			if(m_MassTable.size()<1)
			{
				theApp.m_pMainWnd->MessageBox("Error in file format.   ","File Read Error",MB_ICONEXCLAMATION|MB_OK);
				goto EndRead;
			}
			if(DataType == eRegionData)
			{
				if(sscanf((LPCSTR)str, "%lf, %lf", &Time,&Intensity)!=2)
				{
					theApp.m_pMainWnd->MessageBox("Error in file format [RegionData].   ","File Read Error",MB_ICONEXCLAMATION|MB_OK);
					goto EndRead;
				}
				m_MassTable[m_MassTable.size()-1].vTime.push_back(Time);
				if(m_MassTable[m_MassTable.size()-1].vIntensity.empty())
					m_MassTable[m_MassTable.size()-1].IntensityMax=Intensity*2.;
				else
					m_MassTable[m_MassTable.size()-1].IntensityMax=max(m_MassTable[m_MassTable.size()-1].IntensityMax,Intensity);
				m_MassTable[m_MassTable.size()-1].vIntensity.push_back(Intensity);
			}
			else if(DataType == eTempData)
			{
				if(sscanf((LPCSTR)str, "%lf, %lf", &TRef,&TMeas)!=2)
				{
					theApp.m_pMainWnd->MessageBox("Error in file format [TempData].   ","File Read Error",MB_ICONEXCLAMATION|MB_OK);
					goto EndRead;
				}
				m_vRefTemp.push_back(TRef);
				m_vMeasTemp.push_back(TMeas);
			}
		}

		SetModifiedFlag(false);
EndRead:;
		//ar.Abort();
		if(!m_MassTable.empty())
			SaveDefData();
	}
	m_DocModifyCS.Unlock();

}

/////////////////////////////////////////////////////////////////////////////
// CMassSpecDoc diagnostics

#ifdef _DEBUG
void CMassSpecDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMassSpecDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMassSpecDoc commands

void CMassSpecDoc::InitDoc()
{
	char EntryName[50];
	char EntryContents[256];
	CMassTable Row;
	int i, Checked, EntryCount;
	m_MassTable.clear();
	m_vRefTemp.clear();
	m_vMeasTemp.clear();
	m_PassedCommonTime=0;
	m_bMeasurementInProgress=false;

	GetPrivateProfileString("Mass Table", "EntryCount", "0", EntryContents, 256, theApp.m_psIniFileName);
	EntryCount=atoi(EntryContents);
	for(i=0; i<min(EntryCount,MAX_MASS_COUNT);i++)
	{
		sprintf(EntryName, "Entry_%.2i", i);
		GetPrivateProfileString("Mass Table", EntryName, "0, 0,", EntryContents, 256, theApp.m_psIniFileName);
		if(strcmp(EntryContents,"0, 0,")==0)
			break;
		memset(Row.Comments,0,sizeof(Row.Comments));
		sscanf(EntryContents, "%i,%lf,%63c", &Checked, &Row.Mass, Row.Comments);
		Row.Checked=Checked;
		m_MassTable.push_back(Row);
	}
	GetPrivateProfileString("Default Doc", "ScanTime", "60000", EntryContents, 256, theApp.m_psIniFileName);
	m_ScanTime=(atoi(EntryContents) >= 0)? atoi(EntryContents) : 60000;
	
}

void CMassSpecDoc::SaveDefData()
{
	char EntryName[50];
	char EntryContents[256];
	int j=0;
	for(int i=0; i<m_MassTable.size();i++)
	{
		CMassTable *pRow=&m_MassTable[i];
		if(pRow->Mass<=0)
			continue;
		sprintf(EntryName, "Entry_%.2i", j);
		sprintf(EntryContents, "%i,%f,%s", (int)pRow->Checked, pRow->Mass, pRow->Comments);
		WritePrivateProfileString("Mass Table", EntryName, EntryContents, theApp.m_psIniFileName);
		j++;
	}
	sprintf(EntryContents, "%i", j);
	WritePrivateProfileString("Mass Table", "EntryCount", EntryContents, theApp.m_psIniFileName);
	sprintf(EntryContents, "%i", m_ScanTime);
	WritePrivateProfileString("Default Doc", "ScanTime", EntryContents, theApp.m_psIniFileName);
}

void CMassSpecDoc::OnFileSaveAs() 
{
	CDocument::OnFileSaveAs();	
}	

BOOL CMassSpecDoc::OnSaveDocument(LPCTSTR lpszPathName) 
{
	return CDocument::OnSaveDocument(lpszPathName);
}

void CMassSpecDoc::SetTitle(LPCTSTR lpszTitle) 
{
	if(lpszTitle)
		CDocument::SetTitle(lpszTitle);

	if(IsWindow(theApp.m_pMainWnd->GetSafeHwnd()))
	{
		CString str="Spec - ";
		if(GetTitle()=="Unnamed")
			str+="[Unnamed]";
		else
		str+=GetTitle();
		if(IsModified())
			str+="*";
		theApp.m_pMainWnd->SetWindowText((LPCSTR)str);
	}
}

void CMassSpecDoc::SaveEasyPlot(char *FileName)
{
	FILE *fp=fopen(FileName,"w");
	CTime time = CTime::GetCurrentTime();
	int i, RegionsOff=0;
	for(i=0;i<m_MassTable.size();i++)
		if(!m_MassTable[i].Checked)
			RegionsOff++;
	fprintf(fp, ";File \"%s\", saved by \'Spec\' in %s\n", FileName, time.Format("%b %d, %Y at %H:%M:%S"));
	fprintf(fp, ";Total scan time: %.2i:%.2i:%.2i. ",  m_PassedCommonTime/1000/60/60,(m_PassedCommonTime/1000/60)%60, m_PassedCommonTime/1000%60);
	if(RegionsOff==0)
		fprintf(fp, "Number of regions: %i\n", m_MassTable.size());
	else 
		fprintf(fp, ";Number of regions: %i (%i of them %s off)\n", m_MassTable.size(), RegionsOff,(RegionsOff==1)?"is":"are");
	fprintf(fp, ";| Reg| Mass | Comments \n");
	int Count=1;
	for (i=0;i<m_MassTable.size();i++)
	{
		if(!m_MassTable[i].Checked)
			continue;
		fprintf(fp, "; %2i.  %2s  %s\n", Count, (LPCSTR)PrintfFTruncate(m_MassTable[i].Mass,2), m_MassTable[i].Comments);
		Count++;
	}
	fprintf(fp,(LPCSTR)PrintEasyPlotData());

	fclose(fp);
}

CString CMassSpecDoc::PrintEasyPlotData()
{
	CString sData, str;
	//Добавление легенды = Comments
	int i, Count=2;
	if(!m_vRefTemp.empty() && !m_vMeasTemp.empty())
	{
		str.Format("/sa l \"%s\" %i\n","T Ref.", Count++);
		sData+=str;
		str.Format("/sa l \"%s\" %i\n", "T Meas." , Count++);
		sData+=str;
	}
	for (i=0;i<m_MassTable.size();i++)
	{
		if(!m_MassTable[i].Checked)
			continue;
		if( strlen(m_MassTable[i].Comments)>0 && strlen(m_MassTable[i].Comments) < sizeof(m_MassTable[i].Comments) )
			str.Format("/sa l \"%s\" %i\n",m_MassTable[i].Comments, Count);
		else
			str.Format("/sa l \"%s\" %i\n",(LPCSTR)PrintfFTruncate(m_MassTable[i].Mass,2), Count);
		sData+=str;
		Count++;
	}
	str.Format("/td \"x");
	sData+=str;

	if(!m_vRefTemp.empty() && !m_vMeasTemp.empty())
		sData+="pp";
	for (i=0;i<m_MassTable.size();i++)
	{
		if(!m_MassTable[i].Checked)
			continue;
		str.Format("y");
		sData+=str;
	}
	str.Format("\"\n");
	sData+=str;
	int j;
	bool FirstVal;
	for (j=0; ; j++)
	{
		FirstVal=true;
		for (i=0;i<m_MassTable.size();i++)
		{
			if(!m_MassTable[i].Checked)
				continue;
			if(j>=m_MassTable[i].vIntensity.size())
				goto End;
			if(FirstVal)
			{
				str.Format("%.2f", m_MassTable[i].vTime[j]);
				sData+=str;
				if(j<m_vRefTemp.size() && j<m_vMeasTemp.size())
				{
					str.Format(" %.1f %.1f", m_vRefTemp[j], m_vMeasTemp[j]);
					sData+=str;
				}
			}
			FirstVal=false;
			str.Format(" %.3f", m_MassTable[i].vIntensity[j]);
			sData+=str;
		}
		str.Format("\n");
		sData+=str;
	}
End:
	return sData;
}
void CMassSpecDoc::CopyToClipboard() 
{
    LPTSTR  lptstrCopy; 
    HGLOBAL hglbCopy; 
	CString sData=PrintEasyPlotData();

    if (!OpenClipboard(theApp.m_pMainWnd->GetSafeHwnd())) 
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
    CloseClipboard(); 
}

void CMassSpecDoc::OnUpdateEditCopy(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
}

void CMassSpecDoc::SetModifiedFlag(bool Modified)
{
	CDocument::SetModifiedFlag(Modified);
	SetTitle(NULL);
}

void CMassSpecDoc::FillCurrentTemperatures()
{
	CSpecView* pView=GetView();
	if(!pView)
		return;
	m_vRefTemp.push_back(pView->m_ThermoReg.m_Hist.LastRefTemp);
	m_vMeasTemp.push_back(pView->m_ThermoReg.m_Hist.LastMeasTemp);
}

CSpecView* CMassSpecDoc::GetView()
{
   POSITION pos = GetFirstViewPosition();
   if (pos != NULL)
   {
      CSpecView* pView = (CSpecView*)GetNextView(pos);
	  return pView;
   }
   return NULL;
}

bool CMassSpecDoc::IsEmpty()
{
	bool MassTableEmpty=true;
	for(size_t i=0; i<m_MassTable.size(); i++)
		if(m_MassTable[i].vIntensity.size()>0)
			MassTableEmpty=false;
	return MassTableEmpty && (m_PassedCommonTime==0) && m_vMeasTemp.empty() && m_vRefTemp.empty();
}
