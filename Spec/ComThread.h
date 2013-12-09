// ComThread.h: interface for the CComThread class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COMTHREAD_H__AB0353E0_7D8E_4C43_A361_1B9778913F88__INCLUDED_)
#define AFX_COMTHREAD_H__AB0353E0_7D8E_4C43_A361_1B9778913F88__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ComThreadBasic.h"
#include "MsgPool.h"

#define WM_COM_EVENT (WM_APP+13)

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

#define SERIAL_SYNC_MSG			1
#define SERIAL_ASYNC_MSG		2
#define SERIAL_ERROR_COM_FAILED	3
#define SERIAL_ERROR_TIMEOUT	4


class CComThread : public CComThreadBasic  
{
protected:
	CString LogFileName;
	//Анализирует сообщение, выполняет соответствующее действие, 
	//возвращает 1, если принято ожидаемое сообщение, 0 - в остальных случаях
	//Если SendMsg==NULL, анализирует асинхронное сообщение. Возвращает TRUE, если
	//RecievedMsg - правильный ответ на SentMsg и сообщение обработано
	virtual int AnalyseCorrectMsg(ComMsg* RecievedMsg, ComMsg* SentMsg);

	//Может запрашивать действие пользователя и заканчивать поток
	virtual void ReportComErrorAndExit();
	
	//Может выводить сырые данные при установленных флагах m_bViewSentRawData и
	//m_bViewRecievedRawData
	virtual void DumpRawData(const char* Discript, BYTE* ReadBuf, int ReadFromPort);

	//Вызывается после отправки сообщения
	virtual void DumpSentMsg(ComMsg* SentMsg);
	
	//Вызывается при некорректных/незаконченных сообщениях в приемном буфере
	virtual void ReportJunkMessage(BYTE *Buf, int Size, char Reason);
	//Вызывается при истекшем тайм-ауте ожидания ответа
	virtual void ReportTimeOut(ComMsg* SentMsg);

public:
	CMsgPool m_Pool;
	bool SendWaitForReply(ComMsg* msg);
	CComThread();
	//virtual ~CComThread();

};

#endif // !defined(AFX_COMTHREAD_H__AB0353E0_7D8E_4C43_A361_1B9778913F88__INCLUDED_)
