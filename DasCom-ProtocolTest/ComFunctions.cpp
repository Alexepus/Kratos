#include "stdafx.h"
#include "termo.h"
#include "ComFunctions.h"
#include "ThreadSafeQueue.h"

extern CTermoApp theApp;

BOOL ConfigureComPort(HANDLE hCommDev)
{
DCB dcb;
FillMemory(&dcb, sizeof(dcb), 0);


/*
if (!BuildCommDCB("parity=N data=8 stop=1 ", &dcb)) 
   {   Msg("Ошибка при конфигурации COM порта.");
      // Couldn't build the DCB. Usually a problem
      // with the communications specification string.
      return FALSE;
   }
   else
*/
   
  
dcb.DCBlength=(DWORD)sizeof(DCB);
//dcb.BaudRate=GetPrivateProfileInt("COM PORT", "BaudRate", 19200, pIniPath);
dcb.ByteSize=8;
dcb.StopBits=ONESTOPBIT;
dcb.Parity=NOPARITY;
dcb.fAbortOnError=TRUE;
dcb.fBinary=TRUE;
dcb.fOutxDsrFlow=FALSE;




   {
	   dcb.BaudRate=theApp.BaudRate;
	   dcb.fRtsControl=RTS_CONTROL_ENABLE;//Компьютер выставляет готовность. 
				//Устройство должно при получении RTS возвратить сигнал CTS
	   dcb.fOutxCtsFlow=FALSE;//Ошибка записи, если не выставлен сигнал Clear To Send
	   //dcb.fDtrControl=DTR_CONTROL_ENABLE; 
	   //dcb.fOutxDsrFlow=TRUE; 
		
		// Set new state.
		if (!SetCommState(hCommDev, &dcb))
			Msg("Ошибка при установке конфигурации COM порта.");
		// Error in SetCommState. Possibly a problem with the communications 
		// port handle or a problem with the DCB structure itself.
   }
BOOL SetConfigResult=1;
 /*
COMMCONFIG CommConfig;
 

CommConfig.dwSize=sizeof(COMMCONFIG);
//dcb structure defines the control setting for a serial communications device
CommConfig.dcb.DCBlength=(DWORD)sizeof(DCB);
CommConfig.dcb.BaudRate=GetPrivateProfileInt("COM PORT", "BaudRate", 19200, pIniPath);
CommConfig.dcb.ByteSize=8;
CommConfig.dcb.StopBits=ONESTOPBIT;
CommConfig.dcb.Parity=NOPARITY;
CommConfig.dcb.fAbortOnError=FALSE;
CommConfig.dcb.fBinary=TRUE;
//CommConfig.dcb.fOutxDsrFlow=FALSE; // DSR output flow control 
SetConfigResult=SetCommConfig(hCommDev, &CommConfig, CommConfig.dwSize);
if (!SetConfigResult)
	Msg("Ошибка при установке конфигурации COM порта.");
*/
COMMTIMEOUTS timeouts;
BOOL SetTimeoutsResult;
timeouts.ReadIntervalTimeout = MAXDWORD; 
timeouts.ReadTotalTimeoutMultiplier = 0;
timeouts.ReadTotalTimeoutConstant = 20;
timeouts.WriteTotalTimeoutMultiplier = 5+10000/dcb.BaudRate;
timeouts.WriteTotalTimeoutConstant = 20;

SetTimeoutsResult=SetCommTimeouts(hCommDev, &timeouts);
if (!SetTimeoutsResult)
   // Error setting time-outs.
   Msg("Ошибка при установке тайм-аутов COM порта.");


return SetConfigResult && SetTimeoutsResult;        
}

