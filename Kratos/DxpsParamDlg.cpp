// DxpsParamDlg.cpp : implementation file
//

#include "stdafx.h"
#include "prognew.h"
#include "Main.h"
#include "hardware.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CProgNewApp theApp;
/////////////////////////////////////////////////////////////////////////////
// CDxpsParamDlg dialog


CDxpsParamDlg::CDxpsParamDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDxpsParamDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDxpsParamDlg)
	m_TypeBE = 0;
	m_Comments = _T("");
	m_Dwell = 0.0;
	m_HV = 0;
	m_SwitchOn = 0;
	m_BE = 0.0;
	m_Delay = 0.0;
	m_NewRegion=0;
	//}}AFX_DATA_INIT
	m_NewRegion=0;
	m_pReg=NULL;
}


void CDxpsParamDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDxpsParamDlg)
	DDX_Control(pDX, IDC_COMBO_DXPSPAR_DIVISOR, m_ComboDivisor);
	DDX_Control(pDX, IDC_COMBO_DXPSPAR_DIVIDENT, m_ComboDivident);
	DDX_Control(pDX, IDC_COMBO_DXPSPAR_HV, m_ComboHV);
	DDX_Control(pDX, IDC_BUTTON_DXPSPAR_RESET_REG, m_ButtonResetReg);
	DDX_Control(pDX, IDC_BUTTON_DXPSPAR_RESET_ALL, m_ButtonResetAll);
	DDX_Control(pDX, IDC_COMBO_COLOR, m_ComboColor);
	DDX_Control(pDX, IDC_COMBO_ANODE, m_ComboAnode);
	DDX_Radio(pDX, IDC_RADIO_DXPSPAR_TYPE_BE, m_TypeBE);
	DDX_Text(pDX, IDC_EDIT_DXPSPAR_COMMENTS, m_Comments);
	DDX_Text(pDX, IDC_EDIT_DXPSPAR_DWELL, m_Dwell);
	DDX_Text(pDX, IDC_EDIT_DXPSPAR_HV, m_HV);
	DDX_Radio(pDX, IDC_RADIO_DXPSPAR_ON, m_SwitchOn);
	DDX_Text(pDX, IDC_EDIT_DXPSPAR_BE, m_BE);
	DDX_Text(pDX, IDC_EDIT_DXPSPAR_DELAY, m_Delay);
	//}}AFX_DATA_MAP

}


BEGIN_MESSAGE_MAP(CDxpsParamDlg, CDialog)
	//{{AFX_MSG_MAP(CDxpsParamDlg)
	ON_BN_CLICKED(IDC_RADIO_DXPSPAR_TYPE_BE, OnRadioDxpsParTypeBE)
	ON_BN_CLICKED(IDC_RADIO_DXPSPAR_TYPE_DIV, OnRadioDxpsParTypeDiv)
	ON_BN_CLICKED(IDC_BUTTON_DXPSPAR_HV_TABLE, OnButtonDxpsParHvTable)
	ON_WM_DRAWITEM()
	ON_WM_MEASUREITEM()
	ON_BN_CLICKED(IDC_BUTTON_DXPSPAR_RESET_REG, OnButtonResetReg)
	ON_BN_CLICKED(IDC_BUTTON_DXPSPAR_RESET_ALL, OnButtonResetAll)
	ON_EN_KILLFOCUS(IDC_EDIT_DXPSPAR_HV, OnKillfocusEditHv)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDxpsParamDlg message handlers

void CDxpsParamDlg::OnRadioDxpsParTypeBE() 
{
m_TypeBE=TYPE_BE;
CheckDlgButton(IDC_RADIO_DXPSPAR_TYPE_DIV, FALSE);
EnableDisableControls();
}

void CDxpsParamDlg::OnRadioDxpsParTypeDiv() 
{
m_TypeBE=TYPE_DIV;
CheckDlgButton(IDC_RADIO_DXPSPAR_TYPE_BE, FALSE);
EnableDisableControls();
}

