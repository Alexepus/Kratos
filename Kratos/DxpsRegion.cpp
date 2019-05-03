// DxpsRegion.cpp: implementation of the CDxpsRegion class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Main.h"
#include "DxpsRegion.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

int CDxpsRegion::m_NReg = 0;
double CDxpsRegion::ScanStartDateTime = 0;
CDxpsRegion* CDxpsRegion::m_pFirst = NULL;
CDxpsRegion* CDxpsRegion::m_pEnd = NULL;
DxpsOutList CDxpsRegion::OutData=DxpsOutList();
double CDxpsRegion::ScanTime=30;
double CDxpsRegion::PassedCommonTime=0;
int CDxpsRegion::PassedNumberOfPoints=0;

CDxpsRegion::~CDxpsRegion()
{
DxpsOutList::iterator j;
//Удаление всех данных, связанных с данным регионом, переобозначение данных
for (j = OutData.begin(); j != OutData.end();)
if(j->RegionN==ID)
{
	j=OutData.erase(j);
	if(PassedNumberOfPoints>0)
		PassedNumberOfPoints--;
}
else
{
	if(j->RegionN>ID)
		j->RegionN--;
	++j;
}
	
if(m_pNext!=NULL)
	m_pNext->m_pPrev=m_pPrev;
if(m_pPrev!=NULL)
	m_pPrev->m_pNext=m_pNext;
if (this==m_pFirst)
	m_pFirst=m_pNext;
if (this==m_pEnd)
	m_pEnd=m_pPrev;

CDxpsRegion *pReg;
int i;
for(pReg=GetFirst(), i=0;pReg!=NULL;pReg=GetNext(pReg), i++)
	{
	pReg->ID=i;
	}
m_NReg--;
}

bool CompareOutDataByTimeAndId(const DxpsOutData & a, const DxpsOutData & b)
{
	return a.Time < b.Time || (a.Time == b.Time && a.RegionN < b.RegionN);
}

//Расчитать данные для всех DIV регионов, для которых нет данных
void CDxpsRegion::CalcAllMissingDivRegions(std::function<void(DxpsOutData)> onOutDataAddedFunc)
{
	std::map<int, DxpsOutData> lastOutData;
	double measureTime = -1;
	for(auto i = OutData.begin(); i != OutData.end(); ++i)
	{
		if(i->Time != measureTime && !lastOutData.empty())
		{
			CalcDivDxpsRegionData(lastOutData, onOutDataAddedFunc);
			lastOutData.clear();
		}
		measureTime = i->Time;
		lastOutData[i->RegionN] = *i;
	}
	CalcDivDxpsRegionData(lastOutData, onOutDataAddedFunc);
	OutData.sort(CompareOutDataByTimeAndId);
}

CDxpsRegion::CDxpsRegion()
{
ID=CDxpsRegion::m_NReg; //
++m_NReg;
memset(&Parameters, 0, sizeof(DxpsRegPar));
Parameters.HV = 1000.;
Parameters.Dwell = 10.;
Parameters.Off = FALSE;

m_pNext = NULL;
if(m_NReg==1)
	{ m_pPrev = NULL;	m_pFirst = m_pEnd = this; }
else
	{ m_pPrev = m_pEnd; m_pPrev->m_pNext = m_pEnd =this; }
}

CDxpsRegion* CDxpsRegion::CreateNewRegion()
{
return new(CDxpsRegion);
}


CDxpsRegion* CDxpsRegion::GetRegByN(int N)
{
	if(N<0) 
		return NULL;
	CDxpsRegion *reg=CDxpsRegion::GetFirst();
	for(int i=0;i<N;i++)
	{
		if(reg!=NULL)
			reg=CDxpsRegion::GetNext(reg);
	}
	ASSERT(reg);
	return reg; //May be = NULL!!!
}

//__________________________________________________________________

BOOL CDxpsRegion::HasData() const
{
for(auto i=CDxpsRegion::OutData.begin(); i!=CDxpsRegion::OutData.end(); i++)
	if(i->RegionN==ID)
	{		//There is any data in this region
			return TRUE;
	}
return FALSE;
}
