#include"stdafx.h"
#include"Main.h"
#include <limits>
#include "Time.h"

extern CProgNewApp theApp;
extern CString FileSaveOpenErrorDescription;

#define KRATOS_KEY 0x0103
#define DXPS_TYPE 0xbd
#define XPS_VER1 0x13
#define XPS_VER2 0x14
#define DXPS_VER1 0x13
#define DXPS_VER2 0x14

BOOL SaveBinaryFile(FILE* fp)
{
	if(theApp.m_pMainFrame->m_Doc.m_DocType==CDoc::XPS||theApp.m_pMainFrame->m_Doc.m_DocType==CDoc::NoDoc)
	{
		UINT ptrCurr = 0;
		unsigned char key[4];
		key[0]=0x01; key[1]=0x03; key[2]=0xbc; key[3]=XPS_VER2;
		fseek(fp,ptrCurr, SEEK_SET);
		ptrCurr += (UINT) sizeof(unsigned char)*fwrite(key, sizeof(unsigned char), 4, fp);
		ptrCurr += (UINT) sizeof(int)*fwrite(&CRegion::m_NReg, sizeof(int), 1, fp);
		for(CRegion* pReg = CRegion::GetFirst(); pReg!=NULL; pReg=CRegion::GetNext(pReg))
		{
			ptrCurr+=sizeof(UINT);
			fwrite(&ptrCurr, sizeof(UINT), 1, fp);
			pReg->m_ptrInFile = ptrCurr;
			ptrCurr += (UINT) sizeof(DATA_IN)*fwrite(&pReg->m_DataIn, sizeof(DATA_IN), 1, fp);
			ptrCurr += (UINT) sizeof(time_t)*fwrite(&pReg->m_BeginTime, sizeof(time_t), 1, fp);
			ptrCurr += (UINT) sizeof(time_t)*fwrite(&pReg->m_EndTime, sizeof(time_t), 1, fp);
			ptrCurr += (UINT) sizeof(int)*fwrite(&pReg->m_NDataOutCurr, sizeof(int), 1, fp);
			ptrCurr += (UINT) sizeof(int)*fwrite(&pReg->m_NDataOut, sizeof(int), 1, fp);
			ptrCurr += (UINT) sizeof(DATA_OUT)*fwrite(pReg->m_pDataOut,
											sizeof(DATA_OUT), pReg->m_NDataOut, fp);
		}
	}
	else if(theApp.m_pMainFrame->m_Doc.m_DocType==CDoc::DXPS)
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
		fseek(fp,ptrCurr, SEEK_SET);
		unsigned char key[4];
		key[0]=(KRATOS_KEY&0xff00)>>8; key[1]=KRATOS_KEY&0xff; key[2]=DXPS_TYPE; key[3]=DXPS_VER2;
		ptrCurr += (UINT) sizeof(unsigned char)*fwrite(key, sizeof(unsigned char), 4, fp);
		WriteDxpsRegionsParam(fp);
		WriteDxpsPoints(fp);
	}
	fflush(fp);
return TRUE;
}

#define ERROR_READ_NREG 1
#define INCORRECT_FILE_VERSION 9
#define NEWER_XPS_FILE_VERSION 10
#define NEWER_DXPS_FILE_VERSION 11
#define ERROR_READ_OUT_DATA 12
#define ERROR_READ_OUTPUT_PARAMS 13
#define ERROR_READ_DATA_OF_ABSENT_REG 14
#define ERROR_TOO_MUCH_DATA_POINTS 15

