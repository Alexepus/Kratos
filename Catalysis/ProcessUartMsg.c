#include "Main.h"
#include "ProcessUartMsg.h"
#include "string.h"

FIFO TxFifo;
extern MessageBuffer RcBuf;
extern MessageBuffer TxBuf;

//Task 0. Обработчик сообщений UART
void ProcessUartMsg(void)
{
	static BYTE Cmd;
	while (1) 
	{
		OS_Wait_Sem(&UartRcMsgSem);
		if(ExtractRcMessage() == 0)
			continue;
		Cmd=RcBuf.Data[0];
//#pragma switch direct
		switch(Cmd)
		{
		//_________________________________
		case CMD_GET_EXT_ADC: //Запрос канала внешнего АЦП
		{
			BYTE *pSrc, *pDest;
			pDest=TxBuf.Data;
			pSrc=(BYTE*)ADC8x14Data+(RcBuf.Data[1]<<1);
			*pDest++=CMD_GET_EXT_ADC;
			*pDest++=RcBuf.Data[1];
			*pDest++=*pSrc++;
			*pDest=*pSrc;
			TxBuf.Length=4;
			UartStartTx();
			break;		
		}
		case CMD_LOAD_DAC: //Загрузка значения в ЦАП
		{							//Формат:Номер канала, DataL, DataH
			BYTE *pSrc, *pDest;
			pSrc=&RcBuf.Data[1];
			pDest=(BYTE*)(DacData+ *pSrc++);
			*pDest++=*pSrc++;			
			*pDest--=*pSrc;
			LoadDac(RcBuf.Data[1],*(UINT*)pDest);
			TxBuf.Data[0]=CMD_LOAD_DAC;
			TxBuf.Length=1;
			UartStartTx();
			break;			
		}
		case CMD_GET_AVERAGED_ADC: //Запрос усредненного значения АЦП
		{
			BYTE *pSrc=(BYTE*)ADC8x14DataAveraged+(RcBuf.Data[1]<<1);
			TxBuf.Data[0]=CMD_GET_AVERAGED_ADC;
			TxBuf.Data[1]=RcBuf.Data[1];
			TxBuf.Data[2]=*pSrc++;
			TxBuf.Data[3]=*pSrc;
			TxBuf.Length=4;
			UartStartTx();
			break;		
		}
		case CMD_LOAD_PIDCOEFS: //Загрузка новых PID-коэффициентов
		{
			BYTE *pSrc, *pDest;
			pSrc=RcBuf.Data+1;
			pDest=(BYTE*)&Kp;
			*pDest++=*pSrc++;
			*pDest=*pSrc++;
			pDest=(BYTE*)&Ki;
			*pDest++=*pSrc++;
			*pDest=*pSrc++;
			pDest=(BYTE*)&Kd;
			*pDest++=*pSrc++;
			*pDest++=*pSrc++;
			pDest=(BYTE*)&Kff;
			*pDest++=*pSrc++;
			*pDest=*pSrc;
			TxBuf.Data[0]=CMD_LOAD_PIDCOEFS;
			TxBuf.Length=1;
			UartStartTx();			
			break;		
		}
		case CMD_GET_PIDVALS: //Отправка значений PID-регулятора
		{
			BYTE *pDest;
			pDest=TxBuf.Data;
			*pDest++=CMD_GET_PIDVALS;
			*pDest++=*((BYTE*)&Vp);
			*pDest++=*((BYTE*)&Vp+1);
			*pDest++=*((BYTE*)&Vi);
			*pDest++=*((BYTE*)&Vi+1);
			*pDest++=*((BYTE*)&Vd);
			*pDest++=*((BYTE*)&Vd+1);
			*pDest++=*((BYTE*)&PidOut);
			*pDest++=*((BYTE*)&PidOut+1);
			*pDest++=*((BYTE*)&Vff);
			*pDest++=*((BYTE*)&Vff+1);
			*pDest++=*((BYTE*)&Vout);
			*pDest=*((BYTE*)&Vout+1);
			TxBuf.Length=13;
			UartStartTx();			
			break;		
		}
		case CMD_LOAD_TCOUPLE_TABLE: //Загрузка таблицы термопары
		{
			memmove_opt(((BYTE*)TableV)+RcBuf.Data[1]*16, RcBuf.Data+2, 16);

			if(RcBuf.Data[1]==3)
			{
				FillTableInvDV();
				TableVLoaded=1;
			}
			TxBuf.Data[0]=CMD_LOAD_TCOUPLE_TABLE;
			TxBuf.Length=1;
			UartStartTx();			
			break;		
		}
		case CMD_LOAD_T0_DT: //Загрузка начальной температуры и dT таблицы термопары
		{
			BYTE *pSrc, *pDest;
			pSrc=RcBuf.Data+1;
			pDest=(BYTE*)TableT;
			*pDest++=*pSrc++;
			*pDest=*pSrc++;
			pDest=(BYTE*)&TableDT;
			*pDest=*pSrc;
			FillTableT();
			TdTLoaded=1;
			TxBuf.Data[0]=CMD_LOAD_T0_DT;
			TxBuf.Length=1;
			UartStartTx();			
			break;		
		}
		case CMD_GET_T_BY_V: //Вычисление и отправка значения температуры по полученному напряжению
		{
			INT16Q8 mV;
			INT16Q4 T;
			BYTE *pSrc, *pDest;
			TxBuf.Data[0]=CMD_GET_T_BY_V;
			pSrc=RcBuf.Data+1;
			pDest=(BYTE*)&mV;
			*pDest++=*pSrc++;
			*pDest=*pSrc;
			
			T=CalcTByV(mV);
			pSrc=(BYTE*)&T;
			pDest=TxBuf.Data+1;
			*pDest++=*pSrc++;
			*pDest=*pSrc;
			TxBuf.Length=3;
			UartStartTx();			
			break;		
		}
		case CMD_GET_ADC_TC_I_U_TREF:
		{	
			BYTE *pDest=TxBuf.Data;
			*pDest++=CMD_GET_ADC_TC_I_U_TREF;
			*pDest++=*(BYTE*)&ADC8x14DataAveraged[0];
			*pDest++=*((BYTE*)&ADC8x14DataAveraged[0]+1);
			*pDest++=*(BYTE*)&ADC8x14DataAveraged[1];
			*pDest++=*((BYTE*)&ADC8x14DataAveraged[1]+1);
			*pDest++=*(BYTE*)&ADC8x14DataAveraged[2];
			*pDest++=*((BYTE*)&ADC8x14DataAveraged[2]+1);
			*pDest++=*(BYTE*)&TRef;
			*pDest=*((BYTE*)&TRef+1);
			TxBuf.Length=9;
			UartStartTx();			
			break;		
		}
		case CMD_START_TEMP_CHANGE:
		{
			BYTE *pSrc;
			pSrc=&RcBuf.Data[1];
			*((BYTE*)&TEnd)=*pSrc++;
			*((BYTE*)&TEnd+1)=*pSrc++;
			*((BYTE*)&DeltaT)=*pSrc++;
			*((BYTE*)&DeltaT+1)=*pSrc++;
			*((BYTE*)&DeltaT+2)=*pSrc++;
			*((BYTE*)&DeltaT+3)=*pSrc;
			FlagMainOff=0;
			TxBuf.Data[0]=CMD_START_TEMP_CHANGE;
			TxBuf.Length=1;
			UartStartTx();
			break;		
		}
		case CMD_LOAD_PIDPARAMS: //Загрузка параметров PID-компонентов
		{
			BYTE *pSrc;
			pSrc=&RcBuf.Data[1];
			DifShift=*pSrc++;
			IDec=*pSrc++;
			*((BYTE*)&ViMax)=*pSrc++;
			*((BYTE*)&ViMax+1)=*pSrc++;
			*((BYTE*)&ViOff)=*pSrc++;
			*((BYTE*)&ViOff+1)=*pSrc;			
			TxBuf.Data[0]=CMD_LOAD_PIDPARAMS;
			TxBuf.Length=1;
			UartStartTx();			
			break;		
		}
		case CMD_WAIT_CONTINUE_OFF: //Приостановка, продолжение, выключение PID
		{
			if(RcBuf.Data[1]==VAL_WAIT)
			{
				FlagWait=1;
			}
			else if(RcBuf.Data[1]==VAL_CONTINUE)
			{
				FlagWait=0;
			}
			else if(RcBuf.Data[1]==VAL_MAIN_OFF)
			{
				FlagMainOff=1;
				TRef=0;
				*((INT16Q4*)&TAccumulator+1)=0;

			}
			TxBuf.Data[0]=CMD_WAIT_CONTINUE_OFF;
			TxBuf.Length=1;
			UartStartTx();
			ManualHeaterControl=0;
			break;		
		}
		case CMD_LOAD_FF_TABLE: //Загрузка таблицы Feed-Forward
		{
			BYTE *pSrc;
			pSrc=&RcBuf.Data[1];
			*((BYTE*)FFTableTemp+0)=*pSrc++;
			*((BYTE*)FFTableTemp+1)=*pSrc++;
			*((BYTE*)FFTableTemp+2)=*pSrc++;
			*((BYTE*)FFTableTemp+3)=*pSrc++;
			*((BYTE*)FFTableTemp+4)=*pSrc++;
			*((BYTE*)FFTableTemp+5)=*pSrc++;
			*((BYTE*)FFTableTemp+6)=*pSrc++;
			*((BYTE*)FFTableTemp+7)=*pSrc++;
			*((BYTE*)FFTableOut+0)=*pSrc++;
			*((BYTE*)FFTableOut+1)=*pSrc++;
			*((BYTE*)FFTableOut+2)=*pSrc++;
			*((BYTE*)FFTableOut+3)=*pSrc++;
			*((BYTE*)FFTableOut+4)=*pSrc++;
			*((BYTE*)FFTableOut+5)=*pSrc++;
			*((BYTE*)FFTableOut+6)=*pSrc++;
			*((BYTE*)FFTableOut+7)=*pSrc;
			TxBuf.Data[0]=CMD_LOAD_FF_TABLE;
			TxBuf.Length=1;
			UartStartTx();			
			break;		
		}
		case CMD_GET_MASS_ADC: //Отправка значения ADC[3]
		{
			TxBuf.Data[0]=CMD_GET_MASS_ADC;
			TxBuf.Data[1]=*((BYTE*)&ADC8x14DataAveraged[3]);
			TxBuf.Data[2]=*((BYTE*)&ADC8x14DataAveraged[3]+1);
			TxBuf.Length=3;
			UartStartTx();			
			break;		
		}
		case CMD_GET_DAC_VAL: //Отправляет значение, которое пишется в ЦАП
		{
			BYTE *pSrc=(BYTE*)DacData+(RcBuf.Data[1]<<1);
			TxBuf.Data[0]=CMD_GET_DAC_VAL;
			TxBuf.Data[1]=RcBuf.Data[1];
			TxBuf.Data[2]=*pSrc++;
			TxBuf.Data[3]=*pSrc;
			TxBuf.Length=4;
			UartStartTx();
			break;		
		}
		case CMD_SET_MANUAL_HEATER_MODE: //Включает и выключает ручной режим записи DAC[0] 
		{
			if(RcBuf.Data[1]==0)
				ManualHeaterControl=0;
			else
				ManualHeaterControl=1;
			TxBuf.Data[0]=CMD_SET_MANUAL_HEATER_MODE;
			TxBuf.Length=1;
			UartStartTx();			
			break;		
		}
		case CMD_LOAD_PROTECTION_VALS: //Команда установки величин максимального тока/напряжения/мощности
		{
			*((BYTE*)&MaxHeaterI) = RcBuf.Data[1];	
			*((BYTE*)&MaxHeaterI+1) = RcBuf.Data[2];
			*((BYTE*)&MaxHeaterU) = RcBuf.Data[3];	
			*((BYTE*)&MaxHeaterU+1) = RcBuf.Data[4];
			*((BYTE*)&MaxHeaterP) = RcBuf.Data[5];	
			*((BYTE*)&MaxHeaterP+1) = RcBuf.Data[6];
			*((BYTE*)&MaxHeaterISoft) = RcBuf.Data[7];
			*((BYTE*)&MaxHeaterISoft+1) = RcBuf.Data[8];
			TxBuf.Data[0]=CMD_LOAD_PROTECTION_VALS;
			TxBuf.Length=1;
			UartStartTx();		
			break;		
		}
		case CMD_RESET_PROTECTION: //Команда сброса защиты по току/напряжению/мощности
		{
			MaxHeaterIGuard = 0;	
			MaxHeaterUGuard = 0;
			MaxHeaterPGuard = 0;
			TxBuf.Data[0]=CMD_RESET_PROTECTION;
			TxBuf.Length=1;
			UartStartTx();		
			break;		
		}
		case CMD_READ_MEMORY_VAL: //Запрос числа по определенному адресу
		{							//Формат:AddressH, AddressL
			if(RcBuf.Length==3)
			{
				BYTE *Address=(BYTE*)(((UINT)RcBuf.Data[1]<<8)+RcBuf.Data[2]);
				TxBuf.Data[0]=CMD_READ_MEMORY_VAL;
				TxBuf.Data[1]=*Address;
				TxBuf.Length=2;
				UartStartTx();				
			}
			else
				SendErrorCommandFormat();
			break;		
		}
		case CMD_WRITE_MEMORY_VAL: //Запись числа по определенному адресу
		{							//Формат:AddressH, AddressL, ByteToWrite
			if(RcBuf.Length==4)
			{
				BYTE *Address=(BYTE*)(((UINT)RcBuf.Data[1]<<8)+RcBuf.Data[2]);
				*Address=RcBuf.Data[3];
				TxBuf.Data[0]=CMD_WRITE_MEMORY_VAL;
				TxBuf.Data[1]=ANSWER_OK;
				TxBuf.Length=2;
				UartStartTx();				
			}
			else
				SendErrorCommandFormat();
			break;		
		}
		case CMD_RESET: //Сброс контроллера
		{
			if((RcBuf.Length==2) && (RcBuf.Data[1]==RESET_MAGIC_NUM))//0x55 - "контрольное число"
				asm("RESET");
			else
				SendErrorCommandFormat();
			break;		
		}
		case CMD_GET_STATUS: //Запрос статуса
		{
			TxBuf.Data[0]=CMD_GET_STATUS;
			TxBuf.Data[1]=(TableVLoaded&TdTLoaded)?1:0;
			TxBuf.Data[2]=FlagWait|(FlagMainOff<<1)|(FlagRefTempAchived<<2);
			TxBuf.Data[3]=MaxHeaterIGuard|(MaxHeaterUGuard<<1)|(MaxHeaterPGuard<<2)|((MaxHeaterISoftCount!=0)<<3);
			TxBuf.Length=4;
			UartStartTx();		
			break;		
		}
		default:
		{
			TxBuf.Data[0]=ANSWER_UNKNOWN_COMMAND;
			TxBuf.Data[1]=RcBuf.Data[0];
			TxBuf.Length=2;
			UartStartTx();		
		}	
		}	
	}
}

