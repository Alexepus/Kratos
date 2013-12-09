// AdcDacDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Spec.h"
#include "AdcDacDlg.h"
#include "ThermoReg.h"
#include "Functions.h"
#include <math.h>
#include "Resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAdcDacDlg dialog


CAdcDacDlg::CAdcDacDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAdcDacDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAdcDacDlg)
	//}}AFX_DATA_INIT
}


void CAdcDacDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAdcDacDlg)
	DDX_Control(pDX, IDC_CHECK_ADC_ON, m_CheckAdcOn);
	DDX_Control(pDX, IDC_STATIC_DAC1, m_StaticDac1);
	DDX_Control(pDX, IDC_STATIC_DAC0, m_StaticDac0);
	DDX_Control(pDX, IDC_CHECK_MANUAL_MSP, m_CheckManualMsp);
	DDX_Control(pDX, IDC_CHECK_MANUAL_HEATER, m_CheckManualHeater);
	DDX_Control(pDX, IDC_SLIDER_MSP, m_SliderMsp);
	DDX_Control(pDX, IDC_SLIDER_HEATER, m_SliderHeater);
	DDX_Control(pDX, IDC_STATIC_AVER_ADC2, m_StaticAverAdc2);
	DDX_Control(pDX, IDC_STATIC_AVER_ADC3, m_StaticAverAdc3);
	DDX_Control(pDX, IDC_STATIC_AVER_ADC1, m_StaticAverAdc1);
	DDX_Control(pDX, IDC_STATIC_AVER_ADC0, m_StaticAverAdc0);
	DDX_Control(pDX, IDC_STATIC_ADC3, m_StaticAdc3);
	DDX_Control(pDX, IDC_STATIC_ADC2, m_StaticAdc2);
	DDX_Control(pDX, IDC_STATIC_ADC1, m_StaticAdc1);
	DDX_Control(pDX, IDC_STATIC_ADC0, m_StaticAdc0);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAdcDacDlg, CDialog)
	//{{AFX_MSG_MAP(CAdcDacDlg)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_CHECK_MANUAL_HEATER, OnCheckManualHeater)
	ON_BN_CLICKED(IDC_CHECK_MANUAL_MSP, OnCheckManualMsp)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_HEATER, OnCustomdrawSliderHeater)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_MSP, OnCustomdrawSliderMsp)
	ON_BN_CLICKED(IDC_CHECK_ADC_ON, OnCheckAdcOn)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_COM_EVENT, OnSerialLinkEvent)

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAdcDacDlg message handlers

BOOL CAdcDacDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	theApp.m_SerialLink.m_Pool.RegisterMsgReception(GetSafeHwnd());
	SetTimer(TIMER_POLL_SERIAL,500,NULL);
	m_SliderHeater.SetRange(0,1023);
	m_SliderMsp.SetRange(0,1023);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CAdcDacDlg::OnDestroy() 
{
	theApp.m_SerialLink.m_Pool.UnRegisterMsgReception(GetSafeHwnd());
	CThermoReg::SendManualHeaterMode(false);
	CDialog::OnDestroy();
}

