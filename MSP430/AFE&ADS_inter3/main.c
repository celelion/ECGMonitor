/*----------------------------------------------------------------
 * Copyright (C) 2018 ������ͨ��ѧ ����ѧԺ �����
 * ��Ȩ���С� 
 * 
 * �ļ����� main.c
 * 
 * �ļ�����������   
 *          MSP430F149����ADS1293��AFE4400�ɼ��źŲ�ͨ���������͸�
 *          �ֻ���
 *      ˵����  
 *          ���к��ȹرտ��Ź��ͳ�ʼ��ʱ��ϵͳ��Ȼ���ʼ��ADS1293
 *          ��AFE4400��Ȼ��ͨ�����ڶ�ȡָ����շ����ݡ�������ͨ��
 *          ��ʱ���жϽ��ж�ʱ������ÿ�ν��ɼ����������ĵ��һ��Ѫ��
 *          �ź�ѹ��������͸��ֻ�
 *   
**----------------------------------------------------------------*/
#include <stdint.h>
#include "msp430x14x.h"   //430�Ĵ���ͷ�ļ�
#include "Uart.h"         //����ͨѶ�����ͷ�ļ�
#include "Spi.h"
#include "MSP430_AFE_SPI.h"
#include "TI_AFE4400.h"
#include "TI_AFE4400_setting.h"
#include "MSP430_Spi.h"
#include "TI_ADS1293.h"
#include "TI_ADS1293_setting.h"

uint8_t c=1;
uint32_t val;
char str[] = "UartWriteChar"; //ROM��һ���ַ���
char str1[] = "start conversation!"; //ROM��һ���ַ���
char str2[] = "stop conversation!"; //ROM��һ���ַ���
char q = 'Q';
char chr;
/****************************************************************************
* ��    �ƣ�ClkInit
* ��    �ܣ�ʱ��ϵͳ��ʼ��  MCLKΪ8MHz��SMCLKΪ1MHz
* ��ڲ�������
* ���ڲ�������
****************************************************************************/
void ClkInit()
{
    char i;
    BCSCTL1 &= ~XT2OFF;             //��XT2����
    IFG1&=~OFIFG;                   //����񵴴����־
    while((IFG1&OFIFG)!=0)
    {
        for(i=0;i<0xff;i++);
        IFG1&=~OFIFG;               //����񵴴����־
    }
    BCSCTL2 |= SELM_2+SELS+DIVS_0;  //MCLKΪ8MHz��SMCLKΪ1MHz
}

/*******************************************
�������ƣ�Delays
��    �ܣ���ʱһ��
��    ������
����ֵ  ����
********************************************/
void Delays(uint32_t k)
{
    uint8_t i=100;
    uint32_t j;
    while(i--)
    {
            j=k;
            while(j--);
    }
}

