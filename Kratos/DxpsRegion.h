// DxpsRegion.h: interface for the CDxpsRegion class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DXPSREGION_H__61D1EF66_AF22_4915_8DA9_2B3DAC194F9E__INCLUDED_)
#define AFX_DXPSREGION_H__61D1EF66_AF22_4915_8DA9_2B3DAC194F9E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Region.h"
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

class CDxpsData
{
protected:
	CDxpsData* m_pNext;

public:
	static CDxpsData* m_pFirst;
	CDxpsData(int regionN=255, double time=0, double freq=0)
	{OutData.RegionN=regionN;OutData.Time=time;OutData.Freq=freq;};
	static void DeleteRegData(int regionN);
	static CDxpsData* GetFirst(){return CDxpsData::m_pFirst;};
	CDxpsData* GetNext(){return m_pNext;};
	DxpsOutData OutData;
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
	BOOL HasData();
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
	//UINT m_ptrInFile;
	//DWORD m_NDataOut;
	//DWORD m_NDataOutCurr;
};

#endif // !defined(AFX_DXPSREGION_H__61D1EF66_AF22_4915_8DA9_2B3DAC194F9E__INCLUDED_)
