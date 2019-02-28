#include "stdafx.h"
#include "XpsProjectFile.h"
#include "Exceptions.h"
#include "ProgNew.h"

#define CHECK_FILE_POINTER_FOR_NULL() if (_fpPrj == nullptr) throw EXCEPTION("Ошибка при сохранении проекта. Файл проекта не был открыт: _fpPrj == nullptr.");

#define XPS_VER1 0x13
#define XPS_VER2 0x14
#define XPS_VER3 0x15

XpsProjectFile::XpsProjectFile(IProjectFilePointerProvider* projectFilePointerProvider)
{
	_projectFilePointerProvider = projectFilePointerProvider;
}

XpsProjectFile::~XpsProjectFile()
{
}

//Записывает N-ую точку data региона pReg
void XpsProjectFile::SaveDataOutPointToFile(CRegion* pReg, int N, DATA_OUT* data)
{
	if (!pReg)
		throw EXCEPTION("Writing data point to project file: Pointer to region is NULL");

	auto fp = GetFilePointer();
	fseek(fp, (pReg->m_ptrInFile + sizeof(DATA_IN) + 2 * sizeof(time_t) + 2 * sizeof(int) + (N * sizeof(DATA_OUT))),
		SEEK_SET);
	fwrite(data, sizeof(DATA_OUT), 1, fp);
	fflush(fp);
}

//Записывает входные параметры региона pReg, время начала и конца, число снятых в нем точек и общее число точек
void XpsProjectFile::SaveDataInToFile(CRegion* pReg)
{
	if (!pReg)
		throw EXCEPTION("Writing data point to project file: Pointer to region is NULL");

	auto fp = GetFilePointer();
	fseek(fp, pReg->m_ptrInFile - sizeof(UINT), SEEK_SET);
	int currentPos;
	fread(&currentPos, sizeof(UINT), 1, fp);
	if (currentPos != pReg->m_ptrInFile)
		throw EXCEPTION("Файл поврежден: неверная запись о смещении хранения настроек региона");
	fseek(fp, pReg->m_ptrInFile, SEEK_SET);
	auto dataIn = DATA_IN_V2(pReg->m_DataIn);
	fwrite(&dataIn, sizeof(dataIn), 1, fp);
	fwrite(&pReg->m_BeginTime, sizeof(time_t), 1, fp);
	fwrite(&pReg->m_EndTime, sizeof(time_t), 1, fp);
	fwrite(&pReg->m_NDataOutCurr, sizeof(int), 1, fp);
	fwrite(&pReg->m_NDataOut, sizeof(int), 1, fp);
	fflush(fp);
}

//Записывает в файл все данные о регионе pReg
void XpsProjectFile::SaveXpsFullRegionDataToFile(CRegion* pReg)
{
	auto fp = GetFilePointer();

	SaveDataInToFile(pReg);

	fwrite(pReg->m_pDataOut, sizeof(DATA_OUT), pReg->m_NDataOut, fp);
	fflush(fp);
	int currentPos = ftell(fp);
	auto nextReg = CRegion::GetNext(pReg);
	if (!nextReg || nextReg->m_ptrInFile == currentPos + sizeof(int))
		return;
	throw EXCEPTION("Ошибка при сохранении региона. Файл поврежден: неверная запись о смещении следующего региона.");
}

void XpsProjectFile::SaveProject(FILE* fp)
{
	SaveProjectV2(fp);
}

void XpsProjectFile::SaveProjectV2(FILE* fp)
{
	UINT ptrCurr = 0;
	unsigned char key[4];
	key[0] = 0x01; key[1] = 0x03; key[2] = 0xbc; key[3] = XPS_VER2;
	fseek(fp, ptrCurr, SEEK_SET);
	ptrCurr += (UINT) sizeof(unsigned char)*fwrite(key, sizeof(unsigned char), 4, fp);
	ptrCurr += (UINT) sizeof(int)*fwrite(&CRegion::m_NReg, sizeof(int), 1, fp);
	for (CRegion* pReg = CRegion::GetFirst(); pReg != NULL; pReg = CRegion::GetNext(pReg))
	{
		ptrCurr += sizeof(UINT);
		fwrite(&ptrCurr, sizeof(UINT), 1, fp);
		pReg->m_ptrInFile = ptrCurr;
		auto dataIn = DATA_IN_V2(pReg->m_DataIn);
		ptrCurr += (UINT) sizeof(dataIn)*fwrite(&dataIn, sizeof(dataIn), 1, fp);
		ptrCurr += (UINT) sizeof(time_t)*fwrite(&pReg->m_BeginTime, sizeof(time_t), 1, fp);
		ptrCurr += (UINT) sizeof(time_t)*fwrite(&pReg->m_EndTime, sizeof(time_t), 1, fp);
		ptrCurr += (UINT) sizeof(int)*fwrite(&pReg->m_NDataOutCurr, sizeof(int), 1, fp);
		ptrCurr += (UINT) sizeof(int)*fwrite(&pReg->m_NDataOut, sizeof(int), 1, fp);
		ptrCurr += (UINT) sizeof(DATA_OUT)*fwrite(pReg->m_pDataOut,
			sizeof(DATA_OUT), pReg->m_NDataOut, fp);
	}
}

