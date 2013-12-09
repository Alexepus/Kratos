// TermoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Termo.h"
#include "TermoDlg.h"
#include "ComFunctions.h"
#include "ComThread.h"
#include "ThreadSafeQueue2.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CTermoApp theApp;
CThreadSafeQueue2 Queue;

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTermoDlg dialog

CTermoDlg::CTermoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTermoDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTermoDlg)
	m_EditResponse = _T("");
	m_EditCommand3_0 = "0";
	m_EditCommand3_1 = "0";
	m_EditCommand3_2 = "0";
	m_CheckScan = FALSE;
	m_CheckSkipProcessing = FALSE;
	m_CheckViewRawData = FALSE;
	m_CheckViewSent = FALSE;
	m_CheckRequestReply = TRUE;
	m_EditCommand2_0 = "0";
	m_EditCommand1_0 = "0";
	m_EditCommand2_1 = "0";
	m_EditCommand4_0 = "0";
	m_EditCommand4_1 = "0";
	m_EditCommand4_2 = "0";
	m_EditCommand4_3 = "0";
	m_CheckHex = FALSE;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	theApp.pTermoDlg=this;
}

void CTermoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTermoDlg)
	DDX_Control(pDX, IDC_CHECK_SKIP_PROCESSING, m_ControlCheckSkip);
	DDX_Text(pDX, IDC_EDIT_RESPONSE, m_EditResponse);
	DDX_Text(pDX, IDC_EDIT_COMMAND3_0, m_EditCommand3_0);
	DDX_Text(pDX, IDC_EDIT_COMMAND3_1, m_EditCommand3_1);
	DDX_Text(pDX, IDC_EDIT_COMMAND3_2, m_EditCommand3_2);
	DDX_Check(pDX, IDC_CHECK_SKIP_PROCESSING, m_CheckSkipProcessing);
	DDX_Check(pDX, IDC_CHECK_RAW, m_CheckViewRawData);
	DDX_Check(pDX, IDC_CHECK_VIEW_SENT, m_CheckViewSent);
	DDX_Check(pDX, IDC_CHECK_REQUEST_REPLY, m_CheckRequestReply);
	DDX_Text(pDX, IDC_EDIT_COMMAND2_0, m_EditCommand2_0);
	DDX_Text(pDX, IDC_EDIT_COMMAND1_0, m_EditCommand1_0);
	DDX_Text(pDX, IDC_EDIT_COMMAND2_1, m_EditCommand2_1);
	DDX_Text(pDX, IDC_EDIT_COMMAND4_0, m_EditCommand4_0);
	DDX_Text(pDX, IDC_EDIT_COMMAND4_1, m_EditCommand4_1);
	DDX_Text(pDX, IDC_EDIT_COMMAND4_2, m_EditCommand4_2);
	DDX_Text(pDX, IDC_EDIT_COMMAND4_3, m_EditCommand4_3);
	DDX_Check(pDX, IDC_CHECK_HEX, m_CheckHex);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTermoDlg, CDialog)
	//{{AFX_MSG_MAP(CTermoDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_CLEAN, OnButtonClean)
	ON_BN_CLICKED(IDC_BUTTON_COMMAND3, OnButtonCommand3)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_CHECK_RAW, OnCheckRaw)
	ON_BN_CLICKED(IDC_CHECK_SKIP_PROCESSING, OnCheckSkipProcessing)
	ON_BN_CLICKED(IDC_CHECK_VIEW_SENT, OnCheckViewSent)
	ON_BN_CLICKED(IDC_CHECK_REQUEST_REPLY, OnCheckRequestReply)
	ON_BN_CLICKED(IDC_BUTTON_COMMAND1, OnButtonCommand1)
	ON_BN_CLICKED(IDC_BUTTON_COMMAND2, OnButtonCommand2)
	ON_BN_CLICKED(IDC_BUTTON_COMMAND4, OnButtonCommand4)
	ON_BN_CLICKED(IDC_CHECK_HEX, OnCheckHex)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTermoDlg message handlers

