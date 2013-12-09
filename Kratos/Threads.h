#ifndef _THREADS_H_INCLUDED_
#define _THREADS_H_INCLUDED_

class CMainFrame;

struct THREAD_COMMON
{
	FILE* fp;
	CRegion* pRegEdit;
	CRegion* pRegNow;
	//CRegionWnd* pRegWnd;
	double NSigma; //Number of RMS deviation of Y
	int Attempts; //Number of attempts to remeasure if delta Y > NSigma*sigma
	int RegionDelay; //Delay before starting new region, ms
	int SubMeasurings;
	BOOL EnableRemeasure; //If TRUE, on 2nd passage minimum value is calculated; on 3rd - remeasure if delta Y > NSigma*sigma
	HWND hWnd;
	CMainFrame* pMainFrame;
	enum {Stop, Continue} StopContinue;
	CFiTable FiTable;
	int TIME;
	double RetardCalibration;
	double LastRefTemperature; // Последняя полученная опорная температура
	double LastCurTemperature; // Последняя полученная измеренная температура
	double LastTemperatureTime; //Время получения температуры в формате COleDateTime

	THREAD_COMMON():LastTemperatureTime(0), LastRefTemperature(0), LastCurTemperature(0){}
};

UINT MeasuringThread(LPVOID pParam);
UINT DxpsThread(LPVOID pParam);

#endif //_THREADS_H_INCLUDED_ 