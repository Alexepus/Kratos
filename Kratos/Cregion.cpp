#include "stdafx.h"
#include "CRegion.h"
#include "CppLinq.h"

int D2I(double d) { return (int)(d*1000.0); }
double I2D(int i) { return ((double)i) / 1000.0; }

int CRegion::m_NReg = 0;
CRegion* CRegion::m_pFirst = nullptr;
CRegion* CRegion::m_pEnd = nullptr;
C_h_nu_Info CRegion::h_nu_Info;

bool operator==(const DATA_IN& l, const DATA_IN& r)
{
	return l.HV == r.HV
		&& l.KE_Start == r.KE_Start
		&& l.KE_End == r.KE_End
		&& l.Step == r.Step
		&& l.Time == r.Time
		&& l.N_ == r.N_
		&& l.Curr_N == r.Curr_N
		&& l.Off == r.Off
		&& l.KE_BE == r.KE_BE
		&& l.N_h_nu == r.N_h_nu
		&& l.Comments == r.Comments
		&& l.Priority == r.Priority
		&& l.LastEditTime == r.LastEditTime
		&& l.PassagesWhenSaveCopy == r.PassagesWhenSaveCopy;
}

bool operator!=(const DATA_IN& l, const DATA_IN& r)
{
	return !operator==(l, r);
}

CRegion::CRegion()
{
	m_NDataOut = 0;
	InitNewRegion();
}

CRegion::CRegion(int dataOutCount) : m_NDataOut(dataOutCount)
{
	InitNewRegion();
	int bytesCount = m_NDataOut*sizeof(DATA_OUT);
	m_pDataOut = (DATA_OUT*) malloc(bytesCount);
	if(m_pDataOut) memset(m_pDataOut, 0, bytesCount);
}

CRegion::CRegion(CRegion* regionToCopy) : CRegion(regionToCopy->m_NDataOut)
{
	m_DataIn = regionToCopy->m_DataIn;
	m_BeginTime = regionToCopy->m_BeginTime;
	m_EndTime = regionToCopy->m_EndTime;
	m_NDataOutCurr = regionToCopy->m_NDataOutCurr;
	m_NewOrEdit = regionToCopy->m_NewOrEdit;
	memcpy_s(m_pDataOut, m_NDataOut * sizeof(DATA_OUT), regionToCopy->m_pDataOut, regionToCopy->m_NDataOut * sizeof(DATA_OUT));
	UpdateStrValues();
}

void CRegion::InitNewRegion()
{
	ID=CRegion::m_NReg;
	++m_NReg;
	m_DataIn.N_ = 1;
	m_DataIn.Curr_N = 0;
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
}

/**
* \brief ќбновить значени€ в структуре STR_PAR str из данных DATA_IN m_DataIn
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
	sprintf(str.Comments, "%s", m_DataIn.Comments.GetString());
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

void CRegion::DeleteAllRegions()
{
	CRegion* reg = GetFirst();
	while(reg)
	{
		delete reg;
		reg = GetFirst();
	}
}

void CRegion::ResetMeasuredData()
{
	for (int i = 0; i<m_NDataOut; ++i)
		m_pDataOut[i].y = 0;
	m_NDataOutCurr = 0;
	m_DataIn.Curr_N = 0;
	m_BeginTime = 0;
	m_EndTime = 0;
	UpdateStrValues();
}

CRegion* CRegion::GetFirst()
{
	if(m_pFirst) 
		return m_pFirst;
	return nullptr;
}

CRegion* CRegion::GetNext(CRegion* reg)
{
	if(!m_pFirst || !reg) 
		return nullptr;
	return reg->m_pNext;
}

std::vector<CRegion*> CRegion::GetAsVector()
{
	std::vector<CRegion*> res;
	auto region = GetFirst();
	while(region != nullptr)
	{
		res.push_back(region);
		region = GetNext(region);
	}
	return res;
}

/**
* \brief Ќаходит следующий регион: среди включенных, среди незавершенных, с минимальным приоритетом,
*  следующий (закольцовано) за регионом с индексом prevIndex. ≈сли skipRegion задан, то его должен пропустить
*/
CRegion* CRegion::GetNextByPriority(int prevIndex, CRegion* skipRegion)
{
	auto regions = GetAsVector();
	auto regWithLeastPriority = Linq::from(regions)
		.where([skipRegion](CRegion *r) {return !r->m_DataIn.Off && r->m_DataIn.Curr_N < r->m_DataIn.N_ && r != skipRegion; })
		.orderBy([](CRegion *r) {return r->m_DataIn.Priority; })
		.firstOrDefault();
	if (!regWithLeastPriority)
		return nullptr;
	auto selectedPriority = regWithLeastPriority->m_DataIn.Priority;
	auto regsWithSelectedPriority = Linq::from(regions)
		.where([skipRegion, selectedPriority](CRegion *r)
			{
				return !r->m_DataIn.Off
					&& r->m_DataIn.Curr_N < r->m_DataIn.N_
					&& r != skipRegion && r->m_DataIn.Priority == selectedPriority;
		})
		.orderBy([](CRegion *r) {return r->ID; })
		.toVector();
	auto next = Linq::from(regsWithSelectedPriority)
		.where([prevIndex](CRegion *r) {return r->ID > prevIndex; })
		.firstOrDefault();
	if (!next)
		next = regsWithSelectedPriority[0];
	return next;
}