void ReadXpsFileV1(FILE* fp)
{
	int NReg;
	if (fread(&NReg, sizeof(int), 1, fp) != 1)
		throw(ERROR_READ_NREG);

	for (int i = 0; i<NReg; ++i)
	{
		CRegion* pReg = new CRegion;
		if (pReg != NULL)
		{
			if (fread(&pReg->m_ptrInFile, sizeof(UINT), 1, fp) != 1)
				throw std::exception(Format("Error read file when reading pointer to file location in R%i", pReg->ID));
			int currentPos = ftell(fp);
			if (currentPos != pReg->m_ptrInFile)
				throw std::exception(Format("Неверная позиция региона R%i, записанная в файле. Вероятно, файл поврежден", pReg->ID));

			DATA_IN_V1 dataInV1;
			if (fread(&dataInV1, sizeof(DATA_IN_V1), 1, fp) != 1)
				throw std::exception(Format("Error read file when reading region input parameters in R%i", pReg->ID));
			pReg->m_DataIn = dataInV1.ToDataIn();

			if (fread(&pReg->m_NDataOutCurr, sizeof(int), 1, fp) != 1)
				throw std::exception(Format("Error read file when reading current point number in R%i", pReg->ID));

			if (fread(&pReg->m_NDataOut, sizeof(int), 1, fp) != 1)
				throw std::exception(Format("Error read file when reading number of output points in R%i", pReg->ID));
			
			if (pReg->m_NDataOut<0 || pReg->m_NDataOut>60000)
			{
				FileSaveOpenErrorDescription.Format("Region R%i: incorrect number of data points (%i). ", pReg->ID + 1, pReg->m_NDataOut);
				pReg->m_NDataOut = 60000;
				throw(ERROR_TOO_MUCH_DATA_POINTS);
			}
			if (pReg->m_NDataOut)
			{
				pReg->m_pDataOut = (DATA_OUT*)malloc(pReg->m_NDataOut * sizeof(DATA_OUT));
				if (pReg->m_pDataOut != NULL)
				{
					memset(pReg->m_pDataOut, 0, pReg->m_NDataOut * sizeof(DATA_OUT));
					int Read = fread(pReg->m_pDataOut, sizeof(DATA_OUT), pReg->m_NDataOut, fp);
					if (Read != pReg->m_NDataOut)
					{
						throw std::exception(Format("Ошибка при чтении данных региона (Region R%i: open %i data points, need %i. Current point measured: %i)",
							pReg->ID + 1, Read, pReg->m_NDataOut, pReg->m_NDataOutCurr));
					}
				}
				else
				{
					delete pReg;
					AfxMessageBox(Format("Can`t alloc memory for DataOut of region number %i", CRegion::m_NReg).GetString());
					break;
				}
			}

			if (pReg->m_DataIn.N_h_nu<0) pReg->m_DataIn.N_h_nu = 0;
			if (pReg->m_DataIn.N_h_nu>3) pReg->m_DataIn.N_h_nu = 3;
			pReg->UpdateStrValues();
		}  // end if(pReg != NULL)
		else
		{
			AfxMessageBox(Format("Can`t alloc memory for object CRegion number %i", CRegion::m_NReg + 1).GetString());
			break;
		}
	}
}

void ReadXpsFileV2(FILE* fp)
{
	int NReg;
	if (fread(&NReg, sizeof(int), 1, fp) != 1)
		throw(ERROR_READ_NREG);

	for (int i = 0; i<NReg; ++i)
	{
		CRegion* pReg = new CRegion;
		if (pReg != NULL)
		{
			if (fread(&pReg->m_ptrInFile, sizeof(UINT), 1, fp) != 1)
				throw std::exception(Format("Error read file when reading pointer to file location in R%i", pReg->ID));
			int currentPos = ftell(fp);
			if (currentPos != pReg->m_ptrInFile)
				throw std::exception(Format("Неверная позиция региона R%i, записанная в файле. Вероятно, файл поврежден", pReg->ID));

			if (fread(&pReg->m_DataIn, sizeof(DATA_IN), 1, fp) != 1)
				throw std::exception(Format("Error read file when reading region input parameters in R%i", pReg->ID));
			
			if (fread(&pReg->m_BeginTime, sizeof(time_t), 1, fp) != 1)
				throw std::exception(Format("Error read file when reading region begin time in R%i", pReg->ID));

			if (fread(&pReg->m_EndTime, sizeof(time_t), 1, fp) != 1)
				throw std::exception(Format("Error read file when reading region begin time in R%i", pReg->ID));

			if (fread(&pReg->m_NDataOutCurr, sizeof(int), 1, fp) != 1)
				throw std::exception(Format("Error read file when reading current point number in R%i", pReg->ID));

			if (fread(&pReg->m_NDataOut, sizeof(int), 1, fp) != 1)
				throw std::exception(Format("Error read file when reading number of output points in R%i", pReg->ID));

			if (pReg->m_NDataOut<0 || pReg->m_NDataOut>60000)
			{
				FileSaveOpenErrorDescription.Format("Region R%i: incorrect number of data points (%i). ", pReg->ID + 1, pReg->m_NDataOut);
				pReg->m_NDataOut = 60000;
				throw(ERROR_TOO_MUCH_DATA_POINTS);
			}
			if (pReg->m_NDataOut)
			{
				pReg->m_pDataOut = (DATA_OUT*)malloc(pReg->m_NDataOut * sizeof(DATA_OUT));
				if (pReg->m_pDataOut != NULL)
				{
					memset(pReg->m_pDataOut, 0, pReg->m_NDataOut * sizeof(DATA_OUT));
					int Read = fread(pReg->m_pDataOut, sizeof(DATA_OUT), pReg->m_NDataOut, fp);
					if (Read != pReg->m_NDataOut)
					{
						throw std::exception(Format("Ошибка при чтении данных региона (Region R%i: open %i data points, need %i. Current point measured: %i)",
							pReg->ID + 1, Read, pReg->m_NDataOut, pReg->m_NDataOutCurr));
					}
				}
				else
				{
					delete pReg;
					AfxMessageBox(Format("Can`t alloc memory for DataOut of region number %i", CRegion::m_NReg).GetString());
					break;
				}
			}

			if (pReg->m_DataIn.N_h_nu<0) pReg->m_DataIn.N_h_nu = 0;
			if (pReg->m_DataIn.N_h_nu>3) pReg->m_DataIn.N_h_nu = 3;
			pReg->UpdateStrValues();
		}  // end if(pReg != NULL)
		else
		{
			AfxMessageBox(Format("Can`t alloc memory for object CRegion number %i", CRegion::m_NReg + 1).GetString());
			break;
		}
	}
}

