#define IPS6
#ifdef IPS6
#include "ips6.h"
#else
#include "vcmcdm.h"
#pragma comment( lib, "VCMCDM.LIB" )
#endif


#define REGISTER_POT_WRITE_F	16		//Функция записи в выходной регистр "133.004"
#define REGISTER_KE_A			0		//Субадрес КЕ (Retard) в выходном регистре
#define REGISTER_HV_A			1		//Субадрес HV в выходном регистре
#define TIMER_WRITE_START_F		25		//Функция запуска таймера "Preset Scaler 420A"
#define TIMER_WRITE_PRESET_F	16		//Функция записи времени в таймер
#define TIMER_SERVICE_A			9		//Субадрес служебного регистра в таймере
#define TIMER_SERVICE_F			1
#define TIMER_L					0x200	//  1<<9 = 512
#define TIMER_RESET_F				9
#define COUNTER_OVERFLOW_CHECK_F  8
#define COUNTER_RESET_F			9
#define COUNTER_OVERFLOW_LAM_RESET_F  10
#define COUNTER_READ_RESET_F	2		//Функция чтения числа импульсов из пересчетки
#define WRONG_INI_VALUE			13666	//Для ini-файла

//Регистр выходной 5Р.850.17
#define REGISTER_5P850_A		0
#define REGISTER_5P850_WRITE_F	16
#define REGISTER_5P850_CLEAR_F	9
#define REGISTER_5P850_READ_F		0

#define HP_RETARD_GUAGE 50			//50 мВ на один бит кода
#define HP_MAX_HV_CODE	6