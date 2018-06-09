#include "stdafx.h"
#include "Main.h"
#include "hardware.h"
#include "DialogParamRegion.h"
#include "Time.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogParamRegion dialog

extern FARPROC EditNormalProc;
extern FARPROC EditNumberProc;
extern CMutex MutexThread;
extern CProgNewApp theApp;

class CMainFrame;

CDialogParamRegion::CDialogParamRegion(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogParamRegion::IDD, pParent)
	, m_Priority(0)
	, m_BeginTime(_T("--.--.--  --:--"))
	, m_EndTime(_T("--.--.--  --:--"))
	, m_LastEditTime(_T(""))
{
	//{{AFX_DATA_INIT(CDialogParamRegion)
	m_HV = 0.0;
	m_KE_Start = 10.0;
	m_KE_End = 0.0;
	m_N = 0;
	m_Step = 0.1;
	m_Comments = _T("");
	m_Anode = -1;
	m_Time = 0.0;
	//}}AFX_DATA_INIT
}


void CDialogParamRegion::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	///*	
	//{{AFX_DATA_MAP(CDialogParamRegion)
	DDX_Control(pDX, IDC_COMBO_HV, m_ComboHV);
	DDX_Text(pDX, IDC_EDIT_KE_BEGIN, m_KE_Start);
	DDX_Text(pDX, IDC_EDIT_KE_END, m_KE_End);
	DDX_Text(pDX, IDC_EDIT_N, m_N);
	DDV_MinMaxInt(pDX, m_N, 0, 268000000);
	DDX_Text(pDX, IDC_EDIT_STEP, m_Step);
	DDV_MinMaxDouble(pDX, m_Step, 2.5e-002, 1517.975);
	DDX_Text(pDX, IDC_EDIT_COMMENTS, m_Comments);
	DDV_MaxChars(pDX, m_Comments, 250);
	DDX_CBIndex(pDX, IDC_COMBO_ANODE, m_Anode);
	DDX_Text(pDX, IDC_EDIT_TIME, m_Time);
	DDV_MinMaxDouble(pDX, m_Time, 1.e-002, 50);
	DDX_Text(pDX, IDC_EDIT_PRIORITY, m_Priority);
	DDV_MinMaxInt(pDX, m_Priority, 1, 99);
	DDX_Control(pDX, IDC_STATIC_ANODE_TXT, m_AnodeTxtControl);
	DDX_Control(pDX, IDC_COMBO_ANODE, m_ComboAnodeControl);
	//}}AFX_DATA_MAP
	//*/
	if (!theApp.Ini.HighPressureMode.Value) //KRATOS
	{
		DDX_Text(pDX, IDC_EDIT_HV, m_HV);
		DDV_MinMaxDouble(pDX, m_HV, 1., 255.);
	}

	double MinKE = -50.0;
	double MaxKE = 1518;  //?????????????????????
	double MinBegin;
	double MaxEnd;
	char MessageString[256];

	if (pDX->m_bSaveAndValidate)
	{

		if (m_KE_BE == DATA_IN::EnergyType::KE) { MinBegin = MinKE;  MaxEnd = MaxKE; }
		else
		{
			MinBegin = CRegion::h_nu_Info.Value_h_nu[m_Anode] - MaxKE;
			MaxEnd = CRegion::h_nu_Info.Value_h_nu[m_Anode] - MinKE;
		}

		HWND hWndCtrl = pDX->PrepareEditCtrl(IDC_EDIT_HV);

		if (D2I(m_HV) % 1000)
		{
			::MessageBox(this->m_hWnd, "HV must be integer", "Attention", MB_OK);
			sprintf(MessageString, "%.0lf", m_HV);
			::SetWindowText(hWndCtrl, MessageString);
			pDX->Fail();
		}

		hWndCtrl = pDX->PrepareEditCtrl(IDC_EDIT_STEP);
		if (D2I(m_KE_Start + m_Step) > D2I(MaxEnd))
		{
			::MessageBox(this->m_hWnd, "Parametr Step (or Start) too large",
				"Attention", MB_OK);

			sprintf(MessageString, "%.3lf", m_Step);
			::SetWindowText(hWndCtrl, MessageString);
			pDX->Fail();
		}
		if (D2I(m_Step) % 25)
		{
			::MessageBox(this->m_hWnd, "Step must be divisible by 0.025", "Attention", MB_OK);
			sprintf(MessageString, "%.3lf", m_Step);
			::SetWindowText(hWndCtrl, MessageString);
			pDX->Fail();
		}

		if (D2I(m_KE_Start) < D2I(MinBegin) || D2I(m_KE_Start) > D2I(MaxEnd - 0.024999999999))
		{
			hWndCtrl = pDX->PrepareEditCtrl(IDC_EDIT_KE_BEGIN);
			sprintf(MessageString, "Start must be within :\n\n  Min : %.3lf\n\n  Max : %.3lf",
				MinBegin, MaxEnd - 0.025);
			::MessageBox(this->m_hWnd, MessageString, "Attention", MB_OK);
			sprintf(MessageString, "%.3lf", m_KE_Start);
			::SetWindowText(hWndCtrl, MessageString);
			pDX->Fail();
		}

		if (D2I(m_KE_Start) % 25)
		{
			hWndCtrl = pDX->PrepareEditCtrl(IDC_EDIT_KE_BEGIN);
			::MessageBox(this->m_hWnd, "Start must be divisible by 0.025", "Attention", MB_OK);
			sprintf(MessageString, "%.3lf", m_KE_Start);
			::SetWindowText(hWndCtrl, MessageString);
			pDX->Fail();
		}

		if ((m_pReg->m_NewOrEdit == m_pReg->Edit)
			&& (m_pReg->m_DataIn.Curr_N > 0 || m_pReg->m_NDataOutCurr > 0)
			&& (I2D(m_pReg->m_DataIn.KE_Start) != m_KE_Start))
		{
			hWndCtrl = pDX->PrepareEditCtrl(IDC_EDIT_KE_BEGIN);
			if (m_pReg->m_DataIn.KE_Start > D2I(m_KE_Start))
			{
				sprintf(MessageString, "Start must be more or equal %.3lf",
					I2D(m_pReg->m_DataIn.KE_Start));
				::MessageBox(this->m_hWnd, MessageString, "Attention", MB_OK);
				sprintf(MessageString, "%.3lf", m_KE_Start);
				::SetWindowText(hWndCtrl, MessageString);
				pDX->Fail();
			}
			int del = abs((int)((m_pReg->m_DataIn.KE_Start) - D2I(m_KE_Start)));
			int N_St = abs((int)((I2D(m_pReg->m_DataIn.KE_Start) - m_KE_Start) / (m_Step)));
			if (del % (D2I(m_Step)))
			{
				double FirstStart;// = m_KE_Start + (N_Step+1)*m_Step;
				double SecondStart;// = m_KE_Start + N_Step*m_Step;
				if (I2D(m_pReg->m_DataIn.KE_Start) > m_KE_Start)
				{
					FirstStart = I2D(m_pReg->m_DataIn.KE_Start - (N_St + 1) * D2I(m_Step));
					SecondStart = I2D(m_pReg->m_DataIn.KE_Start - (N_St)* D2I(m_Step));
					if (FirstStart < MinBegin) FirstStart = SecondStart;
				}
				else
				{
					FirstStart = I2D(m_pReg->m_DataIn.KE_Start + N_St * D2I(m_Step));
					SecondStart = I2D(m_pReg->m_DataIn.KE_Start + (N_St + 1) * D2I(m_Step));
				}

				char sFirst[30];
				char sSecond[30];
				sprintf(sFirst, "%.3lf", FirstStart);
				sprintf(sSecond, "%.3lf", SecondStart);

				m_pDlgKEEnd = NULL;
				m_pDlgKEEnd = new CDlgKEEnd((CWnd*)this);
				if (m_pDlgKEEnd)
				{
					m_pDlgKEEnd->m_StaticBig += sFirst;
					m_pDlgKEEnd->m_StaticSmall += sSecond;
					m_pDlgKEEnd->m_StaticCaption += "Parametr Start must be equal:";
					m_pDlgKEEnd->m_StaticCaptionBig += "Start = ";
					m_pDlgKEEnd->m_StaticCaptionSmall += "Start = ";
					if (m_pDlgKEEnd->DoModal() == IDOK)
					{
						if (m_pDlgKEEnd->m_Radio == 0) m_KE_Start = FirstStart;
						else if (m_pDlgKEEnd->m_Radio == 1) m_KE_Start = SecondStart;
						else if (m_pDlgKEEnd->m_Radio == 2) pDX->Fail();

						HWND hWnd = ::GetDlgItem(this->m_hWnd, IDC_EDIT_KE_BEGIN);
						char strWin[32];
						sprintf(strWin, "%.3lf", m_KE_Start);
						::SetWindowText(hWnd, strWin);

					}
					delete m_pDlgKEEnd;
				}
			}
		} // end if((m_pReg->m_NewOrEdit==m_pReg->Edit) && (m_pReg->m_DataIn.Curr_N > 0))

		  //--------
		  //ÏÎÑÌÎÒÐÅÒÜ ÃÄÅ ÏÅÐÅÂÛÄÅËßÅÒÑß ÏÀÌßÒÜ
		hWndCtrl = pDX->PrepareEditCtrl(IDC_EDIT_KE_END);
		if (D2I(m_KE_End) > D2I(MaxEnd) || D2I(m_KE_End) < D2I(MinBegin + 0.025))
		{
			sprintf(MessageString, "Finish must be within :\n\n  Min : %.3lf\n\n  Max : %.3lf",
				MinBegin + 0.025, MaxEnd);
			::MessageBox(this->m_hWnd, MessageString, "Attention", MB_OK);
			sprintf(MessageString, "%.3lf", m_KE_End);
			::SetWindowText(hWndCtrl, MessageString);
			pDX->Fail();
		}

		if ((m_pReg->m_NewOrEdit == m_pReg->Edit)
			&& (m_pReg->m_DataIn.Curr_N > 0)
			&& (I2D(m_pReg->m_DataIn.KE_End) != m_KE_End))
		{
			if (m_pReg->m_DataIn.KE_End < D2I(m_KE_End))
			{
				sprintf(MessageString, "Finish must be less or equal %.3lf",
					I2D(m_pReg->m_DataIn.KE_End));
				::MessageBox(this->m_hWnd, MessageString, "Attention", MB_OK);
				sprintf(MessageString, "%.3lf", m_KE_End);
				::SetWindowText(hWndCtrl, MessageString);
				pDX->Fail();
			}
		}
		if (D2I(m_KE_End) < D2I(m_KE_Start + m_Step))
		{
			::MessageBox(this->m_hWnd, "Parameter Finish must be more (or equal)\n then Start + Step",
				"Attention", MB_OK);
			sprintf(MessageString, "%.3lf", m_KE_End);
			::SetWindowText(hWndCtrl, MessageString);
			pDX->Fail();
		}

		int delta = D2I(m_KE_End) - D2I(m_KE_Start);
		int N_Step = (int)((m_KE_End - m_KE_Start) / m_Step);
		if (delta %  D2I(m_Step))
		{
			double BigEnd = I2D(D2I(m_KE_Start) + (N_Step + 1) * D2I(m_Step));
			double SmallEnd = I2D(D2I(m_KE_Start) + (N_Step)* D2I(m_Step));//m_KE_Start + N_Step*m_Step;
			if (BigEnd > MaxEnd) BigEnd = SmallEnd;
			char sBig[30];
			char sSmall[30];
			sprintf(sBig, "%.3lf", BigEnd);
			sprintf(sSmall, "%.3lf", SmallEnd);

			m_pDlgKEEnd = NULL;
			m_pDlgKEEnd = new CDlgKEEnd((CWnd*)this);
			if (m_pDlgKEEnd)
			{
				m_pDlgKEEnd->m_StaticBig += sBig;
				m_pDlgKEEnd->m_StaticSmall += sSmall;
				m_pDlgKEEnd->m_StaticCaption += "Parameter Finish must be equal  Start + integer * Step";
				m_pDlgKEEnd->m_StaticCaptionBig += "Finish = ";
				m_pDlgKEEnd->m_StaticCaptionSmall += "Finish = ";
				if (m_pDlgKEEnd->DoModal() == IDOK)
				{
					if (m_pDlgKEEnd->m_Radio == 0)
					{
						m_KE_End = BigEnd; ++N_Step;
					}
					else if (m_pDlgKEEnd->m_Radio == 1)
					{
						this->m_KE_End = SmallEnd;
					}
					else if (m_pDlgKEEnd->m_Radio == 2) { pDX->Fail(); }
					else pDX->Fail();

					HWND hWnd = ::GetDlgItem(this->m_hWnd, IDC_EDIT_KE_END);
					char strWin[32];
					sprintf(strWin, "%.3lf", m_KE_End);
					::SetWindowText(hWnd, strWin);
				}
				delete m_pDlgKEEnd;
			}
		} //end if( delta %  D2I(m_Step)  )		

		if ((m_pReg->m_NewOrEdit == m_pReg->Edit)
			&& (m_pReg->m_DataIn.Curr_N > 0)
			&& (m_pReg->m_DataIn.Curr_N > m_N))
		{
			hWndCtrl = pDX->PrepareEditCtrl(IDC_EDIT_N);
			char sp[80];
			sprintf(sp, "You can't set number of passage less then %i", m_pReg->m_DataIn.Curr_N);
			::MessageBox(this->m_hWnd, sp, "Attention", MB_OK);
			sprintf(MessageString, "%.i", m_N);
			::SetWindowText(hWndCtrl, MessageString);
			pDX->Fail();
		}

		hWndCtrl = pDX->PrepareEditCtrl(IDC_EDIT_TIME);
		if (D2I(m_Time) % 10)
		{
			::MessageBox(this->m_hWnd, "Time must be divisible by 0.01", "Attention", MB_OK);
			char strTime[16];
			sprintf(strTime, "%.2lf", m_Time);
			::SetWindowText(hWndCtrl, strTime);
			pDX->Fail();
		}
	} // end if(pDX->m_bSaveAndValidate)
	DDX_Text(pDX, IDC_STATIC_BEGIN_TIME, m_BeginTime);
	DDX_Text(pDX, IDC_STATIC_END_TIME, m_EndTime);
	DDX_Text(pDX, IDC_STATIC_LAST_EDIT_TIME, m_LastEditTime);
}