void ReadXpsFile(FILE* fp, int FileVersion)
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
	DATA_IN m_DataIn;
	time_t m_BeginTime; // Время начала изменения
	time_t m_EndTime; // Время последней записи выходных данных в файл
	int		m_NDataOutCurr; // Текущая измеряемая позиция
	int		m_NDataOut; // Количество выходных элементов DATA_OUT
	DATA_OUT (*m_pDataOut)[m_NDataOut];
	} FullRegionData[NumberOfRegions];
	*/

	//ParseXPSFile(fp);
	//fseek(fp, 4, SEEK_SET);

	if(FileVersion<XPS_VER1)
		throw (INCORRECT_FILE_VERSION);
	if (FileVersion == XPS_VER1)
		ReadXpsFileV1(fp);
	else if (FileVersion == XPS_VER2)
		ReadXpsFileV2(fp);
	else 
		throw (NEWER_XPS_FILE_VERSION);
}

BOOL ReadBinaryFile(FILE* fp)
{
FileSaveOpenErrorDescription="";
int NReg;
unsigned char key[4];
int i;

try{
if(!fread(key, sizeof(unsigned char), 4, fp)) //return FALSE;
	{AfxMessageBox("ERROR read file.   "); throw (7);}
int FileVersion = key[3];
if(key[0]==0x01 && key[1]==0x03 && key[2]==0xbc) theApp.m_pMainFrame->m_Doc.m_DocType=CDoc::XPS;
else if(key[0]==0x01 && key[1]==0x03 && key[2]==0xbd) theApp.m_pMainFrame->m_Doc.m_DocType=CDoc::DXPS;
else {AfxMessageBox("This file is not a project file."); throw (8);}

if(theApp.m_pMainFrame->m_Doc.m_DocType==CDoc::XPS)
	ReadXpsFile(fp, FileVersion);
else if(theApp.m_pMainFrame->m_Doc.m_DocType==CDoc::DXPS)
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
		if(FileVersion<DXPS_VER1)
			throw (INCORRECT_FILE_VERSION);
		if(FileVersion>DXPS_VER2)
			throw (NEWER_DXPS_FILE_VERSION);
		
		// Чтение DXPS_VER1 или DXPS_VER2:
		fpos_t pos=	0;
		fgetpos(fp, &pos);
		if(fread(&NReg, sizeof(int), 1, fp)!=1)
			throw(ERROR_READ_NREG);
		fgetpos(fp, &pos);
		for(i=0; i<NReg; ++i)
		{
			CDxpsRegion * pDxpsReg = CDxpsRegion::CreateNewRegion();
			if (fread(&pDxpsReg->Parameters, sizeof(DxpsRegPar), 1, fp)!=1)
				throw std::exception("Ошибка при чтении параметров региона");
		}
		fgetpos(fp, &pos);
		if (fread(&CDxpsRegion::ScanTime, sizeof(double), 1, fp)!=1)
			throw std::exception("Ошибка при чтении параметров региона");
		if(FileVersion == DXPS_VER2)
		{
			if (fread(&CDxpsRegion::ScanStartDateTime, sizeof(double), 1, fp)!=1)
				throw std::exception("Ошибка при чтении параметров региона");
		}
		if (fread(&CDxpsRegion::PassedCommonTime, sizeof(double), 1, fp)!=1)
			throw(ERROR_READ_OUTPUT_PARAMS);
		if (fread(&CDxpsRegion::PassedNumberOfPoints, sizeof(int), 1, fp)!=1)
			throw(ERROR_READ_OUTPUT_PARAMS);
		fgetpos(fp, &pos);
		if(FileVersion == DXPS_VER1)
		{
			DxpsOutData OutData;
			for(i=0; i<CDxpsRegion::PassedNumberOfPoints; ++i)
			{
				if (fread(&OutData, sizeof(DxpsOutData_v1), 1, fp)!=1)
					throw(ERROR_READ_OUT_DATA);
				if((OutData.RegionN>NReg-1) || (OutData.RegionN<0))
				{
					FileSaveOpenErrorDescription.Format("Region R%i is not valid. ", OutData.RegionN);
					throw(ERROR_READ_DATA_OF_ABSENT_REG);
				}
				OutData.Tref = std::numeric_limits<double>::quiet_NaN();
				OutData.Tcur = std::numeric_limits<double>::quiet_NaN();
				CDxpsRegion::OutData.push_back(OutData);
			}
		}
		else // DXPS_VER2
		{
			DxpsOutData OutData;
			for(i=0; i<CDxpsRegion::PassedNumberOfPoints; ++i)
			{
				if (fread(&OutData, sizeof(DxpsOutData), 1, fp)!=1)
					throw(ERROR_READ_OUT_DATA);
				if((OutData.RegionN>NReg-1) || (OutData.RegionN<0))
				{
					FileSaveOpenErrorDescription.Format("Region R%i is not valid. ", OutData.RegionN);
					throw(ERROR_READ_DATA_OF_ABSENT_REG);
				}
				CDxpsRegion::OutData.push_back(OutData);
			}
		}
	}

	if(!feof(fp)) //must be: eof==0
	{
		char ch='\0';
		fread(&ch,1,1,fp);
		if(feof(fp)) //must be: eof!=0
			return TRUE;
		FileSaveOpenErrorDescription+=" Project file has extra data.";
		
	}
}
catch(int Place)
{
	CString str2;
	fpos_t pos;
	switch(Place)
	{
	case ERROR_READ_NREG:
		str2.Format("Number of regions");
		FileSaveOpenErrorDescription+="Error read file when reading "+str2;
		break;
	case 7:
		fseek( fp, 0, SEEK_END);
		fgetpos(fp, &pos);
		str2.Format("File size: %I64i.", pos);
		FileSaveOpenErrorDescription+="The file is empty. "+str2;
		break;
	case 8:
		fseek( fp, 0, SEEK_END);
		fgetpos(fp, &pos);
		str2.Format("File size: %I64i", pos);
		FileSaveOpenErrorDescription+="Project type specificator is different. "+str2;
		break;
	case INCORRECT_FILE_VERSION:
		FileSaveOpenErrorDescription+="Incorrect file version. ";
		break;
	case NEWER_XPS_FILE_VERSION:
		FileSaveOpenErrorDescription+="This new version of XPS file is not supported by this program. ";
		break;
	case NEWER_DXPS_FILE_VERSION:
		FileSaveOpenErrorDescription+="This new version of DXPS file is not supported by this program. ";
		break;
	case ERROR_READ_OUT_DATA:
		FileSaveOpenErrorDescription+="Error read of measured data. ";
		break;
	case ERROR_READ_OUTPUT_PARAMS:
		FileSaveOpenErrorDescription+="Error read parameters of output data. ";
		break;
	case ERROR_READ_DATA_OF_ABSENT_REG:
		FileSaveOpenErrorDescription+="Read data of absent DXPS region. ";
		break;
	case ERROR_TOO_MUCH_DATA_POINTS:
		FileSaveOpenErrorDescription+="Too many data points. ";
		break;
	}
	CDxpsRegion::PassedNumberOfPoints=CDxpsRegion::OutData.size();	
}
catch (std::exception& ex)
{
	FileSaveOpenErrorDescription += ex.what();
}
return FALSE;
}

