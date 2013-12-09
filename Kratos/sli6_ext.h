//////////////////////////////////////////////////////////////////////
// File - sli6_ext.h
//
// Library for accessing the SLI6 card, version 1.10.
// Code was composed by Kirill Bezmaternykh 
// INP, lab 6-1 (ppi6@gorodok.net)
// It accesses the hardware via WinDriver functions.
// 
//////////////////////////////////////////////////////////////////////

#ifndef _SLI6_EXT_H_
#define _SLI6_EXT_H_

#ifdef __cplusplus
extern "C" {
#endif


#define crate int

////////////// внешние функции LIB /////////////////////////////////////

crate	__stdcall		OpenCrate(unsigned int CardN, unsigned int CrateN);
int	__stdcall		CloseCrate(int c);
	// CardN - номер карточки в компе по порядку считая не от проца (MIN_CARDS - MAX_CARDS)
	// CrateN - номер канал в карточке ( MIN_CRATES - MAX_CRATES )
	// возвращает CN - идентификатор канала

WORD	__stdcall		cam16write(crate c, int N, int A, WORD *data);
WORD	__stdcall		cam24write(crate c, int N, int A, DWORD *data);
WORD	__stdcall		cam16read(crate c, int N, int A, WORD *data);
WORD	__stdcall		cam24read(crate c, int N, int A, DWORD *data);
WORD	__stdcall		cam16(crate c, int N, int A, int F, WORD *data);
WORD	__stdcall		cam24(crate c, int N, int A, int F, DWORD *data);

WORD	__stdcall		cam16Block(crate c, int N, int A, int F,unsigned long Nblock, WORD *data);
WORD	__stdcall		cam24Block(crate c, int N, int A, int F, unsigned int Nblock, DWORD *data);
WORD	__stdcall		camZ(crate c);
WORD	__stdcall		camC(crate c);

// функции возвращают статусный регистр соответствующего канала в формате
// XQToEr

int	__stdcall		ClearStatus(crate c);			 // стирает накопленный статус
int	__stdcall		GetChannels(unsigned int CardN); // возвращает количество каналов

	// дополнительные фукнции для тестирования платы на скорость(без проверки параметров)
WORD	__stdcall		cam16RW(crate c, int N, int A, WORD *dataw,WORD *datar);  // чтение и запись 16
WORD	__stdcall		cam24RW(crate c, int N, int A, DWORD *dataw,DWORD *datar);// чтение и запись 24
	// возвращают два статуса  |statusR|statusW|


	// прерывания
typedef void (*INTLAMFUNC)(crate c, PVOID param); // определение функции обработчика

int	__stdcall		SetIntOnLAMfunc(crate c, INTLAMFUNC _intf);
int	__stdcall		LAMIntEnable(crate c, PVOID param);
int	__stdcall		LAMIntDisable(crate c);
int	__stdcall		LAMIntIsEnabled(crate c);

	// ошибки 
void	__stdcall		GetErrStr(char* str);

// this string is set to an error message, if one occurs
extern char SLI6_ErrorString[];


#ifdef __cplusplus
}
#endif

#endif