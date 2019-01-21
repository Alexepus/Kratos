#include "stdafx.h"
#include "DxpsProjectFile.h"
#include "DxpsRegion.h"
#include "Exceptions.h"
#include "MfcHelper.h"
#include "ProgNew.h"

#define DXPS_VER1 0x13
#define DXPS_VER2 0x14

DxpsProjectFile::DxpsProjectFile(IProjectFilePointerProvider* projectFilePointerProvider)
{
	_projectFilePointerProvider = projectFilePointerProvider;
}


DxpsProjectFile::~DxpsProjectFile()
{
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
	WriteDxpsRegionsParam();
	WriteDxpsPoints();
}

//________________________________________________________________________
//Записывает входные параметры всех регионов
BOOL DxpsProjectFile::WriteDxpsRegionsParam()
{
	auto fp = _projectFilePointerProvider->GetProjectFilePointer();
	fseek(fp, 4, SEEK_SET);
	int regNumber = CDxpsRegion::GetRegNumber();
	auto ptrCurr = sizeof(int)*fwrite(&regNumber, sizeof(int), 1, fp);
	for (auto pReg = CDxpsRegion::GetFirst(); pReg != NULL; pReg = CDxpsRegion::GetNext(pReg))
	{
		ptrCurr += sizeof(DxpsRegPar)*fwrite(&pReg->Parameters,
			sizeof(DxpsRegPar), 1, fp);
	}
	ptrCurr += sizeof(double)*fwrite(&CDxpsRegion::ScanTime, sizeof(double), 1, fp);
	ptrCurr += sizeof(double)*fwrite(&CDxpsRegion::ScanStartDateTime, sizeof(double), 1, fp);
	if (ptrCurr != sizeof(int) + sizeof(DxpsRegPar)*CDxpsRegion::GetRegNumber() + sizeof(double))
		return FALSE;
	return TRUE;
}

//________________________________________________________________________
//Дописывает данные в конец файла, начиная с точки, на которую указывает iter.
//Версия без iter записывает все точки с позиции начала области данных.
//Кроме того, записывает прошедшее время и количество точек
BOOL DxpsProjectFile::WriteDxpsPoints(DxpsOutList::iterator iter)
{
	auto fp = _projectFilePointerProvider->GetProjectFilePointer();
	const int pos_PassedCommonTime = 24;

	fseek(fp, 0, SEEK_END);
	for (auto i = iter; i != CDxpsRegion::OutData.end(); i++)
	{
		fwrite(&(*i), sizeof(DxpsOutData), 1, fp);
	}
	fseek(fp, pos_PassedCommonTime + sizeof(DxpsRegPar)*CDxpsRegion::GetRegNumber(), SEEK_SET);
	fwrite(&CDxpsRegion::PassedCommonTime, sizeof(double), 1, fp);
	fwrite(&CDxpsRegion::PassedNumberOfPoints, sizeof(int), 1, fp);

	return TRUE;
}

//________________________________________________________________________
//Записывает все точки с позиции начала области данных.
//Кроме того, записывает прошедшее время и количество точек
BOOL DxpsProjectFile::WriteDxpsPoints()
{
		auto fp = _projectFilePointerProvider->GetProjectFilePointer();

	const int pos_PassedCommonTime = 24;
	auto Pos = pos_PassedCommonTime + sizeof(CDxpsRegion::PassedCommonTime) + sizeof(CDxpsRegion::PassedNumberOfPoints) +
		sizeof(DxpsRegPar)*CDxpsRegion::GetRegNumber();
	fseek(fp, Pos, SEEK_SET); // Начало блока измеренных данных
	for (auto i = CDxpsRegion::OutData.begin(); i != CDxpsRegion::OutData.end(); i++)
	{
		if ((i->RegionN>CDxpsRegion::GetRegNumber() - 1) || (i->RegionN<0))
		{
			Msg("Out data of incorrect region %i", i->RegionN);
			return FALSE;
		}
		Pos += sizeof(DxpsOutData)*fwrite(&(*i), sizeof(DxpsOutData), 1, fp);
	}
	Pos = pos_PassedCommonTime + sizeof(DxpsRegPar)*CDxpsRegion::GetRegNumber();
	if (fseek(fp, Pos, SEEK_SET))
		Msg("fseek fails");
	Pos += sizeof(double)*fwrite(&CDxpsRegion::PassedCommonTime, sizeof(double), 1, fp);
	if (CDxpsRegion::OutData.size() != CDxpsRegion::PassedNumberOfPoints)
		Msg("WriteDxpsPoints: Incorrect number of points.\nOutData.size()=%i, CDxpsRegion::PassedNumberOfPoints=%i", CDxpsRegion::OutData.size(), CDxpsRegion::PassedNumberOfPoints);
	Pos += sizeof(int)*fwrite(&CDxpsRegion::PassedNumberOfPoints, sizeof(int), 1, fp);

	return TRUE;
}
