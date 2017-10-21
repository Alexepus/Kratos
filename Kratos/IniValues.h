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

	virtual T ConvertToType(CString strVal){throw std::exception("����� ��� ������� ���� �� ����������.");}
	virtual CString ConvertFromType(T val){throw std::exception("����� ��� ������� ���� �� ����������.");}
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
	T ConvertToType(CString strVal){throw std::exception("����� ��� ������� ���� �� ����������.");}
	CString ConvertFromType(T val){throw std::exception("����� ��� ������� ���� �� ����������.");}
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
	ValueSvLd<bool> UseUsbCounter; // ������������ USB-�������
	ValueSvLd<CString> CounterComPort; // ��� Com-����� USB-��������
	ValueSvLd<int> CounterPortBaudRate; // �������� ������������ COM-����� USB-��������
	ValueSvLd<int> CounterCountTime;	//����� ������ ��������, ������������ 10 �� (0... 65535)
	ValueSvLd<int> CounterDetectionThreshold; // ����� �������������� (0... 255)
	ValueSvLd<int> CounterInterlockThreshold; // ����������� ������� ������������ ���������� � �� (0... 65535)
	ValueSvLd<int> CrateNumber;
	ValueSvLd<int> RegisterPosition;
	ValueSvLd<int> CounterPosition;
	ValueSvLd<int> TimerPosition;
	ValueSvLd<int> RegisterRetardPosition;
	ValueSvLd<int> RegisterHVPosition;
	ValueSvLd<bool> ViewSentRawData; // �������� � ��� ������������ � COM-���� ������
	ValueSvLd<bool> ViewReceivedRawData; // �������� � ��� ���������� �� COM-����� ������
	ValueSvLd<bool> CamacSimulation; // ����� ��������� ������
	ValueSvLd<bool> UsbCounterSimulation; // ����� ��������� �������� USB
};
#endif