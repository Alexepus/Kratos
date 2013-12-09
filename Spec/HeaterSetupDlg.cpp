// HeaterSetupDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Spec.h"
#include "HeaterSetupDlg.h"
#include "Functions.h"
#include "AdcDacDlg.h"
#include "ThermoReg.h"
#include "FeedForwTableDlg.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHeaterSetupDlg dialog


CHeaterSetupDlg::CHeaterSetupDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CHeaterSetupDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CHeaterSetupDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_bExist=false;
}


void CHeaterSetupDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHeaterSetupDlg)
	DDX_Control(pDX, IDC_EDIT_VI_DECAY, m_EditViDecay);
	DDX_Control(pDX, IDC_EDIT_DERIV_SAMPLES, m_EditDerivSamples);
	DDX_Control(pDX, IDC_STATIC_VP_F, m_StaticVpF);
	DDX_Control(pDX, IDC_STATIC_VI_F, m_StaticViF);
	DDX_Control(pDX, IDC_STATIC_VD_F, m_StaticVdF);
	DDX_Control(pDX, IDC_STATIC_PID_OUT, m_StaticPidOut);
	DDX_Control(pDX, IDC_STATIC_FEEDFORW_VF, m_StaticFeedForwVF);
	DDX_Control(pDX, IDC_STATIC_FEEDFORW_V, m_StaticFeedForwV);
	DDX_Control(pDX, IDC_EDIT_ZERO_VI, m_EditZeroVi);
	DDX_Control(pDX, IDC_EDIT_VI_MAX, m_EditViMax);
	DDX_Control(pDX, IDC_EDIT_KFF, m_EditKff);
	DDX_Control(pDX, IDC_EDIT_KP, m_EditKp);
	DDX_Control(pDX, IDC_EDIT_KI, m_EditKi);
	DDX_Control(pDX, IDC_EDIT_KD, m_EditKd);
	DDX_Control(pDX, IDC_STATIC_OUTPUT, m_StaticOutput);
	DDX_Control(pDX, IDC_STATIC_VP, m_StaticVP);
	DDX_Control(pDX, IDC_STATIC_VI, m_StaticVI);
	DDX_Control(pDX, IDC_STATIC_VD, m_StaticVD);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CHeaterSetupDlg, CDialog)
	//{{AFX_MSG_MAP(CHeaterSetupDlg)
	ON_BN_CLICKED(IDC_BUTTON_PID_COEF_LOAD, OnButtonPidCoefLoad)
	ON_WM_SHOWWINDOW()
	ON_EN_KILLFOCUS(IDC_EDIT_KD, OnKillfocusEditKd)
	ON_EN_KILLFOCUS(IDC_EDIT_KI, OnKillfocusEditKi)
	ON_EN_KILLFOCUS(IDC_EDIT_KP, OnKillfocusEditKp)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_ADC_DAC, OnButtonAdcDac)
	ON_BN_CLICKED(IDC_BUTTON_PID_PARAM_LOAD2, OnButtonPidParamLoad)
	ON_EN_KILLFOCUS(IDC_EDIT_KFF, OnKillfocusEditKff)
	ON_EN_KILLFOCUS(IDC_EDIT_DERIV_SAMPLES, OnKillfocusEditDerivSamples)
	ON_EN_KILLFOCUS(IDC_EDIT_VI_MAX, OnKillfocusEditViMax)
	ON_EN_KILLFOCUS(IDC_EDIT_ZERO_VI, OnKillfocusEditZeroVi)
	ON_EN_KILLFOCUS(IDC_EDIT_VI_DECAY, OnKillfocusEditViDecay)
	ON_BN_CLICKED(IDC_BUTTON_FF_TABLE, OnButtonFfTable)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_COM_EVENT, OnComEvent)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHeaterSetupDlg message handlers

void CHeaterSetupDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	if(bShow)
	{
		m_bExist=true;
	}	
}

