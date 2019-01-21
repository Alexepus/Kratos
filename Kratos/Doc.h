#pragma once
#include "ViewWnd.h"
#include "XpsProjectFile.h"
#include "DxpsProjectFile.h"

struct  FILE_NAME
{
char FullPath[MAX_PATH + 1];
char FileName[MAX_PATH + 1];
char Dir[MAX_PATH + 1];
int NeedSave;
};

class CDoc : public IProjectFilePointerProvider
{
	FILE* _fpPrj;

public: //members
	CDoc();

	CGraphics m_Graph;
	FILE_NAME m_EasyPlotFile;
	FILE_NAME m_ProjectFile;
	FILE_NAME m_OriginFile;
	enum {EasyPlot, Project, EasyPlotMC, Origin} m_TypeFile;
	enum {SaveAs, Open} m_SaveAsOpen;
	enum {Need, NoNeed} m_NeedSave;
	enum {NoDoc=-1, XPS, DXPS} m_DocType;

	enum class FileOpenMode
	{
		Existing,
		ExistingOrNew
	};

	CViewWnd m_ViewWnd;
	char m_WindowCaption[MAX_PATH+10];
	CWinThread* m_pMeasThread;
	THREAD_COMMON m_ThrComm;
	int CheckDocType(); //Checks document data if doc is NoDoc, XPS or DXPS;
						//Sets m_DocType, hides inproper windows and enables/disables buttons

	XpsProjectFile XpsProject;
	DxpsProjectFile DxpsProject;
	
	void OpenProjectFile(CString filePath);
	/**
	 * \brief ѕолное пересохранение файла с переоткрытием и с возможным усечением
	 */
	void SaveProjectFile();
	void SaveProjectAs(CString filePath);

	void EmptyAllData();

	bool IsFileOpen() const;
	FILE* GetProjectFilePointer() override;
	void CloseFileIfNeed();

private:
	void OpenFile(CString filePath, FileOpenMode mode);
};