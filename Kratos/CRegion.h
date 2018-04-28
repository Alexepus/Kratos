#pragma once 
#include "StdAfx.h"

int D2I(double d);
double I2D(int i);

struct DATA_IN
{
int HV;
int KE_Start;
int KE_End;
int Step;
int Time;   // ����� ��������� �� ����� � ��
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
int Priority = 10; // ��������� �������. ������� �����������, ������� � ������������ ����������, �� ���������� N_ ������ � ������ ������� � ������ �����������
time_t LastEditTime = 0; //����� ���������� ��������� ���������� �������
};

struct DATA_IN_V1
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
	DATA_IN ToDataIn() const
	{
		DATA_IN din {HV, KE_Start, KE_End, Step, Time, N_, Curr_N, Off, (DATA_IN::EnergyType)(int)KE_BE, N_h_nu };
		std::copy(Comments, std::end(Comments), din.Comments);
		return din;
	}
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
enum class Directions
{
	/**
	 * \brief � ������� ���������� �������
	 */
	UpToBegin,
	/**
	* \brief � ������� ���������� �������
	*/
	DownToEnd
};

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
time_t m_BeginTime; // ����� ������ ������������ �������
time_t m_EndTime; // ����� ��������� ������������ �������

////////////////////////KE, HV, and over data of region
CRegion* m_pNext;
CRegion* m_pPrev;

static CRegion* m_pFirst;
static CRegion* m_pEnd;

CRegion();
explicit CRegion(int n);
~CRegion();

/**
 * \brief ������� ��� ������������ �������
 */
static void DeleteAllRegions();
/**
 * \brief �������� �������� � ��������� STR_PAR str �� ������ DATA_IN m_DataIn
 */
void UpdateStrValues();
static CRegion* GetFirst();
static CRegion* GetNext(CRegion* reg);

static std::vector<CRegion*> GetAsVector();
/**
* \brief ������� ��������� ������: ����� ����������, ����� �������������, � ����������� �����������, � ����������� ����������� ��������, 
* � ����������� ID. ���� skipRegion �����, �� ��� ������ ���������� 
*/
static CRegion* GetNextByPriority(CRegion* skipRegion = nullptr);
static CRegion* GetAtPosition(int position);

/**
* \brief ���������� ���� ��� ����� �������� ����� ��� ����, ����� ��������. ������ �� ������.
*/
static bool CanMoveAnyRegion(std::vector<CRegion*> regs, Directions dir);

/**
* \brief ���������� ���� ��� ����� �������� ����� ��� ����, ����� ��������. ������ �� ������.
*/
static void MoveRegionsIfPossible(std::vector<CRegion*> regs, Directions dir);

/**
* \brief �������� ������� ��� �������, ������� ����� �� ID
*/
static void Swap(CRegion* r1, CRegion* r2);

private:
	void InitNewRegion();
};