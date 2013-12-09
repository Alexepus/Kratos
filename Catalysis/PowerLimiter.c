#include "Main.h"


UINT MaxHeaterISoft;

UINT MaxHeaterI;
UINT MaxHeaterU;
UINT MaxHeaterP;
UINT HeaterP;
ULONG KHeaterIToRef = 0;
ULONG KHeaterIToRef_Medium = 0x0000007f;
UINT DacData0Max = 0xffff;

BYTE MaxHeaterISoftCount = 0;
BYTE MaxHeaterIGuard = 0;
BYTE MaxHeaterUGuard = 0;
BYTE MaxHeaterPGuard = 0;

void Limiter(void)
{
	ULONG_UNION Tmp;

	if(ADC8x14DataAveraged[1] > MaxHeaterI)
	{ // Превышение ограничения по току
		MaxHeaterIGuard = 1;
	}

	if( (DacData[0] > DacData0Max) && (ADC8x14DataAveraged[1] > (MaxHeaterISoft >> 1)) )
	{ // Превышение по мягкому ограничения тока
		MaxHeaterISoftCount = 100;
		DacData[0] = DacData0Max;
	}
	else
	{
		if(MaxHeaterISoftCount != 0)
			--MaxHeaterISoftCount;
	}

	Tmp.AsStruct.MInt = DacData[0];
	Tmp.AsStruct.HByte = 0;
	
	if(ADC8x14DataAveraged[1] != 0)
		KHeaterIToRef = Tmp.AsUlong/(ULONG)ADC8x14DataAveraged[1]; //KHeaterIToRef = DacData[0]/ADC8x14DataAveraged[1]
	else
		KHeaterIToRef = 0xffffffff;
	
	Tmp.AsUlong = KHeaterIToRef_Medium*244 + KHeaterIToRef*12;

	KHeaterIToRef_Medium = Tmp.AsStruct.MInt; // x/256
	MulIShift8(MaxHeaterISoft, KHeaterIToRef_Medium, DacData0Max); //DacData0Max = MaxHeaterISoft*KHeaterIToRef_Medium

	if(ADC8x14DataAveraged[2] > MaxHeaterU)
	{ // Превышение ограничения по напряжению
		MaxHeaterUGuard = 1;
	}

	MulIShift16(ADC8x14DataAveraged[1], ADC8x14DataAveraged[2], HeaterP);
	if(HeaterP > MaxHeaterP)
	{ // Превышение ограничения по мощности
		MaxHeaterPGuard = 1;
	}
	
	if(MaxHeaterIGuard | MaxHeaterUGuard | MaxHeaterPGuard)
		DacData[0] = 0x0000;
}
