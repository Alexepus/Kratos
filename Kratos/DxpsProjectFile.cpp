#include "stdafx.h"
#include "DxpsProjectFile.h"
#include "DxpsRegion.h"
#include "Exceptions.h"
#include "MfcHelper.h"

#define DXPS_VER1 0x13
#define DXPS_VER2 0x14

DxpsProjectFile::DxpsProjectFile()
{
}


DxpsProjectFile::~DxpsProjectFile()
{
	if (_fpPrj != nullptr)
		CloseHandle(_fpPrj);
}

void DxpsProjectFile::ReadProject(FILE* fp, int FileVersion)
{
	/* 	*** Структура DXPS-файла (Ver.1 (0x13)): ***
	____________________________________________________________
	struct key
	{
	00			short	KratosKey=KRATOS_KEY=0x0103;
	02			char	DocumentType=DXPS_TYPE=0xbd;
	03			char	DocumentVersion=DXPS_VER1=0x13;
	};
	04		int	NumberOfRegions;
	08		DxpsRegPar	<SpecificRegion>Parameters[NumberOfRegions];
	08+320N	double		ScanTime;
	16+320N	double		PassedCommonTime;
	24+320N	int			PassedNumberOfPoints;
	28+320N	DxpsOutData	OutData[NumberOfPoints];
	_____________________________________________________________
	где:
	struct DxpsRegPar
	{
	00			BOOL Off;
	04			int Type; //0 if "BE Print", 1 if Division Divident/Divisor
	08			int Divident;
	12			int Divisor;
	16			double BE;
	24			double HV;
	32			int Anode;//0:Al, 1:Mg, 2:He1, 3:He2
	36			double Delay; //Задержка перед началом измерений
	44			double Dwell;	//Время измерения
	52			int ColorIndex;
	56			char Comments[256];
	=312	};

	struct DxpsOutData_v1
	{
	int RegionN;
	double Time;
	double Freq;
	};
	____________________________________________________________________

	*** Структура DXPS-файла (Ver.2 (0x14)): ***
	____________________________________________________________
	struct key
	{
	00			short	KratosKey=KRATOS_KEY=0x0103;
	02			char	DocumentType=DXPS_TYPE=0xbd;
	03			char	DocumentVersion=DXPS_VER2=0x14;
	};
	04		int	NumberOfRegions;
	08		DxpsRegPar	<SpecificRegion>Parameters[NumberOfRegions];
	08+320N	double		ScanTime; //время измерения в секундах
	16+320N double      ScanStartDateTime; // Дата/время начала измерения
	24+320N	double		PassedCommonTime;
	32+320N	int			PassedNumberOfPoints;
	36+320N	DxpsOutData	OutData[NumberOfPoints];
	_____________________________________________________________
	где:
	struct DxpsRegPar
	{
	00			BOOL Off;
	04			int Type; //0 if "BE Print", 1 if Division Divident/Divisor
	08			int Divident;
	12			int Divisor;
	16			double BE;
	24			double HV;
	32			int Anode;//0:Al, 1:Mg, 2:He1, 3:He2
	36			double Delay; //Задержка перед началом измерений
	44			double Dwell;	//Время измерения
	52			int ColorIndex;
	56			char Comments[256];
	=312 	};
	//sizeof(DxpsRegPar) = 320

	struct DxpsOutData
	{
	int RegionN;
	double Time;
	double Freq;
	double Tref; // Задаточная температура, может быть NaN, т.е. не задана
	double Tcur; // Текущая измеренная температура, может быть NaN, т.е. неизвестна
	};
	//sizeof(DxpsOutData) = 40
	*/
	try
	{
		if (FileVersion < DXPS_VER1)
			throw EXCEPTION("Incorrect file version");
		if (FileVersion > DXPS_VER2)
			throw EXCEPTION("Too new project version, not supported by this version of application");

		// Чтение DXPS_VER1 или DXPS_VER2:
		fpos_t pos = 0;
		fgetpos(fp, &pos);
		int NReg;
		if (fread(&NReg, sizeof(int), 1, fp) != 1)
			throw EXCEPTION("Error read file when reading Number of regions");
		fgetpos(fp, &pos);
		for (int i = 0; i < NReg; ++i)
		{
			CDxpsRegion * pDxpsReg = CDxpsRegion::CreateNewRegion();
			if (fread(&pDxpsReg->Parameters, sizeof(DxpsRegPar), 1, fp) != 1)
				throw EXCEPTION("Ошибка при чтении параметров региона");
		}
		fgetpos(fp, &pos);
		if (fread(&CDxpsRegion::ScanTime, sizeof(double), 1, fp) != 1)
			throw std::exception("Ошибка при чтении параметров региона");
		if (FileVersion == DXPS_VER2)
		{
			if (fread(&CDxpsRegion::ScanStartDateTime, sizeof(double), 1, fp) != 1)
				throw EXCEPTION("Ошибка при чтении параметров региона");
		}
		if (fread(&CDxpsRegion::PassedCommonTime, sizeof(double), 1, fp) != 1)
			throw EXCEPTION("Error read parameters of output data");
		if (fread(&CDxpsRegion::PassedNumberOfPoints, sizeof(int), 1, fp) != 1)
			throw EXCEPTION("Error read parameters of output data");
		fgetpos(fp, &pos);
		if (FileVersion == DXPS_VER1)
		{
			DxpsOutData OutData;
			for (int i = 0; i < CDxpsRegion::PassedNumberOfPoints; ++i)
			{
				if (fread(&OutData, sizeof(DxpsOutData_v1), 1, fp) != 1)
					throw EXCEPTION("Error read of measured data");
				if ((OutData.RegionN > NReg - 1) || (OutData.RegionN < 0))
					throw EXCEPTION(Format("Region R%i is not valid. Read data of absent DXPS region.", OutData.RegionN));
				OutData.Tref = std::numeric_limits<double>::quiet_NaN();
				OutData.Tcur = std::numeric_limits<double>::quiet_NaN();
				CDxpsRegion::OutData.push_back(OutData);
			}
		}
		else // DXPS_VER2
		{
			DxpsOutData OutData;
			for (int i = 0; i < CDxpsRegion::PassedNumberOfPoints; ++i)
			{
				if (fread(&OutData, sizeof(DxpsOutData), 1, fp) != 1)
					throw EXCEPTION("Error read of measured data");
				if ((OutData.RegionN > NReg - 1) || (OutData.RegionN < 0))
					throw EXCEPTION(Format("Region R%i is not valid. Read data of absent DXPS region.", OutData.RegionN));
				CDxpsRegion::OutData.push_back(OutData);
			}
		}
	}
	catch(...)
	{
		CDxpsRegion::PassedNumberOfPoints = CDxpsRegion::OutData.size();
		throw;
	}
}

