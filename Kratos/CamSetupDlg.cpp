// CamSetupDlg.cpp : implementation file
//

#include "StdAfx.h"
#include "Hardware.h"
#include "Main.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CProgNewApp theApp;
/////////////////////////////////////////////////////////////////////////////
// CCamSetupDlg dialog


CCamSetupDlg::CCamSetupDlg(CWinApp* pApp, CWnd* pParent /*=NULL*/ )
	: CDialog(CCamSetupDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCamSetupDlg)
	m_UsbCounter = theApp.Ini.UseUsbCounter.Value;
	m_CountPos = 0;
	m_CrateNum = 0;
	m_RegistPos = 0;
	m_TimerPos = 0;
	m_CountTest = m_UsbCounter?_T("N/A") : _T("Unknown");
	m_TimerTest = m_UsbCounter?_T("N/A") : _T("Unknown");
	m_CrateTest = _T("Unknown");
	m_RegistTest = _T("Unknown");

	//}}AFX_DATA_INIT

	m_CrateNum=theApp.Ini.CrateNumber.Value;
	m_RegistPos=theApp.Ini.RegisterPosition.Value;
	m_CountPos=theApp.Ini.CounterPosition.Value;
	m_TimerPos=theApp.Ini.TimerPosition.Value;

	if(m_CrateNum>4||m_CrateNum<0) m_CrateNum=0;
	if(m_RegistPos>23||m_RegistPos<1) m_RegistPos=1;
	if(m_CountPos>23||m_CountPos<1) m_CountPos=5;
	if(m_TimerPos>23||m_TimerPos<1) m_TimerPos=9;
}

void CCamSetupDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCamSetupDlg)
	DDX_Text(pDX, IDC_EDIT_COUNT_POS, m_CountPos);
	DDV_MinMaxUInt(pDX, m_CountPos, 1, 23);
	DDX_Text(pDX, IDC_EDIT_CRATE_NUM, m_CrateNum);
	DDV_MinMaxInt(pDX, m_CrateNum, 0, 1);
	DDX_Text(pDX, IDC_EDIT_REGIST_POS, m_RegistPos);
	DDV_MinMaxUInt(pDX, m_RegistPos, 1, 23);
	DDX_Text(pDX, IDC_EDIT_TIMER_POS, m_TimerPos);
	DDV_MinMaxUInt(pDX, m_TimerPos, 1, 23);
	DDX_Text(pDX, IDC_EDIT_COUNT_TEST, m_CountTest);
	DDX_Text(pDX, IDC_EDIT_CRATE_TEST, m_CrateTest);
	DDX_Text(pDX, IDC_EDIT_REGIST_TEST, m_RegistTest);
	DDX_Text(pDX, IDC_EDIT_TIMER_TEST, m_TimerTest);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_EDIT_COUNT_POS, EditCountPos);
	DDX_Control(pDX, IDC_EDIT_TIMER_POS, EditTimerPos);
}


BEGIN_MESSAGE_MAP(CCamSetupDlg, CDialog)
	//{{AFX_MSG_MAP(CCamSetupDlg)
	ON_BN_CLICKED(IDC_TEST, OnTest)
	ON_EN_KILLFOCUS(IDC_EDIT_CRATE_NUM, OnKillfocusEditCrateNum)
	ON_EN_KILLFOCUS(IDC_EDIT_COUNT_POS, OnKillfocusEditCountPos)
	ON_EN_KILLFOCUS(IDC_EDIT_REGIST_POS, OnKillfocusEditRegistPos)
	ON_EN_KILLFOCUS(IDC_EDIT_TIMER_POS, OnKillfocusEditTimerPos)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CCamSetupDlg::OnInitDialog()
{
	BOOL res = CDialog::OnInitDialog();

	if(m_UsbCounter)
	{
		EditCountPos.EnableWindow(FALSE);
		EditTimerPos.EnableWindow(FALSE);
	}
	return res;
}

/////////////////////////////////////////////////////////////////////////////
// CCamSetupDlg message handlers

void CCamSetupDlg::OnOK() 
{
	UpdateData(TRUE);

	theApp.Ini.CrateNumber.Value = m_CrateNum;
	theApp.Ini.CrateNumber.Write();
	theApp.Ini.RegisterPosition.Value = m_RegistPos;
	theApp.Ini.RegisterPosition.Write();
	theApp.Ini.CounterPosition.Value = m_CountPos;
	theApp.Ini.CounterPosition.Write();
	theApp.Ini.TimerPosition.Value = m_TimerPos;
	theApp.Ini.TimerPosition.Write();
	
	CDialog::OnOK();
}


