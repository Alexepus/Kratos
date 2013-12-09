// MassCalibDoc.cpp: implementation of the CMassCalibDoc class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Spec.h"
#include "MassCalibDoc.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
extern CSpecApp theApp;

CMassCalibDoc::CMassCalibDoc()
{
	ReadTable();
}

CMassCalibDoc::~CMassCalibDoc()
{
	SaveTable();
}

void CMassCalibDoc::ReadTable()
{
	char EntryName[50];
	char EntryContents[256];
	CMassTable Row;
	int i, Checked, TableSize;

	GetPrivateProfileString("Mass Calibration", "a", "0", EntryContents, 256, theApp.m_psIniFileName);
	a=atof(EntryContents);
	GetPrivateProfileString("Mass Calibration", "b", "1", EntryContents, 256, theApp.m_psIniFileName);
	b=atof(EntryContents);
	GetPrivateProfileString("Mass Calibration", "c", "0", EntryContents, 256, theApp.m_psIniFileName);
	c=atof(EntryContents);

	GetPrivateProfileString("Mass Calibration", "EntryCount", "0", EntryContents, 256, theApp.m_psIniFileName);
	TableSize=min(atoi(EntryContents),MAX_MASS_COUNT);
	for(i=0; i<TableSize;i++)
	{
		sprintf(EntryName, "Entry_%.2i", i);
		GetPrivateProfileString("Mass Calibration", EntryName, "0,0,", EntryContents, 256, theApp.m_psIniFileName);
		if(strcmp(EntryContents,"0,0,")==0)
			continue;
		memset(Row.Comments,0,sizeof(Row.Comments));
		sscanf(EntryContents, "%i,%lf,%63c", &Checked, &Row.Mass, Row.Comments);
		Row.Checked=(bool)Checked;
		m_MassTable.push_back(Row);
	}
}

void CMassCalibDoc::SaveTable()
{
	char EntryName[50];
	char EntryContents[256];

	sprintf(EntryContents,"%.9g", a);
	WritePrivateProfileString("Mass Calibration", "a", EntryContents, theApp.m_psIniFileName);
	sprintf(EntryContents,"%.9g", b);
	WritePrivateProfileString("Mass Calibration", "b", EntryContents, theApp.m_psIniFileName);
	sprintf(EntryContents,"%.9g", c);
	WritePrivateProfileString("Mass Calibration", "c", EntryContents, theApp.m_psIniFileName);

	sprintf(EntryContents, "%i", (int)m_MassTable.size());
	WritePrivateProfileString("Mass Calibration", "EntryCount", EntryContents, theApp.m_psIniFileName);

	for(int i=0; i<m_MassTable.size();i++)
	{
		sprintf(EntryName, "Entry_%.2i", i);
		CMassTable *pRow=&m_MassTable[i];
		sprintf(EntryContents, "%i,%f,%s", (int)pRow->Checked, pRow->Mass, pRow->Comments);
		WritePrivateProfileString("Mass Calibration", EntryName, EntryContents, theApp.m_psIniFileName);
	}

}