void CDxpsParamDlg::OnButtonDxpsParHvTable() 
{
	CFiTableDlg dlg((CWnd*) this);
	dlg.m_pFiTable=&theApp.m_pMainFrame->m_Doc.m_ThrComm.FiTable;
	dlg.DoModal();
}

BOOL CDxpsParamDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	char str[50];
	int Index;
	//≈сли измерение запущено, то запрещаетс€ все, кроме комментари€ и Delay
	//≈сли регион содержит данные, то запрещаетс€ HV, BE, Dwell, Anode, TypeBE, TypeDiv
	//но разрешаетс€ кнопка сброса данных региона

	m_TypeBE=pParameters->Type;

	BOOL TypeBE=(m_TypeBE==TYPE_BE);
	BOOL MeasureRun=theApp.m_pMainFrame->m_StartStop;
	BOOL RegionHasData=FALSE;
	if(m_pReg)
		if(m_pReg->HasData())
			RegionHasData=TRUE;

	BOOL EnableTypeDiv= (CDxpsRegion::GetRegNumber()>=2) && !RegionHasData;
	BOOL EnableDivisorDivident=(CDxpsRegion::GetRegNumber()>=2) && !RegionHasData && !TypeBE;
	BOOL EnableDelay= TypeBE;

	EnableDisableControls();

	if(m_NewRegion)
		SetWindowText("New DXPS Region");
	else
	{
		CString str;
		str.Format("DXPS Parameters - D%i", m_RegionID+1);
		SetWindowText((LPCSTR)str);
	}
	m_BE=pParameters->BE;
	m_Comments=pParameters->Comments;
	for(int i=0;i<CDxpsRegion::GetRegNumber();i++)
	{
		if(i==m_RegionID && !m_NewRegion)
			continue;
		sprintf(str,"%i", i+1);
		m_ComboDivident.AddString(str);
		m_ComboDivisor.AddString(str);
	}

	if(EnableTypeDiv || (m_TypeBE==TYPE_DIV))
	{
			sprintf(str,"%i", pParameters->Divident);
			m_ComboDivident.SetWindowText(str);
			sprintf(str,"%i", pParameters->Divisor);
			m_ComboDivisor.SetWindowText(str);
	}

	m_Delay=pParameters->Delay;
	m_Dwell=pParameters->Dwell;
	m_HV=(int)pParameters->HV;
	if(!theApp.Ini.HighPressureMode.Value)
	{
		m_ComboHV.ShowWindow(SW_HIDE);
	}
	else
	{
		GetDlgItem(IDC_EDIT_DXPSPAR_HV)->ShowWindow(SW_HIDE);
		sprintf(str,"%i", (int)pParameters->HV);
		Index = m_ComboHV.FindStringExact(0,str);
		if(Index>0)
			m_ComboHV.SetCurSel(Index);
		else
			m_ComboHV.SetWindowText(str);
	}
	m_SwitchOn=pParameters->Off;
	m_ComboAnode.SetCurSel(pParameters->Anode);
	UpdateData(FALSE);
	for(int i=0;i<15;i++)
	{
		sprintf(str,"Color %i", i);
		m_ComboColor.AddString(str);
	}
	m_ComboColor.SetCurSel(pParameters->ColorIndex);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDxpsParamDlg::OnOK() 
{
	char str[50];
	UpdateData();
	if(!CheckHV())
		return;
	if(m_TypeBE==TYPE_DIV)
	{
		if (!CheckDividentDivisor())
			return;
	}
	else
		if(!CheckBE())
			return;
	pParameters->BE=m_BE;
	pParameters->Type=m_TypeBE;
	strcpy(pParameters->Comments,(LPCSTR)m_Comments);
	pParameters->Delay=m_Delay;
	pParameters->Dwell=m_Dwell;
	if(!theApp.Ini.HighPressureMode.Value)
		pParameters->HV=m_HV;
	else
	{
		m_ComboHV.GetWindowText(str, 49);
		pParameters->HV=atoi(str);
	}
	pParameters->Off=m_SwitchOn;
	pParameters->Anode=m_ComboAnode.GetCurSel();
	pParameters->ColorIndex=m_ComboColor.GetCurSel();
	m_ComboDivident.GetWindowText(str,24);
	pParameters->Divident=atoi(str);
	m_ComboDivisor.GetWindowText(str,24);
	pParameters->Divisor=atoi(str);

	CDialog::OnOK();
}



