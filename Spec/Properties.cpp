/**
 * CProperties.cpp
 *
 * This class is used to make it easy to store application settings in an INI file.
 * The ini file is automatically called the same as the application name with .ini
 * as extention. 
 * 
 * Use:
 *  To use this class you have to add the next lines to stdafx.h:
 *      #include "Properties.h"
 *      extern CProperties prop;
 *
 *  Then add the next line to the main application CPP file (myapp.cpp for example) under 
 *  the line "CMyApp theApp;"
 *      CProperties prop;
 * 
 *  The properties class reads the ini file when it's object is created. When no ini file
 *  yet exists, it creates it. When the object is destroyed, the destructor makes sure that
 *  the data is stored in the ini file.
 *
 *  Adding data to the ini file is easy. Just add a public member of the type CString, int or
 *  double to the header of CProperties and then add a line like the next 
 *  in the constructor of CProperties.
 *  m_proplist.push_back( t_props("Connection",     "ServerIp",      "127.0.0.1",    &m_serverIp)        );
 *                                    ^                  ^                  ^                 ^
 *                        key name ___|     value name __|    default val __|   data member __|
 *
 * To compile the properties class, add properties.(cpp/h) and inifile.(cpp/h) files to your project.
 * 
 * To access a property from your source code you can type: prop.m_serverIp for example.
 *
 * This class is tested with the Visual C++ 6.0 compiler.
 * 
 * Dependencies: CIniFile class
 *
 * Author:  Dennis Kuppens (www.KuppensAutomation.nl)
 * Date:    27 Nov 2006
 * 
 * Changelog:
 *          1.0 | 27 Nov 2006 | Initial version.
 *
 *
 * Credits:
 *          Thanks to Shane Hill for his IniFile class.
 *
 */

