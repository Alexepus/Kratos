// SerialCounterDlg.cpp : implementation file
//
#include "stdafx.h"
#include "..\ProgNew.h"
#include "..\Exceptions.h"
#include "SerialCounterDlg.h"
#include "SerialCounterUnit.h"
#include "..\LogToFile.h"
extern CProgNewApp theApp;

// CSerialCounterDlg dialog
#define WM_POSTCREATEWINDOW WM_USER+ 1023
const int TimerID = 123;

IMPLEMENT_DYNAMIC(CSerialCounterDlg, CDialog)

CSerialCounterDlg::CSerialCounterDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSerialCounterDlg::IDD, pParent), m_firstActivating(true), m_portChanged(false),m_disableMsgBox(false)
{
	m_counterUnit = std::auto_ptr<SerialCounterUnit>(new SerialCounterUnit(&theApp.m_AdamCom));
}

CSerialCounterDlg::~CSerialCounterDlg()
{
}

void CSerialCounterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COM_PORT, EditComPort);
	DDX_Control(pDX, IDC_MODULE_AVAILABLE, CheckModuleAvailable);
	DDX_Control(pDX, IDC_RUN_MODE, CheckRunMode);
	DDX_Control(pDX, IDC_COOLING_OK, CheckCoolingOk);
	DDX_Control(pDX, IDC_COUNT_TIME, EditCountTime);
	DDX_Control(pDX, IDC_DETECT_THRESHOLD, EditDetectThreshold);
	DDX_Control(pDX, IDC_COOLING_THRESHOLD, EditCoolingThreshold);
	DDX_Control(pDX, IDC_BUTTON_APPLY, ButtonApply);
	DDX_Control(pDX, IDC_PORT_AVAILABLE, StaticPortAvailable);
	DDX_Control(pDX, IDC_COUNTER_MODULE_NAME, StaticModuleName);
	DDX_Control(pDX, IDC_STATIC_CUR_COUNT_TIME, StaticCurCountTime);
	DDX_Control(pDX, IDC_STATIC_CUR_DETECT_THRESHOLD, StaticCurDetectThreshold);
	DDX_Control(pDX, IDC_STATIC_CUR_INTERLOCK_THRESHOLD, StaticCurInterlockThreshold);
	DDX_Control(pDX, IDC_STATIC_CUR_COOLING_FREQ, StaticCurCoolingFreq);
	DDX_Control(pDX, IDC_MODULE_CONFIGURED, CheckModuleConfigured);
}


BEGIN_MESSAGE_MAP(CSerialCounterDlg, CDialog)
	ON_EN_CHANGE(IDC_COM_PORT, &CSerialCounterDlg::OnChangeComPort)
	ON_EN_CHANGE(IDC_COUNT_TIME, &CSerialCounterDlg::OnChangeCountTime)
	ON_EN_CHANGE(IDC_DETECT_THRESHOLD, &CSerialCounterDlg::OnChangeDetectThreshold)
	ON_EN_CHANGE(IDC_COOLING_THRESHOLD, &CSerialCounterDlg::OnChangeCoolingThreshold)
	ON_EN_KILLFOCUS(IDC_COUNT_TIME, &CSerialCounterDlg::OnKillfocusCountTime)
	ON_EN_KILLFOCUS(IDC_DETECT_THRESHOLD, &CSerialCounterDlg::OnKillfocusDetectThreshold)
	ON_EN_KILLFOCUS(IDC_COOLING_THRESHOLD, &CSerialCounterDlg::OnKillfocusCoolingThreshold)
	ON_BN_CLICKED(IDC_BUTTON_APPLY, &CSerialCounterDlg::OnButtonApplyClicked)
	ON_MESSAGE(WM_POSTCREATEWINDOW, &CSerialCounterDlg::OnPostCreateWindow)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDOK, &CSerialCounterDlg::OnBnClickedOk)
END_MESSAGE_MAP()


BOOL CSerialCounterDlg::OnInitDialog()
{
	BOOL res = CDialog::OnInitDialog();
	EditComPort.SetWindowText(theApp.Ini.CounterComPort.Value.GetString());
	StaticPortAvailable.SetWindowText(theApp.m_AdamCom.IsPortHandleValid()?"Порт открыт" : "Порт недоступен");
	CString s;
	s.Format("%i", theApp.Ini.CounterCountTime.Value * 10);
	EditCountTime.SetWindowText(s.GetString());

	s.Format("%i", theApp.Ini.CounterDetectionThreshold.Value);
	EditDetectThreshold.SetWindowText(s.GetString());

	s.Format("%i", theApp.Ini.CounterInterlockThreshold.Value);
	EditCoolingThreshold.SetWindowText(s.GetString());

	ButtonApply.EnableWindow(FALSE);
	ShowWindow(SW_SHOW);
	PostMessage(WM_POSTCREATEWINDOW,0,0);
	SetTimer(TimerID, 500, 0);
	return res;
}

LRESULT CSerialCounterDlg::OnPostCreateWindow(WPARAM WParam, LPARAM LParam)
{
	if(!theApp.m_AdamCom.IsPortHandleValid())
		return 0;
	CString moduleName;
	try
	{
		moduleName = m_counterUnit->ReadModuleName();
		CheckDetectThreshold();
		CheckModuleAvailable.SetCheck(true);
	}
	catch (DetailedException e)
	{
		m_disableMsgBox = true;
		LogFileFormat("Ошибка: %s \nв %s", e.what(), e.Place.c_str());
		Msg("Ошибка: %s \nв %s", e.what(), e.Place.c_str());
		moduleName = e.what();
		CheckModuleAvailable.SetCheck(false);
	}
	StaticModuleName.SetWindowText(moduleName.GetString());
	return 0;
}

