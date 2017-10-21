#ifndef _INIVALUES_H_
#define _INIVALUES_H_

#include <exception>
#include <windows.h>
#include <afxwin.h>


template<class T>
class ValueSvLdBase
{
protected:
	CString m_Section;
	CString m_Key;
	CString m_IniFileName;

	CString GetSettingString();
	void WriteSettingString(CString val);

	virtual T ConvertToType(CString strVal){throw std::exception("Метод для данного типа не реализован.");}
	virtual CString ConvertFromType(T val){throw std::exception("Метод для данного типа не реализован.");}
public:
	T m_DefVal;
	T Value;
	void Describe(T DefaultVal, LPCTSTR Section, LPCTSTR Key, LPCTSTR IniFileName);

	virtual ~ValueSvLdBase();
	void Read();
	void Write();
};

template<class T>
class ValueSvLd : public ValueSvLdBase<T>
{
	T ConvertToType(CString strVal){throw std::exception("Метод для данного типа не реализован.");}
	CString ConvertFromType(T val){throw std::exception("Метод для данного типа не реализован.");}
	~ValueSvLd(){Write();}
};

template<>
class ValueSvLd<bool> : public ValueSvLdBase<bool>
{
protected:
	bool ConvertToType(CString strVal);
	CString ConvertFromType(bool val);
public:
	~ValueSvLd(){Write();}
};

template<>
class ValueSvLd<int> : public ValueSvLdBase<int>
{
protected:
	int ConvertToType(CString strVal);
	CString ConvertFromType(int val);
public:
	~ValueSvLd(){Write();}
};

template<>
class ValueSvLd<double> : public ValueSvLdBase<double>
{
protected:
	double ConvertToType(CString strVal);
	CString ConvertFromType(double val);
public:
	~ValueSvLd(){Write();}
};

template<>
class ValueSvLd<CString> : public ValueSvLdBase<CString>
{
protected:
	CString ConvertToType(CString strVal){return strVal;}
	CString ConvertFromType(CString val){return val;}
public:
	~ValueSvLd(){Write();}
};

class IniValues
{
public:
	char IniFileName[MAX_PATH];
	IniValues();

//Data members
	ValueSvLd<CString> ProjectFile[4];
	ValueSvLd<CString> OriginFile;
	ValueSvLd<CString> EasyPlotFile;
	ValueSvLd<bool> HighPressureMode;
	ValueSvLd<double> RetardCalibration;
	ValueSvLd<bool> UseUsbCounter; // Использовать USB-счетчик
	ValueSvLd<CString> CounterComPort; // Имя Com-порта USB-счетчика
	ValueSvLd<int> CounterPortBaudRate; // Скорость виртуального COM-порта USB-счетчика
	ValueSvLd<int> CounterCountTime;	//Время работы счетчика, дискретность 10 мс (0... 65535)
	ValueSvLd<int> CounterDetectionThreshold; // Порог детектирования (0... 255)
	ValueSvLd<int> CounterInterlockThreshold; // Минимальная частота сигнализации охлаждения в Гц (0... 65535)
	ValueSvLd<int> CrateNumber;
	ValueSvLd<int> RegisterPosition;
	ValueSvLd<int> CounterPosition;
	ValueSvLd<int> TimerPosition;
	ValueSvLd<int> RegisterRetardPosition;
	ValueSvLd<int> RegisterHVPosition;
	ValueSvLd<bool> ViewSentRawData; // Выводить в лог отправленные в COM-порт данные
	ValueSvLd<bool> ViewReceivedRawData; // Выводить в лог полученные из COM-порта данные
	ValueSvLd<bool> CamacSimulation; // Режим симуляции КАМАКа
	ValueSvLd<bool> UsbCounterSimulation; // Режим симуляции счетчика USB
};
#endif