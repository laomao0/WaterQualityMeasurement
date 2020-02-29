#ifndef __RED_H
#define __RED_H 
#include "sys.h"   
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK战舰STM32开发板V3
//红外遥控解码驱动 代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2015/1/15
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////

//key value以及对应的值
/*
				case 0:  str="ERROR";		break;			   
				case 162:str="1";				break;	     //1    
				case 98: str="2";				break;	     //2
				case 2:  str="5";				break;		   //5
				case 226:str="3";				break;		   //3  
				case 194:str="6";				break;	     //6
				case 34: str="4";				break;		   //4
				case 224:str="7";				break;		   //7
				case 168:str="8";				break;		   //8
				case 144:str="9";				break;		   //9
				case 104:str="*";				break;		   //*
				case 152:str="0";				break;	     //0
				case 176:str="#";				break;	     //#
				case 48: str="no1";			break;		    
				case 24: str="up";			break;		   //up
				case 122:str="no2";			break;		  
				case 16: str="left";		break;		   //left   					
				case 56: str="ok";			break;	     //ok
				case 90: str="right";		break;       //right
				case 66: str="no3";			break;
				case 82: str="DELETE";	break;		
*/
//----------------------------------------------------------------------------
//-------------------修改GPIO-------------------------------------------------
//----------------------------------------------------------------------------
//#define RDATA_PIN GPIO_Pin_9
//#define RDATA 	PBin(9)	 	//红外数据输入脚
//#define RDATA_TIM_CH TIM_Channel_4
//#define RDATA_TIM_IT TIM_IT_CC4
//#define RDATA_TIM_GetCapture  TIM_GetCapture4 
//#define RDATA_TIM_OCxNPolarityConfig TIM_OC4PolarityConfig

// PB_8 TIM4 Ch3
#define RDATA_PIN GPIO_Pin_8
#define RDATA 	PBin(8)	 	//红外数据输入脚
#define RDATA_TIM_CH TIM_Channel_3
#define RDATA_TIM_IT TIM_IT_CC3
#define RDATA_TIM_GetCapture  TIM_GetCapture3 
#define RDATA_TIM_OCxNPolarityConfig TIM_OC3PolarityConfig

#define RDATA_RCC RCC_APB2Periph_GPIOB
#define RDATA_PORT GPIOB
#define RDATA_TIM_RCC RCC_APB1Periph_TIM4
#define RDATA_TIM TIM4
//----------------------------------------------------------------------------
//-------------------修改GPIO-------------------------------------------------
//----------------------------------------------------------------------------


//红外遥控识别码(ID),每款遥控器的该值基本都不一样,但也有一样的.
//我们选用的遥控器识别码为0
#define REMOTE_ID 0      		   

extern u8 RmtCnt;			//按键按下的次数

void Remote_Init(void);    	//红外传感器接收头引脚初始化
u8 Remote_Scan(void);	    
#endif