void CSerialCounterDlg::OnChangeComPort()
{
	ButtonApply.EnableWindow(TRUE);
	m_portChanged = true;
	StaticPortAvailable.SetWindowText("");
}

void CSerialCounterDlg::OnChangeCountTime()
{
	ButtonApply.EnableWindow(TRUE);
}

void CSerialCounterDlg::OnChangeDetectThreshold()
{
	ButtonApply.EnableWindow(TRUE);
}

void CSerialCounterDlg::OnChangeCoolingThreshold()
{
	ButtonApply.EnableWindow(TRUE);
}

void CSerialCounterDlg::OnKillfocusCountTime()
{
	Coerce(EditCountTime, 0, 655350, 10);
}

void CSerialCounterDlg::OnKillfocusDetectThreshold()
{
	Coerce(EditDetectThreshold, 0, 255);
}

void CSerialCounterDlg::OnKillfocusCoolingThreshold()
{
	Coerce(EditCoolingThreshold, 0, 65535);
}

void CSerialCounterDlg::OnButtonApplyClicked()
{
	m_disableMsgBox = false;
	ButtonApply.EnableWindow(FALSE);

	if(m_portChanged)
	{
		CString s;
		EditComPort.GetWindowText(s);
		theApp.m_AdamCom.ReconnectCom(s.GetString(), nullptr);
		StaticPortAvailable.SetWindowText(theApp.m_AdamCom.IsPortHandleValid()?"Порт открыт" : "Порт недоступен");
		theApp.Ini.CounterComPort.Value = s;
		theApp.Ini.CounterComPort.Write();

		OnPostCreateWindow((WPARAM)0,(LPARAM)0);
	}

	if(theApp.m_AdamCom.IsPortHandleValid())
	{	
		CString s;
		EditCountTime.GetWindowText(s);
		theApp.Ini.CounterCountTime.Value = atol(s.GetString())/10;
		theApp.Ini.CounterCountTime.Write();

		EditDetectThreshold.GetWindowText(s);
		theApp.Ini.CounterDetectionThreshold.Value = atol(s.GetString());
		theApp.Ini.CounterDetectionThreshold.Write();

		EditCoolingThreshold.GetWindowText(s);
		theApp.Ini.CounterInterlockThreshold.Value = atol(s.GetString());
		theApp.Ini.CounterInterlockThreshold.Write();

		StaticCurCountTime.SetWindowText("?");
		StaticCurDetectThreshold.SetWindowText("?");
		StaticCurInterlockThreshold.SetWindowText("?");
		StaticCurCoolingFreq.SetWindowText("?");

		try
		{
			m_counterUnit->SetUnitConfig(&theApp.Ini);
			CheckDetectThreshold();
		}
		catch (DetailedException e)
		{
			LogFileFormat("Ошибка: %s \nв %s", e.what(), e.Place.c_str());
			if(!m_disableMsgBox)
			{
				m_disableMsgBox = true;
				Msg("Ошибка: %s \nв %s", e.what(), e.Place.c_str());
			}
		}		
	}
}

void CSerialCounterDlg::OnTimer(UINT_PTR nIDEvent)
{
	if(!theApp.m_AdamCom.IsPortHandleValid())
		return;

	static bool odd = false;
	odd = !odd;

	if(nIDEvent == TimerID)
	{
		try
		{
			CheckUnitState();
			if(odd)
				CheckCountTime();
			else
				CheckCoolingParams();
			CheckModuleAvailable.SetCheck(true);
		}
		catch (DetailedException e)
		{
			LogFileFormat("Ошибка: %s \nв %s", e.what(), e.Place.c_str());
			if(!m_disableMsgBox)
			{
				m_disableMsgBox = true;
				Msg("Ошибка: %s \nв %s", e.what(), e.Place.c_str());
			}
			CheckModuleAvailable.SetCheck(false);
		}
	}

	CDialog::OnTimer(nIDEvent);
}

void CSerialCounterDlg::CheckUnitState()
{
	CounterUnitState cus = m_counterUnit->ReadState();
	CheckCoolingOk.SetCheck(!cus.CoolingState);
	CheckRunMode.SetCheck(cus.StartState);
	CheckModuleConfigured.SetCheck(cus.ConfigLoaded? 1:0);
}

int CSerialCounterDlg::Coerce(int val, int minVal, int maxVal)
{
	if(val < minVal)
		val = minVal;
	if(val > maxVal)
		val = maxVal;
	return val;
}

void CSerialCounterDlg::Coerce(CEdit &edit, int minVal, int maxVal, int factor)
{
	CString s;
	edit.GetWindowText(s);
	int val = atol(s.GetString());
	s.Format("%i", Coerce(val, minVal, maxVal)/factor*factor);
	edit.SetWindowText(s.GetString());
}

void CSerialCounterDlg::CheckDetectThreshold()
{
	CString s;
	s.Format("%i", (int)m_counterUnit->ReadDetectionThreshold());
	StaticCurDetectThreshold.SetWindowText(s.GetString());
}

void CSerialCounterDlg::CheckCoolingParams()
{
	CoolingParams par = m_counterUnit->ReadCoolingThreshold();
	CString s;
	s.Format("%i  1/сек", par.SetThreshold);
	StaticCurInterlockThreshold.SetWindowText(s.GetString());
	s.Format("%i  1/сек", par.CountedValue);
	StaticCurCoolingFreq.SetWindowText(s.GetString());
}

void CSerialCounterDlg::CheckCountTime()
{
	CString s;
	s.Format("%i мс", m_counterUnit->GetCountTime()*10);
	StaticCurCountTime.SetWindowText(s.GetString());
}

void CSerialCounterDlg::OnBnClickedOk()
{
	OnButtonApplyClicked();
	OnOK();
}
