// ThermoReg.h: interface for the CThermoReg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_THERMOREG_H__12D22E4B_6B64_4653_BA2C_B12A1984D984__INCLUDED_)
#define AFX_THERMOREG_H__12D22E4B_6B64_4653_BA2C_B12A1984D984__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "ThermoHistDoc.h"
#include "HeaterCommands.h"

#define TIMER_THERMOREG_1SEC 5
#define TIMER_THERMOREG_THERMOFUNC 6
#define TIMER_THERMOREG_STARTDELAY 7
using namespace std;

class CThermoReg  
{
private:
	CSpecView* m_pSpecView;
	vector<double> m_ThermoPair_mV; //Таблица в мВ
	double m_ThermoPairStartT, m_ThermoPairDeltaT;

	//Статусные флаги
	bool m_StatusThermoPairLoad; // Флаг от контроллера, что данные термопары загружены
	bool m_StatusComPort;		 // Флаг, что COM порт не имеет ошибок
	bool m_StatusController;	 // Флаг, что контроллер не выставил ошибок

	UINT wm_CurrentTempMessage;  // Сообщение о текущей температуре, посылаемое в Kratos 
	ComMsg msg;

	//Для теста
	int TestTC_i;
	FILE* TestTC_pFile;

public:
	UINT m_RegDxpsRemoteStartID;
	int RemoteDxpsStateCount;
	static void LoadDacValue(int Channel, int Value);
	static void SendManualHeaterMode(bool Manual);
	static void UploadFfTable();
	static void UploadPidParams();
	static void UploadPidCoefs();
	static void LoadProtectionVals();
	static void ResetProtection();
	void UpLoadThermoPairToController();
	void OnTimer(UINT nIDEvent);
	void TestTC(int i);
	bool IsEverythingOK(){return m_StatusThermoPairLoad && m_StatusComPort&&m_StatusController; };
	void Init(CSpecView* pSpecView);
	bool LoadThermoPairFile(CString FilePath);
	double GetTByThermoPairmV(double mV);
	double GetThermoPairmVByT(double T);
	LRESULT OnSerialEvent(WPARAM WParam, LPARAM LParam);
	LRESULT OnRemoteDxpsState(WPARAM WParam, LPARAM LParam);
	CThermoHistDoc m_Hist;
	CHeaterCommands m_Commands;

	CThermoReg();
	virtual ~CThermoReg();
};

#endif // !defined(AFX_THERMOREG_H__12D22E4B_6B64_4653_BA2C_B12A1984D984__INCLUDED_)