void XpsProjectFile::SaveProjectV3(FILE* fp)
{
}

void XpsProjectFile::ReadXpsFileV1(FILE* fp)
{
	int NReg;
	if (fread(&NReg, sizeof(int), 1, fp) != 1)
		throw EXCEPTION("Error read file when reading number of regions");

	for (int i = 0; i<NReg; ++i)
	{
		CRegion* pReg = new CRegion;
		if (pReg != NULL)
		{
			if (fread(&pReg->m_ptrInFile, sizeof(UINT), 1, fp) != 1)
				throw EXCEPTION(Format("Error read file when reading pointer to file location in R%i", pReg->ID));
			int currentPos = ftell(fp);
			if (currentPos != pReg->m_ptrInFile)
				throw EXCEPTION(Format("Неверная позиция региона R%i, записанная в файле. Вероятно, файл поврежден", pReg->ID));

			DATA_IN_V1 dataInV1;
			if (fread(&dataInV1, sizeof(DATA_IN_V1), 1, fp) != 1)
				throw EXCEPTION(Format("Error read file when reading region input parameters in R%i", pReg->ID));
			pReg->m_DataIn = dataInV1.ToDataIn();

			if (fread(&pReg->m_NDataOutCurr, sizeof(int), 1, fp) != 1)
				throw EXCEPTION(Format("Error read file when reading current point number in R%i", pReg->ID));

			if (fread(&pReg->m_NDataOut, sizeof(int), 1, fp) != 1)
				throw EXCEPTION(Format("Error read file when reading number of output points in R%i", pReg->ID));

			if (pReg->m_NDataOut<0 || pReg->m_NDataOut>60000)
				throw EXCEPTION(Format("Region R%i: incorrect number of data points (%i). ", pReg->ID + 1, pReg->m_NDataOut));

			if (pReg->m_NDataOut)
			{
				pReg->m_pDataOut = (DATA_OUT*)malloc(pReg->m_NDataOut * sizeof(DATA_OUT));
				if (pReg->m_pDataOut != NULL)
				{
					memset(pReg->m_pDataOut, 0, pReg->m_NDataOut * sizeof(DATA_OUT));
					int Read = fread(pReg->m_pDataOut, sizeof(DATA_OUT), pReg->m_NDataOut, fp);
					if (Read != pReg->m_NDataOut)
					{
						throw EXCEPTION(Format("Ошибка при чтении данных региона (Region R%i: open %i data points, need %i. Current point measured: %i)",
							pReg->ID + 1, Read, pReg->m_NDataOut, pReg->m_NDataOutCurr));
					}
				}
				else
				{
					delete pReg;
					throw EXCEPTION(Format("Can`t alloc memory for DataOut of region number %i", CRegion::m_NReg).GetString());
				}
			}

			if (pReg->m_DataIn.N_h_nu<0) pReg->m_DataIn.N_h_nu = 0;
			if (pReg->m_DataIn.N_h_nu>3) pReg->m_DataIn.N_h_nu = 3;
			pReg->UpdateStrValues();
		}  // end if(pReg != NULL)
		else
			throw EXCEPTION(Format("Can`t alloc memory for object CRegion number %i", CRegion::m_NReg + 1).GetString());
	}
}

