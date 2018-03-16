#pragma once 
#include "Time.h"

struct DATA_IN
{
int HV;
int KE_Start;
int KE_End;
int Step;
int Time;   // ����� ��������� �� �����
int N_;     // �������� ����� ������
int Curr_N; // ����� ���������� ������
BOOL Off;
enum class EnergyType
{
	KE,
	BE
};
EnergyType KE_BE;
char N_h_nu; // ����� ������ ����
int DeltaVolts;
char Comments[256];
int Priority; // ��������� �������. ������� �����������, ������� � ������������ ����������, �� ���������� N_ ������ � ������ ������� � ������ �����������
doubleTime LastEditTime; //����� ���������� ��������� ���������� �������
};

// ��� ��������� �������� ������� XPS ��� ����������� � �������
struct STR_PAR
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
char Priority[16];
} ;

//������� �������� �������� ������� XPS
typedef struct
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
int m_NDataOut; // ����� ����� ������� (����� ������� m_pDataOut) 
int m_NDataOutCurr; // ����� ����� � ������� �����
DATA_OUT* m_pDataOut;
DATA_IN  m_DataIn;
STR_PAR str;
int ID;
UINT m_ptrInFile;
enum {New, Edit} m_NewOrEdit;

////////////////////////KE, HV, and over data of region
CRegion* m_pNext;
CRegion* m_pPrev;

static CRegion* m_pFirst;
static CRegion* m_pEnd;

CRegion();
CRegion(int n);
~CRegion();
void CreateNewRegion();

/**
 * \brief �������� �������� � ��������� STR_PAR str �� ������ DATA_IN m_DataIn
 */
void UpdateStrValues();
static CRegion* GetFirst();
static CRegion* GetNext(CRegion* reg);
static CRegion* GetAtPosition(int position);
};