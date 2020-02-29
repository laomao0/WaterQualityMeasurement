#ifndef __DS18B20_H
#define __DS18B20_H 
#include "sys.h"   
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//DS18B20��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/12
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////

//----------------------------------------------------------------------------
//-------------------�޸�GPIO-------------------------------------------------
//----------------------------------------------------------------------------
#define RCC_B20 	RCC_APB2Periph_GPIOB
#define PORT_B20	GPIOB
#define PIN_B20		GPIO_Pin_10
#define pin_b2n 10
#define p18b20_out PBout
#define p18b20_in PBin
#define CRH_in_and  0XFFFFF0FF
#define CRH_in_or 0X00000800
#define CRH_out_and  0XFFFFF0FF
#define CRH_out_or 0X00000300
//#define RCC_B20 	RCC_APB2Periph_GPIOB
//#define PORT_B20	GPIOB
//#define PIN_B20		GPIO_Pin_5
//#define pin_b2n 5
//#define p18b20_out PBout
//#define p18b20_in PBin
//#define CRH_in_and  0XFF0FFFFF
//#define CRH_in_or   0X00800000
//#define CRH_out_and 0XFF0FFFFF
//#define CRH_out_or 0X00300000
//----------------------------------------------------------------------------
//-------------------�޸�GPIO-------------------------------------------------
//----------------------------------------------------------------------------

//IO�������� ��Ҫ�޸ĵ� port�Լ����
#define DS18B20_IO_IN()  {PORT_B20->CRH&=CRH_in_and; PORT_B20->CRH|=CRH_in_or;}
#define DS18B20_IO_OUT() {PORT_B20->CRH&=CRH_out_and;PORT_B20->CRH|=CRH_out_or;}
////IO��������											   
#define	DS18B20_DQ_OUT p18b20_out(pin_b2n) //���ݶ˿�	
#define	DS18B20_DQ_IN  p18b20_in(pin_b2n)  //���ݶ˿�	
   	
u8 DS18B20_Init(void);//��ʼ��DS18B20
short DS18B20_Get_Temp(void);//��ȡ�¶�
void DS18B20_Start(void);//��ʼ�¶�ת��
void DS18B20_Write_Byte(u8 dat);//д��һ���ֽ�
u8 DS18B20_Read_Byte(void);//����һ���ֽ�
u8 DS18B20_Read_Bit(void);//����һ��λ
u8 DS18B20_Check(void);//����Ƿ����DS18B20
void DS18B20_Rst(void);//��λDS18B20    
#endif















