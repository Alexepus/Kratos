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
	void SaveProjectV2(FILE* fp);
	void SaveProjectV3(FILE* fp);

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
		int Time;   // Время измерения на точку в мс
		int N_;     // Заданное число сканов
		int Curr_N; // Число пройденных сканов
		BOOL Off;
		enum class EnergyType
		{
			KE,
			BE
		};
		EnergyType KE_BE;
		char N_h_nu; // Здесь просто байт
		int DeltaVolts;
		char Comments[256];
		int Priority = 10; // Приоритет региона. Регионы сканируются, начиная с минимального приоритета, до выполнения N_ сканов в каждом регионе с данным приоритетом
		time_t LastEditTime = 0; //Время последнего изменения параметров региона
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
		int Time;   // Время измерения на точку в мс
		int N_;     // Заданное число сканов
		int Curr_N; // Число пройденных сканов
		BOOL Off;
		enum class EnergyType
		{
			KE,
			BE
		};
		EnergyType KE_BE;
		char N_h_nu; // Здесь просто байт
		int DeltaVolts = 0;
		char Comments[256];
		int Priority = 10; // Приоритет региона. Регионы сканируются, начиная с минимального приоритета, до выполнения N_ сканов в каждом регионе с данным приоритетом
		time_t LastEditTime = 0; //Время последнего изменения параметров региона
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
		int Time;   // Время измерения на точку
		int N_;     // Заданное число сканов
		int Curr_N; // Число пройденных сканов
		BOOL Off;
		enum class EnergyType
		{
			KE,
			BE
		};
		EnergyType KE_BE;
		char N_h_nu; // Здесь просто байт
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

