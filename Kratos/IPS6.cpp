// IPS6.h/IPS6.cpp реализуют интерфейс, подобный VCMCD(M).H за единственным исключением: 
// драйвер надо открывать и закрывать 
//#ifdef IPS6
#include "stdafx.h"
#include "ProgNew.h"
#include "IPS6.h"
#ifndef _DEBUG
#pragma comment( lib, "opsli.lib" )
#else
#pragma comment( lib, "opslid.lib" )
#endif

#define FORCE_CHECKS // Дополнительные проверки на правильность параметров
//#define SHOW_HW_ERRORS // Проверяет возвращаемые значения функций на предмет ошибок, о чем выводит сообщение

#define MIN_CARDS		1
#define MAX_CARDS		8
#define MIN_CRATES		1
#define MAX_CRATES		6
#define CRATES_COUNT	(MAX_CRATES-MIN_CRATES+1)
#define MIN_POS			0
#define MAX_POS			23
#define MIN_A			0
#define MAX_A			15
#define MIN_F			0
#define MAX_F			31

#ifdef FORCE_CHECKS
#define CheckParam( c, n, a, f) \
	if( c < 0 || c >= CRATES_COUNT)\
		Msg("Неверный номер канала: %i.", c);\
	if( n < 0 || n > MAX_POS)\
		Msg("Неверный номер блока: %i.", n);\
	if( a < 0 || a > MAX_A)\
		Msg("Неверный субадрес: %i.", a);\
	if( f < 0 || f > MAX_F)\
		Msg("Неверный номер функции: %i.", f);
#else
#define CheckParam( c, n, a, f)
#endif

int CamacChannels[CRATES_COUNT] = {-1,-1,-1,-1,-1,-1};
WORD ReturnCode = 0;

int Camac_Open(int c)
{
	#ifdef FORCE_CHECKS
	if( c < 0 || c >= CRATES_COUNT)
	{
		Msg("Camac_Open: Неверный номер канала: %i.", c);
		throw CString("Camac_Open: Неверный номер канала");
		return -1;
	}
	if(CamacChannels[c] != -1)
	{
		Msg("Camac_Open: Указанный канал %i канал уже открыт." , c);
		return -1;
	}
	#endif
	for(int cardN = MIN_CARDS; cardN < MAX_CARDS; cardN++ )
	{
		CamacChannels[c] = OpenCrate(cardN, c + MIN_CRATES);
		if(CamacChannels[c] != -1)
			break;
	}
	return CamacChannels[c];
}
void Camac_Close(int c)
{
	#ifdef FORCE_CHECKS
	if( c < 0 || c >= CRATES_COUNT)
	{
		Msg("Camac_Close: Неверный номер канала: %i.", c);
		throw CString("CamacClose: Неверный номер канала");
		return;
	}
	if(CamacChannels[c] == -1)
	{
		Msg("Camac_Close: Указанный канал %i канал уже закрыт." , c);
		return;
	}
	#endif

	#ifdef SHOW_HW_ERRORS
	if(CloseCrate(CamacChannels[c]) == -1)
		Msg("Camac_Close: Ошибка закрытия канала КАМАК.");
	#else
	CloseCrate(CamacChannels[c]);
	#endif
	CamacChannels[c] = -1;
}
void Camac_CloseAll()
{
	for (int c = 0; c < CRATES_COUNT; c++)
	{
		if(CamacChannels[c] != -1)
		{
			#ifdef SHOW_HW_ERRORS
			if(CloseCrate(CamacChannels[c]) == -1)
				Msg("Camac_CloseAll: Ошибка закрытия канала КАМАК.");
			#else
			CloseCrate(CamacChannels[c]);
			#endif
			CamacChannels[c] = -1;
		}
	}
}

