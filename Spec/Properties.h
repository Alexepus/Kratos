// Properties.h: interface for the CProperties class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PROPERTIES_H__FC2CE3B6_ADD5_45B9_B814_5630DDECE3D3__INCLUDED_)
#define AFX_PROPERTIES_H__FC2CE3B6_ADD5_45B9_B814_5630DDECE3D3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning(disable: 4786)
#pragma warning(disable: 4710)

#include <vector>

// forward declaration
class t_props;

struct CThermoFunc
{
	double StartDelay;
	double PrimeTemp;
	double BetaPlus;
	double Temp1;
	double Time1;
	double BetaMinus;
	double Temp2;
	double Time2;
	double HeatingRate;
	double CoolingRate;
	BOOL Repeat;

};
struct CSeriesSetup
{
	enum EPlotType{eLineDots, eLine, eDots, eTrapezium};
	EPlotType PlotType;
	int Size;
	int Color;
	int Size2;
	int Color2;
};

class CProperties  
{
public:
    CString GetIniFilePath();
    CString GetProgramDir();
    bool Load();
	bool Save();
    bool Create();
	CProperties();
	virtual ~CProperties();

	double CalibMassStart;
	double CalibMassStop;
	double CalibMassStep;
	double MassSetupDelay;
	double CalibMassSetupDelay;
	int AdcIntegration;
	int AdcRange;
	CString EasyPlotDir;
	CString ThermoCoupleFile;
	CString TRFunctionFile;
	CThermoFunc TRFunc;
	double ColdEndsTemp;
	RECT MainToolBarRect;
	RECT TRToolBarRect;

	CString ComPortName;
	int BaudRate;
	double PidKp, PidKi, PidKd, TRegKff;
	int PidDifShift;
	double PidViMinMax, PidZeroVi, PidViDecay;
	double FeedForwTemp[4], FeedForwOut[4];
	int ThermoHistKeepTime;
	bool ThermoHistShowPower;
	bool SyncStartDxps;
	bool SyncStartMsp;
	double ProtectionMaxI, ProtectionMaxU, ProtectionMaxP, LimitationMaxI;
	CSeriesSetup CalibSeriesNew;
	CSeriesSetup CalibSeriesLast;
	CSeriesSetup MspSeriesAll;
	CSeriesSetup ThermoSeriesTRef;
	CSeriesSetup ThermoSeriesTMeas;
	CSeriesSetup ThermoSeriesPower;
	CString SerialLinkOutServer;


private:

    std::vector<t_props> m_proplist;
};

class CSplitPath
{ 
// Construction
public: 
    CSplitPath( LPCTSTR lpszPath = NULL );

// Operations
    BOOL    Split(LPCTSTR lpszPath );
    CString GetPath( void ) { return path_buffer; }
    CString GetDrive( void ) { return drive; }
    CString GetDirectory( void ) { return dir; }
    CString GetFileName( void ) { return fname; }
    CString GetExtension( void ) { return ext; }

// Attributes
protected:
    TCHAR path_buffer[_MAX_PATH];
    TCHAR drive[_MAX_DRIVE];
    TCHAR dir[_MAX_DIR];
    TCHAR fname[_MAX_FNAME];
    TCHAR ext[_MAX_EXT];
}; 

enum type{T_STRING, T_INT, T_FLOAT, T_BOOL, T_BINARY};
class t_props
{
public:
    t_props(CString key, CString valname, CString def, CString * str)
    {
        this->key = key;
        this->valname = valname;
        this->defStr = def;
        type = T_STRING;
        value = (void*)str;
    }

    t_props(CString key, CString valname, int def, int * val)
    {
        this->key = key;
        this->valname = valname;
        this->defInt = def;
        type = T_INT;
        value = (void*)val;
    }

    t_props(CString key, CString valname, double def, double * val)
    {
        this->key = key;
        this->valname = valname;
        this->defFloat = def;
        type = T_FLOAT;
        value = (void*)val;
    }

    t_props(CString key, CString valname, bool def, bool * val)
    {
        this->key = key;
        this->valname = valname;
        this->defBool = def;
        type = T_BOOL;
        value = (void*)val;
    }

    t_props(CString key, CString valname, void * val, int size)
    {
        this->key = key;
        this->valname = valname;
        type = T_BINARY;
		binarySize = size;
        value = (void*)val;
    }

    CString key;
    CString valname;
    int     type;
    CString defStr;
    int     defInt;
    double  defFloat;
    bool	defBool;
	int		binarySize;
    void *  value;
};


#endif // !defined(AFX_PROPERTIES_H__FC2CE3B6_ADD5_45B9_B814_5630DDECE3D3__INCLUDED_)