#include "stdafx.h"
#include "Properties.h"
#include "IniFile.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CProperties::CProperties()
{
	m_proplist.push_back( t_props("Measuring Options", "CalibMassStart",0., &CalibMassStart));
	m_proplist.push_back( t_props("Measuring Options", "CalibMassStop",50.,&CalibMassStop));
	m_proplist.push_back( t_props("Measuring Options", "CalibMassStep",0.25,&CalibMassStep));
	m_proplist.push_back( t_props("Measuring Options", "MassSetupDelay",0.15,&MassSetupDelay));
	m_proplist.push_back( t_props("Measuring Options", "CalibMassSetupDelay",0.010,&CalibMassSetupDelay));
	m_proplist.push_back( t_props("CAMAC", "AdcIntegration",3,&AdcIntegration));
	m_proplist.push_back( t_props("CAMAC", "AdcRange",0,&AdcRange));
	m_proplist.push_back( t_props("Recent Files", "EasyPlotDir", "",    &EasyPlotDir));
	m_proplist.push_back( t_props("ThermoRegulator", "ThermoCoupleFile", "", &ThermoCoupleFile));
	m_proplist.push_back( t_props("ThermoRegulator", "TRFunctionFile", "", &TRFunctionFile));
	m_proplist.push_back( t_props("ThermoRegulator", "StartDelay", 0., &TRFunc.StartDelay));
	m_proplist.push_back( t_props("ThermoRegulator", "PrimeTemp", 70., &TRFunc.PrimeTemp));
	m_proplist.push_back( t_props("ThermoRegulator", "BetaPlus", 3., &TRFunc.BetaPlus));
	m_proplist.push_back( t_props("ThermoRegulator", "Temp1", 150., &TRFunc.Temp1));
	m_proplist.push_back( t_props("ThermoRegulator", "Time1", 10., &TRFunc.Time1));
	m_proplist.push_back( t_props("ThermoRegulator", "BetaMinus", 3., &TRFunc.BetaMinus));
	m_proplist.push_back( t_props("ThermoRegulator", "Temp2", 10., &TRFunc.Temp2));
	m_proplist.push_back( t_props("ThermoRegulator", "Time2", 10., &TRFunc.Time2));
	m_proplist.push_back( t_props("ThermoRegulator", "HeatingRate", 3., &TRFunc.HeatingRate));
	m_proplist.push_back( t_props("ThermoRegulator", "CoolingRate", 3., &TRFunc.CoolingRate));
	m_proplist.push_back( t_props("ThermoRegulator", "Repeat", 0, &TRFunc.Repeat));
	m_proplist.push_back( t_props("ThermoRegulator", "ColdEndsTemp", 20, &ColdEndsTemp));

	m_proplist.push_back( t_props("Interface", "MainToolBarRect.left", 0, (int*)&MainToolBarRect.left));
	m_proplist.push_back( t_props("Interface", "MainToolBarRect.top", 0, (int*)&MainToolBarRect.top));
	m_proplist.push_back( t_props("Interface", "MainToolBarRect.right", 0, (int*)&MainToolBarRect.right));
	m_proplist.push_back( t_props("Interface", "MainToolBarRect.bottom", 0, (int*)&MainToolBarRect.bottom));

	m_proplist.push_back( t_props("Interface", "TRToolBarRect.left", 0, (int*)&TRToolBarRect.left));
	m_proplist.push_back( t_props("Interface", "TRToolBarRect.top", 0, (int*)&TRToolBarRect.top));
	m_proplist.push_back( t_props("Interface", "TRToolBarRect.right", 0, (int*)&TRToolBarRect.right));
	m_proplist.push_back( t_props("Interface", "TRToolBarRect.bottom", 0, (int*)&TRToolBarRect.bottom));

	m_proplist.push_back( t_props("SerialLink", "ComPortName", "COM1", &ComPortName));
	m_proplist.push_back( t_props("SerialLink", "BaudRate", 57600, &BaudRate));

	m_proplist.push_back( t_props("TReg", "PidKp", 1., &PidKp));
	m_proplist.push_back( t_props("TReg", "PidKi", 1., &PidKi));
	m_proplist.push_back( t_props("TReg", "PidKd", 1., &PidKd));
	m_proplist.push_back( t_props("TReg", "TRegKff", 1., &TRegKff));
	m_proplist.push_back( t_props("TReg", "PidDifShift", 1, &PidDifShift));
	m_proplist.push_back( t_props("TReg", "PidViDecay", 1, &PidViDecay));
	m_proplist.push_back( t_props("TReg", "PidViMinMax", 1, &PidViMinMax));
	m_proplist.push_back( t_props("TReg", "PidZeroVi", 1, &PidZeroVi));

	m_proplist.push_back( t_props("Protection", "ProtectionMaxI", 10, &ProtectionMaxI));
	m_proplist.push_back( t_props("Protection", "ProtectionMaxU", 10, &ProtectionMaxU));
	m_proplist.push_back( t_props("Protection", "ProtectionMaxP", 100, &ProtectionMaxP));
	m_proplist.push_back( t_props("Protection", "LimitationMaxI", 9, &LimitationMaxI));

	m_proplist.push_back( t_props("TReg", "FeedForwTemp0", 1, &FeedForwTemp[0]));
	m_proplist.push_back( t_props("TReg", "FeedForwTemp1", 1, &FeedForwTemp[1]));
	m_proplist.push_back( t_props("TReg", "FeedForwTemp2", 1, &FeedForwTemp[2]));
	m_proplist.push_back( t_props("TReg", "FeedForwTemp3", 1, &FeedForwTemp[3]));
	m_proplist.push_back( t_props("TReg", "FeedForwOut0", 1, &FeedForwOut[0]));
	m_proplist.push_back( t_props("TReg", "FeedForwOut1", 1, &FeedForwOut[1]));
	m_proplist.push_back( t_props("TReg", "FeedForwOut2", 1, &FeedForwOut[2]));
	m_proplist.push_back( t_props("TReg", "FeedForwOut3", 1, &FeedForwOut[3]));

	m_proplist.push_back( t_props("Appearance", "ThermoHistKeepTime", 30*60, &ThermoHistKeepTime));
	m_proplist.push_back( t_props("Appearance", "ThermoHistShowPower", false, &ThermoHistShowPower));

	m_proplist.push_back( t_props("ThermoRegulator", "SyncStartDxps", false, &SyncStartDxps));
	m_proplist.push_back( t_props("ThermoRegulator", "SyncStartMsp", false, &SyncStartMsp));
	CSeriesSetup DefSeries;
	DefSeries.PlotType=CSeriesSetup::eLine;
	DefSeries.Color=RGB(0,255,0);
	DefSeries.Size=1;
	CalibSeriesNew=DefSeries;
	m_proplist.push_back( t_props("Appearance", "CalibSeriesNew", &CalibSeriesNew, sizeof(CalibSeriesNew)));
	DefSeries.PlotType=CSeriesSetup::eTrapezium;
	DefSeries.Color=RGB(255,0,0);
	DefSeries.Size=1;
	CalibSeriesLast=DefSeries;
	m_proplist.push_back( t_props("Appearance", "CalibSeriesLast", &CalibSeriesLast, sizeof(CalibSeriesLast)));
	DefSeries.PlotType=CSeriesSetup::eLineDots;
	DefSeries.Size=1;
	DefSeries.Size2=3;
	CalibSeriesLast=DefSeries;
	m_proplist.push_back( t_props("Appearance", "MspSeriesAll", &MspSeriesAll, sizeof(MspSeriesAll)));
	m_proplist.push_back( t_props("Appearance", "ThermoSeriesTRef", &ThermoSeriesTRef, sizeof(ThermoSeriesTRef)));
	m_proplist.push_back( t_props("Appearance", "ThermoSeriesTMeas", &ThermoSeriesTMeas, sizeof(ThermoSeriesTMeas)));
	m_proplist.push_back( t_props("Appearance", "ThermoSeriesPower", &ThermoSeriesPower, sizeof(ThermoSeriesPower)));

	m_proplist.push_back( t_props("Communication", "SerialLinkOutServer", ".", &SerialLinkOutServer));
	if (!Load())
    {
        Create();
        Load();
    }
}  

