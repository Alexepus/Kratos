// FiTable.cpp: implementation of the CFiTable class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "main.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////


CFiTable::CFiTable()
{
NValues=0;
memset(&HVFi, 0, sizeof(HV_Fi)*256);
CString Path=GetCommandLine();
Path=Path.Mid(1,Path.ReverseFind('\\'));
FileName = Path+"Kratos.fi";
ReadFromFile();
}


BOOL CFiTable::WriteToFile()
{
SortByHV();
KillSeriatedHV();
char key[9]="Fi Table";
FILE *fp=fopen(FileName, "w");
if(fp==NULL) return FALSE;

fwrite(key, sizeof(char), 8, fp);
fwrite(&NValues, sizeof(int), 1, fp);
for (int i=0; i<NValues; i++)
{
	fwrite(&HVFi[i], sizeof(HV_Fi), 1, fp);
}
fclose (fp);
return TRUE;
}

BOOL CFiTable::ReadFromFile()
{
char key[9];key[8]='\0';
FILE *fp=fopen(FileName, "r");
if(fp==NULL) return FALSE;

if (fread(key, sizeof(char),8,fp)<sizeof(char)*8)
{
	fclose(fp);
	return FALSE;
}

if (!fread(&NValues, sizeof(int),1,fp))
{
	if (NValues>255)
		NValues=255;
	fclose(fp);
	return FALSE;
}
if (NValues>255)
	NValues=255;
for (int i=0; i<NValues; i++)
{
	if (!fread(&HVFi[i], sizeof(HV_Fi), 1, fp))
	{
		fclose(fp);
		return FALSE;
	}
}
fclose (fp);
return TRUE;
}

void CFiTable::Replace(int index, double HV, double Fi)
{
if (index>255||index<0)
	{
		AfxMessageBox("Wrong HV-Fi index");
		return;
	}
if (index+1>NValues) NValues=index+1;
HVFi[index].HV=(int)floor(HV+0.5);
HVFi[index].Fi=(float)Fi;
}

double CFiTable::GetInterpolatedFi(double HV)
{
if (NValues==0)
	return 0.;
else if (NValues==1)
	return HVFi[0].Fi;
if (HV<HVFi[0].HV)
	return ((double)HVFi[0].Fi-(double)HVFi[1].Fi)/((double)HVFi[0].HV-(double)HVFi[1].HV)*(HV-(double)HVFi[0].HV);
int i;
for (i=0; i<NValues-1; i++)
{
	if(HVFi[i].HV<=HV&&HVFi[i+1].HV>=HV)
		break;
}
if (HVFi[i].HV==HVFi[i+1].HV)
	return HVFi[i].HV;
else
	return ((double)HVFi[i].Fi-(double)HVFi[i+1].Fi)/((double)HVFi[i].HV-(double)HVFi[i+1].HV)*(HV-(double)HVFi[i].HV);

}

void CFiTable::SortByHV()
{
bool NeedAPassage=TRUE;
HV_Fi HF;
for (int Passage=1; (Passage<NValues)&&NeedAPassage; Passage++)
{
	NeedAPassage=FALSE;
	for (int i=0; i<NValues-Passage; i++)
	{
		if(HVFi[i].HV>HVFi[i+1].HV)
		{
			NeedAPassage=TRUE;
			HF=HVFi[i];
			HVFi[i]=HVFi[i+1];
			HVFi[i+1]=HF;
		}
	}
}
}

void CFiTable::Append (double HV, double Fi)
{
if (NValues<255)
{
	HVFi[NValues].HV=(int)floor(HV+0.5);
	HVFi[NValues].Fi=(float)Fi;
	NValues+=1;
}
}

BOOL CFiTable::HVExist(int HV)
{
for (int i=0; i<NValues; i++)
{
	if (HVFi[i].HV==HV)
		return TRUE;
}
return FALSE;
}

float CFiTable::GetFiByHV(int HV)
{
for (int i=0; i<NValues; i++)
{
	if (HVFi[i].HV==HV)
		return HVFi[i].Fi;
}
return 0.;
}

void CFiTable::KillSeriatedHV()
{
for (int i=0; i<NValues-1; i++)
{
	if (HVFi[i].HV==HVFi[i+1].HV)
	{
		for(int j=i+1; j<NValues-1; j++)
			HVFi[j]=HVFi[j+1];
	NValues--;
	i--;
	}
}
}