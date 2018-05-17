// ProgNew.cpp : Defines the class behaviors for the application.
//
/*
#include "stdafx.h"
#include "ProgNew.h"
#include "BigClientWnd.h"
#include "HideWnd.h"
#include "MainFrame.h"
*/
#include "stdafx.h"
#include "Main.h"
//#include "ProgNewDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CProgNewApp

BEGIN_MESSAGE_MAP(CProgNewApp, CWinApp)
	//{{AFX_MSG_MAP(CProgNewApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CProgNewApp construction

CProgNewApp::CProgNewApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance

}

/////////////////////////////////////////////////////////////////////////////
// The one and only CProgNewApp object

FARPROC EditNormalProc;
FARPROC EditNumberProc;
UINT MY_WM_RESIZE;
CMutex	MutexThread;
CCriticalSection CrSecGraph;
CProgNewApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CProgNewApp initialization

BOOL CProgNewApp::InitInstance()
{
	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif


	LogFile("--- Start Program ---");
	CMainFrame* mainWnd = new CMainFrame;
	m_pMainFrame=mainWnd;


	if(!FirstInstance())
        return FALSE;

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
      wndcls.lpszClassName = _T("KratosWindowClass");

      // Register the new class and exit if it fails
      AfxRegisterClass(&wndcls);
	
	//mainWnd->m_hWnd=NULL;

	mainWnd->m_pHideWnd=NULL;
	MY_WM_RESIZE = ::RegisterWindowMessage("MY_WM_RESIZE");
	if(!mainWnd) {AfxMessageBox("Can't allock memory for CMainFrame"); return FALSE;};	
	if(!(mainWnd->LoadFrame(IDR_MAINFRAME, WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN)))
		{AfxMessageBox("Can't create MainFrame"); return FALSE;}
	m_pMainWnd = mainWnd;
								
	
	//if(mainWnd->m_hWnd==NULL) AfxMessageBox("mainWnd->m_hWnd == NULL");
	
	m_pMainWnd->ShowWindow(SW_SHOWNORMAL);							
	//mainWnd->m_hMenu = ::GetMenu(m_pMainWnd->m_hWnd);
	//::EnableMenuItem(mainWnd->m_hMenu, ID_FILE_SAVE_AS_EASYPLOT, MF_ENABLED);
	
	WNDCLASS WC;
	::GetClassInfo(AfxGetInstanceHandle(), "edit", &WC);
	EditNumberProc = (FARPROC) WC.lpfnWndProc;

	CString errMessage;
	if(Ini.UseUsbCounter.Value && !Ini.UsbCounterSimulation.Value)
	{
		if(!m_AdamCom.ReconnectCom(Ini.CounterComPort.Value, &errMessage))
			Msg("Ошибка открытия порта %s.\n%s", Ini.CounterComPort.Value.GetString(),errMessage.GetString());
	}
	return TRUE;
}

////=====

// Форматирование текста
CString Format(LPCTSTR lpszFormat, ...)
{
	va_list args;
	va_start(args, lpszFormat);
	CString str;
	str.FormatV(lpszFormat, args);
	return str;
}

void Msg(const std::string message)
{
	::AfxMessageBox(message.c_str(), MB_OK | MB_ICONINFORMATION);
}

void Msg(const CString message)
{
	::AfxMessageBox(message.GetString(), MB_OK | MB_ICONINFORMATION);
}

void Msg(LPCTSTR lpszFormat, ...)
{
//Очень полезная функция для вывода сообщений при отладке. 
//Пример: Msg("Current Point=%i, Problem Value=%f", k, pReg->m_pDataOut[k].y);
va_list args;
va_start(args, lpszFormat);
CString str;
str.FormatV(lpszFormat, args);

::AfxMessageBox(str,MB_OK|MB_ICONINFORMATION);
}