void XpsProjectFile::ReadXpsFileV2(FILE* fp)
{
	int NReg;
	if (fread(&NReg, sizeof(int), 1, fp) != 1)
		throw EXCEPTION("Error read file when reading number of regions");

	for (int i = 0; i<NReg; ++i)
	{
		CRegion* pReg = new CRegion;
		if (pReg != NULL)
		{
			if (fread(&pReg->m_ptrInFile, sizeof(UINT), 1, fp) != 1)
				throw EXCEPTION(Format("Error read file when reading pointer to file location in R%i", pReg->ID));
			int currentPos = ftell(fp);
			if (currentPos != pReg->m_ptrInFile)
				throw EXCEPTION(Format("Неверная позиция региона R%i, записанная в файле. Вероятно, файл поврежден", pReg->ID));

			DATA_IN_V2 dataInV2;
			if (fread(&dataInV2, sizeof(DATA_IN_V2), 1, fp) != 1)
				throw EXCEPTION(Format("Error read file when reading region input parameters in R%i", pReg->ID));
			pReg->m_DataIn = dataInV2.ToDataIn();

			if (fread(&pReg->m_BeginTime, sizeof(time_t), 1, fp) != 1)
				throw EXCEPTION(Format("Error read file when reading region begin time in R%i", pReg->ID));

			if (fread(&pReg->m_EndTime, sizeof(time_t), 1, fp) != 1)
				throw EXCEPTION(Format("Error read file when reading region begin time in R%i", pReg->ID));

			if (fread(&pReg->m_NDataOutCurr, sizeof(int), 1, fp) != 1)
				throw EXCEPTION(Format("Error read file when reading current point number in R%i", pReg->ID));

			if (fread(&pReg->m_NDataOut, sizeof(int), 1, fp) != 1)
				throw EXCEPTION(Format("Error read file when reading number of output points in R%i", pReg->ID));

			if (pReg->m_NDataOut<0 || pReg->m_NDataOut>60000)
				throw EXCEPTION(Format("Region R%i: incorrect number of data points (%i). ", pReg->ID + 1, pReg->m_NDataOut));

			if (pReg->m_NDataOut)
			{
				pReg->m_pDataOut = (DATA_OUT*)malloc(pReg->m_NDataOut * sizeof(DATA_OUT));
				if (pReg->m_pDataOut != NULL)
				{
					memset(pReg->m_pDataOut, 0, pReg->m_NDataOut * sizeof(DATA_OUT));
					int Read = fread(pReg->m_pDataOut, sizeof(DATA_OUT), pReg->m_NDataOut, fp);
					if (Read != pReg->m_NDataOut)
					{
						throw EXCEPTION(Format("Ошибка при чтении данных региона (Region R%i: open %i data points, need %i. Current point measured: %i)",
							pReg->ID + 1, Read, pReg->m_NDataOut, pReg->m_NDataOutCurr));
					}
				}
				else
				{
					delete pReg;
					throw EXCEPTION(Format("Can`t alloc memory for DataOut of region number %i", CRegion::m_NReg).GetString());
				}
			}

			if (pReg->m_DataIn.N_h_nu<0) pReg->m_DataIn.N_h_nu = 0;
			if (pReg->m_DataIn.N_h_nu>3) pReg->m_DataIn.N_h_nu = 3;
			pReg->UpdateStrValues();
		}  // end if(pReg != NULL)
		else
			throw EXCEPTION(Format("Can`t alloc memory for object CRegion number %i", CRegion::m_NReg + 1).GetString());
	}
}

void XpsProjectFile::ReadXpsFileV3(FILE* fp)
{
	
}

FILE* XpsProjectFile::GetFilePointer()
{
	return _projectFilePointerProvider->GetProjectFilePointer();
}

void XpsProjectFile::ReadXpsFile(FILE* fp, int FileVersion)
{
	/* Структура XPS-файла (Ver.1 (0x13)):
	____________________________________________________________
	struct key
	{
	short	KratosKey=KRATOS_KEY={0x01,0x03};
	char	DocumentType=XPS_TYPE=0xbc;
	char	DocumentVersion=XPS_VER1=0x13;
	};
	int	NumberOfRegions;
	struct
	{
	UINT	ptrCurr; //Указывает на m_DataIn
	DATA_IN_V1 m_DataIn;
	int		m_NDataOutCurr;
	int		m_NDataOut;
	DATA_OUT (*m_pDataOut)[m_NDataOut];

	} FullRegionData[NumberOfRegions];


	Структура XPS-файла (Ver.2 (0x14)):
	____________________________________________________________
	struct key
	{
	short	KratosKey=KRATOS_KEY={0x01,0x03};
	char	DocumentType=XPS_TYPE=0xbc;
	char	DocumentVersion=XPS_VER2=0x14;
	};
	int	NumberOfRegions;
	struct
	{
	UINT	ptrCurr; //Указывает на позицию m_DataIn файле (скорее, для проверки корректности файла)
	DATA_IN_V2 m_DataIn;
	time_t m_BeginTime; // Время начала изменения
	time_t m_EndTime; // Время последней записи выходных данных в файл
	int		m_NDataOutCurr; // Текущая измеряемая позиция
	int		m_NDataOut; // Количество выходных элементов DATA_OUT
	DATA_OUT (*m_pDataOut)[m_NDataOut];
	} FullRegionData[NumberOfRegions];


	Структура XPS-файла (Ver.3 (0x15)):
	____________________________________________________________
	struct key
	{
	short	KratosKey=KRATOS_KEY={0x01,0x03};
	char	DocumentType=XPS_TYPE=0xbc;
	char	DocumentVersion=XPS_VER2=0x15;
	};
	int	NumberOfRegions;
	struct
	{
	UINT	ptrCurr; //Указывает на позицию m_DataIn файле (скорее, для проверки корректности файла)
	DATA_IN_V3 m_DataIn;
	time_t m_BeginTime; // Время начала изменения
	time_t m_EndTime; // Время последней записи выходных данных в файл
	int		m_NDataOutCurr; // Текущая измеряемая позиция
	int		m_NDataOut; // Количество выходных элементов DATA_OUT
	char    Reserved[16];
	DATA_OUT (*m_pDataOut)[m_NDataOut];
	} FullRegionData[NumberOfRegions];
	*/

	if (FileVersion<XPS_VER1)
		throw EXCEPTION("Incorrect file version");
	if (FileVersion == XPS_VER1)
		ReadXpsFileV1(fp);
	else if (FileVersion == XPS_VER2)
		ReadXpsFileV2(fp);
	else if (FileVersion == XPS_VER3)
		ReadXpsFileV3(fp);
	else
		throw EXCEPTION("Too new project version, not supported by this version of application");
}