/****************************************************************************
* ��    �ƣ�main������
* ��    �ܣ����ô��ڣ������Ϣ���Ӵ��ڶ�����������������ݣ����Դ����շ�
* ��ڲ�������
* ���ڲ�������
****************************************************************************/
int main( void )
{
  P4DIR = 0x01;
  P4OUT = 0x01;
  P2DIR = 0xfa;
  P2OUT = 0xf0;
  // Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW + WDTHOLD;
  
  ClkInit();
  CCTL1 = CCIE;                            // CCR1 �ж�ʹ��  
  //CCR1 = 6667;
  CCR1 = 50000;  
  TACTL = TASSEL_2 + MC_2 + ID_3;          // SMCLK = 1MHz, ��������ģʽ 
  //uint8_t count=3;

  uint8_t read_buf[3];
  uint8_t read_buff[8] = {0,0,0,0,0,0,0,0};
  
  unsigned long val;
 // int i = 0;

//  count = CH_DATA_SIZE;                                                        // bytes to read: ADC_DOUT2 - ADCDOUT0

  
  //char chr;               //���ڲ����У��յ����ֽ�
  // ����ģʽ��������4000000,8λ����λ������ģʽ��ʱ��ģʽ1�������spi.c��
  SpiMasterInit(4000000,8,3,0);
  UartInit(115200,'n',8,1);//���ڳ�ʼ��,���ó�38400bps,��У��,8λ����,1λֹͣ
  _EINT(); 
  
  P1DIR = 0xff;
  P1SEL = 0x00;
  Delays(10000);
  TI_ADS1293_WriteRegSettings();
  TI_AFE4400_WriteRegSettings();  
  while(1)                    //���ڲ���
  {
    

    //chr = UartReadChar();
    
    //switch(chr)
    //  {
      //case 'S':
        //UartWriteStr(str1);
        TI_ADS1293_SPIWriteReg(0x00, 0x01);
        TI_AFE4400_SPIAutoIncWriteReg(0x00, 1, 3); //enable read 0x000001ul
        //P2OUT = 0x90;
        //UartWriteChar(q);
        //break;
      //case 'T':
        //UartWriteStr(str2);
        //TI_ADS1293_SPIWriteReg(0x00, 0x00);
        //TI_AFE4400_SPIAutoIncWriteReg(0x00, 0, 3);//enable read
        //UartWriteChar(q);
        //break;
      //case 'M':
        for(uint32_t k = 0;k<1000000;k++)
        //while(1)
        {
          //UartWriteChar('C');
          //UartWriteint(c);
          //UartWriteChar('K');
          //UartWriteint(k);
          chr = UartReadChar();
          switch(chr)
          {
          case 'O':
          TI_ADS1293_SPIWriteReg(TI_ADS1293_CH_CNFG_REG, 0x10);
          break;
          case 'T':
          TI_ADS1293_SPIWriteReg(TI_ADS1293_CH_CNFG_REG, 0x20);
          break;
          case 'H':
          TI_ADS1293_SPIWriteReg(TI_ADS1293_CH_CNFG_REG, 0x40);
          break;
          }
          if(c==1)
          {
            //P2OUT = 0xf0;
            //TI_ADS1293_SPIStreamReadReg(read_buf, count);
            read_buf[0] = 0x5C;
            read_buf[1] = 0xB2;
            read_buf[2] = 0x9E;
            P2OUT ^= BIT6; 
            read_buff[0] = read_buf[0] << 4;
            read_buff[0] = read_buff[0] | (( read_buf[1]>>4 ) & 0x3f);
            read_buff[1] = read_buf[1] << 4;
            read_buff[1] = read_buff[1] | (( read_buf[2] >> 4 )&0x0c );
            _BIS_SR(CPUOFF);
            _NOP();
          }
          else if(c==2)
          {
            //P2OUT = 0xff;
            //TI_ADS1293_SPIStreamReadReg(read_buf, count);
            read_buf[0] = 0x5C;
            read_buf[1] = 0xB1;
            read_buf[2] = 0x24;
            P2OUT ^= BIT6; 
            read_buff[1] = read_buff[1] | (( read_buf[0] >> 2 )&0x03 );
            read_buff[2] = read_buf[0] << 6;
            read_buff[2] = read_buff[2] | (( read_buf[1]>>2 ) & 0x3f);
            read_buff[3] = read_buf[1] << 6;
            read_buff[3] = read_buff[3] | (( read_buf[2] >> 2 )&0x30 );
            _BIS_SR(CPUOFF);
            _NOP();
          }
          else
          {
            c = 0;  
            //P2OUT = 0xf0;
            //TI_ADS1293_SPIStreamReadReg(read_buf, count); 
            read_buf[0] = 0x5C;
            read_buf[1] = 0xB3;
            read_buf[2] = 0x73;
            P2OUT ^= BIT6;
            read_buff[3] = read_buff[3] | ( read_buf[0]&0x0f );
            read_buff[4] = read_buf[1];
            read_buff[5] = read_buf[2] & 0xc0;
            //val = TI_AFE4400_SPIAutoIncReadReg(LED1VAL, count);
            val = 0x10A286;
            P2OUT ^= BIT5;
            read_buff[5] = read_buff[5] | ((val>>15) & 0x3F);
            read_buff[6] = (val>>7) & 0xF8;
            //val = TI_AFE4400_SPIAutoIncReadReg(LED2VAL, count);
            val = 0x15FEBD;
            //P2OUT ^= BIT5;
            read_buff[6] = read_buff[6] | (( val>>19 ) & 0x07);
            read_buff[7] = (val>>11) & 0xFF;
            //Delays(10);
            //read_buff[0] = 0x01;
            //read_buff[1] = 0x02;
            //read_buff[2] = 0x03;
            //read_buff[3] = 0x04;
            //read_buff[4] = 0x05;
            //read_buff[5] = 0x06;
            //read_buff[6] = 0x07;
            //read_buff[7] = 0x08;
            UartWriteint(read_buff[0]);
            UartWriteint(read_buff[1]);
            UartWriteint(read_buff[2]);
            UartWriteint(read_buff[3]);
            UartWriteint(read_buff[4]);
            UartWriteint(read_buff[5]);
            UartWriteint(read_buff[6]);
            UartWriteint(read_buff[7]);
            Delays(10);
            // FOR PROCESSING, WE NEED 0D,OA
            //UartWriteChar(0x0d);
            //UartWriteChar(0x0a);
            _BIS_SR(CPUOFF);
            _NOP();
          }
        }
        
          
        

    }
    
  }



#pragma vector=TIMERA1_VECTOR  
__interrupt void Timer_A1(void)  
{  
    switch(TAIV){  
        case 2:  
            CCR1 += 50000;                    
            //CCR1 += 6667;
            c = c + 1;
            __bic_SR_register_on_exit(CPUOFF);            
    }      
}  
