#ifndef _ADC_H_
#define _ADC_H_

//ChipCtl��ADCʹ�ܿ���
//#define ADC_EN_I0 0x01		//ADC_I0ʹ��
//#define	ADC_EN_I4 0x10		//ADC_I4ʹ��
//#define	ADC_EN_I5 0x20		//ADC_I5ʹ��
//#define	ADC_EN_I6 0x40		//ADC_I6ʹ��
//#define	ADC_EN_I7 0x80		//ADC_I7ʹ��
typedef enum {
	ADC_EN_I0 = 0x01,			//ADC_I0ʹ��
	ADC_EN_I4 = 0x10,			//ADC_I4ʹ��
	ADC_EN_I5 = 0x20,			//ADC_I5ʹ��
	ADC_EN_I6 = 0x40,			//ADC_I6ʹ��
	ADC_EN_I7 = 0x80			//ADC_I7ʹ��
}AdcEnFlag;

//AdcCtl�е�ѡ��ͨ��
typedef enum {
	ADC_CHANNEL_I0 = 0,			//ADC_I0
	ADC_CHANNEL_I1,				//1c101��Ч
	ADC_CHANNEL_VCORE,			//MCU�����Դ��ѹ
	ADC_CHANNEL_1V,				//1.0V��׼��ѹ�ο��ɼ�ͨ��
	ADC_CHANNEL_I4,				//ADC_I4
	ADC_CHANNEL_I5,				//ADC_I5
	ADC_CHANNEL_I6,				//ADC_I6
	ADC_CHANNEL_I7				//ADC_I7
}AdcChannel;

//adc��Դ��
void Adc_powerOn(void);
//adc��Դ��
void Adc_powerOff(void);
//��adc����ͨ��
void Adc_open(AdcChannel channel);
//�ر�adc����ͨ��
void Adc_close(AdcChannel channel);
//��adcͨ��������ͬʱ�򿪶��
void Adc_openGroup(AdcEnFlag enFlags);
//�ر�adcͨ��������ͬʱ�رն��
void Adc_closeGroup(AdcEnFlag enFlags);
//���β���
unsigned short Adc_singleMeasure(AdcChannel channel);
//ȥ������
unsigned short Adc_Measure(AdcChannel channel);
//������ѹת��,��λ������
unsigned short Adc_getVoltage(AdcChannel channel);

#endif