int CDxpsParamDlg::DoModal() 
{
	m_pParentWnd=m_pParentWindow;	
	return CDialog::DoModal();
}

void CDxpsParamDlg::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	if(nIDCtl==IDC_COMBO_COLOR)
	{	char str[30];
		CDC dc;
		dc.Attach(lpDrawItemStruct->hDC);
		CRect rect;
		rect=lpDrawItemStruct->rcItem;

		if (lpDrawItemStruct->itemState & ODS_SELECTED)
		{
			dc.SetTextColor(0xFFFFFF&(~theApp.m_pMainFrame->m_pDxpsDlg->m_ColorIndex[lpDrawItemStruct->itemID]));
			if(lpDrawItemStruct->itemID==7)
				dc.SetTextColor(RGB(255,255,255));
		}
		else
		{
			dc.SetTextColor(RGB(0,0,0));
			if(lpDrawItemStruct->itemID==11)
				dc.SetTextColor(RGB(196,196,196));
		}
		dc.SetBkColor(RGB(255,255,255));
		m_ComboColor.GetLBText(lpDrawItemStruct->itemID,str);
		dc.FillSolidRect(rect,theApp.m_pMainFrame->m_pDxpsDlg->m_ColorIndex[lpDrawItemStruct->itemID]);
		CRect rect2=rect;
		rect2.left+=3;
		dc.DrawText(str,rect2,DT_LEFT | DT_WORDBREAK);  
		if (lpDrawItemStruct->itemState & ODS_SELECTED)
		{
			CBrush brush(RGB(0,0,0));
			dc.FrameRect(&rect,&brush);
		}
		dc.Detach(); 
		return;
	}
	
	CDialog::OnDrawItem(nIDCtl, lpDrawItemStruct);
}

void CDxpsParamDlg::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
{
	if(nIDCtl==IDC_COMBO_COLOR)
	{
		RECT rect={1,1,1,9};
		MapDialogRect(&rect);
		lpMeasureItemStruct->itemHeight=rect.bottom;
		return;
	}
	
	CDialog::OnMeasureItem(nIDCtl, lpMeasureItemStruct);
}


void CDxpsParamDlg::OnButtonResetReg() 
{
	if(MessageBox("Are you sure to delete data of this region?  ", "Deleting region data", 
			   MB_ICONQUESTION|MB_OKCANCEL )==IDCANCEL)
		return;
	//”даление всех данных, св€занных с данным регионом
	DxpsOutList::iterator j;
	for (j = CDxpsRegion::OutData.begin(); j != CDxpsRegion::OutData.end();)
	{
		if(j->RegionN==m_RegionID)
		{
			j=CDxpsRegion::OutData.erase(j);
			if(CDxpsRegion::PassedNumberOfPoints>0)
				CDxpsRegion::PassedNumberOfPoints--;

		}
		else
			++j;
	}

	if(CDxpsRegion::PassedNumberOfPoints==0)
		CDxpsRegion::PassedCommonTime=0;

	EnableDisableControls();


	//≈сли проект был сохранен (или открыт), удал€ем его и переписываем заново
	if(theApp.m_pMainFrame->m_Doc.fpPrj)
	{
		fclose(theApp.m_pMainFrame->m_Doc.fpPrj);
		theApp.m_pMainFrame->m_Doc.fpPrj=fopen(theApp.m_pMainFrame->m_Doc.m_ProjectFile.FullPath, "w");
		SaveBinaryFile(theApp.m_pMainFrame->m_Doc.fpPrj);
	}
	theApp.m_pMainFrame->m_Doc.CheckDocType();
}

