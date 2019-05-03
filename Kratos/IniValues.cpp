#include "stdafx.h"
#include "IniValues.h"

const char* InvalidIniValue = "InvalidIniValue_CF9EAA1E-6DED-4C1A-B667-CB8D1B004C49";
IniValues::IniValues()
{
char ModuleName[MAX_PATH];
::GetModuleFileName(NULL, ModuleName, MAX_PATH);
CString cstr=ModuleName, cstr2;
int EndPath=cstr.ReverseFind('\\');
cstr.Delete(EndPath+1,cstr.GetLength()-EndPath-1);
cstr2=cstr+"Kratos.ini";
strcpy(IniFileName,(char*)(LPCTSTR)cstr2);

ProjectFile[0].Describe("", "RECENT FILES", "ProjectFile[0]", IniFileName);
ProjectFile[1].Describe("", "RECENT FILES", "ProjectFile[1]", IniFileName);
ProjectFile[2].Describe("", "RECENT FILES", "ProjectFile[2]", IniFileName);
ProjectFile[3].Describe("", "RECENT FILES", "ProjectFile[3]", IniFileName);
OriginFile.Describe(cstr, "RECENT FILES", "OriginFile", IniFileName);
EasyPlotFile.Describe(cstr, "RECENT FILES", "EasyPlotFile", IniFileName);
HighPressureMode.Describe((int)FALSE, "OPTIONS", "HighPressureMode", IniFileName);
RetardCalibration.Describe((double)1.0, "HARDWARE SETUP", "RetardCalibration", IniFileName);
UseUsbCounter.Describe(true, "HARDWARE SETUP", "UseUsbCounter", IniFileName); // Использовать USB-счетчик
CounterComPort.Describe("COM5", "HARDWARE SETUP", "CounterComPort", IniFileName);// Имя Com-порта USB-счетчика
CounterPortBaudRate.Describe(38400, "HARDWARE SETUP", "CounterPortBaudRate", IniFileName);; // Скорость виртуального COM-порта USB-счетчика
CounterCountTime.Describe(100, "HARDWARE SETUP", "CounterCountTime", IniFileName);	//Время работы счетчика, дискретность 10 мс (0... 65535)
CounterDetectionThreshold.Describe(128, "HARDWARE SETUP", "CounterDetectionThreshold", IniFileName); // Порог детектирования (0... 255)
CounterInterlockThreshold.Describe(100, "HARDWARE SETUP", "CounterInterlockThreshold", IniFileName); // Минимальная частота сигнализации охлаждения в Гц (0... 65535)

CrateNumber.Describe(0, "CAMAC", "CrateNumber", IniFileName);
RegisterPosition.Describe(0, "CAMAC", "RegisterPosition", IniFileName);
CounterPosition.Describe(0, "CAMAC", "CounterPosition", IniFileName);
TimerPosition.Describe(0, "CAMAC", "TimerPosition", IniFileName);
RegisterRetardPosition.Describe(0, "CAMAC", "RegisterRetardPosition", IniFileName);
RegisterHVPosition.Describe(0, "CAMAC", "RegisterHVPosition", IniFileName);

ViewSentRawData.Describe((int)FALSE, "DEBUG", "ViewSentRawData", IniFileName); // Выводить в лог отправленные в COM-порт данные
ViewReceivedRawData.Describe((int)FALSE, "DEBUG", "ViewReceivedRawData", IniFileName); // Выводить в лог полученные из COM-порта данные
CamacSimulation.Describe((int)FALSE, "DEBUG", "CamacSimulation", IniFileName); // Режим симуляции КАМАКа
UsbCounterSimulation.Describe((int)FALSE, "DEBUG", "UsbCounterSimulation", IniFileName); // Режим симуляции счетчика USB

StatisticsMediumExtraDelay.Describe(0., "STATISTICS", "StatisticsMediumExtraDelay", IniFileName); //Среднестатистическая дополнительная задержка каждого измерения
DxpsLastScanTime.Describe(300, "DxpsDlg", "DxpsLastScanTime", IniFileName);; //Последнее время измерения в режиме DXPS
}

//--------------------------------------------------------------------------------------------------

template<class T> void ValueSvLdBase<T>::Describe(T DefaultVal, LPCTSTR Section, LPCTSTR Key, LPCTSTR IniFileName)
{
	m_DefVal = DefaultVal;
	m_Section = Section;
	m_Key = Key;
	m_IniFileName = IniFileName;
	Read();
}

template<class T>
CString ValueSvLdBase<T>::GetSettingString()
{
	char strOut[MAX_PATH];
	GetPrivateProfileString(m_Section, m_Key, InvalidIniValue, strOut, MAX_PATH, m_IniFileName);
	return CString(strOut);
}

template<class T>
void ValueSvLdBase<T>::WriteSettingString( CString val )
{
	WritePrivateProfileString(m_Section, m_Key, (LPCSTR)val, m_IniFileName);
}

template<class T>
void ValueSvLdBase<T>::Read()
{
	CString strOut = GetSettingString();
	if(strOut == InvalidIniValue)
	{
		Value = m_DefVal;
		Write();
	}
	else
		Value = ConvertToType(strOut);
}

template<class T>
void ValueSvLdBase<T>::Write()
{
	CString str = ConvertFromType(Value);
	if(GetSettingString() != str)
		WriteSettingString(str);
}

template<class T> ValueSvLdBase<T>::~ValueSvLdBase()
{
}

//////////////////////////////////////////////////////////////////////////
// Реализация специализаций
//////////////////////////////////////////////////////////////////////////

bool ValueSvLd<bool>::ConvertToType(CString strVal)
{
	return atoi(strVal.GetString())!= 0;
}

CString ValueSvLd<bool>::ConvertFromType(bool val)
{
	CString str;
	str.Format("%i", (int)val);
	return str;
}

int ValueSvLd<int>::ConvertToType(CString strVal)
{
	return atoi(strVal.GetString());
}

CString ValueSvLd<int>::ConvertFromType(int val)
{
	CString str;
	str.Format("%i", val);
	return str;
}

double ValueSvLd<double>::ConvertToType(CString strVal)
{
	return atof(strVal.GetString());
}

CString ValueSvLd<double>::ConvertFromType(double val)
{
	CString str;
	str.Format("%g", val);
	return str;
}
