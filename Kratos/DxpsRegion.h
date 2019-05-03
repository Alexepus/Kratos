#pragma once

#include "CRegion.h"
#include <list>
struct DxpsOutData_v1
{
	int RegionN;
	double Time;
	double Freq;
};

struct DxpsOutData
{
	int RegionN;
	double Time;
	double Freq;
	double Tref; // Задаточная температура, может быть NaN, т.е. не задана
	double Tcur; // Текущая измеренная температура, может быть NaN, т.е. неизвестна
};

#define TYPE_BE 0
#define TYPE_DIV 1

struct DxpsRegPar
{
	BOOL Off;
	int Type; //0 if "BE Print", 1 if Division Divident/Divisor
	int Divident;
	int Divisor;
	double BE;
	double HV;
	int Anode;//0:Al, 1:Mg, 2:He1, 3:He2
	double Delay; //Задержка перед началом измерений
	double Dwell;	//Время измерения
	int ColorIndex;
	char Comments[256];
	//enum{Al, Mg, He1, He2};
};

typedef  std::list<DxpsOutData> DxpsOutList;

class CDxpsRegion 
{
protected:
	static int m_NReg; // Number of regions
	static CDxpsRegion* m_pFirst;
	static CDxpsRegion* m_pEnd;
	//static CDxpsData* m_pData;
	CDxpsRegion* m_pNext;
	CDxpsRegion* m_pPrev;

public:
	BOOL HasData() const;
	static DxpsOutList OutData;
	static double ScanTime;			// Global Scan Time, sec
	static double PassedCommonTime; // Passed Time, common for all regions
	static double ScanStartDateTime; // Дата/время начала измерения
	static int PassedNumberOfPoints;
	static CDxpsRegion* CreateNewRegion();
	static CDxpsRegion* GetFirst(){return m_pFirst;}
	static CDxpsRegion* GetNext(CDxpsRegion* reg){return reg->m_pNext;}
	static CDxpsRegion* GetLast(){return m_pEnd;}
	static CDxpsRegion* GetRegByN(int N);
	static int GetRegNumber(){return m_NReg;}
	CDxpsRegion(); // Это не просто создание объекта, но и построение связного списка 
	~CDxpsRegion();
	int ID;
	DxpsRegPar Parameters;
	static void CalcAllMissingDivRegions(std::function<void(DxpsOutData)> onOutDataAddedFunc);
};