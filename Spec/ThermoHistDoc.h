// ThermoData.h: interface for the CThermoData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_THERMOHISTDOC_H__B588BB5B_91F3_4CDB_B088_153BC73B2F01__INCLUDED_)
#define AFX_THERMOHISTDOC_H__B588BB5B_91F3_4CDB_B088_153BC73B2F01__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <deque>
#include "ThreadSafeQueue.h"
#include "HSChart\\ChartCtrl.h"

using namespace std;
class CSpecView;

class CTempPoint
{
public:
	double Time;
	double Temp;
};

class CTempMessage
{
public:
	CTempPoint Pt;
	enum {eRefTemp, eMeasTemp} PointType;
};

//Класс, занимающийся отрисовкой графика температур. Принимает сообщения от HeatingThread 
//через очередь, чтение из которой инициируется сообщением в CSpecView
// Родительский объект - CSpecView

class CThermoHistDoc  
{
	typedef pair<double, double> PairDbl;
public:
	void DeleteExpiredData();
	void OnClearHistory();
	void CopyToClipboard();
	CString PrintEasyPlotData();
	void AddMeasTempPoint(double MeasT);
	double LastMeasTemp;
	void AddRefTempPoint(double RefT);
	double LastRefTemp;
	void AddPowerPoint(double Pwr);
	CThermoHistDoc();
	virtual ~CThermoHistDoc();
	CSpecView *m_pView;
	std::deque<CTempPoint> RefTemp;
	std::deque<CTempPoint> MeasTemp;
	std::deque<PairDbl> Power;
	//double PowerMax
	double TheVeryFirstTime;
	CThreadSafeQueue<CTempMessage> ThermoPlotQueue;

	CChartCtrl* GetChart();
	void FormatChartToThermoHist();
	BOOL IsThermoHistMode();
	void RecieveNewMessage();
	void RedrawChart(bool Refresh=true);

};

#endif // !defined(AFX_THERMOHISTDOC_H__B588BB5B_91F3_4CDB_B088_153BC73B2F01__INCLUDED_)
