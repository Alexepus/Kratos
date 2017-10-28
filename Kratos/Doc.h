#ifndef _DOC_H_INCLUDED_
#define _DOC_H_INCLUDED_
// File  Doc.h

//#include "Threads.h"	// Added by ClassView
//struct THREAD_COMMON;

#include "ViewWnd.h"	// Added by ClassView
struct  FILE_NAME
{
char FullPath[MAX_PATH + 1];
char FileName[MAX_PATH + 1];
char Dir[MAX_PATH + 1];
int NeedSave;
};



class CDoc
{
public: //members
	CGraphics m_Graph;
	FILE_NAME m_EasyPlotFile;
	FILE_NAME m_ProjectFile;
	FILE_NAME m_OriginFile;
	enum {EasyPlot, Project, EasyPlotMC, Origin} m_TypeFile;
	enum {SaveAs, Open} m_SaveAsOpen;
	enum {Need, NoNeed} m_NeedSave;
	enum {NoDoc=-1, XPS, DXPS} m_DocType;
	FILE* fpPrj;

	CViewWnd m_ViewWnd;
	char m_WindowCaption[MAX_PATH+10];
	CWinThread* m_pMeasThread;
	THREAD_COMMON m_ThrComm;
	CDoc();
	int CheckDocType(); //Checks document data if doc is NoDoc, XPS or DXPS;
						//Sets m_DocType, hides inproper windows and enables/disables buttons
};

#endif // _DOC_H_INCLUDED_