#if !defined(AFX_MAINFRAME_H__113AB9B3_6F91_11D5_9A4B_008048FD9845__INCLUDED_)
#define AFX_MAINFRAME_H__113AB9B3_6F91_11D5_9A4B_008048FD9845__INCLUDED_

#include "ntgraph.h"	// Added by ClassView
#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// MainFrame.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMainFrame frame

/*  
typedef struct _SandTBarHWND
	{
		HWND Sb; 
		HWND Tb;
	} SandTBarHWND;
*/
#define WM_POSTCREATEWINDOW WM_USER+1
class CRegion;
class CRegionWnd;
class CDxpsDlg;
class CHideWnd;

class CMainFrame : public CFrameWnd
{
//	DECLARE_DYNCREATE(CMainFrame)
//protected:
public:
	CMainFrame();           // protected constructor used by dynamic creation

// Attributes
public:

// Operations
public:
	UINT m_RegDxpsMessageID;
	char AppTitle[30];
	CNTGraph m_Graph;
	LRESULT OnPostCreateWindow(WPARAM WParam, LPARAM LParam);
	BOOL m_bSynchronousResize;
//	CWinThread* m_pMeasThread;
//	FILE* fpPrj;
//	HMENU m_hMenu;
	CDoc m_Doc;
	//const char* m_ClassNameRegionWnd;
	CRegion* m_pRegion;
	CRegionWnd* m_pRegionWnd;
	CDxpsDlg* m_pDxpsDlg;

	enum{Start,Stop} m_StartStop;
	void RecalcClient();
	CHideWnd* m_pHideWnd;
	BOOL CreateHideWnd();
	TBBUTTON* CreateStructTBBUTTON();
	HWND m_hToolBar;
	HWND m_hStatusBar;
	SAndTBarHWND m_SAndTBarHWND;	
	static const int m_DefaultDpi = 96;
	int m_ScreenDpi;
	enum StatusBarPart {StatusBarPartCoordinates = 0, StatusBarPartXPSParams = 1, StatusBarPartTime = 2,  StatusBarPartTemperature = 3};

	BOOL CreateClient(LPCREATESTRUCT lpcs); 
	BOOL CreateStatusAndToolBar();
	void ResizeStatusBar();
	void SetRecentProjects();
	int FindMenuItem(CMenu* Menu, LPCTSTR MenuString);
	void InsertProjectToRecent(LPCTSTR ProjectFullPath);
	void OnFileRecentProjects(int Index);
	LRESULT OnRemoteDxpsStart(WPARAM WParam, LPARAM LParam);
	LRESULT OnRemoteCurrentTempMessage(WPARAM WParam, LPARAM LParam);
	void SetStatusTemperature(double Temp, bool Defined = true);
	void SetStatusTime(int Seconds, bool Defined = true);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
protected:
	void OnFileRecentProjectsFile1();
	void OnFileRecentProjectsFile2();
	void OnFileRecentProjectsFile3();
	void OnFileRecentProjectsFile4();
	virtual ~CMainFrame();

	// Generated message map functions
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnFileNew();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	afx_msg void OnRegions();
	afx_msg void OnDestroy();
	afx_msg void OnFileExit();
	afx_msg void OnFileSaveAsEasyPlot();
	afx_msg void OnUpdateFileSaveAsEasyPlot(CCmdUI* pCmdUI);
	afx_msg void OnFileSaveProjectAs();
	afx_msg void OnUpdateFileSaveProjectAs(CCmdUI* pCmdUI);
	afx_msg void OnProgrammStart();
	afx_msg void OnUpdateProgrammStart(CCmdUI* pCmdUI);
	afx_msg void OnProgrammStop();
	afx_msg void OnUpdateProgrammStop(CCmdUI* pCmdUI);
	afx_msg void OnSettingsCamacSetup();
	afx_msg void OnSettingsHardwareSetup();
	afx_msg void OnSettingsMeasuringOptions();
	afx_msg void OnFileOpenProject();
	afx_msg void OnUpdateFileOpenProject(CCmdUI* pCmdUI);
	afx_msg void OnGraphicsFont();
	afx_msg void OnFileSaveProject();
	afx_msg void OnUpdateFileSaveProject(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFileNewProject(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFileExit(CCmdUI* pCmdUI);
	afx_msg void OnClose();
	afx_msg void OnGraphicsGrid();
	afx_msg void OnUpdateGraphicsGrid(CCmdUI* pCmdUI);
	afx_msg void OnGraphicsSettingsForCurvers();
	afx_msg void OnGraphicsSettingsForViewer();
	afx_msg void OnUpdateGraphicsSettingsForViewer(CCmdUI* pCmdUI);
	afx_msg void OnFileSaveAsEasyplotMc();
	afx_msg void OnUpdateFileSaveAsEasyplotMc(CCmdUI* pCmdUI);
	afx_msg void OnSettingsFitable();
	afx_msg void OnFileSaveAsOrigin();
	afx_msg void OnUpdateFileSaveAsOrigin(CCmdUI* pCmdUI);
	afx_msg void OnProgramDxpsTable();
	afx_msg void OnUpdateProgramDxpsTable(CCmdUI* pCmdUI);
	afx_msg void OnUpdateProgramRegionTable(CCmdUI* pCmdUI);
	afx_msg void OnAbout();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	int NumberToolbarsButton;
	
public:
	afx_msg void OnSettingsCountersetup();
	afx_msg void OnUpdateSettingsCounterSetup(CCmdUI *pCmdUI);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRAME_H__113AB9B3_6F91_11D5_9A4B_008048FD9845__INCLUDED_)
