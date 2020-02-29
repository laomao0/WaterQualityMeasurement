#ifndef __DS18B20_H
#define __DS18B20_H 
#include "sys.h"   
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK战舰STM32开发板
//DS18B20驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/12
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////

//----------------------------------------------------------------------------
//-------------------修改GPIO-------------------------------------------------
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
//-------------------修改GPIO-------------------------------------------------
//----------------------------------------------------------------------------

//IO方向设置 需要修改的 port以及针脚
#define DS18B20_IO_IN()  {PORT_B20->CRH&=CRH_in_and; PORT_B20->CRH|=CRH_in_or;}
#define DS18B20_IO_OUT() {PORT_B20->CRH&=CRH_out_and;PORT_B20->CRH|=CRH_out_or;}
////IO操作函数											   
#define	DS18B20_DQ_OUT p18b20_out(pin_b2n) //数据端口	
#define	DS18B20_DQ_IN  p18b20_in(pin_b2n)  //数据端口	
   	
u8 DS18B20_Init(void);//初始化DS18B20
short DS18B20_Get_Temp(void);//获取温度
void DS18B20_Start(void);//开始温度转换
void DS18B20_Write_Byte(u8 dat);//写入一个字节
u8 DS18B20_Read_Byte(void);//读出一个字节
u8 DS18B20_Read_Bit(void);//读出一个位
u8 DS18B20_Check(void);//检测是否存在DS18B20
void DS18B20_Rst(void);//复位DS18B20    
#endif