BOOL CTermoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
/*	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}
*/
	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	theApp.hComThread=AfxBeginThread( (AFX_THREADPROC) ComThread, (LPVOID) NULL);

	m_EditCommand1_0=theApp.GetProfileString("Interface", "EditCommand1_0", 0);

	m_EditCommand2_0=theApp.GetProfileString("Interface", "EditCommand2_0", 0);
	m_EditCommand2_1=theApp.GetProfileString("Interface", "EditCommand2_1", 0);

	m_EditCommand3_0=theApp.GetProfileString("Interface", "EditCommand3_0", 0);
	m_EditCommand3_1=theApp.GetProfileString("Interface", "EditCommand3_1", 0);
	m_EditCommand3_2=theApp.GetProfileString("Interface", "EditCommand3_2", 0);

	m_EditCommand4_0=theApp.GetProfileString("Interface", "EditCommand4_0", 0);
	m_EditCommand4_1=theApp.GetProfileString("Interface", "EditCommand4_1", 0);
	m_EditCommand4_2=theApp.GetProfileString("Interface", "EditCommand4_2", 0);
	m_EditCommand4_3=theApp.GetProfileString("Interface", "EditCommand4_3", 0);

	m_CheckViewSent=theApp.ViewSentData;

	m_CheckViewRawData=theApp.ViewRawData;
	m_CheckSkipProcessing=theApp.SkipProcessing;
	m_CheckHex=theApp.GetProfileInt("Interface", "HEX", 0);

	UpdateData(FALSE);
	OnCheckRaw();



	// TODO: Add extra initialization here
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CTermoDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CTermoDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CTermoDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}



void CTermoDlg::UartCommand(int BytesToTransmit)
{
	unsigned long ReadFromPort, WrittenToPort;
	char Buffer[256];
	LPTSTR lpMsgBuf;
	char ComName[20];
	strcpy(ComName,(LPCSTR)theApp.ComPortName);
	HANDLE hComPort=CreateFile(ComName, GENERIC_WRITE|GENERIC_READ, 0, NULL, OPEN_EXISTING , 
	FILE_ATTRIBUTE_NORMAL, NULL);
	if(hComPort==INVALID_HANDLE_VALUE)
	{

		FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_SYSTEM| 
		FORMAT_MESSAGE_IGNORE_INSERTS, NULL, GetLastError(),MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR) &lpMsgBuf, 0, NULL);
		Msg("Ошибка открытия порта %s.\n%s", &ComName[0],(LPCTSTR)lpMsgBuf);
		LocalFree(lpMsgBuf);
		return;
	}
	ConfigureComPort(hComPort);
	PurgeComm(hComPort,PURGE_RXCLEAR|PURGE_RXABORT);
	if (!WriteFile(hComPort, &UartBuffer, BytesToTransmit , &WrittenToPort, NULL))
		{
					FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_SYSTEM| 
						FORMAT_MESSAGE_IGNORE_INSERTS, NULL, GetLastError(),MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
						(LPTSTR) &lpMsgBuf, 0, NULL);
					Msg("WriteFile: ошибка записи в порт.\n%s",lpMsgBuf);
					return;
		}
	if (DetectSerialErrors(hComPort)!=1)
		return;
	if (!ReadFile(hComPort, Buffer, 255, &ReadFromPort, NULL)) 
		{
				FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_SYSTEM| 
				FORMAT_MESSAGE_IGNORE_INSERTS, NULL, GetLastError(),MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
				(LPTSTR) &lpMsgBuf, 0, NULL);
				Msg("ReadFile: ошибка чтения из порта.\n%s",lpMsgBuf);
				return;
		}
		if (DetectSerialErrors(hComPort)!=1)
		return;

	CString Value;
	if (ReadFromPort==1)
		Value.Format("1 Byte: %i", (BYTE)Buffer[0]);
	else if (ReadFromPort==2)
		Value.Format("2 Bytes: %i (%.2X, %.2X)", (int)(BYTE)(Buffer[0])*256+(BYTE)Buffer[1], (BYTE)Buffer[0], (BYTE)Buffer[1]);
	else 		
		Value.Format("%i Bytes", ReadFromPort);

	m_EditResponse+=Value;
	m_EditResponse+="\r\n>";
	CloseHandle(hComPort);
}
void CTermoDlg::UartCommand(BYTE Byte0, BYTE Byte1)
{
	ComMsg msg;
	msg.Buf[0]=Byte0;
	msg.Buf[1]=Byte1; 
	msg.MsgSize=2;
	SendComMessage(msg);
}
void CTermoDlg::OnButtonClean() 
{
UpdateData(TRUE);
m_EditResponse="";
UpdateData(FALSE);	
}


void CTermoDlg::OnOK()
{
}