void DxpsProjectFile::SaveProject(FILE* fp)
{
	/*
	*** Структура DXPS-файла (Ver.1 (0x13)): ***
	____________________________________________________________
	struct key
	{
	00			short	KratosKey=KRATOS_KEY=0x0103;
	02			char	DocumentType=DXPS_TYPE=0xbd;
	03			char	DocumentVersion=DXPS_VER1=0x13;
	};
	04		int	NumberOfRegions;
	08		DxpsRegPar	<SpecificRegion>Parameters[NumberOfRegions];
	08+320N	double		ScanTime;
	16+320N	double		PassedCommonTime;
	24+320N	int			PassedNumberOfPoints;
	28+320N	DxpsOutData	OutData[NumberOfPoints];
	_____________________________________________________________
	где:
	struct DxpsRegPar
	{
	00			BOOL Off;
	04			int Type; //0 if "BE Print", 1 if Division Divident/Divisor
	08			int Divident;
	12			int Divisor;
	16			double BE;
	24			double HV;
	32			int Anode;//0:Al, 1:Mg, 2:He1, 3:He2
	36			double Delay; //Задержка перед началом измерений
	44			double Dwell;	//Время измерения
	52			int ColorIndex;
	56			char Comments[256];
	=312	};

	struct DxpsOutData
	{
	int RegionN;
	double Time;
	double Freq;
	};
	____________________________________________________________________

	*** Структура DXPS-файла (Ver.2 (0x14)): ***
	____________________________________________________________
	struct key
	{
	00			short	KratosKey=KRATOS_KEY=0x0103;
	02			char	DocumentType=DXPS_TYPE=0xbd;
	03			char	DocumentVersion=DXPS_VER2=0x14;
	};
	04		int	NumberOfRegions;
	08		DxpsRegPar	<SpecificRegion>Parameters[NumberOfRegions];
	08+320N	double		ScanTime;
	16+320N double      ScanStartDateTime; // Дата/время начала измерения
	24+320N	double		PassedCommonTime;
	32+320N	int			PassedNumberOfPoints;
	36+320N	DxpsOutData	OutData[NumberOfPoints];
	_____________________________________________________________
	где:
	struct DxpsRegPar
	{
	00			BOOL Off;
	04			int Type; //0 if "BE Print", 1 if Division Divident/Divisor
	08			int Divident;
	12			int Divisor;
	16			double BE;
	24			double HV;
	32			int Anode;//0:Al, 1:Mg, 2:He1, 3:He2
	36			double Delay; //Задержка перед началом измерений
	44			double Dwell;	//Время измерения
	52			int ColorIndex;
	56			char Comments[256];
	=312	};

	struct DxpsOutData
	{
	int RegionN;
	double Time;
	double Freq;
	double Tref; // Задаточная температура, может быть NaN, т.е. не задана
	double Tcur; // Текущая измеренная температура, может быть NaN, т.е. неизвестна
	};
	____________________________________________________________________

	Пояснения по записи файла:
	При изменении параметра региона переписываются все регионы.
	При изменении количества регионов и очистке данных переписывается весь файл.
	При прохождении одного скана (проход всех DXPS регионов) дописываются
	данные всего скана и переписываются пройденное время и количество точек.

	*/
	UINT ptrCurr = 0;
	fseek(fp, ptrCurr, SEEK_SET);
	unsigned char key[4];
	key[0] = (KRATOS_KEY & 0xff00) >> 8; key[1] = KRATOS_KEY & 0xff; key[2] = DXPS_TYPE; key[3] = DXPS_VER2;
	ptrCurr += (UINT) sizeof(unsigned char)*fwrite(key, sizeof(unsigned char), 4, fp);
	WriteDxpsRegionsParam(fp);
	WriteDxpsPoints(fp);
}
