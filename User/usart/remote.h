#ifndef __RED_H
#define __RED_H 
#include "sys.h"   
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������V3
//����ң�ؽ������� ����	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2015/1/15
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////

//key value�Լ���Ӧ��ֵ
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
//-------------------�޸�GPIO-------------------------------------------------
//----------------------------------------------------------------------------
//#define RDATA_PIN GPIO_Pin_9
//#define RDATA 	PBin(9)	 	//�������������
//#define RDATA_TIM_CH TIM_Channel_4
//#define RDATA_TIM_IT TIM_IT_CC4
//#define RDATA_TIM_GetCapture  TIM_GetCapture4 
//#define RDATA_TIM_OCxNPolarityConfig TIM_OC4PolarityConfig

// PB_8 TIM4 Ch3
#define RDATA_PIN GPIO_Pin_8
#define RDATA 	PBin(8)	 	//�������������
#define RDATA_TIM_CH TIM_Channel_3
#define RDATA_TIM_IT TIM_IT_CC3
#define RDATA_TIM_GetCapture  TIM_GetCapture3 
#define RDATA_TIM_OCxNPolarityConfig TIM_OC3PolarityConfig

#define RDATA_RCC RCC_APB2Periph_GPIOB
#define RDATA_PORT GPIOB
#define RDATA_TIM_RCC RCC_APB1Periph_TIM4
#define RDATA_TIM TIM4
//----------------------------------------------------------------------------
//-------------------�޸�GPIO-------------------------------------------------
//----------------------------------------------------------------------------


//����ң��ʶ����(ID),ÿ��ң�����ĸ�ֵ��������һ��,��Ҳ��һ����.
//����ѡ�õ�ң����ʶ����Ϊ0
#define REMOTE_ID 0      		   

extern u8 RmtCnt;			//�������µĴ���

void Remote_Init(void);    	//���⴫��������ͷ���ų�ʼ��
u8 Remote_Scan(void);	    
#endif















