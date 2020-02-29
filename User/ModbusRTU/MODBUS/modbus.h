#ifndef _modbus_
#define _modbus_

#include "stm32f10x_conf.h"

//定义机器地址
#define Machine_Address  4

//----------------------------------------------------------------------------
//-------------------修改GPIO-------------------------------------------------
//----------------------------------------------------------------------------
#define RS485_CTRL_PORT 		GPIOA
#define RS485_CTRL_PIN 			GPIO_Pin_1
#define RS485_CTRL_RCC 			(RCC_APB2Periph_GPIOA)

#define UART2_BAUD			9600

// 串口2-USART2
#define  DEBUG_USART2                    USART2
#define  DEBUG_USART2_CLK                RCC_APB1Periph_USART2
#define  DEBUG_USART2_APBxClkCmd         RCC_APB1PeriphClockCmd
#define  DEBUG_USART2_BAUDRATE           UART2_BAUD

//// USART GPIO 引脚宏定义
#define  DEBUG_USART2_GPIO_CLK           (RCC_APB2Periph_GPIOA)
#define  DEBUG_USART2_GPIO_APBxClkCmd    RCC_APB2PeriphClockCmd
//    
#define  DEBUG_USART2_TX_GPIO_PORT       GPIOA   
#define  DEBUG_USART2_TX_GPIO_PIN        GPIO_Pin_2
#define  DEBUG_USART2_RX_GPIO_PORT       GPIOA
#define  DEBUG_USART2_RX_GPIO_PIN        GPIO_Pin_3

#define  DEBUG_USART2_IRQ                USART2_IRQn
#define  DEBUG_USART2_IRQHandler         USART2_IRQHandler
//----------------------------------------------------------------------------
//-------------------修改GPIO-------------------------------------------------
//----------------------------------------------------------------------------

#define RS485_RT_1 GPIO_SetBits(RS485_CTRL_PORT, RS485_CTRL_PIN)     //485发送状态
#define RS485_RT_0 GPIO_ResetBits(RS485_CTRL_PORT, RS485_CTRL_PIN)   //485置接收状态

typedef struct
{
 u8 myadd;//本设备的地址
 u8 rcbuf[100]; //MODBUS接收缓冲区
 u16 timout;//MODbus的数据断续时间	
 u8 recount;//MODbus端口已经收到的数据个数
 u8 timrun;//MODbus定时器是否计时的标志
 u8  reflag;//收到一帧数据的标志
 u8 Sendbuf[100]; //MODbus发送缓冲区	

}MODBUS;


extern MODBUS modbus;
void Mosbus_Init(void);
void Mosbus_Event(void);
	

#endif

