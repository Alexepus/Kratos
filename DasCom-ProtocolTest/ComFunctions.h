BOOL ConfigureComPort(HANDLE hCommDev);
int DetectSerialErrors(HANDLE hComm, CString *Err=NULL);

#define RS485_START   (char)0xAA
#define RS485_END     (char)0xAB
#define RS485_ESCAPE  (char)0xAC
#define RS485_TIMEOUT 0.5