BEGIN_MESSAGE_MAP(CDialogParamRegion, CDialog)
	//{{AFX_MSG_MAP(CDialogParamRegion)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_RADIO_ON, OnRadioOn)
	ON_BN_CLICKED(IDC_RADIO_OFF, OnRadioOff)
	ON_BN_CLICKED(IDC_RADIO_KE, OnRadioKE)
	ON_BN_CLICKED(IDC_RADIO_BE, OnRadioBE)
	ON_CBN_SELENDOK(IDC_COMBO_ANODE, OnSelEndOkComboAnode)
	ON_EN_KILLFOCUS(IDC_EDIT_KE_BEGIN, OnKillFocusEditKEBegin)
	ON_EN_KILLFOCUS(IDC_EDIT_KE_END, OnKillFocusEditKEEnd)
	ON_EN_KILLFOCUS(IDC_EDIT_HV, OnKillFocusEditHV)
	ON_EN_KILLFOCUS(IDC_EDIT_STEP, OnKillFocusEditStep)
	ON_EN_KILLFOCUS(IDC_EDIT_N, OnKillFocusEditN)
	ON_EN_KILLFOCUS(IDC_EDIT_TIME, OnKillFocusEditTime)
	ON_EN_KILLFOCUS(IDC_EDIT_COMMENTS, OnKillFocusEditComments)
	ON_BN_CLICKED(IDC_BUTTON_RESET, OnButtonReset)
	ON_BN_CLICKED(IDC_BUTTON_HV_TABLE, OnButtonHVTable)
	ON_BN_CLICKED(IDC_BUTTON_COMMENTS_EDIT, OnButtonCommentsEdit)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_RESET_ALL, &CDialogParamRegion::OnBnClickedButtonResetAll)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogParamRegion message handlers

