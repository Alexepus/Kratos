#include "stdafx.h"
#include "Main.h"
#include "Exceptions.h"
#include "BackupFileManager.h"

extern CProgNewApp theApp;
CDoc::CDoc() : _fpPrj(nullptr), XpsProject(this), DxpsProject(this)
{
	sprintf(m_EasyPlotFile.FileName, "");
	sprintf(m_EasyPlotFile.FullPath, "");
	sprintf(m_EasyPlotFile.Dir, "");

	m_OriginFile.FullPath[0] = '\0';
	m_OriginFile.FileName[0] = '\0';
	m_OriginFile.Dir[0] = '\0';

	m_EasyPlotFile.NeedSave = Need;

	m_ProjectFile.FullPath[0] = '\0';
	m_ProjectFile.FileName[0] = '\0';
	m_ProjectFile.Dir[0] = '\0';
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
			ShowWindow(theApp.m_pMainFrame->m_pHideWnd->m_hWnd, SW_HIDE);
			ShowWindow(theApp.m_pMainFrame->m_Graph.m_hWnd, SW_SHOW);
			theApp.m_pMainFrame->m_Graph.PlotData();
		}
		else
		{
			ShowWindow(theApp.m_pMainFrame->m_pHideWnd->m_hWnd, SW_SHOW);
			ShowWindow(theApp.m_pMainFrame->m_Graph.m_hWnd, SW_HIDE);
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

/**
* \brief Полное пересохранение файла с переоткрытием и с возможным усечением
*/
void CDoc::SaveProjectFile()
{
	CloseFileIfNeed();
	CBackupFileManager backup(m_ProjectFile.FullPath);
	OpenFile(m_ProjectFile.FullPath, FileOpenMode::ExistingOrNew);
	if (theApp.m_pMainFrame->m_Doc.m_DocType == CDoc::XPS || theApp.m_pMainFrame->m_Doc.m_DocType == CDoc::NoDoc)
		XpsProject.SaveProject(_fpPrj);
	else if (theApp.m_pMainFrame->m_Doc.m_DocType == CDoc::DXPS)
		DxpsProject.SaveProject(_fpPrj);
	fflush(_fpPrj);
	m_NeedSave = NoNeed;
	backup.DeleteBackup();
}

void CDoc::SaveProjectAs(CString filePath)
{
	sprintf(m_ProjectFile.FullPath, filePath.GetString());
	OpenFile(filePath, FileOpenMode::ExistingOrNew);
	SaveProjectFile();
}

bool CDoc::IsFileOpen() const
{
	return _fpPrj != nullptr;
}

FILE* CDoc::GetProjectFilePointer()
{
	if (_fpPrj == nullptr)
		throw EXCEPTION("Файл проекта не открыт или открыт неправильно (CDoc::_fpPrj == nullptr)");
	return _fpPrj;
}

void CDoc::CloseFileIfNeed()
{
	if (_fpPrj != nullptr)
		fclose(_fpPrj); // при закрытии внутри делается fflush
	_fpPrj = nullptr;
}

void CDoc::OpenFile(CString filePath, FileOpenMode mode)
{
	_fpPrj = fopen(filePath.GetString(), mode == FileOpenMode::Existing? "rb+" : "wb+");
	if (!_fpPrj)
		throw EXCEPTION_SPECIFIC(FileOpenException, (std::string("Can`t open project file: ") + std::strerror(errno)).c_str());
}

void CDoc::OpenProjectFile(CString filePath)
{
	CloseFileIfNeed();
	OpenFile(filePath, FileOpenMode::Existing);
	unsigned char key[4];

	if (!fread(key, sizeof(unsigned char), 4, _fpPrj))
		throw EXCEPTION("ERROR read 'key' of project file. The file is empty.");
	int FileVersion = key[3];
	if (key[0] == KRATOS_KEY0 && key[1] == KRATOS_KEY1 && key[2] == XPS_TYPE)
		theApp.m_pMainFrame->m_Doc.m_DocType = CDoc::XPS;
	else if (key[0] == KRATOS_KEY0 && key[1] == KRATOS_KEY1 && key[2] == DXPS_TYPE)
		theApp.m_pMainFrame->m_Doc.m_DocType = CDoc::DXPS;
	else 
		throw EXCEPTION("This file is not a Kratos project file");

	if (theApp.m_pMainFrame->m_Doc.m_DocType == CDoc::XPS)
		XpsProject.ReadXpsFile(_fpPrj, FileVersion);
	else if (theApp.m_pMainFrame->m_Doc.m_DocType == CDoc::DXPS)
		DxpsProject.ReadProject(_fpPrj, FileVersion);
		
	if (!feof(_fpPrj)) //must be: eof==0
	{
		char ch = '\0';
		fread(&ch, 1, 1, _fpPrj);
		if (feof(_fpPrj)) //must be: eof!=0
			return;
		Msg("Warning: Project file has extra data.");
	}
}


void CDoc::EmptyAllData()
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
			fprintf(fpOutput, " Comments=%s\n", m_DataIn.Comments.GetString());
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