void CDxpsParamDlg::OnButtonResetAll() 
{
	//CSingleLock sLock(&MutexThread); //Ќе может сюда зайти при работающем потоке

	if(MessageBox("Are you sure to delete ALL data of ALL regions?  ", "Deleting ALL data", 
			   MB_ICONQUESTION|MB_OKCANCEL )==IDCANCEL)
		return;
	//THRI_LOCK();
	CDxpsRegion::OutData.clear();
	CDxpsRegion::PassedCommonTime=0;
	CDxpsRegion::PassedNumberOfPoints=0;
	//≈сли проект был сохранен (или открыт), удал€ем его и переписываем заново
	if(theApp.m_pMainFrame->m_Doc.fpPrj)
	{
		fclose(theApp.m_pMainFrame->m_Doc.fpPrj);
		theApp.m_pMainFrame->m_Doc.fpPrj=fopen(theApp.m_pMainFrame->m_Doc.m_ProjectFile.FullPath, "w");
		SaveBinaryFile(theApp.m_pMainFrame->m_Doc.fpPrj);
	}
	//THRI_UNLOCK();
	EnableDisableControls();
	theApp.m_pMainFrame->m_Doc.CheckDocType();
}

void CDxpsParamDlg::OnKillfocusEditHv() 
{
//CheckHV();
}

BOOL CDxpsParamDlg::CheckHV()
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
		if(m_HV!=(double)(int)m_HV)
		{
			MessageBox("HV must be integer.", "Attention");
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
		if(atof(str)!=(double)(int)atof(str))
		{
			MessageBox("HV must be integer.", "Attention");
			return FALSE;
		}
	}
	return TRUE;
}

BOOL CDxpsParamDlg::CheckDividentDivisor()
{
	char str[25];
	m_ComboDivident.GetWindowText(str,24);
	int Divident=atoi(str);
	m_ComboDivisor.GetWindowText(str,24);
	int Divisor=atoi(str);
	if(Divident==Divisor)
	{
		MessageBox("Divident and divisor cannot be equal.   ", "Invalid value");
		return FALSE;
	}
	if(Divident>CDxpsRegion::GetRegNumber())
	{
		MessageBox("Divident cannot be greater then existing region numbers.   ", "Invalid value");
		return FALSE;
	}
	if(Divisor>CDxpsRegion::GetRegNumber())
	{
		MessageBox("Divisor cannot be greater then existing region numbers.   ", "Invalid value");
		return FALSE;
	}
	if(Divident<1)
	{
		MessageBox("Divident cannot be less then 1.   ", "Invalid value");
		return FALSE;
	}
	if(Divisor<1)
	{
		MessageBox("Divisor cannot be less then 1.   ", "Invalid value");
		return FALSE;
	}
	if((Divident==m_RegionID+1) && !m_NewRegion)
	{
		MessageBox("Divident cannot be equal to current region.   ", "Invalid value");
		return FALSE;
	}
	if((Divisor==m_RegionID+1) && !m_NewRegion)
	{
		MessageBox("Divisor cannot be equal to current region.   ", "Invalid value");
		return FALSE;
	}	
	return TRUE;
}