void CDialogParamRegion::OnOK() 
{
	if(!CheckHV())
		return;
	CDialog::OnOK();
}

BOOL CDialogParamRegion::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	SubClassingWindows();
	char str[64];
	if(theApp.Ini.HighPressureMode.Value && m_pReg->m_NewOrEdit==m_pReg->New)
		m_HV = 10;
	else
		m_HV = I2D( m_pReg->m_DataIn.HV );

	if(!theApp.Ini.HighPressureMode.Value)
	{
		m_ComboHV.ShowWindow(SW_HIDE);
	}
	else
	{
		GetDlgItem(IDC_EDIT_HV)->ShowWindow(SW_HIDE);
		sprintf(str,"%i", (int)m_HV);
		int Index = m_ComboHV.FindStringExact(0,str);
		if(Index>0)
			m_ComboHV.SetCurSel(Index);
		else
			m_ComboHV.SetWindowText(str);
	}
	m_KE_Start = I2D(m_pReg->m_DataIn.KE_Start);
	m_KE_End = I2D(m_pReg->m_DataIn.KE_End);
	m_N = m_pReg->m_DataIn.N_;
	m_Step = I2D(m_pReg->m_DataIn.Step);
	m_Time = I2D(m_pReg->m_DataIn.Time);
	m_Comments = m_pReg->m_DataIn.Comments;
	m_KE_BE = m_pReg->m_DataIn.KE_BE;
	m_Anode = m_pReg->m_DataIn.N_h_nu;
	m_Priority = m_pReg->m_DataIn.Priority;
	m_LastEditTime = m_pReg->m_DataIn.LastEditTime == 0 ? CString() : FormatTime(m_pReg->m_DataIn.LastEditTime, "Last edit: %d.%m.%Y  %H:%M");
	m_BeginTime = m_pReg->m_BeginTime == 0? CString("--.--.--  --:--") : FormatTime(m_pReg->m_BeginTime, "%d.%m.%Y  %H:%M");
	m_EndTime = m_pReg->m_EndTime == 0? CString("--.--.--  --:--") : FormatTime(m_pReg->m_EndTime, "%d.%m.%Y  %H:%M");
	if(m_BeginTime == m_EndTime && m_pReg->m_BeginTime != 0)
	{
		m_BeginTime = m_pReg->m_BeginTime == 0? CString("--.--.--  --:--") : FormatTime(m_pReg->m_BeginTime, "%d.%m.%Y  %H:%M:%S");
		m_EndTime = m_pReg->m_EndTime == 0? CString("--.--.--  --:--") : FormatTime(m_pReg->m_EndTime, "%d.%m.%Y  %H:%M:%S");		
	}

	UpdateData(FALSE);

	HWND hWndChild = ::GetDlgItem(this->m_hWnd, IDC_COMBO_ANODE);
	for(int i = 0; i<CRegion::h_nu_Info.Nelem; ++i)
		{
		sprintf(str, "%s     (%.1lf eV)", CRegion::h_nu_Info.strName_h_nu[i], m_pReg->h_nu_Info.Value_h_nu[i]);
		::SendMessage(hWndChild, CB_ADDSTRING, 0, (LPARAM) str);
		}
	::SendMessage(hWndChild, CB_SETCURSEL , (WPARAM) (m_pReg->m_DataIn.N_h_nu), 0);
	
	if(m_KE_BE == DATA_IN::EnergyType::KE)
		{
		::EnableWindow(hWndChild, FALSE);
		hWndChild = ::GetDlgItem(this->m_hWnd, IDC_STATIC_ANODE_TXT);
		::EnableWindow(hWndChild, FALSE);
		}

	if(m_pReg->m_DataIn.KE_BE == DATA_IN::EnergyType::KE)
		{hWndChild = ::GetDlgItem(this->m_hWnd, IDC_RADIO_KE);
		 m_KE_BE = DATA_IN::EnergyType::KE;
		}
	else 
		{hWndChild = ::GetDlgItem(this->m_hWnd, IDC_RADIO_BE);
		 m_KE_BE = DATA_IN::EnergyType::BE;
		}
	::SendMessage(hWndChild, BM_SETCHECK, BST_CHECKED, 0);
	
	if(m_pReg->m_DataIn.Off == FALSE)
		{hWndChild = ::GetDlgItem(this->m_hWnd, IDC_RADIO_ON);
		 m_Off = FALSE;
		}
	else 
		{hWndChild = ::GetDlgItem(this->m_hWnd, IDC_RADIO_OFF);
		 m_Off = TRUE;
		}
	::SendMessage(hWndChild, BM_SETCHECK, 1, 0);

	if(m_pReg->m_DataIn.Curr_N == 0 && m_pReg->m_NDataOutCurr == 0)
		{	hWndChild = ::GetDlgItem(this->m_hWnd, IDC_BUTTON_RESET);
			::EnableWindow(hWndChild, FALSE);
		}
	
	// Ïðîâåðèì, åñòü ëè äàííûå õîòÿ áû â îäíîì ðåãèîíå
	bool dataExists = false;
	for(CRegion* pReg = CRegion::GetFirst(); pReg != NULL ;pReg = CRegion::GetNext(pReg))
	{
		if( pReg->m_DataIn.Curr_N != 0 || pReg->m_NDataOutCurr != 0 )
		{
			dataExists = true;
			break;
		}
	}
	::EnableWindow(GetDlgItem(IDC_BUTTON_RESET_ALL)->m_hWnd, dataExists);

	if(m_pReg->m_NewOrEdit==m_pReg->New)
	{
		hWndChild = ::GetDlgItem(this->m_hWnd, IDC_RADIO_BE);
		::SendMessage(hWndChild, BM_CLICK, 0, 0);
	}
	
	auto isRegionEmpty = m_pReg->m_DataIn.Curr_N == 0 && m_pReg->m_NDataOutCurr == 0;
	GetDlgItem(IDC_EDIT_HV)->EnableWindow(isRegionEmpty);
	GetDlgItem(IDC_STATIC_PARAM_HV1)->EnableWindow(isRegionEmpty);
	GetDlgItem(IDC_BUTTON_HV_TABLE)->EnableWindow(isRegionEmpty);
	GetDlgItem(IDC_EDIT_STEP)->EnableWindow(isRegionEmpty);
	GetDlgItem(IDC_STATIC_PARAM_STEP)->EnableWindow(isRegionEmpty);
	GetDlgItem(IDC_EDIT_TIME)->EnableWindow(isRegionEmpty);
	GetDlgItem(IDC_STATIC_PARAM_TIME)->EnableWindow(isRegionEmpty);
	GetDlgItem(IDC_STATIC_KE_BE)->EnableWindow(isRegionEmpty);
	GetDlgItem(IDC_RADIO_KE)->EnableWindow(isRegionEmpty);
	GetDlgItem(IDC_RADIO_BE)->EnableWindow(isRegionEmpty);
	GetDlgItem(IDC_COMBO_ANODE)->EnableWindow(isRegionEmpty);
	GetDlgItem(IDC_STATIC_ANODE_TXT)->EnableWindow(isRegionEmpty);

	if(m_pReg->m_NewOrEdit==m_pReg->New) sprintf(str," %s", "New Region");
	else sprintf(str," %s %i", "Region", m_pReg->ID+1);
	::SetWindowText(this->m_hWnd, str);

	return TRUE;  // return TRUE unless you set the focus to a control
}

