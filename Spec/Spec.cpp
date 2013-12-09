// Spec.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "Spec.h"

#include "MainFrame.h"
#include "MassSpecDoc.h"
#include "SpecView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSpecApp

BEGIN_MESSAGE_MAP(CSpecApp, CWinApp)
	//{{AFX_MSG_MAP(CSpecApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSpecApp construction

CSpecApp::CSpecApp()
{
	char ModuleName[MAX_PATH];
	::GetModuleFileName(NULL, ModuleName, MAX_PATH);
	CString cstr=ModuleName;
	int EndPath=cstr.ReverseFind('.');
	cstr.Delete(EndPath+1,cstr.GetLength()-EndPath-1);
	cstr+="ini";
	m_psIniFileName= (char*)malloc(max(cstr.GetLength()+1,_MAX_PATH));
	strcpy((char*)m_psIniFileName, (LPCSTR)cstr);

	m_pMassCalibDoc=new CMassCalibDoc;
	m_pThread=0;
	m_pSpecView=0;
	QueryPerformanceFrequency(&Freq);

}

CSpecApp::~CSpecApp()
{
	delete m_pMassCalibDoc;
	free(m_psIniFileName);
	CloseHandle(m_EventThreadExit);

}

/////////////////////////////////////////////////////////////////////////////
// The one and only CSpecApp object

CSpecApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CSpecApp initialization

BOOL CSpecApp::InitInstance()
{
	// Standard initialization

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	if(!FirstInstance())
        return FALSE;
	RegisterWindowClass();

	m_EventThreadExit=CreateEvent(NULL, TRUE, FALSE, NULL);
	//CCalibDlg dlg;
	//dlg.DoModal();
	// Change the registry key under which our settings are stored.
	SetRegistryKey(_T("Spec"));
	LoadStdProfileSettings();  // Load standard INI file options (including MRU)

	// Register document templates

	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CMassSpecDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CSpecView));
	AddDocTemplate(pDocTemplate);

	// Enable DDE Execute open
	EnableShellOpen();
	RegisterShellFileTypes();

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();

	// Enable drag/drop open
	m_pMainWnd->DragAcceptFiles();

	{//Регистрация расширения .msp, ассоциация с путем текущего модуля 
		char ModuleName[MAX_PATH];
		::GetModuleFileName(NULL, ModuleName, MAX_PATH);
		CString str;
		HKEY hKey1,hKey2,hKey3,hKey4;
		RegCreateKey(HKEY_CLASSES_ROOT,"Spec.MassSpectrometer", &hKey1);
		str.Format("Mass-Spectrometer Data file");
		RegSetValueEx(hKey1,NULL, NULL,REG_SZ,(BYTE*)(LPCSTR)str,str.GetLength()+1);
		RegCreateKey(hKey1,"DefaultIcon", &hKey2);
		str.Format("\"%s\",0",ModuleName);
		RegSetValueEx(hKey2,NULL, NULL,REG_SZ,(BYTE*)(LPCSTR)str,str.GetLength()+1);
		RegCloseKey(hKey2);
		RegCreateKey(hKey1,"shell", &hKey2);
		RegCreateKey(hKey2,"open", &hKey3);
		RegCreateKey(hKey3,"command", &hKey4);
		str.Format("\"%s\" \"%%1\"",ModuleName);
		RegSetValueEx(hKey4,NULL, NULL,REG_SZ,(BYTE*)(LPCSTR)str,str.GetLength()+1);
		RegCloseKey(hKey4);
		RegCloseKey(hKey3);
		RegCloseKey(hKey2);
		RegCloseKey(hKey1);

		RegCreateKey(HKEY_CLASSES_ROOT,".msp", &hKey1);
		str.Format("Spec.MassSpectrometer");
		RegSetValueEx(hKey1,NULL, NULL,REG_SZ,(BYTE*)(LPCSTR)str,str.GetLength()+1);
		RegCloseKey(hKey1);
		
		SHChangeNotify(SHCNE_ASSOCCHANGED,SHCNF_IDLIST,0,0);
	}
	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	CString	m_StaticBuildDate;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	m_StaticBuildDate = _T("");
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	DDX_Text(pDX, IDC_STATIC_BUILD_DATE, m_StaticBuildDate);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CSpecApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CSpecApp message handlers


BOOL CAboutDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	char *Date=__DATE__;
	char *Time=__TIME__;
	m_StaticBuildDate.Format("%s %s", Date, Time);
	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

CDocument* CSpecApp::OpenDocumentFile(LPCTSTR lpszFileName) 
{	
	POSITION pos=GetFirstDocTemplatePosition();
	CDocTemplate* pTemplate=GetNextDocTemplate(pos);
	return pTemplate->OpenDocumentFile(lpszFileName);

	//return CWinApp::OpenDocumentFile(lpszFileName);
}

BOOL CSpecApp::FirstInstance()
{
    CWnd *pWndPrev, *pWndChild;

    // Determine if another window with your class name exists...
    if (pWndPrev = CWnd::FindWindow(WINDOW_CLASS_NAME,NULL))
    {
      // If so, does it have any popups?
      pWndChild = pWndPrev->GetLastActivePopup();

      // If iconic, restore the main window
      if (pWndPrev->IsIconic())
         pWndPrev->ShowWindow(SW_RESTORE);

      // Bring the main window or its popup to
      // the foreground
      pWndChild->SetForegroundWindow();

      // and you are done activating the previous one.
      return FALSE;
    }
    // First instance. Proceed as normal.
    else
      return TRUE;

}

void CSpecApp::RegisterWindowClass()
{
      // Register your unique class name that you wish to use
      WNDCLASS wndcls;

      memset(&wndcls, 0, sizeof(WNDCLASS));   // start with NULL
                                              // defaults

      wndcls.style = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
      wndcls.lpfnWndProc = ::DefWindowProc;
      wndcls.hInstance = AfxGetInstanceHandle();
      wndcls.hIcon = LoadIcon(IDR_MAINFRAME); // or load a different
                                              // icon
      wndcls.hCursor = LoadCursor( IDC_ARROW );
      wndcls.hbrBackground = (HBRUSH) ::GetStockObject(BLACK_BRUSH);//(COLOR_WINDOW + 1)
      wndcls.lpszMenuName = NULL;

      // Specify your own class name for using FindWindow later
      wndcls.lpszClassName = WINDOW_CLASS_NAME;

      // Register the new class and exit if it fails
      AfxRegisterClass(&wndcls);
}

int CSpecApp::ExitInstance() 
{
	// TODO: Add your specialized code here and/or call the base class
   ::UnregisterClass(WINDOW_CLASS_NAME,AfxGetInstanceHandle());

	return CWinApp::ExitInstance();
}
