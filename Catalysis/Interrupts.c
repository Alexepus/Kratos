#include "Main.h"

//________________________________________________________________________________
#pragma interrupt_level 0
interrupt void HiInterrupt(void)
{
	//Обработка событий USART
	if(RCIF)
		UartRcInt();
	if(TXIF&TXIE)
		UartTxInt();

	//Обработка таймера ОС
	if ( OS_Is_Timer())
	{
		OS_Timer_Tick_Set();
		OS_Timer();
	}
}


