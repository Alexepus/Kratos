#ifndef _THREADS_H_
#define _THREADS_H_

//#define SIMULATION

//#define WM_UPDATE_THERMO_DATA	(WM_APP + 10003)
#define WM_MEAS_THREAD_DATA	(WM_APP + 10004)
// Описание сообщения WM_MEAS_THREAD_DATA:
// WPARAM HIWORD:
#define MTHR_REGION_DATA		0 
#define MTHR_REGIONLIST_BEGIN	1 
#define MTHR_REGIONLIST_END		2 
#define MTHR_THREAD_END			3 
// WPARAM LOWORD: 
// Для MTHR_REGION_DATA - номер региона
// LPARAM: - значение АЦП масс-спектрометра 

UINT CalibThread(LPVOID pParam);
UINT MeasuringThread(LPVOID pParam);
DWORD WINAPI HeatingThread(LPVOID pParam);
double EmulateMeasTemp(double RefTemp, bool On);

class CCalibDlg;
double CalcRefTemp(CThermoFunc *pThermoFunc, double TimeFromStart, double &CurrentSlope, double &TimeToInflection, double &TEnd);
void CalibPostNewPoint(double Mass, double Voltage, CCalibDlg* pCalibDlg);
void CalibPostEndSweep(CCalibDlg* pCalibDlg);
bool LoadMassDac(double Voltage);
bool GetMassIntensityAdc(double &Voltage);
bool GetMassIntensityAdcRaw(WORD &Code);

#endif
