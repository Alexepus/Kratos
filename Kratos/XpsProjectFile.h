#pragma once
#include "IProjectFilePointerProvider.h"
#include  "CRegion.h"

class XpsProjectFile
{
	IProjectFilePointerProvider* _projectFilePointerProvider;
public:
	XpsProjectFile(IProjectFilePointerProvider* projectFilePointerProvider);
	~XpsProjectFile();

	void ReadXpsFile(FILE* fp, int FileVersion);
	void SaveDataOutPointToFile(CRegion* pReg, int N, DATA_OUT* data);
	void SaveDataInToFile(CRegion* pReg);
	void SaveXpsFullRegionDataToFile(CRegion* pReg);
	void SaveProject(FILE* fp);

private:
	void ReadXpsFileV1(FILE* fp);
	void ReadXpsFileV2(FILE* fp);
	void ReadXpsFileV3(FILE* fp);

	FILE* GetFilePointer();

	struct DATA_IN_V3
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
		short CountOfSaveCopyPoints;
		short PassagesWhenSaveCopy[30];
		char Reserved[16];
	};

	struct DATA_IN_V2
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
			DATA_IN din{ HV, KE_Start, KE_End, Step, Time, N_, Curr_N, Off, (DATA_IN::EnergyType)(int)KE_BE, N_h_nu };
			std::copy(Comments, std::end(Comments), din.Comments);
			return din;
		}
	};
};