void MsgLog(LPCTSTR lpszFormat, ...)
{
	//Очень полезная функция для вывода сообщений при отладке. 
	//Пример: Msg("Current Point=%i, Problem Value=%f", k, pReg->m_pDataOut[k].y);
	va_list args;
	va_start(args, lpszFormat);
	CString str;
	str.FormatV(lpszFormat, args);

	CString logStr = str;
	logStr.Replace("\n", "");
	int firstNotWhiteSpace, lastNotWhiteSpace;
	for(firstNotWhiteSpace = 0; firstNotWhiteSpace < logStr.GetLength(); firstNotWhiteSpace++)
	{
		if(logStr[firstNotWhiteSpace] != ' ')
			break;
	}
	for(lastNotWhiteSpace = logStr.GetLength() - 1; lastNotWhiteSpace >= firstNotWhiteSpace ; lastNotWhiteSpace--)
	{
		if(logStr[lastNotWhiteSpace] != ' ')
			break;
	}
	logStr = logStr.Mid(firstNotWhiteSpace, lastNotWhiteSpace - firstNotWhiteSpace + 1);
	LogFileFormat(logStr);

	::AfxMessageBox(str,MB_OK|MB_ICONINFORMATION);
}

LRESULT CALLBACK EditSuperNumberProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
switch(msg)
	{
	case WM_CHAR:
		{
		char c = (char) wParam;
		LONG style;
		style = ::GetWindowLong(hWnd, GWL_STYLE);
		switch(c)
			{
			case '+':
			case '-':
			case '.':
			case 'e':
			case 'E':
				if( (style&ES_NUMBER) == ES_NUMBER) 
					{
					style = style & (~ES_NUMBER);
					::SetWindowLong(hWnd, GWL_STYLE, style);
					}
				break;
			default:
				if( (style&ES_NUMBER) != ES_NUMBER) 
					{
					style = style | ES_NUMBER;
					::SetWindowLong(hWnd, GWL_STYLE, style);
					}
				break;

			}// end switch(c)
		return ::CallWindowProc((WNDPROC) EditNumberProc, hWnd, msg, wParam, lParam);
		}//end case WM_CHAR:
	
	/*
	case WM_KEYDOWN:
		//::MessageBeep(0xFFFFFFFF);
		{
		//AfxMessageBox("case WM_KEYDOWN:");
		int VK = (int) wParam;
		LONG style;
		style = ::GetWindowLong(hWnd, GWL_STYLE);
		switch(VK)
			{
			case VK_SPACE:
			//case VK_E:
			//case 'e':
				if( (style&ES_NUMBER) == ES_NUMBER) 
					{
					style = style & (~ES_NUMBER);
					::SetWindowLong(hWnd, GWL_STYLE, style);
					}
				break;
				//return ::CallWindowProc((WNDPROC) EditNumberlProc, hWnd, msg, wParam, lParam);
			default:
				if( (style&ES_NUMBER) != ES_NUMBER) 
					{
					style = style | ES_NUMBER;
					::SetWindowLong(hWnd, GWL_STYLE, style);
					}
				break;
				//return ::CallWindowProc((WNDPROC) EditNumberProc, hWnd, msg, wParam, lParam);
			}
		return ::CallWindowProc((WNDPROC) EditNumberProc, hWnd, msg, wParam, lParam);
		//break;
		}
	*/
	default:
		//AfxMessageBox("SubClassingWindows()");
		return ::CallWindowProc((WNDPROC) EditNumberProc, hWnd, msg, wParam, lParam);
	}
return 0;
}

BOOL CProgNewApp::FirstInstance()
{
    CWnd *pWndPrev, *pWndChild;

    // Determine if another window with your class name exists...
    if (pWndPrev = CWnd::FindWindow(_T("KratosWindowClass"),NULL))
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

int CProgNewApp::ExitInstance() 
{
	// TODO: Add your specialized code here and/or call the base class
   ::UnregisterClass(_T("KratosWindowClass"),AfxGetInstanceHandle());

	return CWinApp::ExitInstance();
}