void CDialogParamRegion::OnRadioOn() 
{
	m_Off = FALSE;
}

void CDialogParamRegion::OnRadioOff() 
{
	m_Off = TRUE;
}

void CDialogParamRegion::SetKeBe(DATA_IN::EnergyType energyType)
{
	m_KE_BE = energyType;	

	m_ComboAnodeControl.EnableWindow(m_KE_BE == DATA_IN::EnergyType::BE);
	m_AnodeTxtControl.EnableWindow(m_KE_BE == DATA_IN::EnergyType::BE);

	int NewValue = D2I( ((int) 10* CRegion::h_nu_Info.Value_h_nu[m_Anode])/10.) - D2I(m_KE_Start);
	m_KE_Start = I2D(NewValue);

	NewValue = D2I( ((int) 10* CRegion::h_nu_Info.Value_h_nu[m_Anode])/10.) - D2I(m_KE_End);
	m_KE_End = I2D(NewValue);
	
	std::swap(m_KE_Start, m_KE_End);

	UpdateData(FALSE);
}

void CDialogParamRegion::OnRadioKE() 
{
	SetKeBe(DATA_IN::EnergyType::KE);
}


void CDialogParamRegion::OnRadioBE() 
{
	SetKeBe(DATA_IN::EnergyType::BE);
}