BOOL CDxpsParamDlg::CheckBE()
{
//Retard>=0 Retard<=h_nu+50 - дл€ HP?

//HP:
//Retard=h_nu-BE-HV+fi(HV)
//BE<=h_nu-HV+fi(HV)
//BE>=-50-HV+fi(HV)
//KRATOS:
//Retard=h_nu-BE+fi(HV)+100
//BE<=h_nu+fi(HV)+100
//BE>=fi(HV)+50
	C_h_nu_Info h_nu;
	char str[250];
	if(theApp.Ini.HighPressureMode.Value) //HP
	{
		m_ComboHV.GetWindowText(str, 49);
		int HV=atoi(str);
		double MaxBE=h_nu.Value_h_nu[m_ComboAnode.GetCurSel()]-HV+theApp.m_pMainFrame->m_Doc.m_ThrComm.FiTable.GetFiByHV(HV);
		if(m_BE>MaxBE)
		{
			sprintf(str, "Maximum BE value is %g  \n\n(h_nu-HV+fi(HV)).", MaxBE);
			MessageBox(str, "Invalid value");
			return FALSE;
		}
		double MinBE=-50-HV+theApp.m_pMainFrame->m_Doc.m_ThrComm.FiTable.GetFiByHV(HV);
		if(m_BE<MinBE)
		{
			sprintf(str, "Minimum BE value is %g  \n\n(fi(HV)-HV-50).", MinBE);
			MessageBox(str, "Invalid value");
			return FALSE;
		}
	}
	else //KRATOS
	{
		double MaxBE=h_nu.Value_h_nu[m_ComboAnode.GetCurSel()]+theApp.m_pMainFrame->m_Doc.m_ThrComm.FiTable.GetFiByHV(m_HV)+100;
		if(m_BE>MaxBE)
		{
			sprintf(str, "Maximum BE value is %g  \n\n(h_nu+fi(HV)+100).", MaxBE);
			MessageBox(str, "Invalid value");
			return FALSE;
		}
		double MinBE=theApp.m_pMainFrame->m_Doc.m_ThrComm.FiTable.GetFiByHV(m_HV)+50;
		if(m_BE<MinBE)
		{
			sprintf(str, "Minimum BE value is %g  \n\n(fi(HV)+50).", MinBE);
			MessageBox(str, "Invalid value");
			return FALSE;
		}
	}
	return TRUE;
}

void CDxpsParamDlg::EnableDisableControls()
{
	BOOL TypeBE=(m_TypeBE==TYPE_BE);

	BOOL MeasureRun=theApp.m_pMainFrame->m_StartStop;
	BOOL RegionHasData=FALSE;
	if(m_pReg)
		if(m_pReg->HasData())
			RegionHasData=TRUE;

	BOOL EnableBE= !MeasureRun && !RegionHasData && TypeBE;
	BOOL EnableHV= !MeasureRun && !RegionHasData && TypeBE;
	BOOL EnableButtonResetAll= !MeasureRun && !CDxpsRegion::OutData.empty();
	BOOL EnableButtonResetReg= !MeasureRun && RegionHasData;
	BOOL EnableAnode= !MeasureRun && !RegionHasData && TypeBE;
	BOOL EnableDwell= !MeasureRun && !RegionHasData && TypeBE;
	BOOL EnableTypeBE= !MeasureRun && !RegionHasData;
	BOOL EnableTypeDiv= (CDxpsRegion::GetRegNumber()>2) && !RegionHasData;
	BOOL EnableDivisorDivident=(CDxpsRegion::GetRegNumber()>2) && !RegionHasData && !TypeBE;
	BOOL EnableDelay= TypeBE;

	m_ButtonResetReg.EnableWindow(EnableButtonResetReg);
	m_ButtonResetAll.EnableWindow(EnableButtonResetAll);
	GetDlgItem(IDC_EDIT_DXPSPAR_BE)->EnableWindow(EnableBE);
	GetDlgItem(IDC_EDIT_DXPSPAR_HV)->EnableWindow(EnableHV);
	m_ComboHV.EnableWindow(EnableHV);
	m_ComboAnode.EnableWindow(EnableAnode);
	GetDlgItem(IDC_EDIT_DXPSPAR_DWELL)->EnableWindow(EnableDwell);
	GetDlgItem(IDC_RADIO_DXPSPAR_TYPE_BE)->EnableWindow(EnableTypeBE);
	GetDlgItem(IDC_RADIO_DXPSPAR_TYPE_DIV)->EnableWindow(EnableTypeDiv);
	GetDlgItem(IDC_COMBO_DXPSPAR_DIVIDENT)->EnableWindow(EnableDivisorDivident);
	GetDlgItem(IDC_COMBO_DXPSPAR_DIVISOR)->EnableWindow(EnableDivisorDivident);
	GetDlgItem(IDC_EDIT_DXPSPAR_DELAY)->EnableWindow(EnableDelay);
}
