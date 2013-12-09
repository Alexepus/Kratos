//File FUNCTION.H
//FARPROC EditNormalProc;
#ifndef _FUNCTION_H_
#define _FUNCTION_H_

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
//BOOL SetFont(void);
BOOL WriteLOGFONTToIniFile(LPCTSTR lpszSection, LOGFONT* LogFont);
BOOL ChooseNewFont(HWND hWnd, LOGFONT* LogFont, COLORREF* Color);

BOOL NewOreEditParamToReg(CRegion* pReg, CDialogParamRegion* pDlgParamReg);

void SetNewRegionItemForListView(CListRegionWnd* pList, CRegion* pReg);

void UpdateTextItem(HWND hWnd, CRegion* pReg);

void SetIconForReg(CListRegionWnd* pList, CRegion* pReg, int Image);

//void RemoveIconForReg(CListRegionWnd* pList, CRegion* pReg);

int FindSelectedItem(HWND hWnd);
#define USER_STOP 1
void LeaveCrSecAndEndThread(CMainFrame* pMainFrame, CRegion* pReg, int Image, CSingleLock &tsLock, BOOL UserStop=FALSE);
void LeaveCrSecAndEndDxpsThread(CMainFrame* pMainFrame, CDxpsRegion* pReg, CSingleLock &tsLock, BOOL UserStop=FALSE);
void SaveMeasuringData(CMainFrame* pMainFrame, DATA_OUT* NewData, int NNewData);

void SaveStyle(HWND hWnd);

int D2I(double d);
double I2D(int i);

void SetNewTIME(int* TIME);
void TIME2Str(int TIME, char* str);
BOOL WriteDxpsRegionsParam(FILE *fp);
BOOL WriteDxpsPoints(FILE *fp, DxpsOutList::iterator iter);
BOOL WriteDxpsPoints(FILE *fp);
long GetRegisterHVCodeFromHV(double HV);
#endif