//===========
//Записывает N-ую точку data региона pReg
BOOL SaveDataOutPointToFile(FILE* fp, CRegion* pReg, int N, DATA_OUT* data)
{
	if(!fp) {AfxMessageBox("Pointer to file is NULL"); return FALSE;}
	if(!pReg) {AfxMessageBox("Pointer to region is NULL"); return FALSE;}
	fseek(fp, (pReg->m_ptrInFile + sizeof(DATA_IN) + 2*sizeof(time_t) + 2*sizeof(int) + (N*sizeof(DATA_OUT))),
		 SEEK_SET);
	fwrite(data, sizeof(DATA_OUT), 1, fp);
	fflush(fp);
	return TRUE;
}

//=================
//Записывает входные параметры региона pReg, время начала и конца, число снятых в нем точек и общее число точек
BOOL SaveDataInToFile(FILE* fp, CRegion* pReg)
{
	if(!fp) {AfxMessageBox("Pointer to file is NULL"); return FALSE;}
	if(!pReg) {AfxMessageBox("Pointer to region is NULL"); return FALSE;}
	fseek(fp, pReg->m_ptrInFile - sizeof(UINT), SEEK_SET);
	int currentPos;
	fread(&currentPos, sizeof(UINT), 1, fp);
	if (currentPos != pReg->m_ptrInFile)
		AfxMessageBox("Файл поврежден: неверная запись о смещении хранения настроек региона");
	fseek(fp, pReg->m_ptrInFile, SEEK_SET);
	fwrite(&pReg->m_DataIn, sizeof(DATA_IN), 1, fp);
	fwrite(&pReg->m_BeginTime, sizeof(time_t), 1, fp);
	fwrite(&pReg->m_EndTime, sizeof(time_t), 1, fp);
	fwrite(&pReg->m_NDataOutCurr, sizeof(int), 1, fp);
	fwrite(&pReg->m_NDataOut, sizeof(int), 1, fp);
	fflush(fp);
	return TRUE;
}

