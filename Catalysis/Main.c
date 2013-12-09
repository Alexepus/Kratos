#define OS_MAIN
#include "Main.h"

__CONFIG(1, OSCSDIS & HSPLL);
__CONFIG(2, PWRTEN & BORV42 & BOREN  & 0xFFFD & WDTEN); //& 0xFFFD==WDTPS64
__CONFIG(4, DEBUGDIS & LVPDIS & STVREN);
 
OST_TASK task0;
OST_TASK task1;
OST_SEMAPHORE UartRcMsgSem;
extern FIFO TxFifo;
extern FIFO RcFifo;
UINT DacUsageFlags; //Какие каналы загружать

//________________________________________________________________________________
int main(void)
{
char i;

	ConfigDevice();
#ifndef SIMULATION
	ConfigDac();
#endif
	FillTableT();
	FillTableInvDV();
	OS_Init();
	OS_Task_Create(ProcessUartMsg, &task0);
	OS_Task_Create(Pid, &task1);
	OS_Sem_Create(&UartRcMsgSem,0);

	PR2     = OS_TMR_TICK;  // timer tick = 1000 instruction cycles=0.1 ms
	T2CON   = 0x05;
	ReportStartUp();
	while (1) 
		{
		if(TMR1IF)
		{
			TMR1IF=0;
			OS_Task_Resume(&task1);
		}
		asm("CLRWDT");
		OS_Scheduler();
		}
}

//________________________________________________________________________________
void ConfigDevice()
{
//Инициализация портов
TRISA=0x2F;//N/A:0 CLKO:O AN4:1 DAC_LD:0 AN3:1 AN2:1 AN1:1 AN0:1
TRISB=0xEB;//PGD:1 PGC:1 PGM:1 RB4:0 CANRX:1 CANTX:0 DAC_BSY:1 ADC_BSY:1
TRISC=0x90;//UART_RX:1 UART_TX:0 SDO:0 SDI:1 SCK:0 DAC_SYN:0 ADC_SYN:0 ADC_CNV:0 
TRISD=0x00;//RD7..RD0:0
TRISE=0x07;//IBF:0 OBF:0 IBOV:0 PSPMODE:0 N/A:0 AN7:1 AN6:1 AN5:1
 
//Конфигурирование прерываний
INTCON=0xC0;//GIE:1 PEIE:1 TMR0IE:0 INT0IE:0 RBIE:0 TMR0IF:0 INT0IF:0 RBIF:0
PIE1=0x22;//PSPIE:0 ADIE:0 RCIE:1 TXIE:0 SSPIE:0 CCP1IE:0 TMR2IE:1 TMR1IE:0
PIE2=0x00;//N/A:0 CMIE:0 N/A:0 EEIE:0 BCLIE:0 LVDIE:0 TMR3IE:0 ECCP1IE:0
PIE3=0x00;//CAN

//Конфигурирование USART
TXSTA=0x24;//CSRC:0 TX9:0 TXEN:1 SYNC:0 N/A:0 BRGH:1 TRMT:0 TX9D:0
RCSTA=0x90;//SPEN:1 RX9:0 SREN:0 CREN:1 ADDEN:0 FERR:0 OERR:0 RX9D:0
SPBRG=42;//42=>58139 (+0.93%) 43=>56818 (-1.36%) 20=>119047 (+3.3%) 21=>113636 (-1.36%)

//Конфигурирование таймеров
T1CON=0b10010001;//RD16:1 None:0 T1CKPS:01 (/2) T1OSCEN:0 T1SYNC:0 TMR1CS:0 (internal clk) TMR1ON:1
TMR1H=(0xFFFF-10000)>>8;
TMR1L=0xFFFF-10000;

//Конфигурирование SPI
SSPSTAT=0xC0;//SMP:1 CKE:1 D/C:0 D/C:0 D/C:0 D/C:0 D/C:0 BF:0
SSPCON1=0x30;//WCOL:0 SSPOV:0 SSPEN:1 CKP:1 SSPM3..SSPM0:0000 (SPI Master, Fosc/4)

//Начальные состояния выводов
ADC_CS=1;
ADC_Conv=1;
DAC_CS=1;
DAC_LD=0;

//Начальные значения данных
FlagWait=0;
FlagMainOff=1;
FlagRefTempAchived=0;
}

static volatile near unsigned int FSR0		@ 0xFE9;
static volatile near unsigned int FSR1		@ 0xFE1;

void memmove_opt(void * pDest, const void * pSrc, register unsigned char n)
{
    UINT ShadowFsr0, ShadowFsr1;
    if(!n)
    	return;
    ShadowFsr0=FSR0;
    ShadowFsr1=FSR1;
    FSR0=(UINT)pSrc;
    FSR1=(UINT)pDest;
    //while(n--)
    	//asm("MOVFF _POSTINC0, _POSTINC1");
    WREG=n;
    #asm
	
        movff _POSTINC0, _POSTINC1;
		decfsz _WREG, W, c
        bra $-6
    #endasm
    FSR0=ShadowFsr0;
    FSR1=ShadowFsr1;
}

int linterp(int A, int ALeft, int ARight, int BLeft, int BRight)
{
	BYTE Count;
	int NewVal;
	for(Count=0; Count<9; Count++)
	{
		NewVal=(ALeft+ARight)>>1;
		if(A<NewVal)
		{
			ARight=NewVal;
			BRight=(BLeft+BRight)>>1;
		}
		else
		{
			ALeft=NewVal;
			BLeft=(BLeft+BRight)>>1;
		}
	}
	return BLeft;
}