BOOL CHeaterSetupDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	theApp.m_SerialLink.m_Pool.RegisterMsgReception(m_hWnd);
	SetTimer(TIMER_POLL_PID_VALS,500,NULL);
	char ch[30];
	
	sprintf(ch,"%.3f",CorrectInt16Q8(theApp.Ini.PidKp));
	m_EditKp.SetWindowText(ch);
	sprintf(ch,"%.3f",CorrectInt16Q8(theApp.Ini.PidKi));
	m_EditKi.SetWindowText(ch);
	sprintf(ch,"%.3f",CorrectInt16Q8(theApp.Ini.PidKd));
	m_EditKd.SetWindowText(ch);
	sprintf(ch,"%.3f",CorrectInt16Q8(theApp.Ini.TRegKff));
	m_EditKff.SetWindowText(ch);
	sprintf(ch,"%i",MinMax(0,16,theApp.Ini.PidDifShift));
	m_EditDerivSamples.SetWindowText(ch);
	sprintf(ch,"%.3f",min(255/256.,floor(theApp.Ini.PidViDecay*256+0.5)/256.));
	m_EditViDecay.SetWindowText(ch);
	sprintf(ch,"%.2f",CorrectInt16Q4(theApp.Ini.PidViMinMax));
	m_EditViMax.SetWindowText(ch);
	sprintf(ch,"%.2f",CorrectInt16Q4(theApp.Ini.PidZeroVi));
	m_EditZeroVi.SetWindowText(ch);

	m_StaticVP.SetWindowText("...");
	m_StaticVpF.SetWindowText("...");			
	m_StaticVI.SetWindowText("...");
	m_StaticViF.SetWindowText("...");
	m_StaticVD.SetWindowText("...");
	m_StaticVdF.SetWindowText("...");
	m_StaticPidOut.SetWindowText("...");
	m_StaticFeedForwV.SetWindowText("...");
	m_StaticFeedForwVF.SetWindowText("...");
	m_StaticOutput.SetWindowText("...");


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CHeaterSetupDlg::OnKillfocusEditKd() 
{
	char ch[30];
	m_EditKd.GetWindowText(ch,sizeof(ch));
	sprintf(ch,"%.3f",CorrectInt16Q8(atof(ch)));
	m_EditKd.SetWindowText(ch);
}

void CHeaterSetupDlg::OnKillfocusEditKi() 
{
	char ch[30];
	m_EditKi.GetWindowText(ch,sizeof(ch));
	sprintf(ch,"%.3f",CorrectInt16Q8(atof(ch)));
	m_EditKi.SetWindowText(ch);
}

void CHeaterSetupDlg::OnKillfocusEditKp() 
{
	char ch[30];
	m_EditKp.GetWindowText(ch,sizeof(ch));
	sprintf(ch,"%.3f",CorrectInt16Q8(atof(ch)));
	m_EditKp.SetWindowText(ch);
}

void CHeaterSetupDlg::OnKillfocusEditKff() 
{
	char ch[30];
	m_EditKff.GetWindowText(ch,sizeof(ch));
	sprintf(ch,"%.3f",CorrectInt16Q8(atof(ch)));
	m_EditKff.SetWindowText(ch);	
}

void CHeaterSetupDlg::OnKillfocusEditDerivSamples() 
{
	char ch[30];
	m_EditDerivSamples.GetWindowText(ch,sizeof(ch));
	sprintf(ch,"%i",MinMax(atoi(ch),1,16));
	m_EditDerivSamples.SetWindowText(ch);	
}

void CHeaterSetupDlg::OnKillfocusEditViDecay() 
{
	char ch[30];
	m_EditViDecay.GetWindowText(ch,sizeof(ch));
	sprintf(ch,"%.3f",min(255/256.,floor(atof(ch)*256+0.5)/256.));
	m_EditViDecay.SetWindowText(ch);	
}

void CHeaterSetupDlg::OnKillfocusEditViMax() 
{
	char ch[30];
	m_EditViMax.GetWindowText(ch,sizeof(ch));
	sprintf(ch,"%.2f",CorrectInt16Q4(atof(ch)));
	m_EditViMax.SetWindowText(ch);	
}

void CHeaterSetupDlg::OnKillfocusEditZeroVi() 
{
	char ch[30];
	m_EditZeroVi.GetWindowText(ch,sizeof(ch));
	sprintf(ch,"%.2f",CorrectInt16Q4(atof(ch)));
	m_EditZeroVi.SetWindowText(ch);	
}

void CHeaterSetupDlg::OnOK() 
{
	SavePidCoefs();
	SavePidParam();
	theApp.Ini.Save();

	CDialog::OnOK();
	m_bExist=false;
	DestroyWindow();
}

void CHeaterSetupDlg::SavePidCoefs()
{
	char ch[30];
	m_EditKd.GetWindowText(ch,sizeof(ch));
	theApp.Ini.PidKd=CorrectInt16Q8(atof(ch));
	m_EditKi.GetWindowText(ch,sizeof(ch));
	theApp.Ini.PidKi=CorrectInt16Q8(atof(ch));
	m_EditKp.GetWindowText(ch,sizeof(ch));
	theApp.Ini.PidKp=CorrectInt16Q8(atof(ch));
	m_EditKff.GetWindowText(ch,sizeof(ch));
	theApp.Ini.TRegKff=CorrectInt16Q8(atof(ch));
}

void CHeaterSetupDlg::OnButtonPidCoefLoad()
{
	SavePidCoefs();
	CThermoReg::UploadPidCoefs();
}