void CDialogParamRegion::OnSelEndOkComboAnode() 
{
	// TODO: Add your control notification handler code here
	UINT res;
	HWND hWndChild = ::GetDlgItem(this->m_hWnd, IDC_COMBO_ANODE);
	res = (UINT) ::SendMessage(hWndChild, CB_GETCURSEL , 0, 0);
	if(res!=CB_ERR) 
	{
		m_Anode = res;
		UpdateData(FALSE);
	}	
}

void CDialogParamRegion::OnKillFocusEditKEBegin() 
{
	char str[64], *endptr;
	HWND hWndChild = ::GetDlgItem(this->m_hWnd, IDC_EDIT_KE_BEGIN);
	::GetWindowText(hWndChild, str, 64);
	m_KE_Start = strtod(str, &endptr);
}

void CDialogParamRegion::OnKillFocusEditKEEnd() 
{
	char str[64], *endptr;
	HWND hWndChild = ::GetDlgItem(this->m_hWnd, IDC_EDIT_KE_END);
	::GetWindowText(hWndChild, str, 64);
	m_KE_End = strtod(str, &endptr);	
}

void CDialogParamRegion::SubClassingWindows()
{
	HWND hWndChild = ::GetDlgItem(this->m_hWnd, IDC_EDIT_KE_BEGIN);
	SetWindowLong(hWndChild, GWL_WNDPROC, (LONG) EditSuperNumberProc);

	hWndChild = ::GetDlgItem(this->m_hWnd, IDC_EDIT_KE_END);
	::SetWindowLong(hWndChild, GWL_WNDPROC, (LONG) EditSuperNumberProc);

	hWndChild = ::GetDlgItem(this->m_hWnd, IDC_EDIT_STEP);
	::SetWindowLong(hWndChild, GWL_WNDPROC, (LONG) EditSuperNumberProc);

	hWndChild = ::GetDlgItem(this->m_hWnd, IDC_EDIT_TIME);
	::SetWindowLong(hWndChild, GWL_WNDPROC, (LONG) EditSuperNumberProc);
}

