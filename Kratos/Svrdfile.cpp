#include"stdafx.h"
#include"Main.h"
#include <limits>
#include "Time.h"

extern CProgNewApp theApp;
extern CString FileSaveOpenErrorDescription;

void SaveEasyPlotFile(FILE* fp, char* FileName)
{
CRegion* pReg;    //º 186
char* Caption[]={"|Reg |","KE/BE|","Anode|"," HV |","  Start  |","  Finish |"," Step |","  N |","  n |"," Time |","  Start Time  |", "   End Time   |", };
int i;
int NCaption = sizeof(Caption)/sizeof(char*);
int NLine=0;
int RegionsOff=0;
for (pReg=CRegion::GetFirst(); pReg!=NULL; pReg=CRegion::GetNext(pReg))
	if(pReg->m_DataIn.Off) RegionsOff++;
if(RegionsOff==0)
	fprintf(fp, ";Number of regions: %i\n", CRegion::m_NReg);
else 
	fprintf(fp, ";Number of regions: %i (%i of them %s off)\n", CRegion::m_NReg, RegionsOff,(RegionsOff==1)?"is":"are");
++NLine;
fprintf(fp,";");
for(i=0; i<NCaption; ++i) fprintf(fp,"%s",Caption[i]);
fprintf(fp,"\n"); ++NLine;
for(pReg=CRegion::GetFirst(); pReg!=NULL; pReg=CRegion::GetNext(pReg))
	{
	if(pReg->m_DataIn.Off)
		continue;

	if(NLine >= 25)
		{
		fprintf(fp,"\n;");
		for(i=0; i<NCaption; ++i) fprintf(fp,"%s",Caption[i]);
		NLine = 0;
		fprintf(fp,"\n"); ++NLine;
		}
	NLine += SaveEasyPlotTable(fp, pReg);
	}
fprintf(fp,";\n;\n");

for(pReg=CRegion::GetFirst(); pReg!=NULL; pReg=CRegion::GetNext(pReg))
	{
	if(pReg->m_DataIn.Off)
		continue;
	fprintf(fp,";");
	for(i=0; i<NCaption; ++i) fprintf(fp,"%s",Caption[i]);
	fprintf(fp,"\n"); //++NLine;
	SaveEasyPlotTable(fp, pReg);
	fprintf(fp,";Points count %i\n",pReg->m_NDataOut);
	fprintf(fp,"/td \"xy\"\n");
	fprintf(fp,"/sa l \"%s:%i,\" 2\n", FileName, pReg->ID+1);
	for(i=0; i<pReg->m_NDataOut; ++i)
		{fprintf(fp,"%.3lf  %.3f\n", I2D(pReg->m_pDataOut[i].x), pReg->m_pDataOut[i].y);
		}
	fprintf(fp,"//nc\n;\n");
	}
}
//------
int SaveEasyPlotTable(FILE* fp, CRegion* pReg)
{
int NLine = 0;
char Value[16];
	sprintf(Value,"R%i",pReg->ID+1);
	fprintf(fp,";|%4s|",Value);
	if(pReg->m_DataIn.KE_BE == DATA_IN::EnergyType::KE)
		{sprintf(Value," %s", "KE");
		 fprintf(fp,"%5s|",Value);
		 //sprintf(Value,"%s", "None");
		 sprintf(Value,"%s", CRegion::h_nu_Info.strName_h_nu[pReg->m_DataIn.N_h_nu]);
		 fprintf(fp,"%5s|",Value);
		}
	else
		{sprintf(Value," %s", "BE");
		 fprintf(fp,"%5s|",Value);
		 sprintf(Value,"%s", CRegion::h_nu_Info.strName_h_nu[pReg->m_DataIn.N_h_nu]);
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
	fprintf(fp, "%14s|", FormatTime(pReg->m_BeginTime, "%d.%m.%Y %H:%M").GetString());
	fprintf(fp, "%14s|", FormatTime(pReg->m_EndTime, "%d.%m.%Y %H:%M").GetString());
	fprintf(fp,"\n"); ++NLine;
	fprintf(fp,";%s\n",pReg->m_DataIn.Comments); ++NLine;
return NLine;
}

//---------

void SaveOrigin(FILE* fp, char* FileName)
{
CRegion* pReg;
int iL = 0;
int i;

fprintf(fp,"%c ", '-');
for(pReg=CRegion::GetFirst(); pReg!=NULL; pReg=CRegion::GetNext(pReg))
	{fprintf(fp, "R%i ", pReg->ID+1);}
fprintf(fp,"\n");

fprintf(fp,"%s ", "KE/BE");
for(pReg=CRegion::GetFirst(); pReg!=NULL; pReg=CRegion::GetNext(pReg))
	{
	if(pReg->m_DataIn.KE_BE == DATA_IN::EnergyType::KE) fprintf(fp, "%s ", "KE");
	else fprintf(fp, "%s ", "BE"); 
	}
fprintf(fp,"\n");

fprintf(fp,"%s ", "HV");
for(pReg=CRegion::GetFirst(); pReg!=NULL; pReg=CRegion::GetNext(pReg))
	{fprintf(fp, "%i ", (int) I2D(pReg->m_DataIn.HV) ); }
fprintf(fp,"\n");

char strWord[256];
for(i=0; i<3; ++i)
	{
	fprintf(fp,"%c ", '-');
	for(pReg=CRegion::GetFirst(); pReg!=NULL; pReg=CRegion::GetNext(pReg))
		{
		GetWord(pReg, strWord, i+1);
		fprintf(fp, "%s ", strWord);
		}
	fprintf(fp,"\n");
	}

char strFormat[512];
char strF[32];
char strL[512];
char strR[512];
sprintf(strF, "%c%s ", '%', ".3lf");
for(pReg=CRegion::GetFirst(), iL=0; pReg!=NULL; pReg=CRegion::GetNext(pReg), ++iL)
	{
	strL[0] = '\0';
	strR[0] = '\0';
	for(i=0; i<iL; ++i) strcat(strL, "- ");
	for(i=iL+1; i<CRegion::m_NReg ; ++i) strcat(strR, "- ");
	sprintf(strFormat, "%s%s%s%s\n", strF, strL, strF, strR);
	for(i=0; i<pReg->m_NDataOut; ++i)
		{fprintf(fp,strFormat, I2D(pReg->m_pDataOut[i].x), pReg->m_pDataOut[i].y);
		}
	}
}

void GetWord(CRegion* pReg, char* strWord, int nW)
{
int i=0;
int k=0;
int m=0;
BOOL InWord;
char* str = pReg->m_DataIn.Comments;

sprintf(strWord, "%c", '-');
while(str[i]==' ' || str[i]=='\t') ++i;
InWord = TRUE; //OutWord = FALSE;
m=1;
while(str[i] != '\0')
	{
	if(str[i]!=' ' && str[i]!='\t' ) 
		{
		InWord = TRUE;
		if(m == nW) {strWord[k] = str[i]; ++k; strWord[k] = '\0'; } 
		}
	else
		{
		if(InWord)
			{
			InWord = FALSE;
			strWord[k] = '\0';
			k=0;
			//++m;
			if(m==nW) return;
			else sprintf(strWord, "%c", '-');
			++m;
			}	
		}
	++i;
	}
}