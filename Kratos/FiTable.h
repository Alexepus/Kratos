// FiTable.h: interface for the CFiTable class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FITABLE_H__15DF0A89_DC54_11D5_932F_FE43B12C266D__INCLUDED_)
#define AFX_FITABLE_H__15DF0A89_DC54_11D5_932F_FE43B12C266D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

struct HV_Fi
{
	int HV;
	float Fi;
};

class CFiTable  
{
public:
	int NValues;
	CString FileName;
public:
	CFiTable();
	BOOL ReadFromFile();
	BOOL WriteToFile();
	int GetNValues() {return NValues;};
	int GetHV(int index){if(index>=0&&index<NValues) return HVFi[index].HV; else return 0;};
	float GetFi(int index){if(index>=0&&index<NValues) return HVFi[index].Fi; else return 0.;};
	void Replace(int index, double HV, double Fi);
	void Append (double HV, double Fi);
	void SortByHV();
	double GetInterpolatedFi(double HV);
	BOOL HVExist(int HV);
	float GetFiByHV(int HV);
	void KillSeriatedHV();
//	virtual ~CFiTable();

private:

	HV_Fi HVFi[256];
};

#endif // !defined(AFX_FITABLE_H__15DF0A89_DC54_11D5_932F_FE43B12C266D__INCLUDED_)