int DetectSerialErrors(HANDLE hComm, CString *Err)
{
	CString str, str2;
	COMSTAT comStat;
    DWORD   dwErrors;
    BOOL    fOOP, fOVERRUN, fPTO, fRXOVER, fRXPARITY, fTXFULL;
    BOOL    fBREAK, fDNS, fFRAME, fIOE, fMODE;

    // Get and clear current errors on the port.
    if (!ClearCommError(hComm, &dwErrors, &comStat))
	{
		Msg("Report error in ClearCommError.");
        // Report error in ClearCommError.
        return 0;
	}
/*
#define CE_RXOVER           0x0001  // Receive Queue overflow
#define CE_OVERRUN          0x0002  // Receive Overrun Error
#define CE_RXPARITY         0x0004  // Receive Parity Error
#define CE_FRAME            0x0008  // Receive Framing error
#define CE_BREAK            0x0010  // Break Detected
#define CE_TXFULL           0x0100  // TX Queue is full
#define CE_PTO              0x0200  // LPTx Timeout
#define CE_IOE              0x0400  // LPTx I/O Error
#define CE_DNS              0x0800  // LPTx Device not selected
#define CE_OOP              0x1000  // LPTx Out-Of-Paper
#define CE_MODE             0x8000  // Requested mode unsupported
*/
    // Get error flags.
    if(fDNS = dwErrors & CE_DNS)
		str+="LPTx Device not selected; ";
    if(fIOE = dwErrors & CE_IOE)
		str+="LPTx I/O Error; ";
    if(fOOP = dwErrors & CE_OOP)
		str+="LPTx Out-Of-Paper; ";
    if(fPTO = dwErrors & CE_PTO)
		str+="LPTx Timeout; ";
    if(fMODE = dwErrors & CE_MODE)
		str+="Requested mode unsupported; ";
    if(fBREAK = dwErrors & CE_BREAK)
		str+="Break Detected; ";
    if(fFRAME = dwErrors & CE_FRAME)
		str+="Receive Framing error; ";
    if(fRXOVER = dwErrors & CE_RXOVER)
		str+="Receive Queue overflow; ";
    if(fTXFULL = dwErrors & CE_TXFULL)
		str+="TX Queue is full; ";
    if(fOVERRUN = dwErrors & CE_OVERRUN)
		str+="Receive Overrun Error; ";
    if(fRXPARITY = dwErrors & CE_RXPARITY)
		str+="Receive Parity Error; ";
	str+="\n";
    // COMSTAT structure contains information regarding
    // communications status.
    if (comStat.fCtsHold)
	{
        // Tx waiting for CTS signal
		//return -2; //Особая ошибка
		str+="Tx waiting for CTS signal; ";
	}

    if (comStat.fDsrHold)
	{
        // Tx waiting for DSR signal
		//return -1; //Особая ошибка
		str+="Tx waiting for DSR signal;";
	}

    if (comStat.fRlsdHold)
        // Tx waiting for RLSD signal
		str+="Tx waiting for RLSD signal; ";

    if (comStat.fXoffHold)
        // Tx waiting, XOFF char rec'd
		str+="Tx waiting, XOFF char rec\'d; ";

    if (comStat.fXoffSent)
        // Tx waiting, XOFF char sent
		str+="Tx waiting, XOFF char sent; ";

    if (comStat.fEof)
        // EOF character received
		str+="EOF character received; ";
    if (comStat.fTxim)
        // Character waiting for Tx; char queued with TransmitCommChar
		str+="Character waiting for Tx; char queued with TransmitCommChar; ";

    if (comStat.cbInQue)
        // comStat.cbInQue bytes have been received, but not read
	{
		str2.Format("%i bytes have been received, but not read;", comStat.cbInQue);
		if (comStat.cbInQue>2000)
		{
			PurgeComm(hComm,PURGE_RXCLEAR|PURGE_RXABORT);
			str+=str2;
		}
	}

    if (comStat.cbOutQue)
        // comStat.cbOutQue bytes are awaiting transfer
		str+="comStat.cbOutQue bytes are awaiting transfer; ";
	*Err=str;
	if((str.GetLength())>1 && (Err==NULL))
	{
		Msg("DetectSerialErrors: %s", str);
		return 0; //Есть какие-то ошибки
	}
	else
		return 1; //Все ОК

}