void CDialogParamRegion::OnKillFocusEditHV() 
{	
	if(!theApp.Ini.HighPressureMode.Value) //KRATOS
	{
		char str[64], *endptr;
		HWND hWndChild = ::GetDlgItem(this->m_hWnd, IDC_EDIT_HV);
		::GetWindowText(hWndChild, str, 64);
		m_HV = strtod(str, &endptr);
	}
}

void CDialogParamRegion::OnKillFocusEditStep() 
{
	char str[64], *endptr;
	HWND hWndChild = ::GetDlgItem(this->m_hWnd, IDC_EDIT_STEP);
	::GetWindowText(hWndChild, str, 64);
	m_Step = strtod(str, &endptr);	
}

void CDialogParamRegion::OnKillFocusEditN() 
{
	char str[64];
	HWND hWndChild = ::GetDlgItem(this->m_hWnd, IDC_EDIT_N);
	::GetWindowText(hWndChild, str, 64);
	m_N = atoi(str);	
}

void CDialogParamRegion::OnKillFocusEditTime() 
{
	char str[64], *endptr;
	HWND hWndChild = ::GetDlgItem(this->m_hWnd, IDC_EDIT_TIME);
	::GetWindowText(hWndChild, str, 64);
	m_Time = strtod(str, &endptr);	
}

