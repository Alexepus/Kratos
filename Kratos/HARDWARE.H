#define IPS6
#ifdef IPS6
#include "ips6.h"
#else
#include "vcmcdm.h"
#pragma comment( lib, "VCMCDM.LIB" )
#endif


#define REGISTER_POT_WRITE_F	16		//������� ������ � �������� ������� "133.004"
#define REGISTER_KE_A			0		//�������� �� (Retard) � �������� ��������
#define REGISTER_HV_A			1		//�������� HV � �������� ��������
#define TIMER_WRITE_START_F		25		//������� ������� ������� "Preset Scaler 420A"
#define TIMER_WRITE_PRESET_F	16		//������� ������ ������� � ������
#define TIMER_SERVICE_A			9		//�������� ���������� �������� � �������
#define TIMER_SERVICE_F			1
#define TIMER_L					0x200	//  1<<9 = 512
#define TIMER_RESET_F				9
#define COUNTER_OVERFLOW_CHECK_F  8
#define COUNTER_RESET_F			9
#define COUNTER_OVERFLOW_LAM_RESET_F  10
#define COUNTER_READ_RESET_F	2		//������� ������ ����� ��������� �� ����������
#define WRONG_INI_VALUE			13666	//��� ini-�����

//������� �������� 5�.850.17
#define REGISTER_5P850_A		0
#define REGISTER_5P850_WRITE_F	16
#define REGISTER_5P850_CLEAR_F	9
#define REGISTER_5P850_READ_F		0

#define HP_RETARD_GUAGE 50			//50 �� �� ���� ��� ����
#define HP_MAX_HV_CODE	6