#include "stdafx.h"
#include "Functions.h"
#include <math.h>

int VerifyThermoFunction(CThermoFunc *pTF)
{
	if((pTF->BetaPlus<1e-9) | (pTF->BetaMinus<1e-9))
		return TF_INVALID_BETAS;
	if(fabs((pTF->Temp1-pTF->Temp2)/pTF->BetaPlus)+	pTF->Time1+fabs((pTF->Temp1-pTF->Temp2)
		/pTF->BetaMinus)+pTF->Time2 <0.5)
		return TF_INVALID_TOTAL_PERIOD;
	if((pTF->CoolingRate<1e-9) | (pTF->HeatingRate<1e-9))
		return TF_INVALID_COOLING_HEATING_RATE;
	if(pTF->Temp1<pTF->Temp2)
		return TF_ERROR_T1_LW_T2;
	return TF_OK;
}

void ReportThermoFunctionError(int ErrCode, CWnd *pWnd)
{
	switch(ErrCode)
	{
		case TF_INVALID_BETAS:
			pWnd->MessageBox("ThermoFunction parameters (beta) are incorrect.     ", "Spec - TR Error", MB_OK|MB_ICONEXCLAMATION);
			break;
		case TF_INVALID_TOTAL_PERIOD:
			pWnd->MessageBox("ThermoFunction parameters (total period) are incorrect.     ", "Spec - TR Error", MB_OK|MB_ICONEXCLAMATION);
			break;
		case TF_INVALID_COOLING_HEATING_RATE:
			pWnd->MessageBox("ThermoFunction parameters (cooling or heating rate)         \nare incorrect.", "Spec - TR Error", MB_OK|MB_ICONEXCLAMATION);
			break;
		case TF_ERROR_T1_LW_T2:
			pWnd->MessageBox("ThermoFunction parameters (T1 < T2) are incorrect        .", "Spec - TR Error", MB_OK|MB_ICONEXCLAMATION);
			break;
	}
}

double GetTimeDouble()
{
	SYSTEMTIME SysTime;
	FILETIME FileTime; 
	GetSystemTime(&SysTime);
	SystemTimeToFileTime(&SysTime, &FileTime);
	return FileTime.dwLowDateTime*100e-9+FileTime.dwHighDateTime*429.4967296;
}

//Возвращает остаток о деления Dividen/Divisor. 
//Аналог % для целых чисел
//Mod(5,3)==2; Mod(-5,3)==-2; Mod(5,-3)==2; Mod(-5,-3)==-2;
double Mod(double Divident, double Divisor)
{
	double AbsDivisor=fabs(Divisor);
	double AbsDivident=fabs(Divident);
	double Sign=(Divident<0)? -1.:1;

	return (AbsDivident-floor(fabs(AbsDivident/AbsDivisor))*AbsDivisor)*Sign;
}

bool VerifyGUIObject(CWnd *pWnd)
{
	if(pWnd)
		if(IsWindow(pWnd->GetSafeHwnd()))
			return true;
	return false;
}

CString PrintfFTruncate(double Val, int Precision)
{
	CString str;
	str.Format("%.*f", Precision, Val);
	if (Precision>0) // Должна быть точка
	{
		while (str.GetLength()>0)
		{
			if(str[str.GetLength()-1] != '0' && str[str.GetLength()-1] != '.')
				break;
			if(str[str.GetLength()-1] == '.')
			{
				str.Delete(str.GetLength()-1);
				break;
			}
			str.Delete(str.GetLength()-1);
		}
	}
	return str;
}

// Округляет число с точностью до RoundBy
double Round( double Val, double RoundBy)
{
	return floor(Val/RoundBy +0.5)*RoundBy;
}