// CamSetupHPDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Hardware.h"
#include "Main.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CProgNewApp theApp;
/////////////////////////////////////////////////////////////////////////////
// CCamSetupHPDlg dialog


CCamSetupHPDlg::CCamSetupHPDlg(CWinApp* pApp, CWnd* pParent /*=NULL*/)
	: CDialog(CCamSetupHPDlg::IDD, pParent)
{	
	m_UsbCounter = theApp.Ini.UseUsbCounter.Value;

	//{{AFX_DATA_INIT(CCamSetupDlg)
	m_CountPos = 0;
	m_CrateNum = 0;
	m_RegistRetardPos = 0;
	m_RegistHVPos = 0;
	m_TimerPos = 0;
	m_CrateTest = _T("Unknown");
	m_RegistRetardTest = _T("Unknown");
	m_RegistHVTest = _T("Unknown");
	m_CountTest = m_UsbCounter?_T("N/A") : _T("Unknown");
	m_TimerTest = m_UsbCounter?_T("N/A") : _T("Unknown");
	//}}AFX_DATA_INIT

	m_CrateNum=theApp.Ini.CrateNumber.Value;
	m_RegistRetardPos=theApp.Ini.RegisterRetardPosition.Value;
	m_RegistHVPos=theApp.Ini.RegisterHVPosition.Value;
	m_CountPos=theApp.Ini.CounterPosition.Value;
	m_TimerPos=theApp.Ini.TimerPosition.Value;

	if(m_CrateNum>4||m_CrateNum<0) m_CrateNum=0;
	if(m_RegistRetardPos>23||m_RegistRetardPos<1) m_RegistRetardPos=20;
	if(m_RegistHVPos>23||m_RegistHVPos<1) m_RegistHVPos=22;
	if(m_CountPos>23||m_CountPos<1) m_CountPos=5;
	if(m_TimerPos>23||m_TimerPos<1) m_TimerPos=9;
}


void CCamSetupHPDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCamSetupHPDlg)
	DDX_Text(pDX, IDC_EDIT_HP_COUNT_POS, m_CountPos);
	DDV_MinMaxUInt(pDX, m_CountPos, 1, 23);
	DDX_Text(pDX, IDC_EDIT_HP_CRATE_NUM, m_CrateNum);
	DDV_MinMaxInt(pDX, m_CrateNum, 0, 1);
	DDX_Text(pDX, IDC_EDIT_HP_REGIST_RETARD_POS, m_RegistRetardPos);
	DDV_MinMaxUInt(pDX, m_RegistRetardPos, 1, 23);
	DDX_Text(pDX, IDC_EDIT_HP_REGIST_HV_POS, m_RegistHVPos);
	DDV_MinMaxUInt(pDX, m_RegistHVPos, 1, 23);
	DDX_Text(pDX, IDC_EDIT_HP_TIMER_POS, m_TimerPos);
	DDV_MinMaxUInt(pDX, m_TimerPos, 1, 23);
	DDX_Text(pDX, IDC_EDIT_HP_COUNT_TEST, m_CountTest);
	DDX_Text(pDX, IDC_EDIT_HP_CRATE_TEST, m_CrateTest);
	DDX_Text(pDX, IDC_EDIT_HP_REGIST_RETARD_TEST, m_RegistRetardTest);
	DDX_Text(pDX, IDC_EDIT_HP_REGIST_HV_TEST, m_RegistHVTest);
	DDX_Text(pDX, IDC_EDIT_HP_TIMER_TEST, m_TimerTest);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_EDIT_HP_COUNT_POS, EditCountPos);
	DDX_Control(pDX, IDC_EDIT_HP_TIMER_POS, EditTimerPos);
}


BEGIN_MESSAGE_MAP(CCamSetupHPDlg, CDialog)
	//{{AFX_MSG_MAP(CCamSetupHPDlg)
	ON_BN_CLICKED(IDC_BUTTON_HP_TEST, OnTest)
	ON_EN_KILLFOCUS(IDC_EDIT_HP_CRATE_NUM, OnKillfocusEditCrateNum)
	ON_EN_KILLFOCUS(IDC_EDIT_HP_COUNT_POS, OnKillfocusEditCountPos)
	ON_EN_KILLFOCUS(IDC_EDIT_HP_REGIST_RETARD_POS, OnKillfocusEditRegistRetardPos)
	ON_EN_KILLFOCUS(IDC_EDIT_HP_REGIST_HV_POS, OnKillfocusEditRegistHVPos)
	ON_EN_KILLFOCUS(IDC_EDIT_HP_TIMER_POS, OnKillfocusEditTimerPos)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CCamSetupHPDlg::OnInitDialog()
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
// CCamSetupHPDlg message handlers
void CCamSetupHPDlg::OnKillfocusEditCrateNum() 
{
UINT CrateNum=m_CrateNum;
UpdateData(TRUE);
if(CrateNum!=m_CrateNum)
{
	m_CrateTest = _T("Unknown");
	UpdateData(FALSE);
}	
}