void CTermoDlg::OnCheckScan() 
{
UpdateData();
static BOOL PortWasOpen=FALSE;
if(m_CheckScan)
{
	LPTSTR lpMsgBuf;
	char ComName[20];
	strcpy(ComName,(LPCSTR)theApp.ComPortName);
	PortWasOpen=FALSE;
	if((hComPort==NULL) ||(hComPort==INVALID_HANDLE_VALUE))
	{
		hComPort=CreateFile(ComName, GENERIC_WRITE|GENERIC_READ, 0, NULL, OPEN_EXISTING , 
			FILE_ATTRIBUTE_NORMAL, NULL);
		PortWasOpen=TRUE;
		if(hComPort==INVALID_HANDLE_VALUE)
		{

			FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_SYSTEM| 
			FORMAT_MESSAGE_IGNORE_INSERTS, NULL, GetLastError(),MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPTSTR) &lpMsgBuf, 0, NULL);
			Msg("Ошибка открытия порта %s.\n%s", &ComName[0],(LPCTSTR)lpMsgBuf);
			LocalFree(lpMsgBuf);
			return;
		}
		ConfigureComPort(hComPort);
		PurgeComm(hComPort,PURGE_RXCLEAR|PURGE_RXABORT);
	}

	SetTimer(15,0,NULL);
	SetTimer(16,0,NULL);
	SetTimer(17,0,NULL);
	SetTimer(18,0,NULL);
	SetTimer(19,0,NULL);
}
else
{
	KillTimer(15);
	KillTimer(16);
	KillTimer(17);
	KillTimer(18);
	KillTimer(19);

	if(PortWasOpen)
	{
		CloseHandle(hComPort);
		hComPort=INVALID_HANDLE_VALUE;
	}
}
}

void CTermoDlg::OnTimer(UINT nIDEvent) 
{
	BYTE Buffer[255];
	DWORD ReadFromPort;
	char lpMsgBuf;
	CString TotalStr;
	if(	(nIDEvent>=15) && (nIDEvent<=19))
	{
		for(int j=0; j<10; j++)
		{
			if (!ReadFile(hComPort, Buffer, 3, &ReadFromPort, NULL)) 
			{
					FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_SYSTEM| 
					FORMAT_MESSAGE_IGNORE_INSERTS, NULL, GetLastError(),MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
					(LPTSTR) &lpMsgBuf, 0, NULL);
					Msg("ReadFile: ошибка чтения из порта.\n%s",lpMsgBuf);
					LocalFree(&lpMsgBuf);
					return;
			}

			CString Value;
			if (ReadFromPort==1)
				Value.Format("0x%.2X (%i)", (BYTE)Buffer[0],(BYTE)Buffer[0]);
			else
			{
				if (ReadFromPort==2)
					Value.Format("0x%.2X, 0x%.2X", (BYTE)Buffer[0], (BYTE)Buffer[1]);
			
				else 
				{
					if (ReadFromPort==3)		
						Value.Format("0x%.2X, 0x%.2X, 0x%.2X", (BYTE)Buffer[0], (BYTE)Buffer[1], (BYTE)Buffer[2]);
					else 
					{
						if (ReadFromPort>0)
							Value.Format("0x%.2X, 0x%.2X, 0x%.2X, 0x%.2X... (%i B)", (BYTE)Buffer[0], (BYTE)Buffer[1], (BYTE)Buffer[2], (BYTE)Buffer[2], ReadFromPort);
						else
						{
							CDialog::OnTimer(nIDEvent);
							continue;
							//return;
						}
					}
				}
			}
			TotalStr+=Value;
			TotalStr+="\r\n>";
			//UpdateData(FALSE);
		}
		if(TotalStr.GetLength()>0)
		{
			char *ch;
			int len=GetDlgItem(IDC_EDIT_RESPONSE)->SendMessage(WM_GETTEXTLENGTH,NULL,NULL);
			ch=new char[len+1];
			theApp.pTermoDlg->GetDlgItemText(IDC_EDIT_RESPONSE,ch,len+1);
			TotalStr.Insert(0, ch+((len>10000)?5000:0));
			GetDlgItem(IDC_EDIT_RESPONSE)->SetWindowText(TotalStr);
			delete[] ch;
		}
	}
	CDialog::OnTimer(nIDEvent);
}

void CTermoDlg::OnCheckRaw() 
{
UpdateData();
theApp.ViewRawData=m_CheckViewRawData;
m_ControlCheckSkip.EnableWindow(m_CheckViewRawData);	
}

void CTermoDlg::OnCheckSkipProcessing() 
{
UpdateData();
theApp.SkipProcessing=m_CheckSkipProcessing;	
}

