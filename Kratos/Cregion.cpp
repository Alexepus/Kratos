#include "stdafx.h"
#include "Region.h"

int D2I(double d) { return ((int)(d*1000.0)); }
double I2D(int i) { return (((double)i) / 1000.0); }

int CRegion::m_NReg = 0;
CRegion* CRegion::m_pFirst = NULL;
CRegion* CRegion::m_pEnd = NULL;

CRegion::CRegion() { CreateNewRegion(); }

CRegion::CRegion(int n) : m_NDataOut(n)
{
	CreateNewRegion();
	int N = m_NDataOut*sizeof(DATA_OUT);
	m_pDataOut = (DATA_OUT*) malloc(N);
	if(m_pDataOut) memset(m_pDataOut, 0, N);
}

void CRegion::CreateNewRegion()
{
	ID=CRegion::m_NReg;
	++m_NReg;
	memset(&m_DataIn, 0, sizeof(DATA_IN));
	m_DataIn.N_ = 1;
	m_DataIn.Step = D2I(0.1);
	m_DataIn.KE_Start = D2I(-50.0);
	m_DataIn.KE_End = D2I(-50.0 + 0.1);

	m_DataIn.HV = D2I(25);
	m_DataIn.Time = D2I(0.3);
	m_DataIn.Off = TRUE;
	m_DataIn.KE_BE = DATA_IN::EnergyType::KE;
	m_DataIn.N_h_nu = 0; 
	m_DataIn.DeltaVolts = 0;
	m_DataIn.Priority = 10;
	m_DataIn.LastEditTime = 0;

	m_NDataOut=0;
	m_NDataOutCurr=0;
	m_BeginTime = 0;
	m_EndTime = 0;

	UpdateStrValues();	

	m_pDataOut = NULL;
	m_pNext = NULL;
	if(m_NReg==1)
	{
		m_pPrev = NULL;	
		m_pFirst = m_pEnd = this;
	}
	else
	{
		m_pPrev = m_pEnd; m_pPrev->m_pNext = this; m_pEnd = this;
	}
	// Initial KE, HV, and over data of region
}

/**
* \brief Обновить значения в структуре STR_PAR str из данных DATA_IN m_DataIn
*/
void CRegion::UpdateStrValues()
{
	sprintf(str.HV, "%.0lf", I2D(m_DataIn.HV));
	sprintf(str.KE_Start, "%.3lf", I2D(m_DataIn.KE_Start));
	sprintf(str.KE_End, "%.3lf", I2D(m_DataIn.KE_End));
	sprintf(str.Step, "%.3lf", I2D(m_DataIn.Step));
	sprintf(str.Time, "%.2lf", I2D(m_DataIn.Time));
	sprintf(str.N_, "%i", m_DataIn.N_);
	sprintf(str.Curr_N, "%i", m_DataIn.Curr_N);
	if (m_DataIn.KE_BE == DATA_IN::EnergyType::KE)
		sprintf(str.KE_BE, "%s", "KE");
	else  
		sprintf(str.KE_BE, "%s", "BE");
	sprintf(str.Name_h_nu, "%s", h_nu_Info.strName_h_nu[m_DataIn.N_h_nu]);
	str.Comments = m_DataIn.Comments;
	sprintf(str.Priority, "%i", m_DataIn.Priority);
}


CRegion::~CRegion()
{
	if(m_pDataOut!=NULL) free(m_pDataOut);
	if(m_pNext==NULL)
	{	
		if(m_pPrev!=NULL)
				{ m_pPrev->m_pNext = NULL;
					m_pEnd = m_pPrev;
				}
	}
	else if(m_pPrev==NULL)
	{
		m_pNext->m_pPrev = NULL; 
		m_pFirst = m_pNext;
	}
	else
	{
		m_pPrev->m_pNext = m_pNext; 
		m_pNext->m_pPrev = m_pPrev;
	}
	--m_NReg;
	if(m_NReg == 0) 
		m_pFirst = m_pEnd = NULL;

	if(m_pFirst != NULL)
	{
		m_pFirst->ID = 0;
		CRegion* reg = m_pFirst;
		while(reg = CRegion::GetNext(reg)) 
			reg->ID = reg->m_pPrev->ID + 1;
	}
}

CRegion* CRegion::GetFirst(void)
{
	if(m_pFirst) 
		return m_pFirst;
	return NULL;
}

CRegion* CRegion::GetNext(CRegion* reg)
{
	if(!m_pFirst || !reg) 
		return NULL;
	return reg->m_pNext;
}

CRegion* CRegion::GetAtPosition(int position)
{
	auto reg = GetFirst();
	for (int i = 0; reg != nullptr && i < position; i++, reg = GetNext(reg));
	return reg;
}


C_h_nu_Info::C_h_nu_Info()
{
	Nelem = sizeof(Value_h_nu)/sizeof(double);

	strName_h_nu[0] = new char[strlen("Al") + 1];
	sprintf(strName_h_nu[0], "%s", "Al");
	strName_h_nu[1] = new char[strlen("Mg") + 1];
	sprintf(strName_h_nu[1], "%s", "Mg");
	strName_h_nu[2] = new char[strlen("He I") + 1];
	sprintf(strName_h_nu[2], "%s", "He I");
	strName_h_nu[3] = new char[strlen("He II") + 1];
	sprintf(strName_h_nu[3], "%s", "He II");

	Value_h_nu[0] = 1486.6;
	Value_h_nu[1] = 1253.6;
	Value_h_nu[2] = 21.2;
	Value_h_nu[3] = 40.8;
}

C_h_nu_Info::~C_h_nu_Info()
{
	for(int i=0; i<Nelem; ++i) 
		delete[] strName_h_nu[i];
}