LRESULT CAdcDacDlg::OnSerialLinkEvent(WPARAM WParam, LPARAM LParam)
{
	int MsgType=HIWORD(LParam);
	BYTE Cmd=(BYTE)LOWORD(LParam);
	int AdcVal, Channel;
	CStatic* pStaticAdc[]={&m_StaticAdc0,&m_StaticAdc1,&m_StaticAdc2,&m_StaticAdc3};
	CStatic* pStaticAverAdc[]={&m_StaticAverAdc0,&m_StaticAverAdc1,&m_StaticAverAdc2,&m_StaticAverAdc3};
	CStatic* pStaticDac[]={&m_StaticDac0,&m_StaticDac1};
	static int NSample[4]={0,0,0,0},NAverage[4]={0,0,0,0}, 
		SumSample[4]={0,0,0,0}, SumAverage[4]={0,0,0,0};
	static double DNSample[4]={0,0,0,0}, DNAverage[4]={0,0,0,0};
	if(MsgType==SERIAL_SYNC_MSG)
	{
		if(Cmd==CMD_GET_EXT_ADC || Cmd==CMD_GET_AVERAGED_ADC)
		{
			ComMsg msg=theApp.m_SerialLink.m_Pool.GetMsg(WParam);

			AdcVal=BytesToShort(msg.Buf[2],msg.Buf[3]);
			Channel=msg.Buf[1];
			if((Channel<0)||(Channel>3))
				return (LRESULT)0;
			char ch[30];
			double delta, sigma;
			if(Cmd==CMD_GET_EXT_ADC)
			{
				NSample[Channel]++;
				SumSample[Channel]+=AdcVal;
				delta=AdcVal-SumSample[Channel]/(double)NSample[Channel];
				DNSample[Channel]+=delta*delta;
				sigma=sqrt(DNSample[Channel]/NSample[Channel]);

			}
			else
			{
				NAverage[Channel]++;
				SumAverage[Channel]+=AdcVal;
				delta=AdcVal-SumAverage[Channel]/(double)NAverage[Channel];
				DNAverage[Channel]+=delta*delta;
				sigma=sqrt(DNAverage[Channel]/NAverage[Channel]);
			}
			double U,I;
			switch(Channel)
			{
			case 1:
				I=BytesToShort(msg.Buf[2],msg.Buf[3])*(4.096/16384.)*(25./2.5); //В Амперах
				sprintf(ch,"%4d, %2.1fA",AdcVal,I);
				break;
			case 2:
				U=BytesToShort(msg.Buf[2],msg.Buf[3])*(4.096/16384.)*(25./2.5); //В Вольтах
				sprintf(ch,"%4d, %2.1fВ",AdcVal,U);
				break;
			default:
				sprintf(ch,"%4d",AdcVal);
			}
			if(Cmd==CMD_GET_EXT_ADC)
				pStaticAdc[Channel]->SetWindowText(ch);
			else
				pStaticAverAdc[Channel]->SetWindowText(ch);
		}
		else if(Cmd==CMD_GET_ADC_TC_I_U_TREF)
		{
			ComMsg msg=theApp.m_SerialLink.m_Pool.GetMsg(WParam);
			double I, U;
			int Adc0, Adc1, Adc2;
			char ch[30];
			Adc0=BytesToShort(msg.Buf[1],msg.Buf[2]); // Напряжение с термопары
			Adc1=BytesToShort(msg.Buf[3],msg.Buf[4]); // Ток нагревателя
			Adc2=BytesToShort(msg.Buf[5],msg.Buf[6]); // Напряжение на нагревателе
			I=Adc1*(4.096/16384.)*(25./2.5); //В Амперах
			U=Adc2*(4.096/16384.)*(25./2.5); //В Вольтах
			sprintf(ch,"%4d",Adc0);
			pStaticAverAdc[0]->SetWindowText(ch);
			sprintf(ch,"%4d, %2.1fA",Adc1,I);
			pStaticAverAdc[1]->SetWindowText(ch);
			sprintf(ch,"%4d, %2.1fV",Adc2,U);
			pStaticAverAdc[2]->SetWindowText(ch);

		}
		else if(Cmd==CMD_GET_DAC_VAL)
		{
			ComMsg msg=theApp.m_SerialLink.m_Pool.GetMsg(WParam);
			Channel=msg.Buf[1];
			if((Channel<0)||(Channel>1))
				return (LRESULT)0;
			char ch[30];
			AdcVal=BytesToShort(msg.Buf[2],msg.Buf[3]);
			sprintf(ch,"%4d, %2.2fV",AdcVal,AdcVal*(5./16384.));
			pStaticDac[Channel]->SetWindowText(ch);

		}
	}
	else if(MsgType==SERIAL_ERROR_COM_FAILED || MsgType==SERIAL_ERROR_TIMEOUT || MsgType==SERIAL_ASYNC_MSG)
	{
		for(Channel=0;Channel<4;Channel++)
		{
			pStaticAdc[Channel]->SetWindowText("...");
			pStaticAverAdc[Channel]->SetWindowText("...");
			if(Channel<sizeof(pStaticDac)/sizeof(void*))
				pStaticDac[Channel]->SetWindowText("...");
		}

	}
	return (LRESULT)0;
}

