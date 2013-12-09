#ifndef _INIVALUES_H_
#define _INIVALUES_H_

class ValueSvLd
{
	//friend class IniValues;
	char *m_Section;
	char *m_Key;
	char *m_IniFileName;
	char *DefStr;
	double DefDouble;
	int DefInt;

public:
	enum ValType{t_str=1, t_int, t_double} Type;
	CString *sVal; //Read/Write length=MAX_PATH
	double dVal;
	int iVal;
	ValueSvLd();
	void Describe(LPCTSTR DefaultVal, LPCTSTR Section, LPCTSTR Key, LPCTSTR IniFileName=NULL);
	void Describe(int DefaultVal,LPCTSTR Section, LPCTSTR Key, LPCTSTR IniFileName=NULL);
	void Describe(double DefaultVal,LPCTSTR Section, LPCTSTR Key, LPCTSTR IniFileName=NULL);
	~ValueSvLd();
	void Read();
	void Write();
	ValueSvLd& operator = (char *str) {if(Type==t_str) *sVal=str; return *this;};
	operator const char*() const {if(Type!=t_str)return NULL; else return (LPCSTR)*sVal;};
	operator CString() const {return *sVal;};
	const ValueSvLd& operator = (const int NewVal){iVal=NewVal; return *this;}; 
	operator int() const {return iVal;};
	const ValueSvLd& operator = (const double NewVal){dVal=NewVal; return *this;}; 
	operator double() const {return dVal;};

	//operator = (ValueSvLd *pV, int val) {iVal=val;};
	//operator = (int &val, ValueSvLd *pV) {val=iVal;};
	//operator = (ValueSvLd *pV, double val) {dVal=val;};
	//operator = (double &val, ValueSvLd *pV) {val=dVal;};


};
	//char * operator = (char *str, ValueSvLd& V) {if(V.Type==ValueSvLd::t_str) strcpy(str, (LPCSTR)*V.sVal); return str;};

class SvLdInt
{
	char *m_Section;
	char *m_Key;
	char *m_IniFileName;

public:
	int iVal;
	SvLdInt();
	void Describe(int DefaultVal,LPCTSTR Section, LPCTSTR Key, LPCTSTR IniFileName=NULL);
	~SvLdInt();
	const SvLdInt& operator = (const int NewVal){iVal=NewVal; return *this;}; 
	operator int() const {return iVal;};
};

class SvLdDouble
{
	char *m_Section;
	char *m_Key;
	char *m_IniFileName;

public:
	double dVal;
	SvLdDouble();
	void Describe(double DefaultVal,LPCTSTR Section, LPCTSTR Key, LPCTSTR IniFileName=NULL);
	~SvLdDouble();
	const SvLdDouble& operator = (const double NewVal){dVal=NewVal; return *this;}; 
	operator double() const {return dVal;};
};

class IniValues
{
public:
	char IniFileName[MAX_PATH];
	IniValues();

//Data members
	ValueSvLd ProjectFile;
	ValueSvLd ShowList;
	ValueSvLd ListWidth;
};

#endif