CProperties::~CProperties()
{
    Save();
}

bool CProperties::Load()
{
    CIniFile ini;
    ini.SetPath((char*)(LPCTSTR)GetIniFilePath());

    if (ini.ReadFile())
    {
        std::vector<t_props>::iterator it;
        for(it = m_proplist.begin(); it!= m_proplist.end(); it++)
        {
            switch(it->type)
            {
            case T_STRING:
                *((CString*)it->value) = ini.GetValue((LPCTSTR)it->key, (LPCTSTR)it->valname, (LPCTSTR)it->defStr).c_str();
                break;
            case T_INT:
                *((int*)it->value) = ini.GetValueI((LPCTSTR)it->key, (LPCTSTR)it->valname, it->defInt);
                break;
            case T_FLOAT:
                *((double*)it->value) = ini.GetValueF((LPCTSTR)it->key, (LPCTSTR)it->valname, it->defFloat);
                break;
            case T_BOOL:
                *((bool*)it->value) = ini.GetValueB((LPCTSTR)it->key, (LPCTSTR)it->valname, it->defBool);
                break;
            case T_BINARY:
				{
					std::string str = ini.GetValue((LPCTSTR)it->key, (LPCTSTR)it->valname, "");
					for(DWORD i=0; i < str.size()/2 && i < it->binarySize; i++)
						*(((BYTE*)it->value)+i)=(BYTE)atoi(str.substr(2*i,2).c_str());
					break;
				}
            }
        }
    }
    else
    {
        return false;
    }
    return true;
}

bool CProperties::Create()
{
    CIniFile ini;
    ini.SetPath((char*)(LPCTSTR)GetIniFilePath());
    std::vector<t_props>::iterator it;
    for(it = m_proplist.begin(); it!= m_proplist.end(); it++)
    {
        switch(it->type)
        {
        case T_STRING:
            ini.SetValue((LPCTSTR)it->key, (LPCTSTR)it->valname, (LPCTSTR)it->defStr);
            break;
        case T_INT:
            ini.SetValueI((LPCTSTR)it->key, (LPCTSTR)it->valname, it->defInt);
            break;
        case T_FLOAT:
            ini.SetValueF((LPCTSTR)it->key, (LPCTSTR)it->valname, it->defFloat);
            break;
        case T_BOOL:
            ini.SetValueB((LPCTSTR)it->key, (LPCTSTR)it->valname, it->defBool);
            break;
        case T_BINARY:
			{
				CString str, str1;
				for(int i=0; i<it->binarySize; i++)
				{
					str1.Format("%.2X", *((BYTE*)it->value)+i);
					str+=str1;
				}
				ini.SetValue((LPCTSTR)it->key, (LPCTSTR)it->valname, (LPCTSTR)str);
				break;
			}
        }
    }
    return ini.WriteFile();
}