BOOL CTermoDlg::DestroyWindow() 
{
	UpdateData();

	theApp.WriteProfileString("Interface", "EditCommand1_0", m_EditCommand1_0);

	theApp.WriteProfileString("Interface", "EditCommand2_0", m_EditCommand2_0);
	theApp.WriteProfileString("Interface", "EditCommand2_1", m_EditCommand2_1);

	theApp.WriteProfileString("Interface", "EditCommand3_0", m_EditCommand3_0);
	theApp.WriteProfileString("Interface", "EditCommand3_1", m_EditCommand3_1);
	theApp.WriteProfileString("Interface", "EditCommand3_2", m_EditCommand3_2);
	
	theApp.WriteProfileString("Interface", "EditCommand4_0", m_EditCommand4_0);
	theApp.WriteProfileString("Interface", "EditCommand4_1", m_EditCommand4_1);
	theApp.WriteProfileString("Interface", "EditCommand4_2", m_EditCommand4_2);
	theApp.WriteProfileString("Interface", "EditCommand4_3", m_EditCommand4_3);

	theApp.WriteProfileInt("Interface", "ViewRawData", m_CheckViewRawData);
	theApp.WriteProfileInt("Interface", "SkipProcessing", m_CheckSkipProcessing);
	theApp.WriteProfileInt("Interface", "ViewSentData", m_CheckViewSent);
	theApp.WriteProfileInt("Interface", "HEX", m_CheckHex);

	return CDialog::DestroyWindow();
}

void CTermoDlg::OnCheckViewSent() 
{
UpdateData();
theApp.ViewSentData=m_CheckViewSent;	
}

void CTermoDlg::OnCheckRequestReply() 
{
UpdateData();
}

void CTermoDlg::OnButtonCommand1() 
{
UpdateData();

	BYTE Byte0=(BYTE)strtoul((LPCSTR)m_EditCommand1_0, NULL, m_CheckHex? 16:10);
	if(m_CheckHex) 	m_EditCommand1_0.Format("%.2X", (int)Byte0);
	else			m_EditCommand1_0.Format("%i", (int)Byte0);
	ComMsg msg;
	msg.Buf[0]=Byte0;
	msg.MsgSize=1;
	msg.WaitForReply=(m_CheckRequestReply==TRUE);
	SendComMessage(msg);
UpdateData(FALSE);}

void CTermoDlg::OnButtonCommand2() 
{
UpdateData();
	BYTE Byte0=(BYTE)strtoul((LPCSTR)m_EditCommand2_0, NULL, m_CheckHex? 16:10);
	if(m_CheckHex) 	m_EditCommand2_0.Format("%.2X", (int)Byte0);
	else			m_EditCommand2_0.Format("%i", (int)Byte0);
	BYTE Byte1=(BYTE)strtoul((LPCSTR)m_EditCommand2_1, NULL, m_CheckHex? 16:10);
	if(m_CheckHex) 	m_EditCommand2_1.Format("%.2X", (int)Byte1);
	else			m_EditCommand2_1.Format("%i", (int)Byte1);

	ComMsg msg;
	msg.Buf[0]=Byte0;
	msg.Buf[1]=Byte1; 
	msg.MsgSize=2;
	msg.WaitForReply=(m_CheckRequestReply==TRUE);
	SendComMessage(msg);
UpdateData(FALSE);}

void CTermoDlg::OnButtonCommand3() 
{
UpdateData();
	BYTE Byte0=(BYTE)strtoul((LPCSTR)m_EditCommand3_0, NULL, m_CheckHex? 16:10);
	if(m_CheckHex) 	m_EditCommand3_0.Format("%.2X", (int)Byte0);
	else			m_EditCommand3_0.Format("%i", (int)Byte0);
	BYTE Byte1=(BYTE)strtoul((LPCSTR)m_EditCommand3_1, NULL, m_CheckHex? 16:10);
	if(m_CheckHex) 	m_EditCommand3_1.Format("%.2X", (int)Byte1);
	else			m_EditCommand3_1.Format("%i", (int)Byte1);
	BYTE Byte2=(BYTE)strtoul((LPCSTR)m_EditCommand3_2, NULL, m_CheckHex? 16:10);
	if(m_CheckHex) 	m_EditCommand3_2.Format("%.2X", (int)Byte2);
	else			m_EditCommand3_2.Format("%i", (int)Byte2);

	ComMsg msg;
	msg.Buf[0]=Byte0;
	msg.Buf[1]=Byte1; 
	msg.Buf[2]=Byte2;
	msg.MsgSize=3;
	msg.WaitForReply=(m_CheckRequestReply==TRUE);
	SendComMessage(msg);
UpdateData(FALSE);	
}