//Записывает в файл все данные о регионе pReg
bool SaveXpsFullRegionDataToFile(FILE* fp, CRegion* pReg)
{
	auto isSuccessfulSavedDataIn = SaveDataInToFile(fp, pReg);
	if (!isSuccessfulSavedDataIn)
		return false;
	fwrite(pReg->m_pDataOut, sizeof(DATA_OUT), pReg->m_NDataOut, fp);
	fflush(fp);
	int currentPos = ftell(fp);
	auto nextReg = CRegion::GetNext(pReg);
	if(!nextReg || nextReg->m_ptrInFile == currentPos + sizeof(int))
		return true;
	AfxMessageBox("Ошибка при сохранении региона. Файл поврежден: неверная запись о смещении следующего региона.");
	return false;
}

//=============

void SaveEasyPlotFile(FILE* fp, char* FileName)
{
CRegion* pReg;    //є 186
char* Caption[]={"|Reg |","KE/BE|","Anode|"," HV |","  Start  |","  Finish |"," Step |","  N |","  n |"," Time |","  Start Time  |", "   End Time   |", };
int i;
int NCaption = sizeof(Caption)/sizeof(char*);
int NLine=0;
int RegionsOff=0;
for (pReg=CRegion::GetFirst(); pReg!=NULL; pReg=CRegion::GetNext(pReg))
	if(pReg->m_DataIn.Off) RegionsOff++;
if(RegionsOff==0)
	fprintf(fp, ";Number of regions: %i\n", CRegion::m_NReg);
else 
	fprintf(fp, ";Number of regions: %i (%i of them %s off)\n", CRegion::m_NReg, RegionsOff,(RegionsOff==1)?"is":"are");
++NLine;
fprintf(fp,";");
for(i=0; i<NCaption; ++i) fprintf(fp,"%s",Caption[i]);
fprintf(fp,"\n"); ++NLine;
for(pReg=CRegion::GetFirst(); pReg!=NULL; pReg=CRegion::GetNext(pReg))
	{
	if(pReg->m_DataIn.Off)
		continue;

	if(NLine >= 25)
		{
		fprintf(fp,"\n;");
		for(i=0; i<NCaption; ++i) fprintf(fp,"%s",Caption[i]);
		NLine = 0;
		fprintf(fp,"\n"); ++NLine;
		}
	NLine += SaveEasyPlotTable(fp, pReg);
	}
fprintf(fp,";\n;\n");

for(pReg=CRegion::GetFirst(); pReg!=NULL; pReg=CRegion::GetNext(pReg))
	{
	if(pReg->m_DataIn.Off)
		continue;
	fprintf(fp,";");
	for(i=0; i<NCaption; ++i) fprintf(fp,"%s",Caption[i]);
	fprintf(fp,"\n"); //++NLine;
	SaveEasyPlotTable(fp, pReg);
	fprintf(fp,";Points count %i\n",pReg->m_NDataOut);
	fprintf(fp,"/td \"xy\"\n");
	fprintf(fp,"/sa l \"%s:%i,\" 2\n", FileName, pReg->ID+1);
	for(i=0; i<pReg->m_NDataOut; ++i)
		{fprintf(fp,"%.3lf  %.3f\n", I2D(pReg->m_pDataOut[i].x), pReg->m_pDataOut[i].y);
		}
	fprintf(fp,"//nc\n;\n");
	}
}
//------
int SaveEasyPlotTable(FILE* fp, CRegion* pReg)
{
int NLine = 0;
char Value[16];
	sprintf(Value,"R%i",pReg->ID+1);
	fprintf(fp,";|%4s|",Value);
	if(pReg->m_DataIn.KE_BE == DATA_IN::EnergyType::KE)
		{sprintf(Value," %s", "KE");
		 fprintf(fp,"%5s|",Value);
		 //sprintf(Value,"%s", "None");
		 sprintf(Value,"%s", CRegion::h_nu_Info.strName_h_nu[pReg->m_DataIn.N_h_nu]);
		 fprintf(fp,"%5s|",Value);
		}
	else
		{sprintf(Value," %s", "BE");
		 fprintf(fp,"%5s|",Value);
		 sprintf(Value,"%s", CRegion::h_nu_Info.strName_h_nu[pReg->m_DataIn.N_h_nu]);
		 fprintf(fp,"%5s|",Value);
		}

	sprintf(Value,"%.0lf", I2D(pReg->m_DataIn.HV) );
	fprintf(fp,"%4s|",Value);
	sprintf(Value,"%.3lf", I2D(pReg->m_DataIn.KE_Start) );
	fprintf(fp,"%9s|",Value);
	sprintf(Value,"%.3lf", I2D(pReg->m_DataIn.KE_End) );
	fprintf(fp,"%9s|",Value);
	sprintf(Value,"%.3lf", I2D(pReg->m_DataIn.Step));
	fprintf(fp,"%6s|",Value);
	sprintf(Value,"%i", pReg->m_DataIn.N_);
	fprintf(fp,"%4s|",Value);
	sprintf(Value,"%i", pReg->m_DataIn.Curr_N);
	fprintf(fp,"%4s|",Value);
	sprintf(Value,"%.3lf", I2D(pReg->m_DataIn.Time));
	fprintf(fp,"%6s|",Value);
	fprintf(fp, "%14s|", FormatTime(pReg->m_BeginTime, "%d.%m.%Y %H:%M").GetString());
	fprintf(fp, "%14s|", FormatTime(pReg->m_EndTime, "%d.%m.%Y %H:%M").GetString());
	fprintf(fp,"\n"); ++NLine;
	fprintf(fp,";%s\n",pReg->m_DataIn.Comments); ++NLine;
return NLine;
}

