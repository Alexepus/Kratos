// DlgAbout.cpp : implementation file
//

#include "stdafx.h"
#include "prognew.h"
#include "DlgAbout.h"
#include "ProgNew.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CProgNewApp theApp;
/////////////////////////////////////////////////////////////////////////////
// CDlgAbout dialog


CDlgAbout::CDlgAbout(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgAbout::IDD, pParent)
	, m_StaticFileCreateDate(_T(""))
{
	//{{AFX_DATA_INIT(CDlgAbout)
	m_StaticBuildDate = _T("");
	//}}AFX_DATA_INIT
}


void CDlgAbout::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAbout)
	DDX_Text(pDX, IDC_STATIC_BUILD_DATE, m_StaticBuildDate);
	DDX_Text(pDX, IDC_STATIC_MODE, m_StaticMode);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_STATIC_VER, m_StaticVer);
	DDX_Text(pDX, IDC_STATIC_FILE_DATE, m_StaticFileCreateDate);
}


BEGIN_MESSAGE_MAP(CDlgAbout, CDialog)
	//{{AFX_MSG_MAP(CDlgAbout)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgAbout message handlers

BOOL CDlgAbout::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	char *Date=__DATE__;
	char *Time=__TIME__;
	if(!theApp.Ini.HighPressureMode.Value)
		m_StaticMode="(KRATOS Mode)";
	else
		m_StaticMode="(High Pressure Mode)";
	CString fileVersion, fileModifyDateTime;
	if(GetFileVersion(fileVersion, fileModifyDateTime))
	{
		CString verStr;
		m_StaticVer.GetWindowTextA(verStr);
		verStr.Replace("<version>", fileVersion);
		m_StaticVer.SetWindowTextA(verStr);
		m_StaticFileCreateDate = fileModifyDateTime;
	}
	m_StaticBuildDate.Format("%s %s (full rebuild)", Date, Time);
	
	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CDlgAbout::GetFileVersion(CString &fileVersion, CString &fileCreateDateTime)
{
	char moduleName[MAX_PATH];
	::GetModuleFileName(NULL, moduleName, MAX_PATH);

	DWORD	dwDummy;
	UINT nSize = ::GetFileVersionInfoSize(moduleName, &dwDummy);
	if (!nSize) return FALSE;	// no version information available

	// allocate memory for the information block
	BYTE* fileVersionInfo = new BYTE[nSize];

	// retrieve version information block
	if (!::GetFileVersionInfo ( moduleName, 0L, nSize, fileVersionInfo) ) 
	{
		delete fileVersionInfo;
		fileVersionInfo = NULL;
		return FALSE;
	}

	// Extract VS_FIXEDFILEINFO part from memory block. This part contains
	// language independant version information.
	void	*lpVoid;
	VS_FIXEDFILEINFO fixedFileInfo;
	if (!::VerQueryValue(fileVersionInfo, _T ("\\"), &lpVoid, &nSize))
		return FALSE;
	memcpy(&fixedFileInfo, lpVoid, nSize);

	fileVersion.Format("%d.%d.%d", HIWORD(fixedFileInfo.dwFileVersionMS),
		LOWORD(fixedFileInfo.dwFileVersionMS), HIWORD(fixedFileInfo.dwFileVersionLS));
	delete fileVersionInfo;
	fileVersionInfo = NULL;
	
	HANDLE hFile = CreateFile(moduleName, GENERIC_READ, FILE_SHARE_READ, NULL,
        OPEN_EXISTING, 0, NULL);
	if(hFile == INVALID_HANDLE_VALUE)
		return FALSE;
    FILETIME ftCreate, ftAccess, ftWrite;
    SYSTEMTIME stUTC, stLocal;

    // Retrieve the file times for the file.
    if (!GetFileTime(hFile, &ftCreate, &ftAccess, &ftWrite))
	{
		CloseHandle(hFile);
        return FALSE;
	}
	CloseHandle(hFile);
    FileTimeToSystemTime(&ftWrite, &stUTC);
    SystemTimeToTzSpecificLocalTime(NULL, &stUTC, &stLocal);

	fileCreateDateTime.Format("%02d.%02d.%d  %02d:%02d:%02d",
        stLocal.wDay, stLocal.wMonth, stLocal.wYear,
		stLocal.wHour, stLocal.wMinute, stLocal.wSecond);
	return TRUE;
}