void CCamSetupDlg::OnKillfocusEditCrateNum() 
{
UINT CrateNum=m_CrateNum;
UpdateData(TRUE);
if(CrateNum!=m_CrateNum)
{
	m_CrateTest = _T("Unknown");
	UpdateData(FALSE);
}	
}

void CCamSetupDlg::OnKillfocusEditRegistPos() 
{
UINT RegistPos=m_RegistPos;
UpdateData(TRUE);
if(RegistPos!=m_RegistPos)
{
	m_RegistTest = _T("Unknown");
	UpdateData(FALSE);
}		
}

void CCamSetupDlg::OnKillfocusEditCountPos() 
{
UINT CountPos=m_CountPos;
UpdateData(TRUE);
if(CountPos!=m_CountPos)
{
	m_CountTest = _T("Unknown");
	UpdateData(FALSE);
}	
}

void CCamSetupDlg::OnKillfocusEditTimerPos() 
{
	UINT TimerPos=m_TimerPos;
	UpdateData(TRUE);
	if(TimerPos!=m_TimerPos)
	{
		m_TimerTest = _T("Unknown");
		UpdateData(FALSE);
	}	
}

void CCamSetupDlg::OnTest() 
{
	if(Camac_Open(m_CrateNum) == -1)
	{
		m_CrateTest = _T("Failed");
		UpdateData(FALSE);
		return;
	}

	if(!Camac_Version())
	{
		AfxMessageBox("Camac driver is not installed.", MB_ICONSTOP|MB_OK, 0);
		return;
	}
	UpdateData(TRUE);
	Camac_EnableMessage(FALSE); //camac driver won't display MessageBoxes
	Camac_Reset(m_CrateNum);                                 
	if (!Camac_ErrorCode())
	{	
		BOOL AnyBlockExist=FALSE;
		for(int i=1; i<=23; i++) //scanning CAMAC block positions
		{
			Camac_WriteWord(m_CrateNum, i, 0, 16,0);
			if(Camac_x()&&(!Camac_ErrorCode()))
				AnyBlockExist=TRUE;
		}
		if (AnyBlockExist)
		{
			m_CrateTest = _T("OK");
			UpdateData(FALSE);
		}
		else
		{
			m_CrateTest = _T("Failed");
			UpdateData(FALSE);
		}
	}
	else
	{
		m_CrateTest = _T("Failed");
		UpdateData(FALSE);
	}
		
	Camac_WriteLong(m_CrateNum, m_RegistPos, 0, REGISTER_POT_WRITE_F, 0L);
	if ((!Camac_ErrorCode())&&Camac_x()) //Q is not used in the block
	{
		m_RegistTest = _T("OK");
		UpdateData(FALSE);
	}
	else
	{
		m_RegistTest = _T("Failed");
		UpdateData(FALSE);
	}

	if(!m_UsbCounter)
	{
		Camac_ReadLong(m_CrateNum, m_CountPos, 0, COUNTER_READ_RESET_F);
		if ((!Camac_ErrorCode())&&Camac_x()&&Camac_q()) 
		{
			Camac_WriteWord(m_CrateNum, m_CountPos, 0, 9, 0);
			Camac_WriteWord(m_CrateNum, m_CountPos, 0, 8, 0);
			if((!Camac_ErrorCode())&&Camac_x()&&(!Camac_q()))
			{
				m_CountTest = _T("OK");
				UpdateData(FALSE);
			}
			else
			{
			m_CountTest = _T("Failed");
			UpdateData(FALSE);
			}	
		}
		else
		{
			m_CountTest = _T("Failed");
			UpdateData(FALSE);
		}	

		Camac_WriteWord(m_CrateNum, m_TimerPos, 1, TIMER_WRITE_START_F, 0);
		if ((!Camac_ErrorCode())&&Camac_x()&&(!Camac_q())) 
		{
			Camac_WriteWord(m_CrateNum, m_TimerPos, 1, TIMER_RESET_F, 0);
			Camac_WriteWord(m_CrateNum, m_TimerPos, 1, 8, 0);
			if((!Camac_ErrorCode())&&Camac_x()&&Camac_q())
			{
				m_TimerTest = _T("OK");
				UpdateData(FALSE);
			}
			else
			{
				m_TimerTest = _T("Failed");
				UpdateData(FALSE);
			}	
		}
		else
		{
			m_TimerTest = _T("Failed");
			UpdateData(FALSE);
		}
	}
	Camac_Close(m_CrateNum);
}