void CTermoDlg::OnButtonCommand4() 
{
UpdateData();

	BYTE Byte0=(BYTE)strtoul((LPCSTR)m_EditCommand4_0, NULL, m_CheckHex? 16:10);
	if(m_CheckHex) 	m_EditCommand4_0.Format("%.2X", (int)Byte0);
	else			m_EditCommand4_0.Format("%i", (int)Byte0);
	BYTE Byte1=(BYTE)strtoul((LPCSTR)m_EditCommand4_1, NULL, m_CheckHex? 16:10);
	if(m_CheckHex) 	m_EditCommand4_1.Format("%.2X", (int)Byte1);
	else			m_EditCommand4_1.Format("%i", (int)Byte1);
	BYTE Byte2=(BYTE)strtoul((LPCSTR)m_EditCommand4_2, NULL, m_CheckHex? 16:10);
	if(m_CheckHex) 	m_EditCommand4_2.Format("%.2X", (int)Byte2);
	else			m_EditCommand4_2.Format("%i", (int)Byte2);
	BYTE Byte3=(BYTE)strtoul((LPCSTR)m_EditCommand4_3, NULL, m_CheckHex? 16:10);
	if(m_CheckHex) 	m_EditCommand4_3.Format("%.2X", (int)Byte3);
	else			m_EditCommand4_3.Format("%i", (int)Byte3);

	ComMsg msg;
	msg.Buf[0]=Byte0;
	msg.Buf[1]=Byte1; 
	msg.Buf[2]=Byte2;
	msg.Buf[3]=Byte3;
	msg.MsgSize=4;
	msg.WaitForReply=(m_CheckRequestReply==TRUE);
	SendComMessage(msg);
UpdateData(FALSE);}

void CTermoDlg::OnCheckHex() 
{
	UpdateData();
	BYTE Byte0=(BYTE)strtoul((LPCSTR)m_EditCommand1_0, NULL, m_CheckHex? 10:16);
	if(m_CheckHex) 	m_EditCommand1_0.Format("%.2X", (int)Byte0);
	else			m_EditCommand1_0.Format("%i", (int)Byte0);


	Byte0=(BYTE)strtoul((LPCSTR)m_EditCommand2_0, NULL, m_CheckHex? 10:16);
	if(m_CheckHex) 	m_EditCommand2_0.Format("%.2X", (int)Byte0);
	else			m_EditCommand2_0.Format("%i", (int)Byte0);
	BYTE Byte1=(BYTE)strtoul((LPCSTR)m_EditCommand2_1, NULL, m_CheckHex? 10:16);
	if(m_CheckHex) 	m_EditCommand2_1.Format("%.2X", (int)Byte1);
	else			m_EditCommand2_1.Format("%i", (int)Byte1);


	Byte0=(BYTE)strtoul((LPCSTR)m_EditCommand3_0, NULL, m_CheckHex? 10:16);
	if(m_CheckHex) 	m_EditCommand3_0.Format("%.2X", (int)Byte0);
	else			m_EditCommand3_0.Format("%i", (int)Byte0);
	Byte1=(BYTE)strtoul((LPCSTR)m_EditCommand3_1, NULL, m_CheckHex? 10:16);
	if(m_CheckHex) 	m_EditCommand3_1.Format("%.2X", (int)Byte1);
	else			m_EditCommand3_1.Format("%i", (int)Byte1);
	BYTE Byte2=(BYTE)strtoul((LPCSTR)m_EditCommand3_2, NULL, m_CheckHex? 10:16);
	if(m_CheckHex) 	m_EditCommand3_2.Format("%.2X", (int)Byte2);
	else			m_EditCommand3_2.Format("%i", (int)Byte2);


	Byte0=(BYTE)strtoul((LPCSTR)m_EditCommand4_0, NULL, m_CheckHex? 10:16);
	if(m_CheckHex) 	m_EditCommand4_0.Format("%.2X", (int)Byte0);
	else			m_EditCommand4_0.Format("%i", (int)Byte0);
	Byte1=(BYTE)strtoul((LPCSTR)m_EditCommand4_1, NULL, m_CheckHex? 10:16);
	if(m_CheckHex) 	m_EditCommand4_1.Format("%.2X", (int)Byte1);
	else			m_EditCommand4_1.Format("%i", (int)Byte1);
	Byte2=(BYTE)strtoul((LPCSTR)m_EditCommand4_2, NULL, m_CheckHex? 10:16);
	if(m_CheckHex) 	m_EditCommand4_2.Format("%.2X", (int)Byte2);
	else			m_EditCommand4_2.Format("%i", (int)Byte2);
	BYTE Byte3=(BYTE)strtoul((LPCSTR)m_EditCommand4_3, NULL, m_CheckHex? 10:16);
	if(m_CheckHex) 	m_EditCommand4_3.Format("%.2X", (int)Byte3);
	else			m_EditCommand4_3.Format("%i", (int)Byte3);
	UpdateData(FALSE);


}