void CDialogParamRegion::OnKillFocusEditComments() 
{
	char str[64];
	HWND hWndChild = ::GetDlgItem(this->m_hWnd, IDC_EDIT_COMMENTS);
	::GetWindowText(hWndChild, str, 64);
	m_Comments = str;			
}

void CDialogParamRegion::OnButtonReset()
{
	if(::MessageBox(this->m_hWnd, "Are you sure you want\nto reset all measured data?", "Attention",MB_YESNO) == IDYES)
	{
		m_pReg->ResetMeasuredData();
		SaveXpsFullRegionDataToFile(m_pMainFrame->m_Doc.fpPrj, m_pReg);
		m_pMainFrame->m_pRegionWnd->m_pListRegionWnd->UpdateItem(m_pReg);
		GetXpsTimeRemainedToEnd(&m_pMainFrame->m_Doc.m_ThrComm.TIME);
		m_pMainFrame->SetStatusTime(m_pMainFrame->m_Doc.m_ThrComm.TIME);
		OnInitDialog();
	}		
}

void CDialogParamRegion::OnButtonHVTable() 
{
	CFiTableDlg dlg((CWnd*) this);
	dlg.m_pFiTable=&m_pMainFrame->m_Doc.m_ThrComm.FiTable;
	dlg.DoModal();
}

