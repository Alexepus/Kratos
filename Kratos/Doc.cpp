#include "stdafx.h"
#include "Main.h"
#include "Exceptions.h"

extern CProgNewApp theApp;
CDoc::CDoc()
{
sprintf(m_EasyPlotFile.FileName,"");
sprintf(m_EasyPlotFile.FullPath,"");
sprintf(m_EasyPlotFile.Dir,"");

m_OriginFile.FullPath[0]='\0';
m_OriginFile.FileName[0]='\0';
m_OriginFile.Dir[0]='\0';

m_EasyPlotFile.NeedSave = Need;

m_ProjectFile.FullPath[0]='\0';
m_ProjectFile.FileName[0]='\0';
m_ProjectFile.Dir[0]='\0';
m_ProjectFile.NeedSave = Need;
}

int CDoc::CheckDocType()
{
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
	if(_fpPrj)
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

#define KRATOS_KEY 0x0103
#define DXPS_TYPE 0xbd

void SaveProjectFile(FILE* fp)
{
	if (theApp.m_pMainFrame->m_Doc.m_DocType == CDoc::XPS || theApp.m_pMainFrame->m_Doc.m_DocType == CDoc::NoDoc)
	{
		UINT ptrCurr = 0;
		unsigned char key[4];
		key[0] = 0x01; key[1] = 0x03; key[2] = 0xbc; key[3] = XPS_VER2;
		fseek(fp, ptrCurr, SEEK_SET);
		ptrCurr += (UINT) sizeof(unsigned char)*fwrite(key, sizeof(unsigned char), 4, fp);
		ptrCurr += (UINT) sizeof(int)*fwrite(&CRegion::m_NReg, sizeof(int), 1, fp);
		for (CRegion* pReg = CRegion::GetFirst(); pReg != NULL; pReg = CRegion::GetNext(pReg))
		{
			ptrCurr += sizeof(UINT);
			fwrite(&ptrCurr, sizeof(UINT), 1, fp);
			pReg->m_ptrInFile = ptrCurr;
			ptrCurr += (UINT) sizeof(DATA_IN)*fwrite(&pReg->m_DataIn, sizeof(DATA_IN), 1, fp);
			ptrCurr += (UINT) sizeof(time_t)*fwrite(&pReg->m_BeginTime, sizeof(time_t), 1, fp);
			ptrCurr += (UINT) sizeof(time_t)*fwrite(&pReg->m_EndTime, sizeof(time_t), 1, fp);
			ptrCurr += (UINT) sizeof(int)*fwrite(&pReg->m_NDataOutCurr, sizeof(int), 1, fp);
			ptrCurr += (UINT) sizeof(int)*fwrite(&pReg->m_NDataOut, sizeof(int), 1, fp);
			ptrCurr += (UINT) sizeof(DATA_OUT)*fwrite(pReg->m_pDataOut,
				sizeof(DATA_OUT), pReg->m_NDataOut, fp);
		}
	}
	else if (theApp.m_pMainFrame->m_Doc.m_DocType == CDoc::DXPS)
	{
		/*
		*** Структура DXPS-файла (Ver.1 (0x13)): ***
		____________________________________________________________
		struct key
		{
		00			short	KratosKey=KRATOS_KEY=0x0103;
		02			char	DocumentType=DXPS_TYPE=0xbd;
		03			char	DocumentVersion=DXPS_VER1=0x13;
		};
		04		int	NumberOfRegions;
		08		DxpsRegPar	<SpecificRegion>Parameters[NumberOfRegions];
		08+320N	double		ScanTime;
		16+320N	double		PassedCommonTime;
		24+320N	int			PassedNumberOfPoints;
		28+320N	DxpsOutData	OutData[NumberOfPoints];
		_____________________________________________________________
		где:
		struct DxpsRegPar
		{
		00			BOOL Off;
		04			int Type; //0 if "BE Print", 1 if Division Divident/Divisor
		08			int Divident;
		12			int Divisor;
		16			double BE;
		24			double HV;
		32			int Anode;//0:Al, 1:Mg, 2:He1, 3:He2
		36			double Delay; //Задержка перед началом измерений
		44			double Dwell;	//Время измерения
		52			int ColorIndex;
		56			char Comments[256];
		=312	};

		struct DxpsOutData
		{
		int RegionN;
		double Time;
		double Freq;
		};
		____________________________________________________________________

		*** Структура DXPS-файла (Ver.2 (0x14)): ***
		____________________________________________________________
		struct key
		{
		00			short	KratosKey=KRATOS_KEY=0x0103;
		02			char	DocumentType=DXPS_TYPE=0xbd;
		03			char	DocumentVersion=DXPS_VER2=0x14;
		};
		04		int	NumberOfRegions;
		08		DxpsRegPar	<SpecificRegion>Parameters[NumberOfRegions];
		08+320N	double		ScanTime;
		16+320N double      ScanStartDateTime; // Дата/время начала измерения
		24+320N	double		PassedCommonTime;
		32+320N	int			PassedNumberOfPoints;
		36+320N	DxpsOutData	OutData[NumberOfPoints];
		_____________________________________________________________
		где:
		struct DxpsRegPar
		{
		00			BOOL Off;
		04			int Type; //0 if "BE Print", 1 if Division Divident/Divisor
		08			int Divident;
		12			int Divisor;
		16			double BE;
		24			double HV;
		32			int Anode;//0:Al, 1:Mg, 2:He1, 3:He2
		36			double Delay; //Задержка перед началом измерений
		44			double Dwell;	//Время измерения
		52			int ColorIndex;
		56			char Comments[256];
		=312	};

		struct DxpsOutData
		{
		int RegionN;
		double Time;
		double Freq;
		double Tref; // Задаточная температура, может быть NaN, т.е. не задана
		double Tcur; // Текущая измеренная температура, может быть NaN, т.е. неизвестна
		};
		____________________________________________________________________

		Пояснения по записи файла:
		При изменении параметра региона переписываются все регионы.
		При изменении количества регионов и очистке данных переписывается весь файл.
		При прохождении одного скана (проход всех DXPS регионов) дописываются
		данные всего скана и переписываются пройденное время и количество точек.

		*/
		UINT ptrCurr = 0;
		fseek(fp, ptrCurr, SEEK_SET);
		unsigned char key[4];
		key[0] = (KRATOS_KEY & 0xff00) >> 8; key[1] = KRATOS_KEY & 0xff; key[2] = DXPS_TYPE; key[3] = DXPS_VER2;
		ptrCurr += (UINT) sizeof(unsigned char)*fwrite(key, sizeof(unsigned char), 4, fp);
		WriteDxpsRegionsParam(fp);
		WriteDxpsPoints(fp);
	}
	fflush(fp);
}

void CDoc::OpenProjectFile(CString filePath)
{
	auto fp = fopen(filePath.GetString(), "rb+");
	if (!fp) 
		throw EXCEPTION("Can`t open project file");
	unsigned char key[4];

	try {
		if (!fread(key, sizeof(unsigned char), 4, fp)) //return FALSE;
			throw EXCEPTION("ERROR read 'key' of project file. The file is empty.");
		int FileVersion = key[3];
		if (key[0] == 0x01 && key[1] == 0x03 && key[2] == 0xbc) 
			theApp.m_pMainFrame->m_Doc.m_DocType = CDoc::XPS;
		else if (key[0] == 0x01 && key[1] == 0x03 && key[2] == 0xbd) 
			theApp.m_pMainFrame->m_Doc.m_DocType = CDoc::DXPS;
		else 
			throw EXCEPTION("This file is not a project file");

		if (theApp.m_pMainFrame->m_Doc.m_DocType == CDoc::XPS)
			XpsProject.ReadXpsFile(fp, FileVersion);
		else if (theApp.m_pMainFrame->m_Doc.m_DocType == CDoc::DXPS)
			DxpsProject.ReadProject(fp, FileVersion);
		
		if (!feof(fp)) //must be: eof==0
		{
			char ch = '\0';
			fread(&ch, 1, 1, fp);
			if (feof(fp)) //must be: eof!=0
				return;
			Msg("Warning: Project file has extra data.");
		}
	}
	catch (std::exception& ex)
	{
		FileSaveOpenErrorDescription += ex.what();
	}
}


void EmptyAllData()
{
	CRegion* pReg;
	while ((pReg = CRegion::GetFirst()) != NULL)
	{
		delete pReg;
	}
	if (::IsWindow(theApp.m_pMainFrame->m_pRegionWnd->m_pListRegionWnd->m_hWnd))
		::SendMessage(theApp.m_pMainFrame->m_pRegionWnd->m_pListRegionWnd->m_hWnd, LVM_DELETEALLITEMS, 0, 0);

	CDxpsRegion *pDxpsReg;
	while ((pDxpsReg = CDxpsRegion::GetFirst()) != NULL)
		delete pDxpsReg;
	CDxpsRegion::PassedCommonTime = 0;
	CDxpsRegion::PassedNumberOfPoints = 0;

	if (CDxpsRegion::OutData.size())
		Msg("There are still some DXPS data.");
	theApp.m_pMainFrame->m_pDxpsDlg->FillTable();
}

//________________________________________________________________________
//Записывает входные переметры всех регионов
BOOL WriteDxpsRegionsParam(FILE *fp)
{
	CDxpsRegion *pReg;
	size_t ptrCurr = 0;
	fseek(fp, 4, SEEK_SET);
	int a = CDxpsRegion::GetRegNumber();
	ptrCurr += sizeof(int)*fwrite(&a, sizeof(int), 1, fp);
	for (pReg = CDxpsRegion::GetFirst(); pReg != NULL; pReg = CDxpsRegion::GetNext(pReg))
	{
		ptrCurr += sizeof(DxpsRegPar)*fwrite(&pReg->Parameters,
			sizeof(DxpsRegPar), 1, fp);
	}
	ptrCurr += sizeof(double)*fwrite(&CDxpsRegion::ScanTime, sizeof(double), 1, fp);
	ptrCurr += sizeof(double)*fwrite(&CDxpsRegion::ScanStartDateTime, sizeof(double), 1, fp);
	if (ptrCurr != sizeof(int) + sizeof(DxpsRegPar)*CDxpsRegion::GetRegNumber() + sizeof(double))
		return FALSE;
	return TRUE;
}
//________________________________________________________________________
//Дописывает данные в конец файла, начиная с точки, на которую указывает iter.
//Версия без iter записывает все точки с позиции начала области данных.
//Кроме того, записывает прошедшее время и количество точек
BOOL WriteDxpsPoints(FILE *fp, DxpsOutList::iterator iter)
{
	DxpsOutList::iterator i;
	const int pos_PassedCommonTime = 24;

	fseek(fp, 0, SEEK_END);
	for (i = iter; i != CDxpsRegion::OutData.end(); i++)
	{
		fwrite(&(*i), sizeof(DxpsOutData), 1, fp);
	}
	fseek(fp, pos_PassedCommonTime + sizeof(DxpsRegPar)*CDxpsRegion::GetRegNumber(), SEEK_SET);
	fwrite(&CDxpsRegion::PassedCommonTime, sizeof(double), 1, fp);
	fwrite(&CDxpsRegion::PassedNumberOfPoints, sizeof(int), 1, fp);

	return TRUE;
}

//________________________________________________________________________
//Записывает все точки с позиции начала области данных.
//Кроме того, записывает прошедшее время и количество точек
BOOL WriteDxpsPoints(FILE *fp)
{
	DxpsOutList::iterator i;

	size_t Pos;
	const int pos_PassedCommonTime = 24;
	Pos = pos_PassedCommonTime + sizeof(CDxpsRegion::PassedCommonTime) + sizeof(CDxpsRegion::PassedNumberOfPoints) +
		sizeof(DxpsRegPar)*CDxpsRegion::GetRegNumber();
	fseek(fp, Pos, SEEK_SET); // Начало блока измеренных данных
	for (i = CDxpsRegion::OutData.begin(); i != CDxpsRegion::OutData.end(); i++)
	{
		if ((i->RegionN>CDxpsRegion::GetRegNumber() - 1) || (i->RegionN<0))
		{
			Msg("Out data of incorrect region %i", i->RegionN);
			return FALSE;
		}
		Pos += sizeof(DxpsOutData)*fwrite(&(*i), sizeof(DxpsOutData), 1, fp);
	}
	Pos = pos_PassedCommonTime + sizeof(DxpsRegPar)*CDxpsRegion::GetRegNumber();
	if (fseek(fp, Pos, SEEK_SET))
		Msg("fseek fails");
	Pos += sizeof(double)*fwrite(&CDxpsRegion::PassedCommonTime, sizeof(double), 1, fp);
	if (CDxpsRegion::OutData.size() != CDxpsRegion::PassedNumberOfPoints)
		Msg("WriteDxpsPoints: Incorrect number of points.\nOutData.size()=%i, CDxpsRegion::PassedNumberOfPoints=%i", CDxpsRegion::OutData.size(), CDxpsRegion::PassedNumberOfPoints);
	Pos += sizeof(int)*fwrite(&CDxpsRegion::PassedNumberOfPoints, sizeof(int), 1, fp);

	return TRUE;
}

void ParseXPSFile(FILE *fp)
{
	int i = 0, j, pointer;
	int	NumberOfRegions;
	DATA_IN m_DataIn;
	int		m_NDataOutCurr;
	int		m_NDataOut;
	DATA_OUT DataOut;
	int StartOfDataOut;
	int EstimatedEndOfDataOut;
	int FoundEndOfDataOut;
	char ModuleName[MAX_PATH];
	::GetModuleFileName(NULL, ModuleName, MAX_PATH);
	CString cstr = ModuleName, cstr2;
	int EndPath = cstr.ReverseFind('\\');
	cstr.Delete(EndPath + 1, cstr.GetLength() - EndPath - 1);
	cstr2 = cstr + "ParseXPS.txt";

	FILE* fpOutput = fopen((LPCSTR)cstr2, "wt");
	fseek(fp, 4, SEEK_SET);
	fread(&NumberOfRegions, sizeof(int), 1, fp);
	fprintf(fpOutput, "0x%.4x - NumberOfRegions=%i", 4, NumberOfRegions);
	do
	{
		fseek(fp, i, SEEK_SET);
		fread(&pointer, sizeof(int), 1, fp);
		if (pointer == i + 4)
		{
			fprintf(fpOutput, "\n0x%.4x - ptrCurr\n", i);
			fseek(fp, pointer, SEEK_SET);
			j = sizeof(DATA_IN)*fread(&m_DataIn, sizeof(DATA_IN), 1, fp);
			fprintf(fpOutput, "0x%.4x - m_DataIn:\n", pointer);
			fprintf(fpOutput, " HV=%.3f\n", m_DataIn.HV / 1000.);
			fprintf(fpOutput, " KE_Start=%.3f\n", m_DataIn.KE_Start / 1000.);
			fprintf(fpOutput, " KE_End=%.3f\n", m_DataIn.KE_End / 1000.);
			fprintf(fpOutput, " Step=%.3f\n", m_DataIn.Step / 1000.);
			fprintf(fpOutput, " Time=%.3f\n", m_DataIn.Time / 1000.);
			fprintf(fpOutput, " N_=%i\n", m_DataIn.N_);
			fprintf(fpOutput, " Curr_N=%i\n", m_DataIn.Curr_N);
			fprintf(fpOutput, " Off=%i\n", m_DataIn.Off);
			fprintf(fpOutput, " KE_BE=%i\n", m_DataIn.KE_BE);
			fprintf(fpOutput, " N_h_nu=%i\n", (int)m_DataIn.N_h_nu);
			fprintf(fpOutput, " DeltaVolts=%.3f\n", m_DataIn.DeltaVolts / 1000.);
			fprintf(fpOutput, " Comments=%s\n", m_DataIn.Comments);
			pointer += j;
			fseek(fp, pointer, SEEK_SET);
			j = sizeof(int)*fread(&m_NDataOutCurr, sizeof(int), 1, fp);
			fprintf(fpOutput, "0x%.4x - m_NDataOutCurr=%i\n", pointer, m_NDataOutCurr);
			pointer += j;
			fseek(fp, pointer, SEEK_SET);
			j = sizeof(int)*fread(&m_NDataOut, sizeof(int), 1, fp);
			fprintf(fpOutput, "0x%.4x - m_NDataOut=%i\n", pointer, m_NDataOut);
			pointer += j;
			StartOfDataOut = pointer;
			fprintf(fpOutput, "0x%.4x - (*m_pDataOut)[%i]\n", StartOfDataOut, m_NDataOut);
			EstimatedEndOfDataOut = StartOfDataOut + sizeof(DATA_OUT)*m_NDataOut;
			fprintf(fpOutput, "Estimated end of DataOut: 0x%.4x\n", EstimatedEndOfDataOut);
			fseek(fp, EstimatedEndOfDataOut, SEEK_SET);
			j = sizeof(int)*fread(&pointer, sizeof(int), 1, fp);
			if ((pointer != EstimatedEndOfDataOut + 4) && j>0) //Структура файла нарушена. Выводим весь диапазон как DataOut.
			{
				fprintf(fpOutput, "  Структура файла нарушена\n");
				fseek(fp, StartOfDataOut, SEEK_SET); //Обратно к началу
				j = StartOfDataOut;
				while (1) //Ищем следующий регион
				{
					fseek(fp, j, SEEK_SET);
					fread(&pointer, sizeof(int), 1, fp);
					if ((pointer == j + 4) || feof(fp))
					{
						FoundEndOfDataOut = j;
						fprintf(fpOutput, "  Найден конец DataOut: 0x%.4x\n", FoundEndOfDataOut);
						break;
					}
					j++;
				}

				//Выводим диапазон до найденного конца как DataOut
				int k = 0;
				for (j = StartOfDataOut; j<FoundEndOfDataOut; j += sizeof(DATA_OUT))
				{
					k++;
					fseek(fp, j, SEEK_SET);
					fread(&DataOut, sizeof(int), 1, fp);
					fprintf(fpOutput, "  %3i: 0x%.4x %.3f, %.3f\n", k, j, DataOut.x / 1000., DataOut.y / 1000.);
				}

			}
		}
		i++;
	} while (!feof(fp));
	fclose(fpOutput);
}