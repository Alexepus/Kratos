#define UART_START 		0xAA
#define UART_STOP 		0xAB
#define UART_SHIFT 		0xAC

void UartRcInt(void);
void UartTxInt(void);
void UartStartTx(void);
BYTE ExtractRcMessage(void);
void WindRcFifo(void);

#define BUFFER_LENGTH 20
typedef struct
{
	BYTE Data[BUFFER_LENGTH];
	BYTE Length;
} MessageBuffer;

#define FIFO_DEPTH 128
typedef struct
{
	BYTE Buffer[FIFO_DEPTH];
	BYTE NIn; 	//������������� ����� ������ � Buffer[NIn]
	BYTE NOut;	//������������� ����� ���������� �� Buffer[NOut]
	//����� ������: NIn-NOut
} FIFO;

extern bit FifoRxOverflow;