void CCamSetupHPDlg::OnKillfocusEditRegistRetardPos() 
{
UINT RegistRetardPos=m_RegistRetardPos;
UpdateData(TRUE);
if(RegistRetardPos!=m_RegistRetardPos)
{
	m_RegistRetardTest = _T("Unknown");
	UpdateData(FALSE);
}		
}

void CCamSetupHPDlg::OnKillfocusEditRegistHVPos() 
{
UINT RegistHVPos=m_RegistHVPos;
UpdateData(TRUE);
if(RegistHVPos!=m_RegistHVPos)
{
	m_RegistHVTest = _T("Unknown");
	UpdateData(FALSE);
}		
}

void CCamSetupHPDlg::OnKillfocusEditCountPos() 
{
UINT CountPos=m_CountPos;
UpdateData(TRUE);
if(CountPos!=m_CountPos)
{
	m_CountTest = _T("Unknown");
	UpdateData(FALSE);
}	
}

void CCamSetupHPDlg::OnKillfocusEditTimerPos() 
{
	UINT TimerPos=m_TimerPos;
	UpdateData(TRUE);
	if(TimerPos!=m_TimerPos)
	{
		m_TimerTest = _T("Unknown");
		UpdateData(FALSE);
	}	
}

void CCamSetupHPDlg::OnOK() 
{
	UpdateData(TRUE);

	theApp.Ini.CrateNumber.Value = m_CrateNum;
	theApp.Ini.CrateNumber.Write();	
	theApp.Ini.RegisterRetardPosition.Value =  m_RegistRetardPos;
	theApp.Ini.RegisterRetardPosition.Write();
	theApp.Ini.RegisterHVPosition.Value = m_RegistHVPos;
	theApp.Ini.RegisterHVPosition.Write();
	theApp.Ini.CounterPosition.Value = m_CountPos;
	theApp.Ini.CounterPosition.Write();
	theApp.Ini.TimerPosition.Value = m_TimerPos;
	theApp.Ini.TimerPosition.Write();
	
	CDialog::OnOK();
}

void CCamSetupHPDlg::OnTest() 
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
	
	//Регистр выходной 5Р.850.17, Retard (тормозящеее поле)
	//Запись в регистр данных по A(0) F(16) некоторого небольшого числа,
	//затем проверка по A(0) F(0),
	//затем сброс регистра по A(0) F(9),
	//затем проверка нуля по A(0) F(0).
	//При всех операциях проверяем наличие X и Q (оба должны быть)
	BOOL Success=FALSE;
	Camac_WriteLong(m_CrateNum, m_RegistRetardPos, REGISTER_5P850_A, REGISTER_5P850_WRITE_F, 2L);
	if ((!Camac_ErrorCode())&&Camac_x()&&Camac_q()) 
	{
		long Read;
		Read=Camac_ReadLong(m_CrateNum, m_RegistRetardPos, REGISTER_5P850_A, REGISTER_5P850_READ_F);
		if ((!Camac_ErrorCode())&&Camac_x()&&Camac_q()&&(Read==2L)) 
		{
			Camac_WriteWord(m_CrateNum, m_RegistRetardPos, REGISTER_5P850_A, REGISTER_5P850_CLEAR_F, 0);
			if ((!Camac_ErrorCode())&&Camac_x()&&Camac_q()) 
			{
				Read=Camac_ReadLong(m_CrateNum, m_RegistRetardPos, REGISTER_5P850_A, REGISTER_5P850_READ_F);
				if ((!Camac_ErrorCode())&&Camac_x()&&Camac_q()&&(Read==0L)) 
					Success=TRUE;
			}

		}
	}
	if(Success==TRUE)
		m_RegistRetardTest = _T("OK");
	else
		m_RegistRetardTest = _T("Failed");
	UpdateData(FALSE);

	//Регистр выходной 5Р.850.17, HV
	Success=FALSE;
	Camac_WriteLong(m_CrateNum, m_RegistHVPos, REGISTER_5P850_A, REGISTER_5P850_WRITE_F, 2L);
	if ((!Camac_ErrorCode())&&Camac_x()&&Camac_q()) 
	{
		long Read;
		Read=Camac_ReadLong(m_CrateNum, m_RegistHVPos, REGISTER_5P850_A, REGISTER_5P850_READ_F);
		if ((!Camac_ErrorCode())&&Camac_x()&&Camac_q()&&(Read==2L)) 
		{
			Camac_WriteWord(m_CrateNum, m_RegistHVPos, REGISTER_5P850_A, REGISTER_5P850_CLEAR_F, 0);
			if ((!Camac_ErrorCode())&&Camac_x()&&Camac_q()) 
			{
				Read=Camac_ReadLong(m_CrateNum, m_RegistHVPos, REGISTER_5P850_A, REGISTER_5P850_READ_F);
				if ((!Camac_ErrorCode())&&Camac_x()&&Camac_q()&&(Read==0L)) 
					Success=TRUE;
			}

		}
	}
	if(Success==TRUE)
		m_RegistHVTest = _T("OK");
	else
		m_RegistHVTest = _T("Failed");
	UpdateData(FALSE);
	
	if(!m_UsbCounter)
	{
		//Пересчетка
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