bool CProperties::Save()
{
    bool retval=false;
    CIniFile ini;
    ini.SetPath((char*)(LPCTSTR)GetIniFilePath());

    if (ini.ReadFile())
    {
        std::vector<t_props>::iterator it;
        for(it = m_proplist.begin(); it!= m_proplist.end(); it++)
        {
            switch(it->type)
            {
            case T_STRING:
                ini.SetValue((LPCTSTR)it->key, (LPCTSTR)it->valname, (LPCTSTR)*(CString*)(it->value));
                break;
            case T_INT:
                ini.SetValueI((LPCTSTR)it->key, (LPCTSTR)it->valname, *(int*)(it->value));
                break;
            case T_FLOAT:
                ini.SetValueF((LPCTSTR)it->key, (LPCTSTR)it->valname, *(double*)(it->value));
                break;
            case T_BOOL:
                ini.SetValueB((LPCTSTR)it->key, (LPCTSTR)it->valname, *(bool*)(it->value));
                break;
            case T_BINARY:
				{
					CString str, str1;
					for(int i=0; i<it->binarySize; i++)
					{
						str1.Format("%.2X", *(((BYTE*)it->value)+i));
						str+=str1;
					}
					ini.SetValue((LPCTSTR)it->key, (LPCTSTR)it->valname, (LPCTSTR)str);
					break;
				}
            }
        }

        retval = ini.WriteFile();
    }
    
    return retval;
}

/**
 * Returns the program directory
 * The result does NOT contains the trailing '\' slash.
 */
CString CProperties::GetProgramDir()
{
    CString programPath;
	TCHAR szEXEPathname[_MAX_PATH];
	GetModuleFileName(NULL, szEXEPathname, _MAX_PATH);
	CSplitPath sp;
	sp.Split(szEXEPathname);

    programPath=sp.GetDrive();
    programPath+=sp.GetDirectory();

    programPath.TrimRight();

    if(programPath[programPath.GetLength()-1]=='\\')
    {
        programPath.Delete(programPath.GetLength()-1);
    }

    return programPath;
}

/**
 * Returns the full path to the ini file for this application.
 */
CString CProperties::GetIniFilePath()
{
	TCHAR szEXEPathname[_MAX_PATH];
	GetModuleFileName(NULL, szEXEPathname, _MAX_PATH);
	CSplitPath sp;
	sp.Split(szEXEPathname);

	CString iniFileName = GetProgramDir();
    iniFileName+="\\";
	iniFileName+=sp.GetFileName();
    iniFileName+=".ini";

    return iniFileName;
}


CSplitPath::CSplitPath( LPCTSTR lpszPath )
{
    // Initialize our objects
    memset( path_buffer, 0, sizeof( path_buffer ) );
    memset( drive, 0, sizeof( drive ) );
    memset( dir, 0, sizeof( dir ) );
    memset( fname, 0, sizeof( fname ) );
    memset( ext, 0, sizeof( ext ) );

    // If we were given a path, split it
    if ( lpszPath ) {
        Split( lpszPath );
    }
}

BOOL CSplitPath::Split( LPCTSTR lpszPath )
{
    // If we weren't given a path, fail
    if ( lpszPath == NULL ) {
        // Return failure
        return FALSE;
    }

    // Copy the path
    _tcsncpy( path_buffer, lpszPath, sizeof( path_buffer ) - 1 );
    // Split the given path
    _tsplitpath( path_buffer, drive, dir, fname, ext );

    return TRUE;
}
