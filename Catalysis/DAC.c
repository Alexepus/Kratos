#include "Main.h"
int DacData[16];

void LoadDac(BYTE Channel, UINT Val)
{
	BYTE Byte;
	DAC_CS=0;
	SSPBUF=Channel;
	Byte=*((BYTE*)&Val+1)|0xC0;
	BF=0;
	while(!BF);
	SSPBUF=Byte;
	BF=0;
	while(!BF);
	SSPBUF=*((BYTE*)&Val);
	BF=0;
	while(!BF);
	DAC_CS=1;
}

void ConfigDac()
{
	DAC_CS=0;
	SSPBUF=0x0C; //Control Reg
	BF=0;
	while(!BF);
	SSPBUF=0x1D; //15..14:00 - Control Reg; 13:0 - PD Amp Out; 12:1 - 2.5V;
	//11:1 - Boost Mode On; 10:1 - Int Ref; 9:0 - Monitor Out Z; 8:1 - Thermo Mon On
	BF=0;
	while(!BF);
	SSPBUF=0x00;
	BF=0;
	while(!BF);
	DAC_CS=1;
}