//---------

void SaveOrigin(FILE* fp, char* FileName)
{
CRegion* pReg;
int iL = 0;
int i;

fprintf(fp,"%c ", '-');
for(pReg=CRegion::GetFirst(); pReg!=NULL; pReg=CRegion::GetNext(pReg))
	{fprintf(fp, "R%i ", pReg->ID+1);}
fprintf(fp,"\n");

fprintf(fp,"%s ", "KE/BE");
for(pReg=CRegion::GetFirst(); pReg!=NULL; pReg=CRegion::GetNext(pReg))
	{
	if(pReg->m_DataIn.KE_BE == DATA_IN::EnergyType::KE) fprintf(fp, "%s ", "KE");
	else fprintf(fp, "%s ", "BE"); 
	}
fprintf(fp,"\n");

fprintf(fp,"%s ", "HV");
for(pReg=CRegion::GetFirst(); pReg!=NULL; pReg=CRegion::GetNext(pReg))
	{fprintf(fp, "%i ", (int) I2D(pReg->m_DataIn.HV) ); }
fprintf(fp,"\n");

char strWord[256];
for(i=0; i<3; ++i)
	{
	fprintf(fp,"%c ", '-');
	for(pReg=CRegion::GetFirst(); pReg!=NULL; pReg=CRegion::GetNext(pReg))
		{
		GetWord(pReg, strWord, i+1);
		fprintf(fp, "%s ", strWord);
		}
	fprintf(fp,"\n");
	}

char strFormat[512];
char strF[32];
char strL[512];
char strR[512];
sprintf(strF, "%c%s ", '%', ".3lf");
for(pReg=CRegion::GetFirst(), iL=0; pReg!=NULL; pReg=CRegion::GetNext(pReg), ++iL)
	{
	strL[0] = '\0';
	strR[0] = '\0';
	for(i=0; i<iL; ++i) strcat(strL, "- ");
	for(i=iL+1; i<CRegion::m_NReg ; ++i) strcat(strR, "- ");
	sprintf(strFormat, "%s%s%s%s\n", strF, strL, strF, strR);
	for(i=0; i<pReg->m_NDataOut; ++i)
		{fprintf(fp,strFormat, I2D(pReg->m_pDataOut[i].x), pReg->m_pDataOut[i].y);
		}
	}
}

void GetWord(CRegion* pReg, char* strWord, int nW)
{
int i=0;
int k=0;
int m=0;
BOOL InWord;
char* str = pReg->m_DataIn.Comments;

sprintf(strWord, "%c", '-');
while(str[i]==' ' || str[i]=='\t') ++i;
InWord = TRUE; //OutWord = FALSE;
m=1;
while(str[i] != '\0')
	{
	if(str[i]!=' ' && str[i]!='\t' ) 
		{
		InWord = TRUE;
		if(m == nW) {strWord[k] = str[i]; ++k; strWord[k] = '\0'; } 
		}
	else
		{
		if(InWord)
			{
			InWord = FALSE;
			strWord[k] = '\0';
			k=0;
			//++m;
			if(m==nW) return;
			else sprintf(strWord, "%c", '-');
			++m;
			}	
		}
	++i;
	}
}

void EmptyAllData()
{
	CRegion* pReg;
	while( (pReg=CRegion::GetFirst()) != NULL) 
	{
		delete pReg;
	}
	if(::IsWindow(theApp.m_pMainFrame->m_pRegionWnd->m_pListRegionWnd->m_hWnd))
		::SendMessage(theApp.m_pMainFrame->m_pRegionWnd->m_pListRegionWnd->m_hWnd, LVM_DELETEALLITEMS, 0, 0);

	CDxpsRegion *pDxpsReg;
	while( (pDxpsReg=CDxpsRegion::GetFirst()) != NULL) 
		delete pDxpsReg;
	CDxpsRegion::PassedCommonTime=0;
	CDxpsRegion::PassedNumberOfPoints=0;

	if(CDxpsRegion::OutData.size())
		Msg("There are still some DXPS data.");
	theApp.m_pMainFrame->m_pDxpsDlg->FillTable();
}