//________________________________________________________________________________
void ReportFifoRxOverflow(void)
{
	TxBuf.Data[0]=ANSWER_ERROR_RX_FIFO_OVERFLOW;
	TxBuf.Length=1;
	UartStartTx();
}

//________________________________________________________________________________
void ReportFifoTxOverflow(void)
{
	TxBuf.Data[0]=ANSWER_ERROR_TX_FIFO_OVERFLOW;
	TxBuf.Length=1;
	UartStartTx();
}

//________________________________________________________________________________
void ReportRcProtocolError(void)
{
	TxBuf.Data[0]=ANSWER_ERROR_RC_PROTOCOL;
	TxBuf.Length=1;
	UartStartTx();
}

//________________________________________________________________________________
//Сообщает о факте и причинах сброса/включения
void ReportStartUp(void)
{
	TxBuf.Data[0]=MESSAGE_RESET;
	if(RI==1 && TO==1 && PD==1 && POR==0 && BOR==0)
		TxBuf.Data[1]=1; //Power On *OK
	else if( (RI==0) && (POR==1) && (STKFUL)==0 && (STKUNF)==0)
		TxBuf.Data[1]=3; //Software Reset *OK
	else if( (STKFUL==1) || (STKUNF==1))
		TxBuf.Data[1]=4; //Stack Overflow/Underflow Reset *OK
	else if( (TO==0) && (PD==1))
		TxBuf.Data[1]=5; //WatchDog Timer Reset *OK
	else if( (RI==1) && (POR==1) && (BOR==1) && (STKFUL==0) && (STKUNF==0))
		TxBuf.Data[1]=2; //MCLR Reset *OK
	else if( (RI==1) && (TO==1) && (PD==1) && (POR==1) && (BOR==0))
		TxBuf.Data[1]=6; //Brown-out Reset *OK -после восстановления питания до нормы
	else
		TxBuf.Data[1]=7; //Unknown Reset
	
	TxBuf.Data[2]=ANSWER_OK;
	TxBuf.Length=3;
	UartStartTx();
	RI=1;
	POR=1;
	BOR=1;
	STKFUL=0; STKUNF=0;
}

