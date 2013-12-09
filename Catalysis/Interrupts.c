#include "Main.h"

//________________________________________________________________________________
#pragma interrupt_level 0
interrupt void HiInterrupt(void)
{
	//��������� ������� USART
	if(RCIF)
		UartRcInt();
	if(TXIF&TXIE)
		UartTxInt();

	//��������� ������� ��
	if ( OS_Is_Timer())
	{
		OS_Timer_Tick_Set();
		OS_Timer();
	}
}