//________________________________________________________________________
//Записывает входные переметры всех регионов
BOOL WriteDxpsRegionsParam(FILE *fp)
{
	CDxpsRegion *pReg;
	size_t ptrCurr=0;
	fseek(fp,4, SEEK_SET);
	int a=CDxpsRegion::GetRegNumber();
	ptrCurr += sizeof(int)*fwrite(&a, sizeof(int), 1, fp);
	for(pReg=CDxpsRegion::GetFirst(); pReg!=NULL; pReg=CDxpsRegion::GetNext(pReg))
	{
		ptrCurr += sizeof(DxpsRegPar)*fwrite(&pReg->Parameters,
										sizeof(DxpsRegPar), 1, fp);
	}
	ptrCurr += sizeof(double)*fwrite(&CDxpsRegion::ScanTime, sizeof(double), 1, fp);
	ptrCurr += sizeof(double)*fwrite(&CDxpsRegion::ScanStartDateTime, sizeof(double), 1, fp);
	if(ptrCurr!=sizeof(int)+sizeof(DxpsRegPar)*CDxpsRegion::GetRegNumber()+sizeof(double))
		return FALSE;
	return TRUE;
}
//________________________________________________________________________
//Дописывает данные в конец файла, начиная с точки, на которую указывает iter.
//Версия без iter записывает все точки с позиции начала области данных.
//Кроме того, записывает прошедшее время и количество точек
BOOL WriteDxpsPoints(FILE *fp, DxpsOutList::iterator iter)
{
	DxpsOutList::iterator i;
	const int pos_PassedCommonTime = 24;

	fseek(fp,0, SEEK_END);
	for(i=iter; i!=CDxpsRegion::OutData.end(); i++)
	{
		fwrite(&(*i), sizeof(DxpsOutData), 1, fp);
	}
	fseek(fp,pos_PassedCommonTime+sizeof(DxpsRegPar)*CDxpsRegion::GetRegNumber(), SEEK_SET);
	fwrite(&CDxpsRegion::PassedCommonTime, sizeof(double), 1, fp);
	fwrite(&CDxpsRegion::PassedNumberOfPoints, sizeof(int), 1, fp);

	return TRUE;
}

//________________________________________________________________________
//Записывает все точки с позиции начала области данных.
//Кроме того, записывает прошедшее время и количество точек
BOOL WriteDxpsPoints(FILE *fp)
{
	DxpsOutList::iterator i;

	size_t Pos;
	const int pos_PassedCommonTime = 24;
	Pos=pos_PassedCommonTime+sizeof(CDxpsRegion::PassedCommonTime)+sizeof(CDxpsRegion::PassedNumberOfPoints)+
		sizeof(DxpsRegPar)*CDxpsRegion::GetRegNumber();
	fseek(fp,Pos, SEEK_SET); // Начало блока измеренных данных
	for(i=CDxpsRegion::OutData.begin(); i!=CDxpsRegion::OutData.end(); i++)
	{
		if((i->RegionN>CDxpsRegion::GetRegNumber()-1) || (i->RegionN<0))
		{
			Msg("Out data of incorrect region %i", i->RegionN);
			return FALSE;
		}
		Pos+=sizeof(DxpsOutData)*fwrite(&(*i), sizeof(DxpsOutData), 1, fp);
	}
	Pos=pos_PassedCommonTime+sizeof(DxpsRegPar)*CDxpsRegion::GetRegNumber();
	if(fseek(fp,Pos, SEEK_SET))
		Msg("fseek fails");
	Pos+=sizeof(double)*fwrite(&CDxpsRegion::PassedCommonTime, sizeof(double), 1, fp);
	if(CDxpsRegion::OutData.size()!=CDxpsRegion::PassedNumberOfPoints)
		Msg("WriteDxpsPoints: Incorrect number of points.\nOutData.size()=%i, CDxpsRegion::PassedNumberOfPoints=%i",CDxpsRegion::OutData.size(), CDxpsRegion::PassedNumberOfPoints); 
	Pos+=sizeof(int)*fwrite(&CDxpsRegion::PassedNumberOfPoints, sizeof(int), 1, fp);

return TRUE;
}

