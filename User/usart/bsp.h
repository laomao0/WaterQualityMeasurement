/*
*********************************************************************************************************
*
*	模块名称 : BSP模块
*	文件名称 : bsp.h
*	说    明 : 这是底层驱动模块所有的h文件的汇总文件。 应用程序只需 #include bsp.h 即可，
*			  不需要#include 每个模块的 h 文件
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

//宏定义用来区分采用什么型号的开发板
//#define STM32_Fx
#define STM32_F1

/* 检查是否定义了开发板型号 */
#if !defined (STM32_Fx) && !defined (STM32_F1)
	#error "Please define the board model : STM32_F1 or STM32_V5"
#endif

/* 定义 BSP 版本号 */
#define __STM32F1_BSP_VERSION		"1.2"

/* 开关全局中断的宏 */
#define ENABLE_INT()	__set_PRIMASK(0)	/* 使能全局中断 */
#define DISABLE_INT()	__set_PRIMASK(1)	/* 禁止全局中断 */

/* 这个宏仅用于调试阶段排错 */
#define BSP_Printf		printf
//#define BSP_Printf(...)

#ifndef TRUE
	#define TRUE  1
#endif

#ifndef FALSE
	#define FALSE 0
#endif

#define ad7705 0  //1 -7705 1-7706


//需要修改的设置
//bsp_led.h         3 个gpio
//bsp_tm7705.h      6 个gpio
//bsp_uart_fifo.h   usart1  2个gpio
//ds18b20.h         1个gpio
//modbus.h          usart2 2-gpio   rs485 control  1-gpio
//oled.h            ii2c  sda,scl 2-gpio
//bsp_tm7705.h      若采用ad7706 .c文件中已经在自校准模式中加入7706的支持，但需要将单极性改为双极性，通过修改define来选择


/* 通过取消注释或者添加注释的方式控制是否包含底层驱动模块 */
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
//#include "delay.h"  //已用bsp_timer.h 功能相同

/* 提供给其他C文件调用的函数 */
void bsp_Init(void);
void bsp_Idle(void);

#endif