CRegion* CRegion::GetAtPosition(int position)
{
	auto reg = GetFirst();
	for (int i = 0; reg != nullptr && i < position; i++, reg = GetNext(reg));
	return reg;
}

bool CRegion::CanMoveAnyRegion(std::vector<CRegion*> regs, Directions dir)
{
	regs = dir == Directions::UpToBegin 
		? Linq::from(regs).orderBy([](CRegion* r) {return r->ID; }).toVector()
		: Linq::from(regs).orderByDesc([](CRegion* r) {return r->ID; }).toVector();
	int borderIndex = dir == Directions::UpToBegin? -1 : m_NReg;
	bool canMoveAny = false;
	for (CRegion* r : regs)
	{
		auto supposedNewIndex = r->ID + (dir == Directions::UpToBegin ? -1 : 1);
		if ((dir == Directions::UpToBegin && supposedNewIndex > borderIndex) || (dir == Directions::DownToEnd && supposedNewIndex < borderIndex))
		{
			borderIndex = supposedNewIndex;
			canMoveAny = true;
		}
		else
			borderIndex = r->ID;
	}
	return canMoveAny;
}

void CRegion::MoveRegionsIfPossible(std::vector<CRegion*> regs, Directions dir)
{
	regs = dir == Directions::UpToBegin
		? Linq::from(regs).orderBy([](CRegion* r) {return r->ID; }).toVector()
		: Linq::from(regs).orderByDesc([](CRegion* r) {return r->ID; }).toVector();
	int borderIndex = dir == Directions::UpToBegin ? -1 : m_NReg;
	for (CRegion* r : regs)
	{
		auto supposedNewIndex = r->ID + (dir == Directions::UpToBegin ? -1 : 1);
		if ((dir == Directions::UpToBegin && supposedNewIndex > borderIndex) || (dir == Directions::DownToEnd && supposedNewIndex < borderIndex))
		{
			Swap(r, GetAtPosition(supposedNewIndex));
			borderIndex = supposedNewIndex;			
		}
		else
			borderIndex = r->ID;
	}
}

void CRegion::Swap(CRegion* i, CRegion* j)
{
	if (i == j)
		return;
	if (j->m_pNext == i)
		std::swap(i, j);
	
	CRegion* swapperVector[4];
	swapperVector[0] = i->m_pPrev;
	swapperVector[1] = j->m_pPrev;
	swapperVector[2] = i->m_pNext;
	swapperVector[3] = j->m_pNext;

	if (i->m_pNext == j)
	{
		i->m_pPrev = swapperVector[2];
		j->m_pPrev = swapperVector[0];
		i->m_pNext = swapperVector[3];
		j->m_pNext = swapperVector[1];
	}
	 else 
	 {
		 i->m_pPrev = swapperVector[1];
		 j->m_pPrev = swapperVector[0];
		 i->m_pNext = swapperVector[3];
		 j->m_pNext = swapperVector[2];
	 }

	if (i->m_pPrev)
		i->m_pPrev->m_pNext = i;
	if (j->m_pPrev)
		j->m_pPrev->m_pNext = j;
	if (i->m_pNext)
		i->m_pNext->m_pPrev = i;
	if (j->m_pNext)
		j->m_pNext->m_pPrev = j;

	if (i->m_pPrev == nullptr)
		m_pFirst = i;
	if (j->m_pPrev == nullptr)
		m_pFirst = j;
	if (i->m_pNext == nullptr)
		m_pEnd = i;
	if (j->m_pNext == nullptr)
		m_pEnd = j;

	std::swap(i->ID, j->ID);
	i->UpdateStrValues();
	j->UpdateStrValues();
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