void ParseXPSFile(FILE *fp)
{
	int i=0,j, pointer;
	int	NumberOfRegions;
	DATA_IN m_DataIn;
	int		m_NDataOutCurr;
	int		m_NDataOut;
	DATA_OUT DataOut;
	int StartOfDataOut;
	int EstimatedEndOfDataOut;
	int FoundEndOfDataOut;
	char ModuleName[MAX_PATH];
	::GetModuleFileName(NULL, ModuleName, MAX_PATH);
	CString cstr=ModuleName, cstr2;
	int EndPath=cstr.ReverseFind('\\');
	cstr.Delete(EndPath+1,cstr.GetLength()-EndPath-1);
	cstr2=cstr+"ParseXPS.txt";

	FILE* fpOutput=fopen((LPCSTR)cstr2,"wt");
	fseek(fp, 4, SEEK_SET);
	fread(&NumberOfRegions, sizeof(int), 1, fp);
	fprintf(fpOutput, "0x%.4x - NumberOfRegions=%i", 4, NumberOfRegions);
	do
	{ 
		fseek(fp, i, SEEK_SET);
		fread(&pointer, sizeof(int), 1, fp);
		if(pointer==i+4)
		{
			fprintf(fpOutput, "\n0x%.4x - ptrCurr\n", i);
			fseek(fp, pointer, SEEK_SET);
			j=sizeof(DATA_IN)*fread(&m_DataIn, sizeof(DATA_IN), 1, fp);
			fprintf(fpOutput, "0x%.4x - m_DataIn:\n", pointer);
			fprintf(fpOutput, " HV=%.3f\n", m_DataIn.HV/1000.);
			fprintf(fpOutput, " KE_Start=%.3f\n", m_DataIn.KE_Start/1000.);
			fprintf(fpOutput, " KE_End=%.3f\n", m_DataIn.KE_End/1000.);
			fprintf(fpOutput, " Step=%.3f\n", m_DataIn.Step/1000.);
			fprintf(fpOutput, " Time=%.3f\n", m_DataIn.Time/1000.);
			fprintf(fpOutput, " N_=%i\n", m_DataIn.N_);
			fprintf(fpOutput, " Curr_N=%i\n", m_DataIn.Curr_N);
			fprintf(fpOutput, " Off=%i\n", m_DataIn.Off);
			fprintf(fpOutput, " KE_BE=%i\n", m_DataIn.KE_BE);
			fprintf(fpOutput, " N_h_nu=%i\n", (int)m_DataIn.N_h_nu);
			fprintf(fpOutput, " DeltaVolts=%.3f\n", m_DataIn.DeltaVolts/1000.);
			fprintf(fpOutput, " Comments=%s\n", m_DataIn.Comments);
			pointer+=j;
			fseek(fp, pointer, SEEK_SET);
			j=sizeof(int)*fread(&m_NDataOutCurr, sizeof(int), 1, fp);
			fprintf(fpOutput, "0x%.4x - m_NDataOutCurr=%i\n", pointer, m_NDataOutCurr);
			pointer+=j;
			fseek(fp, pointer, SEEK_SET);
			j=sizeof(int)*fread(&m_NDataOut, sizeof(int), 1, fp);
			fprintf(fpOutput, "0x%.4x - m_NDataOut=%i\n", pointer, m_NDataOut);
			pointer+=j;
			StartOfDataOut=pointer;
			fprintf(fpOutput, "0x%.4x - (*m_pDataOut)[%i]\n", StartOfDataOut, m_NDataOut);
			EstimatedEndOfDataOut=StartOfDataOut+sizeof(DATA_OUT)*m_NDataOut;
			fprintf(fpOutput, "Estimated end of DataOut: 0x%.4x\n", EstimatedEndOfDataOut);
			fseek(fp, EstimatedEndOfDataOut, SEEK_SET);
			j=sizeof(int)*fread(&pointer, sizeof(int), 1, fp);
			if((pointer!=EstimatedEndOfDataOut+4)&&j>0) //Структура файла нарушена. Выводим весь диапазон как DataOut.
			{
			fprintf(fpOutput, "  Структура файла нарушена\n");
				fseek(fp, StartOfDataOut, SEEK_SET); //Обратно к началу
				j=StartOfDataOut;
				while(1) //Ищем следующий регион
				{ 
					fseek(fp,  j, SEEK_SET);
					fread(&pointer, sizeof(int), 1, fp);
					if((pointer==j+4) || feof(fp))
					{
						FoundEndOfDataOut=j;
						fprintf(fpOutput, "  Найден конец DataOut: 0x%.4x\n", FoundEndOfDataOut);
						break;
					}
					j++;
				}
				
				//Выводим диапазон до найденного конца как DataOut
				int k=0;
				for(j=StartOfDataOut; j<FoundEndOfDataOut; j+=sizeof(DATA_OUT))
				{
					k++;
					fseek(fp,  j, SEEK_SET);
					fread(&DataOut, sizeof(int), 1, fp);
					fprintf(fpOutput, "  %3i: 0x%.4x %.3f, %.3f\n", k, j, DataOut.x/1000., DataOut.y/1000.);
				}

			}
		}
		i++;
	}while(!feof(fp));
	fclose(fpOutput);
}