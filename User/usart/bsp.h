/*
*********************************************************************************************************
*
*	ģ������ : BSPģ��
*	�ļ����� : bsp.h
*	˵    �� : ���ǵײ�����ģ�����е�h�ļ��Ļ����ļ��� Ӧ�ó���ֻ�� #include bsp.h ���ɣ�
*			  ����Ҫ#include ÿ��ģ��� h �ļ�
*
*	Copyright (C), 2019, shenwang@sjtu.edu.cn
*
*********************************************************************************************************
*/

#ifndef _BSP_H
#define _BSP_H

#include "stm32f10x.h"
#include <stdio.h>
#include <string.h>

//�궨���������ֲ���ʲô�ͺŵĿ�����
//#define STM32_Fx
#define STM32_F1

/* ����Ƿ����˿������ͺ� */
#if !defined (STM32_Fx) && !defined (STM32_F1)
	#error "Please define the board model : STM32_F1 or STM32_V5"
#endif

/* ���� BSP �汾�� */
#define __STM32F1_BSP_VERSION		"1.2"

/* ����ȫ���жϵĺ� */
#define ENABLE_INT()	__set_PRIMASK(0)	/* ʹ��ȫ���ж� */
#define DISABLE_INT()	__set_PRIMASK(1)	/* ��ֹȫ���ж� */

/* ���������ڵ��Խ׶��Ŵ� */
#define BSP_Printf		printf
//#define BSP_Printf(...)

#ifndef TRUE
	#define TRUE  1
#endif

#ifndef FALSE
	#define FALSE 0
#endif

#define ad7705 0  //1 -7705 1-7706


//��Ҫ�޸ĵ�����
//bsp_led.h         3 ��gpio
//bsp_tm7705.h      6 ��gpio
//bsp_uart_fifo.h   usart1  2��gpio
//ds18b20.h         1��gpio
//modbus.h          usart2 2-gpio   rs485 control  1-gpio
//oled.h            ii2c  sda,scl 2-gpio
//bsp_tm7705.h      ������ad7706 .c�ļ����Ѿ�����У׼ģʽ�м���7706��֧�֣�����Ҫ�������Ը�Ϊ˫���ԣ�ͨ���޸�define��ѡ��


/* ͨ��ȡ��ע�ͻ������ע�͵ķ�ʽ�����Ƿ�����ײ�����ģ�� */
#include "bsp_uart_fifo.h"
#include "bsp_led.h"
#include "bsp_timer.h" //systick timer
//#include "bsp_key.h" 
#include "bsp_tm7705.h"
#include "timer.h" // real timer
#include "modbus_uart.h"
#include "modbus.h" 
#include "bsp_iwdg.h"   
#include "ds18b20.h"
#include "sys.h"
#include "oled.h"
#include "remote.h"
#include "processing.h"
#include "bsp_i2c_ee.h"
//#include "delay.h"  //����bsp_timer.h ������ͬ

/* �ṩ������C�ļ����õĺ��� */
void bsp_Init(void);
void bsp_Idle(void);

#endif