void CAdcDacDlg::OnTimer(UINT nIDEvent) 
{
	switch(nIDEvent)
	{
		case TIMER_POLL_SERIAL:
		{
			ComMsg msg;
			msg.WaitForReply=true;
			msg.MsgSize=2;
			if(m_CheckAdcOn.GetCheck())
			{
			msg.Buf[0]=CMD_GET_EXT_ADC;
			msg.Buf[1]=0;
			theApp.m_SerialLink.SendComMessage(msg);
			msg.Buf[1]=1;
			theApp.m_SerialLink.SendComMessage(msg);
			msg.Buf[1]=2;
			theApp.m_SerialLink.SendComMessage(msg);
			msg.Buf[1]=3;
			theApp.m_SerialLink.SendComMessage(msg);
			}
			msg.Buf[0]=CMD_GET_AVERAGED_ADC;
			//msg.Buf[1]=0;
			//theApp.m_SerialLink.SendComMessage(msg);
			//msg.Buf[1]=1;
			//theApp.m_SerialLink.SendComMessage(msg);
			//msg.Buf[1]=2;
			//theApp.m_SerialLink.SendComMessage(msg);
			msg.Buf[1]=3;
			theApp.m_SerialLink.SendComMessage(msg);
			msg.Buf[0]=CMD_GET_DAC_VAL;
			msg.Buf[1]=0;
			theApp.m_SerialLink.SendComMessage(msg);
			msg.Buf[1]=1;
			theApp.m_SerialLink.SendComMessage(msg);

			break;
		}
		case TIMER_HEATER:
		{
			KillTimer(nIDEvent);
			CThermoReg::LoadDacValue(0, m_SliderHeater.GetPos()*16);
			break;
		}
		case TIMER_MSP:
		{
			KillTimer(nIDEvent);
			CThermoReg::LoadDacValue(1, m_SliderMsp.GetPos()*16);
			break;
		}

	}	
	CDialog::OnTimer(nIDEvent);
}

void CAdcDacDlg::OnCheckManualHeater() 
{
	bool Checked=m_CheckManualHeater.GetCheck();
	m_SliderHeater.EnableWindow(Checked);
	CThermoReg::SendManualHeaterMode(Checked);
	if(Checked)
	{
		CThermoReg::LoadDacValue(0, m_SliderHeater.GetPos()*16);
	}
}

void CAdcDacDlg::OnCheckManualMsp() 
{
	bool Checked=m_CheckManualMsp.GetCheck();
	m_SliderMsp.EnableWindow(Checked);
	if(Checked)
	{
		CThermoReg::LoadDacValue(1, m_SliderMsp.GetPos()*16);
	}
}

void CAdcDacDlg::OnCustomdrawSliderHeater(NMHDR* pNMHDR, LRESULT* pResult) 
{
	SetTimer(TIMER_HEATER,500,NULL);
	*pResult = 0;
}

void CAdcDacDlg::OnCustomdrawSliderMsp(NMHDR* pNMHDR, LRESULT* pResult) 
{
	SetTimer(TIMER_MSP,500,NULL);
	*pResult = 0;
}

void CAdcDacDlg::OnCheckAdcOn() 
{
	CStatic* pStaticAdc[]={&m_StaticAdc0,&m_StaticAdc1,&m_StaticAdc2,&m_StaticAdc3};
	BOOL Check=m_CheckAdcOn.GetCheck();
	for(int Channel=0;Channel<4;Channel++)
		pStaticAdc[Channel]->EnableWindow(Check);

}
