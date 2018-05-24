/*----------------------------------------------------------------
 * Copyright (C) 2011 ���մ�ѧ ����ѧԺ416 ����ԭ
 * ��Ȩ���С� 
 * 
 * �ļ����� AD7708.c
 * 
 * �ļ�����������   
 *          MSP430F16x,MSP430F14Xϵ�е�Ƭ���ⲿAD��������
 *      ˵����  �����������ͨ��֮ǰʵ�ֵ�SPI���������AD7708��ͨ
 *              �ţ�����AD7708����У����ADת���ȡ�
 *              
 *              �����÷�������AD7708.c���뵽��Ŀ��ͬʱ��Ҫ���ô�
 *              �����ĳ����ļ��м������AD7708.h�����;����������
 *              ��ʹ��ͷ�ļ��������ĳ����ˣ���ϸ�ɲο� ������ �� 
 *              main.c
 *              
 *              AD7708 �Ĵ���ѡ��
 *              A3	A2	A1	A0	��ѡ�Ĵ���
 *              0	0	0	0	д������ͨ�żĴ���
 *              0	0	0	0	��������״̬�Ĵ���
 *              0	0	0	1	ģʽ�Ĵ���
 *              0	0	1	0	ADC���ƼĴ���
 *              0	0	1	1	�˲��Ĵ���
 *              0	1	0	0	ADC���ݼĴ���
 *              0	1	0	1	ADCƫ�ƼĴ���
 *              0	1	1	0	ADC����Ĵ���
 *              0	1	1	1	I/O���ƼĴ���
 *              1	0	0	0	δ����
 *              1	0	0	1	δ����
 *              1	0	1	0	δ����
 *              1	0	1	1	δ����
 *              1	1	0	0	���ԼĴ���
 *              1	1	0	1	���ԼĴ���
 *              1	1	1	0	δ����
 *              1	1	1	1	ID�Ĵ���
 * 
 *              ��Ҫ����AD7718����·������ȫ���䣬�����16λ��ȡ��Ϊ
 *              24λ�ļ��ɡ�
 * 		
 * ������ʶ��   ����ԭ20110814
 * 
 * �޸ı�ʶ��   
 * �޸�������   
**----------------------------------------------------------------*/
#include "msp430x14x.h" //430�Ĵ���ͷ�ļ�
#include "AD7708.h"     //ad7708����ͷ�ļ������������ļ������ú���
#include "Spi.h"        //SPIͨѶ�����ͷ�ļ�

char IsLong[16] = {0,0,0,0,1,1,1,0,0,0,0,0,1,1,0,0};
char mode,adccon;

/****************************************************************************
* ��    �ƣ�AD7708WriteRegister
* ��    �ܣ���AD7708�Ĵ���д��
* ��ڲ�����
*           addr��  �ڲ��Ĵ�����ַ
*           dat:    Ҫд�������
* ���ڲ�������
****************************************************************************/
void AD7708WriteRegister(char addr,long dat)
{
    SpiWriteData(addr);     //дͨ�żĴ�����֪ͨ�¸�������дaddr�Ĵ���
    if(IsLong[addr])        //�����16λ�Ĵ���, 7718��24λ����ֲҪ��if�����
    {
        SpiWriteData(dat>>8);
    }
    SpiWriteData(0xFF&dat);       //д���λ����
}

/****************************************************************************
* ��    �ƣ�AD7708ReadRegister
* ��    �ܣ���ĳ�ڲ��Ĵ�����������
* ��ڲ�����addr���ڲ��Ĵ�����ַ
* ���ڲ�����long������������
****************************************************************************/
long AD7708ReadRegister(char addr)
{
    char h = 0,l = 0;           //�ߵ��ֽ�����
    SpiWriteData(0x40|addr);    //дͨ�żĴ�����֪ͨ�¸���������addr�Ĵ���
    if(IsLong[addr])
    {
        h = SpiWriteData(0xFF);
    }
    l = SpiWriteData(0xFF);
    return ((unsigned int)h<<8)|l;
}

