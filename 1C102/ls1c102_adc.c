
#include "ls1x.h"
#include "ls1c102_adc.h"
#include "ls1x_common.h"

//adc��Դ��
void Adc_powerOn(void)
{
	WriteKeyReg(spPMU_ChipCtrl->adc_on,1);    //��adc��Դ
}

//adc��Դ��
void Adc_powerOff(void)
{
	WriteKeyReg(spPMU_ChipCtrl->adc_on,0);    //�ر�adc��Դ
}

//��adcͨ��������ͬʱ�򿪶��
void Adc_open(AdcChannel channel)
{
	AdcEnFlag flag = 1 << channel;

	if((flag & ADC_EN_I0) == ADC_EN_I0)
    {
		//ADC_I0
		WriteKeyReg(spPMU_ChipCtrl->adci0_ien,0);    //�ر�ADCI0Ϊģ���ź�
	}
	else
    {
		//ADC_I4-7
		WriteKeyReg(spPMU_ChipCtrl->adc_en,flag >> 4);    //�ر�ADCI4-7��Ӧͨ��Ϊģ���ź�
	}
}

//�ر�adcͨ��������ͬʱ�رն��
void Adc_close(AdcChannel channel)
{
	AdcEnFlag flag = 1 << channel;

	if((flag & ADC_EN_I0) == ADC_EN_I0)
    {
		//ADC_I0
		WriteKeyReg(spPMU_ChipCtrl->adci0_ien,1);    //��ADCI0Ϊ��������
	}
	else
     {
		//ADC_I4-7
		unsigned char en = spPMU_ChipCtrl->adc_en;
		en &= ~(flag >> 4);
		WriteKeyReg(spPMU_ChipCtrl->adc_en, en);    //��ADCI4-7��Ӧͨ��Ϊ��������
	}
}

//��adcͨ��������ͬʱ�򿪶��
void Adc_openGroup(AdcEnFlag enFlags)
{
	if((enFlags & ADC_EN_I0) == ADC_EN_I0)
    {
		//ADC_I0
		WriteKeyReg(spPMU_ChipCtrl->adci0_ien,0);    //�ر�ADCI0Ϊģ���ź�
	}
	else {
		//ADC_I4-7
		WriteKeyReg(spPMU_ChipCtrl->adc_en,enFlags >> 4);    //�ر�ADCI4-7��Ӧͨ��Ϊģ���ź�
	}
}

//�ر�adcͨ��������ͬʱ�رն��
void Adc_closeGroup(AdcEnFlag enFlags)
{
	if((enFlags & ADC_EN_I0) == ADC_EN_I0)
    {
		//ADC_I0
		WriteKeyReg(spPMU_ChipCtrl->adci0_ien,1);    //��ADCI0Ϊ��������
	}
	else
    {
		//ADC_I4-7
		unsigned char en = spPMU_ChipCtrl->adc_en;
		en &= ~(enFlags >> 4);
		WriteKeyReg(spPMU_ChipCtrl->adc_en, en);    //��ADCI4-7��Ӧͨ��Ϊ��������
	}
}


//���β���
unsigned short Adc_singleMeasure(AdcChannel channel)
{
	unsigned short v;

	DisableInt();
	PMU_CHIPCTRL |= 1 << 16;
    PMU_AdcCtrl = 0x100 | channel;               //��ʼ������ѡ������˿�,2��Ƶ
	while((PMU_AdcCtrl & (1<<8)));               //�жϲ����Ƿ����
	//���ز����������
	v = PMU_AdcDat & 0xFFF;
	EnableInt();
	return v;
}

/*
//ȥ������
unsigned short Adc_Measure(AdcChannel channel)
{
	unsigned short i, rs, max = 0, min = 0xFFF, sum = 0;

	//����5�Σ�ȥ��һ����ߺ���ͣ��м�����ȡƽ��ֵ
	for(i = 0; i<5; i++)
    {
	    PMU_AdcCtrl = 0x100 | channel;               //��ʼ������ѡ������˿�,2��Ƶ
		while((PMU_AdcCtrl & (1<<8)));               //�жϲ����Ƿ����

		rs =  PMU_AdcDat & 0xFFF;
		sum += rs;
		if( rs > max)
			max = rs;
		if(rs < min)
			min = rs;
	}

	return (sum - max - min) / 3;
}
*/

//ȥ������
unsigned short Adc_Measure(AdcChannel channel)
{
	unsigned short i, rs, max = 0, sum = 0;

	//����7�Σ�ȥ��ǰ���κ�һ�����ֵ��ʣ��3��ƽ��
	for(i = 0; i<7; i++)
    {
//		DisableInt();
	    PMU_AdcCtrl = 0x100 | channel;               //��ʼ������ѡ������˿�,2��Ƶ
		while((PMU_AdcCtrl & (1<<8)));               //�жϲ����Ƿ����
		rs =  PMU_AdcDat & 0xFFF;
//		EnableInt();

		if(i >= 3)
		{
			sum += rs;
			if(rs > max)
				max = rs;
		}
	}

	return (sum - max) / 3;
}

//������ѹת��,��λ������
unsigned short Adc_getVoltage(AdcChannel channel)
{
	unsigned int v = Adc_Measure(channel);
	unsigned int b = Adc_Measure(ADC_CHANNEL_1V);
	return v * 1000 / b;
}



