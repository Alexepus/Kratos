#include "Main.h"
static UINT ADC8x14DataTemp;
UINT ADC8x14Data[8];
UINT ADC8x14DataTemp0[4];
UINT ADC8x14DataTemp1[4];
UINT ADC8x14DataAveraged[4];

void ReadADC8x14(void)
{
	ADC_CS=0;
			//��������������, ��� ��� ���������� ����� 0
	START_ADC8x14_CONV();//������ ��������������.
	while(!ADC_Busy); //�������� ��������� ��������������
	CONVERT_CHANNEL(0,ADC_CREG_CH1,0);
	START_ADC8x14_CONV();
	while(!ADC_Busy);
	CONVERT_CHANNEL(1,ADC_CREG_CH2,0);
	START_ADC8x14_CONV();
	while(!ADC_Busy);
	CONVERT_CHANNEL(2,ADC_CREG_CH3,0);
	START_ADC8x14_CONV();
	while(!ADC_Busy);
	CONVERT_CHANNEL(3,ADC_CREG_CH4,0);
	START_ADC8x14_CONV();
	while(!ADC_Busy);
	CONVERT_CHANNEL(4,ADC_CREG_CH5,0);
	START_ADC8x14_CONV();
	while(!ADC_Busy);
	CONVERT_CHANNEL(5,ADC_CREG_CH6,0);
	START_ADC8x14_CONV();
	while(!ADC_Busy);
	CONVERT_CHANNEL(6,ADC_CREG_CH7,0);
	START_ADC8x14_CONV();
	while(!ADC_Busy);
	CONVERT_CHANNEL(7,ADC_CREG_CH0,0);
	
	ADC_CS=1;	
}

void ReadADC8x14_0_3(void)
{
	ADC_CS=0;
			//��������������, ��� ��� ���������� ����� 0
	START_ADC8x14_CONV();//������ ��������������.
	while(!ADC_Busy); //�������� ��������� ��������������
	CONVERT_CHANNEL_TO_TMP(ADC_CREG_CH0);
	START_ADC8x14_CONV();
	ADC8x14Data[0]=ADC8x14DataTemp;
	while(!ADC_Busy);
	CONVERT_CHANNEL_TO_TMP(ADC_CREG_CH0);
	START_ADC8x14_CONV();
	ADC8x14Data[0]+=ADC8x14DataTemp;
	while(!ADC_Busy);
	CONVERT_CHANNEL_TO_TMP(ADC_CREG_CH0);
	START_ADC8x14_CONV();
	ADC8x14Data[0]+=ADC8x14DataTemp;
	while(!ADC_Busy);
	CONVERT_CHANNEL_TO_TMP(ADC_CREG_CH1);
	START_ADC8x14_CONV();
	ADC8x14Data[0]+=ADC8x14DataTemp;
	ADC8x14Data[0]=ADC8x14Data[0]>>2;
	while(!ADC_Busy);

	CONVERT_CHANNEL_TO_TMP(ADC_CREG_CH1);
	START_ADC8x14_CONV();
	ADC8x14Data[1]=ADC8x14DataTemp;
	while(!ADC_Busy);
	CONVERT_CHANNEL_TO_TMP(ADC_CREG_CH1);
	START_ADC8x14_CONV();
	ADC8x14Data[1]+=ADC8x14DataTemp;
	while(!ADC_Busy);
	CONVERT_CHANNEL_TO_TMP(ADC_CREG_CH1);
	START_ADC8x14_CONV();
	ADC8x14Data[1]+=ADC8x14DataTemp;
	while(!ADC_Busy);
	CONVERT_CHANNEL_TO_TMP(ADC_CREG_CH2);
	START_ADC8x14_CONV();
	ADC8x14Data[1]+=ADC8x14DataTemp;
	ADC8x14Data[1]=ADC8x14Data[1]>>2;
	while(!ADC_Busy);

	CONVERT_CHANNEL_TO_TMP(ADC_CREG_CH2);
	START_ADC8x14_CONV();
	ADC8x14Data[2]=ADC8x14DataTemp;
	while(!ADC_Busy);
	CONVERT_CHANNEL_TO_TMP(ADC_CREG_CH2);
	START_ADC8x14_CONV();
	ADC8x14Data[2]+=ADC8x14DataTemp;
	while(!ADC_Busy);
	CONVERT_CHANNEL_TO_TMP(ADC_CREG_CH2);
	START_ADC8x14_CONV();
	ADC8x14Data[2]+=ADC8x14DataTemp;
	while(!ADC_Busy);
	CONVERT_CHANNEL_TO_TMP(ADC_CREG_CH3);
	START_ADC8x14_CONV();
	ADC8x14Data[2]+=ADC8x14DataTemp;
	ADC8x14Data[2]=ADC8x14Data[2]>>2;
	while(!ADC_Busy);

	CONVERT_CHANNEL_TO_TMP(ADC_CREG_CH3);
	START_ADC8x14_CONV();
	ADC8x14Data[3]=ADC8x14DataTemp;
	while(!ADC_Busy);
	CONVERT_CHANNEL_TO_TMP(ADC_CREG_CH3);
	START_ADC8x14_CONV();
	ADC8x14Data[3]+=ADC8x14DataTemp;
	while(!ADC_Busy);
	CONVERT_CHANNEL_TO_TMP(ADC_CREG_CH3);
	START_ADC8x14_CONV();
	ADC8x14Data[3]+=ADC8x14DataTemp;
	while(!ADC_Busy);
	CONVERT_CHANNEL_TO_TMP(ADC_CREG_CH0);
	ADC8x14Data[3]+=ADC8x14DataTemp;
	ADC8x14Data[3]=ADC8x14Data[3]>>2;
	
	ADC_CS=1;
}