/****************************************************************************
* ��    �ƣ�AD7708ReadResultData
* ��    �ܣ���ȡת�����
* ��ڲ�������
* ���ڲ�����AD�������
* ˵    ��: AD7708:16λ���ݣ�����ֲ��7718������24λ���ݣ�����Ķ��ȡһ���ֽ�
****************************************************************************/
long AD7708ReadResultData()
{
    while((AD7708ReadRegister(0x00)&0x80)==0); //�ȴ�ת�����
    return AD7708ReadRegister(0x04);
}

/****************************************************************************
* ��    �ƣ�AD7708Cal
* ��    �ܣ�У׼AD7708
* ��ڲ�����channel:ҪУ׼��ͨ����
* ���ڲ�������
* ˵    ��: ���AD7708��У����������Ϊ�ڲ�У׼����������Ӧ��ƽ���˴�������
*           ��ϵͳУ׼������Ҫ�������������ӡ�
****************************************************************************/
void AD7708Cal(char channel)
{
    adccon = (adccon&0x0f)|(channel<<4);
    mode = (mode&0xf8)|0x04;                //�ڲ�0У׼
    AD7708WriteRegister(0x02,adccon);       //ADC���ƼĴ�����channelͨ��
    AD7708WriteRegister(0x01,mode);         //ģʽ�Ĵ���
    while((AD7708ReadRegister(0x01)&0x07)!=0x01);   //�ȴ�У׼���
    
    mode = (mode&0xf8)|0x05;                //�ڲ� �����У׼
    AD7708WriteRegister(0x01,mode);         //ģʽ�Ĵ���
    while((AD7708ReadRegister(0x01)&0x07)!=0x01);   //�ȴ�У׼���
}

/****************************************************************************
* ��    �ƣ�AD7708Init
* ��    �ܣ���ʼ��AD7708
* ��ڲ�����chop:ն��������־ Ϊ1��ն��������0������
* ���ڲ�������
* ˵    ��: ���AD7708�ĳ�ʼ������
****************************************************************************/
void AD7708Init(char chop)
{
    P3DIR|=BIT0;
    P3OUT&=~BIT0;                       //CSѡ��
    //����ģʽ��115200,8λ����λ������ģʽ��ʱ��ģʽ1�������spi.c��
    SpiMasterInit(115200,8,3,1);        //ʱ�Ӳ���׼ȷ��115200�������spi.c��
    _EINT();                            //���жϣ�spi��д����Ҫ��Ҫ�ж�
    
    char filter;
    adccon = 0x0f;
    if(chop == 0)
    {
        filter = 0x03;                  //�˲��Ĵ�����Ϊ��Сֵ�����Ը���
        mode = 0x91;                    //ն����ֹ��10ͨ�����޻��壬����ģʽ
    }
    else
    {
        filter = 0x0D;                  //�˲��Ĵ�����Ϊ��Сֵ�����Ը���
        mode = 0x11;                    //ն�����ã�10ͨ�����޻��壬����ģʽ
    }
    
    AD7708WriteRegister(0x07,0x00);     //IO�Ĵ���������==
    AD7708WriteRegister(0x03,filter);   //�˲��Ĵ���
    AD7708WriteRegister(0x02,0x0F);     //ADC���ƼĴ�����0ͨ����������
    AD7708WriteRegister(0x01,mode);     //ģʽ�Ĵ���
    if(chop == 0)
        for(int i = 0; i<5;i++)
        {
            //У׼����ֻ��5��ʧ���Ĵ�������ľͻḲ��֮ǰ�ģ�ֻУ׼5������
            AD7708Cal(5);
        }
    
    _DINT();
}

/****************************************************************************
* ��    �ƣ�AD7708Start
* ��    �ܣ�AD7708����ת����ʼ
* ��ڲ�����channel��ͨ��
* ���ڲ�������
* ˵    ��: ����ת��
****************************************************************************/
void AD7708Start(char channel)
{
    adccon = (adccon&0x0f)|(channel<<4);
    mode = (mode&0xf8)|0x02;
    AD7708WriteRegister(0x02,adccon);
    AD7708WriteRegister(0x01,mode);
}