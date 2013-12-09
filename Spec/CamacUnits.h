//Интегрирующий вольтметр Ц0609
#define ADC_A					0
#define ADC_READ_VOLTAGE_F		0
#define ADC_READ_CONFIG_F		2
#define ADC_TEST_LAM_F			8
#define ADC_RESET_LAM_F			10
#define ADC_WRITE_CONFIG_F		16
#define ADC_STOP_F				24
#define ADC_START_F				26
//Режим измерений

//ЦАП 4х10/12 5Р.093.36
#define DAC_READ				0
#define DAC_WRITE				16
//Преобразование напряжения в код
//#define ToDac12Code(Voltage) ((((UINT)(Voltage*200.))-0x800)&0xfff)
#define ToDac12Code(Voltage) (((UINT)(Voltage*200.))&0xfff)
#define ToDac10Code(Voltage) ((abs(Voltage*100.)&0x3ff)|(Voltage>0)?0:0x400)






