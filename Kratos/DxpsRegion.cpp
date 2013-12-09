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
//�������� ���� ������, ��������� � ������ ��������, ��������������� ������
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

CDxpsData* CDxpsData::m_pFirst=NULL;
void CDxpsData::DeleteRegData(int regionN)
{
	CDxpsData *pBeforeDeleted=NULL, *pNextOfDeleted=NULL;
	for(CDxpsData *pData=CDxpsData::GetFirst(); pData!=NULL; )
	{
		if(pData->OutData.RegionN==regionN)
		{
			if(pData==CDxpsData::GetFirst())
			{
				m_pFirst=pData->GetNext();
			}
			if(pBeforeDeleted!=NULL)
				pBeforeDeleted->m_pNext=pData->GetNext();
			pNextOfDeleted=pData->GetNext();
			delete (pData);
			pData=pNextOfDeleted;
		}
		else
		{
			if(pData->OutData.RegionN>regionN)
				pData->OutData.RegionN--;
			pBeforeDeleted=pData;
			pData=pData->GetNext();
		}
	}

}

BOOL CDxpsRegion::HasData()
{
DxpsOutList::iterator i;
for(i=CDxpsRegion::OutData.begin(); i!=CDxpsRegion::OutData.end(); i++)
	if(i->RegionN==ID)
	{		//There is any data in this region
			return TRUE;
	}
return FALSE;
}
