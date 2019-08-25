#pragma once
#include "CRegion.h"
#include "DxpsRegion.h"
class CDialogParamRegion;
class CMainFrame;

#define THRI_LOCK() {\
while(1){\
	sLock.Lock(2000);\
	if(sLock.IsLocked())\
	{\
		LogFile("Lock", __FILE__, __LINE__);\
		break;\
	}\
	else\
	{\
		LogFile("Failed to Lock, query to user", __FILE__, __LINE__);\
		if(AfxMessageBox("Failed to lock Critical Section.\n\nTry again?", MB_RETRYCANCEL|MB_ICONEXCLAMATION)==IDRETRY)\
		{\
			LogFile("Retry was choosen");\
			continue;\
		}\
		else\
		{\
			LogFile("Cancel was choosen");\
			break;\
		}\
	}\
}}

#define THRI_UNLOCK() {sLock.Unlock(); LogFile("UnLock", __FILE__, __LINE__);}

class CListRegionWnd;

BOOL ReadLOGFONTFromIniFile(LPCTSTR lpszSection, LOGFONT* LogFont);
BOOL WriteLOGFONTToIniFile(LPCTSTR lpszSection, LOGFONT* LogFont);
BOOL ChooseNewFont(HWND hWnd, LOGFONT* LogFont, COLORREF* Color);

/**
* \brief ѕримен€ет параметры региона, заданные пользователем, в модели региона
* \param pReg ќбъект региона, в котором нужно изменить параметры
* \param pDlgParamReg ƒиалог, в котором пользователь помен€л параметры
*/
bool SetRegionParametersFromDialog(CRegion* pReg, CDialogParamRegion* pDlgParamReg);

#define USER_STOP 1
void LeaveCrSecAndEndThread(CMainFrame* pMainFrame, CRegion* pReg, int Image, CSingleLock &tsLock, BOOL UserStop=FALSE);
void LeaveCrSecAndEndDxpsThread(CMainFrame* pMainFrame, CDxpsRegion* pReg, CSingleLock &tsLock, BOOL UserStop=FALSE);
void AskAndSaveMeasuringData(CMainFrame* pMainFrame, DATA_OUT* NewData, int NNewData, time_t regionStartMeasureTime);

void GetXpsTimeRemainedToEnd(int* TIME);
void TIME2Str(int TIME, char* str);
long GetRegisterHVCodeFromHV(double HV);
char* GetAppTitle();

/**
 * ¬озвращает правильное окончание дл€ согласованного числительного в русском €зыке
 *
 * @param number     „исло
 * @param nominative именительный падеж;
 * @param genetive   родительный падеж;
 * @param plural     множественное число.
 */
CString GetI18nNumEnding(int number, char* nominative, char* genetive, char* plural);