void CDialogParamRegion::OnButtonCommentsEdit() 
{
	CDlgCommentsEdit dlg((CWnd*) this);
	dlg.m_pDlgParam = this;
	int res = dlg.DoModal();
	if(res == IDOK)
		{ m_Comments = dlg.m_strComments;
			UpdateData(FALSE);
		}
}

BOOL CDialogParamRegion::CheckHV()
{
	char str[50];
	char MessageString[256];

	if(!theApp.Ini.HighPressureMode.Value) //KRATOS
	{
		if( !theApp.m_pMainFrame->m_Doc.m_ThrComm.FiTable.HVExist((int) m_HV) )
			{	sprintf(MessageString, "The HV = %i is not a member of the table Fi(HV)", (int) m_HV);
				::MessageBox(this->m_hWnd, MessageString, "Attention", MB_OK );
				return FALSE;
			}
	}
	else //High Pressure
	{
		m_ComboHV.GetWindowText(str, 49);
		if( !theApp.m_pMainFrame->m_Doc.m_ThrComm.FiTable.HVExist(atoi(str)) || GetRegisterHVCodeFromHV(atoi(str))>HP_MAX_HV_CODE)
			{	
				if( !theApp.m_pMainFrame->m_Doc.m_ThrComm.FiTable.HVExist(atoi(str)) && GetRegisterHVCodeFromHV(atoi(str))>HP_MAX_HV_CODE)
					sprintf(MessageString, "1. The HV = %i eV is not a permitted value for HP hardware.\nPermitted values are: 2, 5, 10, 20, 50, 100, 200 eV.\n\n2. The HV = %i eV is not a member of the table Fi(HV).", atoi(str), atoi(str));
				else if( !theApp.m_pMainFrame->m_Doc.m_ThrComm.FiTable.HVExist((int) m_HV))
					sprintf(MessageString, "The HV = %i eV is not a member of the table Fi(HV).", atoi(str));
				else 
					sprintf(MessageString, "The HV = %i eV is not a permitted value for HP hardware.\nPermitted values are: 2, 5, 10, 20, 50, 100, 200 eV.", atoi(str));

				::MessageBox(this->m_hWnd, MessageString, "Attention", MB_OK );
				return FALSE;
			}
		m_HV=atoi(str);

	}
	return TRUE;
}

void CDialogParamRegion::OnBnClickedButtonResetAll()
{
	if(MessageBox("Are you sure you want to reset \nALL measured data in ALL regions?",
		"Attention", MB_YESNO | MB_ICONWARNING) == IDYES)
	{
		for(CRegion* pReg = CRegion::GetFirst(); pReg != NULL ;pReg = CRegion::GetNext(pReg))
		{
			pReg->ResetMeasuredData();
			SaveXpsFullRegionDataToFile(m_pMainFrame->m_Doc.fpPrj, pReg);
			m_pMainFrame->m_pRegionWnd->m_pListRegionWnd->UpdateItem(pReg);
		}
		GetXpsTimeRemainedToEnd(&m_pMainFrame->m_Doc.m_ThrComm.TIME);
		m_pMainFrame->SetStatusTime(m_pMainFrame->m_Doc.m_ThrComm.TIME);
		OnInitDialog();
	}
}