long Camac_ReadLong(int c, int n, int a, int f)
{
	CheckParam( c, n, a, f);
	#ifdef FORCE_CHECKS
	if(CamacChannels[c] == -1)
	{
		Msg("Camac_ReadLong: Указанный канал %i канал не был открыт." , c);
		throw CString("Camac_ReadLong: Указанный канал канал не был открыт.");
		return 0;
	}
	#endif
	long val;
	ReturnCode = cam24(CamacChannels[c], n, a, f, (DWORD *)&val);

	#ifdef SHOW_HW_ERRORS
	if(Camac_ErrorCode())
		Msg("Ошибка в функции Camac_ReadLong: %s", Camac_ErrorMessage());
	#endif
	return val;
}

void Camac_WriteLong(int c, int n, int a, int f, long d)
{
	CheckParam( c, n, a, f);
	#ifdef FORCE_CHECKS
	if(CamacChannels[c] == -1)
	{
		Msg("Camac_WriteLong: Указанный канал %i канал не был открыт." , c);
		throw CString("Camac_WriteLong: Указанный канал канал не был открыт.");
		return;
	}
	#endif
	ReturnCode = cam24(CamacChannels[c], n, a, f, (DWORD *)&d);

	#ifdef SHOW_HW_ERRORS
	if(Camac_ErrorCode())
		Msg("Ошибка в функции Camac_WriteLong: %s", Camac_ErrorMessage());
	#endif
}

WORD Camac_ReadWord(int c, int n, int a, int f)
{
	CheckParam( c, n, a, f);
	#ifdef FORCE_CHECKS
	if(CamacChannels[c] == -1)
	{
		Msg("Camac_ReadWord: Указанный канал %i канал не был открыт." , c);
		throw CString("Camac_ReadWord: Указанный канал канал не был открыт.");
		return 0;
	}
	#endif
	WORD val;
	ReturnCode = cam16(CamacChannels[c], n, a, f, &val);

	#ifdef SHOW_HW_ERRORS
	if(Camac_ErrorCode())
		Msg("Ошибка в функции Camac_ReadWord: %s", Camac_ErrorMessage());
	#endif
	return val;
}

void Camac_WriteWord(int c, int n, int a, int f, unsigned short d)
{
	CheckParam( c, n, a, f);
	#ifdef FORCE_CHECKS
	if(CamacChannels[c] == -1)
	{
		Msg("Camac_WriteWord: Указанный канал %i канал не был открыт." , c);
		throw CString("Camac_WriteWord: Указанный канал канал не был открыт.");
		return;
	}
	#endif
	ReturnCode = cam16(CamacChannels[c], n, a, f, &d);

	#ifdef SHOW_HW_ERRORS
	if(Camac_ErrorCode())
		Msg("Ошибка в функции Camac_WriteWord: %s", Camac_ErrorMessage());
	#endif
}

void Camac_Reset(int c)
{
	#ifdef FORCE_CHECKS
	if( c < 0 || c > MAX_CRATES-MIN_CRATES)
	{
		Msg("Camac_Reset: Неверный номер канала: %i.", c);
		throw CString("Camac_Reset: Неверный номер канала.");
		return;
	}
	if(CamacChannels[c] == -1)
	{
		Msg("Camac_Reset: Указанный канал %i канал не был открыт." , c);
		throw CString("Camac_Reset: Указанный канал канал не был открыт.");
		return;
	}
	#endif

	//ReturnCode = camZ(CamacChannels[c]);
	Camac_WriteWord(c, 0, 0, 16, 0); // Сброс Inhibit
	#ifdef SHOW_HW_ERRORS
	if(Camac_ErrorCode())
		Msg("Ошибка в функции Camac_Reset: %s", Camac_ErrorMessage());
	#endif
}

WORD Camac_Version(void){return 2;};

int Camac_x(void)
{
	return ((ReturnCode & 0x80) == 0);
}
int Camac_q(void)
{
	return ((ReturnCode & 0x40) == 0);
}
char* Camac_ErrorMessage(void)
{
	return SLI6_ErrorString;
}
int Camac_ErrorCode(void)
{
	int err = 0;
	//if( (ReturnCode & 0x20) != 0 ) // Er - ошибка четности
		//err |= 1;
	if( (ReturnCode & 0x10) != 0 ) // To - отсутствие связи с К0607
		err |= 2;
	return err;
}
void Camac_EnableMessage(int State){};

//#endif