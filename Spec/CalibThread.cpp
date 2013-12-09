#include "stdafx.h"
#include "Spec.h"
#include "CalibDlg.h"
#include "CamacUnits.h"
#include "VCMCDM.h"
#include "LogToFile.h"
#include "Threads.h"

#define FromAdcCodeToV(Code) ((022000-Code/(1<<theApp.Ini.AdcIntegration))/(1.+15*theApp.Ini.AdcRange)*0.001)

UINT CalibThread(LPVOID pParam)
{
	CCalibDlg *pCalibDlg=(CCalibDlg *)pParam;

	double Mass, Intensity=0;

	while(pCalibDlg->m_bMeasureStart)
	{
		for(Mass=theApp.Ini.CalibMassStart; Mass<=theApp.Ini.CalibMassStop && pCalibDlg->m_bMeasureStart; Mass+=theApp.Ini.CalibMassStep)
		{
			#ifndef SIMULATION
			if(!LoadMassDac(max(theApp.m_pMassCalibDoc->VoltageByMass(Mass), 0.)))
			{
				LogFile("CalibThread: LoadMassDac() failed");
				if(!LoadMassDac(theApp.m_pMassCalibDoc->VoltageByMass(Mass)))
					goto ReportHardwareError;
			}
			#endif
			Sleep(theApp.Ini.CalibMassSetupDelay*1000);
			#ifndef SIMULATION
			double OldIntensity=Intensity;
			if(!GetMassIntensityAdc(Intensity))
			{
				LogFile("CalibThread: GetMassIntensityAdc() failed");
				if(!GetMassIntensityAdc(Intensity))
					Intensity=OldIntensity;
				//goto ReportHardwareError;
			}
			#else
			Intensity=fabs(Intensity+rand()*4.096/RAND_MAX-2);
			#endif
			CalibPostNewPoint(Mass,Intensity,pCalibDlg);
		}
		if(Mass>theApp.Ini.CalibMassStop)
		{
			CalibPostEndSweep(pCalibDlg);
		}
	}
	PostMessage(pCalibDlg->GetSafeHwnd(),WM_NEW_CALIB_MSG,0,0);
	LoadMassDac(0);
	return 0;

ReportHardwareError:
	theApp.m_pMainWnd->MessageBox("Error: controller does not respond.\nMeasurement has been stopped!       ", "Spec", MB_OK|MB_ICONINFORMATION);
	LoadMassDac(0);
	return 0;
}

void CalibPostNewPoint(double Mass, double Voltage, CCalibDlg* pCalibDlg)
{
	CMassCalibMsg CalibMsg;
	CalibMsg.Action=CalibMsg.eNewPoint;
	CalibMsg.Mass=Mass;
	CalibMsg.Voltage=Voltage;
	pCalibDlg->m_CalibMsgQueue.Push(CalibMsg);
	PostMessage(pCalibDlg->GetSafeHwnd(),WM_NEW_CALIB_MSG,1,0);
}

void CalibPostEndSweep(CCalibDlg* pCalibDlg)
{
	CMassCalibMsg CalibMsg={CMassCalibMsg::eEndSweep,0,0};
	pCalibDlg->m_CalibMsgQueue.Push(CalibMsg);
	PostMessage(pCalibDlg->GetSafeHwnd(),WM_NEW_CALIB_MSG,1,0);
}
