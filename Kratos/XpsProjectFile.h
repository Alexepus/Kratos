#pragma once
#include "IProjectFilePointerProvider.h"
#include  "CRegion.h"
#include "Exceptions.h"

class XpsProjectFile
{
	IProjectFilePointerProvider* _projectFilePointerProvider;
	int _version = 0;
	const int SupportedVersion = 3;
public:
	XpsProjectFile(IProjectFilePointerProvider* projectFilePointerProvider);
	~XpsProjectFile();

	void ReadXpsFile(FILE* fp, int FileVersion);
	void SaveDataOutPointToFile(CRegion* pReg, int N, DATA_OUT* data);
	void SaveDataInToFile(CRegion* pReg);
	void SaveXpsFullRegionDataToFile(CRegion* pReg);
	void SaveProject(FILE* fp);

private:
	void SaveProjectV2(FILE* fp);
	void SaveProjectV3(FILE* fp);

	void ReadXpsFileV1(FILE* fp);
	void ReadXpsFileV2(FILE* fp);
	void ReadXpsFileV3(FILE* fp);
	void FullResaveIfVersionConflicts();
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
		byte Reserved[256];

		DATA_IN ToDataIn() const
		{
			DATA_IN din{ HV, KE_Start, KE_End, Step, Time, N_, Curr_N, Off, (DATA_IN::EnergyType)(int)KE_BE, N_h_nu, DeltaVolts };
			din.Comments = Comments;
			din.Priority = Priority;
			din.LastEditTime = LastEditTime;
			if (CountOfSaveCopyPoints < 0 || CountOfSaveCopyPoints > sizeof(PassagesWhenSaveCopy))
				throw EXCEPTION("�� ����� ������� ������� �������� �������� CountOfSaveCopyPoints");
			din.PassagesWhenSaveCopy.assign(&PassagesWhenSaveCopy[0], &PassagesWhenSaveCopy[CountOfSaveCopyPoints]);
			return din;
		}

		DATA_IN_V3() = default;
		DATA_IN_V3(DATA_IN &din)
		{
			HV = din.HV;
			KE_Start = din.KE_Start;
			KE_End = din.KE_End;
			Step = din.Step;
			Time = din.Time;
			N_ = din.N_;
			Curr_N = din.Curr_N;
			Off = din.Off;
			KE_BE = (EnergyType)(int)din.KE_BE;
			N_h_nu = din.N_h_nu;
			DeltaVolts = din.DeltaVolts;
			if (din.Comments.GetLength() > sizeof(Comments) - 1)
				din.Comments = din.Comments.Left(sizeof(Comments) - 1);
			strcpy_s(Comments, sizeof(Comments), din.Comments.GetString());
			Priority = din.Priority;
			LastEditTime = din.LastEditTime;
			CountOfSaveCopyPoints = static_cast<short>(din.PassagesWhenSaveCopy.size());
			memcpy_s(PassagesWhenSaveCopy, sizeof(PassagesWhenSaveCopy)*sizeof(short), din.PassagesWhenSaveCopy.data(), din.PassagesWhenSaveCopy.size() * sizeof(short));
			memset(Reserved, 0, sizeof(Reserved));
		}
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
		int DeltaVolts = 0;
		char Comments[256];
		int Priority = 10; // ��������� �������. ������� �����������, ������� � ������������ ����������, �� ���������� N_ ������ � ������ ������� � ������ �����������
		time_t LastEditTime = 0; //����� ���������� ��������� ���������� �������
		DATA_IN ToDataIn() const
		{
			DATA_IN din{ HV, KE_Start, KE_End, Step, Time, N_, Curr_N, Off, (DATA_IN::EnergyType)(int)KE_BE, N_h_nu, DeltaVolts };
			din.Comments = Comments;
			din.Priority = Priority;
			din.LastEditTime = LastEditTime;
			return din;
		}
		DATA_IN_V2() = default;
		DATA_IN_V2(DATA_IN &din)
		{
			HV = din.HV;
			KE_Start = din.KE_Start;
			KE_End = din.KE_End;
			Step = din.Step;
			Time = din.Time;
			N_ = din.N_;
			Curr_N = din.Curr_N;
			Off = din.Off;
			KE_BE = (EnergyType)(int)din.KE_BE;
			N_h_nu = din.N_h_nu;
			DeltaVolts = din.DeltaVolts;
			strcpy_s(Comments, sizeof(Comments), din.Comments.GetString());
			Priority = din.Priority;
			LastEditTime = din.LastEditTime;
		}
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
			din.Comments = Comments;
			return din;
		}
	};
};

