#include <htc.h>
#include "jacnfg.h"
#include <jacos.h>

//#define SIMULATION
typedef unsigned int UINT;
typedef unsigned char BYTE;
typedef unsigned long ULONG;  
typedef int INT16Q4;
typedef int INT16Q8;
typedef int INT16Q16;
typedef long INT32Q20;
typedef struct ULONG_8_16_8
{
	BYTE HByte;
	UINT MInt;
	BYTE LByte;
}

typedef union
{
	ULONG AsUlong;
	UINT AsUint[2];
	BYTE AsByte[4];
	struct
	{
		BYTE HByte;
		UINT MInt;
		BYTE LByte;
	} AsStruct;
} ULONG_UNION;

#define SPI_WAIT_COUNT 8
//#define SPI_WAIT() {
#define ADC_CS RC1 
#define ADC_Busy RB0 
#define ADC_Conv RC0
#define DAC_LD RA4
#define DAC_CS RC2

#include "UART.h"
#include "ADC8x14.h"
#include "DAC.h"
#include "ProcessUartMsg.h"
#include "PID.h"
#include "PowerLimiter.h"

void ConfigDevice(void);
interrupt void HiInterrupt(void);
void ProcessUartMsg(void);
void memmove_opt(void * pDest, const void * pSrc, register unsigned char n);
int linterp(int A, int ALeft, int ARight, int BLeft, int BRight);
