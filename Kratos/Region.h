#ifndef _REGION_H_INCLUDED_
#define _REGION_H_INCLUDED_

typedef struct _DATA_IN
{
int HV;
int KE_Start;
int KE_End;
int Step;
int Time;   // Время измерения на точку
int N_;     // Заданное число сканов
int Curr_N; // Число пройденных сканов
BOOL Off;
enum{KE, BE} KE_BE;
char N_h_nu; // Здесь просто байт
int DeltaVolts;
char Comments[256];
} DATA_IN;

#define TYPE_BE 0
#define TYPE_DIV 1

struct DxpsRegPar
{
BOOL Off;
int Type; //0 if "BE Print", 1 if Division Divident/Divisor
int Divident;
int Divisor;
double BE;
double HV;
int Anode;//0:Al, 1:Mg, 2:He1, 3:He2
double Delay; //Задержка перед началом измерений
double Dwell;	//Время измерения
int ColorIndex;
char Comments[256];
//enum{Al, Mg, He1, He2};
};
//===============
typedef struct _STR_PAR
{
char HV[16];
char KE_Start[16];
char KE_End[16];
char Step[16];
char Time[16];
char N_[16];
char KE_BE[16];
char Name_h_nu[16];
char Curr_N[16];
char* Comments;
} STR_PAR;

//=============
typedef struct _DATA_OUT
{
int x;
float y;
} DATA_OUT;
//==============

class C_h_nu_Info
{
public:
int Nelem;
double Value_h_nu[4];
char* strName_h_nu[4];
C_h_nu_Info();
~C_h_nu_Info();
};

//==============

class CRegion
{
public:
static int m_NReg; //-- Number of regions
C_h_nu_Info h_nu_Info;
int m_NDataOut; // Число точек графика (длина массива m_pDataOut) 
int m_NDataOutCurr; // Число точек в текущем скане
DATA_OUT* m_pDataOut;
DATA_IN  m_DataIn;
STR_PAR str;
int ID;
UINT m_ptrInFile;
enum {New, Edit} m_NewOreEdit;

////////////////////////KE, HV, and over data of region
CRegion* m_pNext;
CRegion* m_pPrev;

static CRegion* m_pFirst;
static CRegion* m_pEnd;

CRegion();
CRegion(int n);
~CRegion();
void CreateNewRegion();
static CRegion* GetFirst();
static CRegion* GetNext(CRegion* reg);
};

#endif //_REGION_H_INCLUDED_