LRESULT CHeaterSetupDlg::OnComEvent(WPARAM WParam, LPARAM LParam)
{
	int MsgType=HIWORD(LParam);
	BYTE Cmd=(BYTE)LOWORD(LParam);
	double Vp,Vi,Vd,VPidOut, Vff, VpF, ViF, VdF, VffF, Vout;
	if(Cmd==CMD_GET_PIDVALS && MsgType==SERIAL_SYNC_MSG)
	{
		ComMsg msg=theApp.m_SerialLink.m_Pool.GetMsg(WParam);
		Vp=BytesToShort(msg.Buf[1],msg.Buf[2])/16.;
		Vi=BytesToShort(msg.Buf[3],msg.Buf[4])/16.;
		Vd=BytesToShort(msg.Buf[5],msg.Buf[6])/16.;
		VPidOut=BytesToShort(msg.Buf[7],msg.Buf[8])/16.;
		Vff=BytesToShort(msg.Buf[9],msg.Buf[10])/16.;
		Vout=BytesToShort(msg.Buf[11],msg.Buf[12])/16.;
		VpF=Vp*theApp.Ini.PidKp;
		ViF=Vi*theApp.Ini.PidKi;
		VdF=Vd*theApp.Ini.PidKd;
		VffF=VffF=Vff*theApp.Ini.TRegKff;
		char ch[30];
		sprintf(ch,"%.2f",Vp);
		m_StaticVP.SetWindowText(ch);
		sprintf(ch,"%.2f",VpF);
		m_StaticVpF.SetWindowText(ch);
		sprintf(ch,"%.2f",Vi);
		m_StaticVI.SetWindowText(ch);
		sprintf(ch,"%.2f",ViF);
		m_StaticViF.SetWindowText(ch);
		sprintf(ch,"%.2f",Vd);
		m_StaticVD.SetWindowText(ch);
		sprintf(ch,"%.2f",VdF);
		m_StaticVdF.SetWindowText(ch);
		sprintf(ch,"%.2f",VPidOut);
		m_StaticPidOut.SetWindowText(ch);
		sprintf(ch,"%.2f",Vff);
		m_StaticFeedForwV.SetWindowText(ch);
		sprintf(ch,"%.2f",VffF);
		m_StaticFeedForwVF.SetWindowText(ch);
		sprintf(ch,"%.2f",Vout);
		m_StaticOutput.SetWindowText(ch);

	}
	else if(MsgType==SERIAL_ERROR_COM_FAILED || MsgType==SERIAL_ERROR_TIMEOUT)
	{
		m_StaticVP.SetWindowText("...");
		m_StaticVpF.SetWindowText("...");			
		m_StaticVI.SetWindowText("...");
		m_StaticViF.SetWindowText("...");
		m_StaticVD.SetWindowText("...");
		m_StaticVdF.SetWindowText("...");
		m_StaticPidOut.SetWindowText("...");
		m_StaticFeedForwV.SetWindowText("...");
		m_StaticFeedForwVF.SetWindowText("...");
		m_StaticOutput.SetWindowText("...");
	}

	return (LRESULT)0;
}


void CHeaterSetupDlg::OnDestroy() 
{
	theApp.m_SerialLink.m_Pool.UnRegisterMsgReception(GetSafeHwnd());
	CDialog::OnDestroy();
}

void CHeaterSetupDlg::OnTimer(UINT nIDEvent) 
{
	switch(nIDEvent)
	{
		case TIMER_POLL_PID_VALS:
		{
			ComMsg msg;
			msg.Buf[0]=CMD_GET_PIDVALS;
			msg.MsgSize=1;
			msg.WaitForReply=true;
			theApp.m_SerialLink.SendComMessage(msg);
			break;
		}
	}
	
	CDialog::OnTimer(nIDEvent);
}

void CHeaterSetupDlg::OnButtonAdcDac() 
{
	CAdcDacDlg dlg;
	dlg.DoModal();	
}

void CHeaterSetupDlg::SavePidParam()
{
	char ch[30];
	m_EditDerivSamples.GetWindowText(ch,sizeof(ch));
	theApp.Ini.PidDifShift=atoi(ch);	
	m_EditViDecay.GetWindowText(ch,sizeof(ch));
	theApp.Ini.PidViDecay=atof(ch);
	m_EditViMax.GetWindowText(ch,sizeof(ch));
	theApp.Ini.PidViMinMax=atof(ch);
	m_EditZeroVi.GetWindowText(ch,sizeof(ch));
	theApp.Ini.PidZeroVi=atof(ch);
}

void CHeaterSetupDlg::OnButtonPidParamLoad() 
{
	SavePidParam();
	CThermoReg::UploadPidParams();
}

void CHeaterSetupDlg::OnCancel()
{
	CDialog::OnCancel();
	m_bExist=false;
	DestroyWindow();
}

void CHeaterSetupDlg::OnButtonFfTable() 
{
	CFeedForwTableDlg dlg;
	dlg.DoModal();
}
