// MassCalibDoc.h: interface for the CMassCalibDoc class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MASSCALIBDOC_H__5914CE4F_2FFD_42E5_88FB_D1D529CF3CF2__INCLUDED_)
#define AFX_MASSCALIBDOC_H__5914CE4F_2FFD_42E5_88FB_D1D529CF3CF2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector>
#include "MassSpecDoc.h"

class CMassCalibDoc  
{
public:
	double a,b,c;		//y=a+b*x+c*x^2
	void SaveTable();
	void ReadTable();
	double VoltageByMass(double Mass){return a+b*Mass+c*Mass*Mass;}
	//double FindMass
	CMassCalibDoc();
	virtual ~CMassCalibDoc();
	std::vector<CMassTable> m_MassTable;

};

#endif // !defined(AFX_MASSCALIBDOC_H__5914CE4F_2FFD_42E5_88FB_D1D529CF3CF2__INCLUDED_)
