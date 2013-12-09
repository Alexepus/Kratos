#define CMD_GET_EXT_ADC 		((BYTE)0x00)
#define CMD_LOAD_DAC 			((BYTE)0x01)
#define CMD_LOAD_PIDCOEFS 		((BYTE)0x02)
#define CMD_GET_PIDVALS 		((BYTE)0x03)
#define CMD_LOAD_TCOUPLE_TABLE	((BYTE)0x04)
#define CMD_LOAD_T0_DT			((BYTE)0x05)
#define CMD_GET_T_BY_V			((BYTE)0x06)
#define CMD_GET_AVERAGED_ADC	((BYTE)0x07)
#define CMD_GET_ADC_TC_I_U_TREF ((BYTE)0x08)
#define CMD_START_TEMP_CHANGE	((BYTE)0x09)
#define CMD_LOAD_PIDPARAMS		((BYTE)0x0A)
#define CMD_WAIT_CONTINUE_OFF	((BYTE)0x0B)
#define CMD_LOAD_FF_TABLE		((BYTE)0x0C)
#define CMD_GET_MASS_ADC		((BYTE)0x0D)
#define CMD_GET_DAC_VAL			((BYTE)0x0E)
#define CMD_SET_MANUAL_HEATER_MODE	((BYTE)0x0F)
#define CMD_LOAD_PROTECTION_VALS	((BYTE)0x10) // Команда установки величин максимального тока/напряжения/мощности
#define CMD_RESET_PROTECTION	((BYTE)0x11) // Команда сброса защиты по току/напряжению/мощности

#define CMD_READ_MEMORY_VAL 	((BYTE)0xF0)
#define CMD_WRITE_MEMORY_VAL 	((BYTE)0xF1)
#define CMD_RESET 				((BYTE)0xFD) // Команда сброса контроллера
#define CMD_GET_STATUS			((BYTE)0xFE)
#define MESSAGE_RESET			((BYTE)0xFF)

#define RESET_MAGIC_NUM 0x55	// Контрольное число команды сброса контроллера

#define ANSWER_OK 					0xA0
#define ANSWER_FAILED				0xAF
#define ANSWER_UNKNOWN_COMMAND 		0xE0
#define ANSWER_ERROR_COMMAND_FORMAT 	0xE1
#define ANSWER_ERROR_RX_FIFO_OVERFLOW 	0xE2
#define ANSWER_ERROR_TX_FIFO_OVERFLOW 	0xE3
#define ANSWER_ERROR_RC_PROTOCOL	 	0xE4

#define VAL_WAIT		0
#define VAL_CONTINUE	1
#define VAL_MAIN_OFF	2


#define SendErrorCommandFormat() {TxBuf.Data[0]=ANSWER_ERROR_COMMAND_FORMAT;\
			TxBuf.Length=1; UartStartTx();}
void ReportFifoRxOverflow(void);
void ReportRcProtocolError(void);
void ReportStartUp(void);

extern OST_SEMAPHORE UartRcMsgSem;
extern FIFO TxFifo;
extern FIFO RcFifo;
