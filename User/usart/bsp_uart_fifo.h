/*
*********************************************************************************************************
*
*	模块名称 : 串口中断+FIFO驱动模块
*	文件名称 : bsp_uart_fifo.h
*	版    本 : V1.0
*	说    明 : 头文件
*
*	Copyright (C), 2019, shenwang@sjtu.edu.cn
*
*********************************************************************************************************
*/

#ifndef _BSP_USART_FIFO_H_
#define _BSP_USART_FIFO_H_

#include "stm32f10x.h"
#include "bsp.h"

/*
	如果需要更改串口对应的管脚，请自行修改 bsp_uart_fifo.c文件中的 static void InitHardUart(void)函数
*/

/* 定义使能的串口, 0 表示不使能（不增加代码大小）， 1表示使能 */
#ifdef STM32_F1	
	/* STM32-F1开发板串口分配：
	  --APB2总线
		PA9/USART1_TX	  --- 打印调试口
		PA10/USART1_RX
		--APB1总线
		PA2/USART2_TX 
		PA3/USART2_RX 
		
		PB10/USART3_TX
		PB11/USART3_RX
		
		PC10/USART4_TX
		PC11/USART4_RX
		
		PC12/USART5_TX
		PD3/USART5_RX
	*/
	#define	UART1_FIFO_EN	1   //Usart1 用作传感器调试窗口
	#define	UART2_FIFO_EN	0   //Usart2 用作 modbus DTU
	#define	UART3_FIFO_EN	0
	#define	UART4_FIFO_EN	0
	#define	UART5_FIFO_EN	0
	#define	UART6_FIFO_EN	0
#else
	#define	UART1_FIFO_EN	1
	#define	UART2_FIFO_EN	1
	#define	UART3_FIFO_EN	1
	#define	UART4_FIFO_EN	0
	#define	UART5_FIFO_EN	0
	#define	UART6_FIFO_EN	1

	/* RS485芯片发送使能GPIO, PB2 */
	#define RCC_RS485_TXEN 	RCC_AHB1Periph_GPIOB
	#define PORT_RS485_TXEN  GPIOB
	#define PIN_RS485_TXEN	 GPIO_Pin_2

	#define RS485_RX_EN()	PORT_RS485_TXEN->BSRRH = PIN_RS485_TXEN
	#define RS485_TX_EN()	PORT_RS485_TXEN->BSRRL = PIN_RS485_TXEN
#endif


/* 定义端口号 */
typedef enum
{
	COM1 = 0,	/* USART1  PA9, PA10*/
	COM2 = 1,	/* USART2, PA2, PA3 */
	COM3 = 2,	/* USART3, PB10, PB11 */
	COM4 = 3,	/* UART4, PC10, PC11 */
	COM5 = 4,	/* UART5, PC12, PD2 */
	COM6 = 5	/* USART6, PC6, PC7 STM32-F1未使用*/
}COM_PORT_E;

/* 串口设备结构体 */
typedef struct
{
	USART_TypeDef *uart;		/* STM32内部串口设备指针 */
	uint8_t *pTxBuf;			/* 发送缓冲区 */
	uint8_t *pRxBuf;			/* 接收缓冲区 */
	uint16_t usTxBufSize;		/* 发送缓冲区大小 */
	uint16_t usRxBufSize;		/* 接收缓冲区大小 */
	uint16_t usTxWrite;			/* 发送缓冲区写指针 */
	uint16_t usTxRead;			/* 发送缓冲区读指针 */
	uint16_t usTxCount;			/* 等待发送的数据个数 */
	
	uint16_t usRxWrite;			/* 接收缓冲区写指针 */
	uint16_t usRxRead;			/* 接收缓冲区读指针 */
	uint16_t usRxCount;			/* 还未读取的新数据个数 */
	
	void (*SendBefor)(void); 	/* 开始发送之前的回调函数指针（主要用于RS485切换到发送模式） */
	void (*SendOver)(void); 	/* 发送完毕的回调函数指针（主要用于RS485将发送模式切换为接收模式） */
	void (*ReciveNew)(uint8_t);	//串口收到数据的回调函数指针，简单地根据收到的调试功能指令发送数据
}UART_T;

/* 定义串口波特率和FIFO缓冲区大小，分为发送缓冲区和接收缓冲区, 支持全双工 */
#if UART1_FIFO_EN == 1

	//----------------------------------------------------------------------------
	//-------------------修改GPIO-------------------------------------------------
	//----------------------------------------------------------------------------
		#define UART1_BAUD						115200
		#define UART1_TX_BUF_SIZE			1*1024
		#define UART1_RX_BUF_SIZE			1*1024
		
			// 串口1-USART1
		#define  DEBUG_USARTx                   USART1
		#define  DEBUG_USART_CLK                RCC_APB2Periph_USART1
		#define  DEBUG_USART_APBxClkCmd         RCC_APB2PeriphClockCmd
		#define  DEBUG_USART_BAUDRATE           UART1_BAUD

		// USART GPIO 引脚宏定义
		#define  DEBUG_USART_GPIO_CLK           (RCC_APB2Periph_GPIOA)
		#define  DEBUG_USART_GPIO_APBxClkCmd    RCC_APB2PeriphClockCmd
				
		#define  DEBUG_USART_TX_GPIO_PORT       GPIOA   
		#define  DEBUG_USART_TX_GPIO_PIN        GPIO_Pin_9
		#define  DEBUG_USART_RX_GPIO_PORT       GPIOA
		#define  DEBUG_USART_RX_GPIO_PIN        GPIO_Pin_10

		#define  DEBUG_USART_IRQ                USART1_IRQn
		#define  DEBUG_USART_IRQHandler         USART1_IRQHandler
	//----------------------------------------------------------------------------
	//-------------------修改GPIO-------------------------------------------------
	//----------------------------------------------------------------------------
	
#endif

#if UART2_FIFO_EN == 1
	#define UART2_BAUD			9600
	#define UART2_TX_BUF_SIZE	100
	#define UART2_RX_BUF_SIZE 100
	
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
#endif

#if UART3_FIFO_EN == 1
	#define UART3_BAUD			9600
	#define UART3_TX_BUF_SIZE	1*1024
	#define UART3_RX_BUF_SIZE	1*1024
	// 串口3-USART3
	//#define  DEBUG_USARTx                   USART3
	//#define  DEBUG_USART_CLK                RCC_APB1Periph_USART3
	//#define  DEBUG_USART_APBxClkCmd         RCC_APB1PeriphClockCmd
	//#define  DEBUG_USART_BAUDRATE           115200

	//// USART GPIO 引脚宏定义
	//#define  DEBUG_USART_GPIO_CLK           (RCC_APB2Periph_GPIOB)
	//#define  DEBUG_USART_GPIO_APBxClkCmd    RCC_APB2PeriphClockCmd
	//    
	//#define  DEBUG_USART_TX_GPIO_PORT       GPIOB   
	//#define  DEBUG_USART_TX_GPIO_PIN        GPIO_Pin_10
	//#define  DEBUG_USART_RX_GPIO_PORT       GPIOB
	//#define  DEBUG_USART_RX_GPIO_PIN        GPIO_Pin_11

	//#define  DEBUG_USART_IRQ                USART3_IRQn
	//#define  DEBUG_USART_IRQHandler         USART3_IRQHandler
#endif

#if UART4_FIFO_EN == 1
	#define UART4_BAUD			115200
	#define UART4_TX_BUF_SIZE	1*1024
	#define UART4_RX_BUF_SIZE	1*1024
	// 串口4-UART4
	//#define  DEBUG_USARTx                   UART4
	//#define  DEBUG_USART_CLK                RCC_APB1Periph_UART4
	//#define  DEBUG_USART_APBxClkCmd         RCC_APB1PeriphClockCmd
	//#define  DEBUG_USART_BAUDRATE           115200

	//// USART GPIO 引脚宏定义
	//#define  DEBUG_USART_GPIO_CLK           (RCC_APB2Periph_GPIOC)
	//#define  DEBUG_USART_GPIO_APBxClkCmd    RCC_APB2PeriphClockCmd
	//    
	//#define  DEBUG_USART_TX_GPIO_PORT       GPIOC   
	//#define  DEBUG_USART_TX_GPIO_PIN        GPIO_Pin_10
	//#define  DEBUG_USART_RX_GPIO_PORT       GPIOC
	//#define  DEBUG_USART_RX_GPIO_PIN        GPIO_Pin_11

	//#define  DEBUG_USART_IRQ                UART4_IRQn
	//#define  DEBUG_USART_IRQHandler         UART4_IRQHandler
#endif

#if UART5_FIFO_EN == 1
	#define UART5_BAUD			115200
	#define UART5_TX_BUF_SIZE	1*1024
	#define UART5_RX_BUF_SIZE	1*1024
	// 串口5-UART5
	//#define  DEBUG_USARTx                   UART5
	//#define  DEBUG_USART_CLK                RCC_APB1Periph_UART5
	//#define  DEBUG_USART_APBxClkCmd         RCC_APB1PeriphClockCmd
	//#define  DEBUG_USART_BAUDRATE           115200

	//// USART GPIO 引脚宏定义
	//#define  DEBUG_USART_GPIO_CLK           (RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD)
	//#define  DEBUG_USART_GPIO_APBxClkCmd    RCC_APB2PeriphClockCmd
	//    
	//#define  DEBUG_USART_TX_GPIO_PORT       GPIOC   
	//#define  DEBUG_USART_TX_GPIO_PIN        GPIO_Pin_12
	//#define  DEBUG_USART_RX_GPIO_PORT       GPIOD
	//#define  DEBUG_USART_RX_GPIO_PIN        GPIO_Pin_2

	//#define  DEBUG_USART_IRQ                UART5_IRQn
	//#define  DEBUG_USART_IRQHandler         UART5_IRQHandler
#endif

#if UART6_FIFO_EN == 1
	#define UART6_BAUD			115200
	#define UART6_TX_BUF_SIZE	1*1024
	#define UART6_RX_BUF_SIZE	1*1024
#endif

/* 定义每个串口结构体变量 */
#if UART1_FIFO_EN == 1
	static UART_T g_tUart1;
	static uint8_t g_TxBuf1[UART1_TX_BUF_SIZE];		/* 发送缓冲区 */
	static uint8_t g_RxBuf1[UART1_RX_BUF_SIZE];		/* 接收缓冲区 */
#endif

#if UART2_FIFO_EN == 1
	static UART_T g_tUart2;
	static uint8_t g_TxBuf2[UART2_TX_BUF_SIZE];		/* 发送缓冲区 */
	static uint8_t g_RxBuf2[UART2_RX_BUF_SIZE];		/* 接收缓冲区 */
#endif

#if UART3_FIFO_EN == 1
	static UART_T g_tUart3;
	static uint8_t g_TxBuf3[UART3_TX_BUF_SIZE];		/* 发送缓冲区 */
	static uint8_t g_RxBuf3[UART3_RX_BUF_SIZE];		/* 接收缓冲区 */
#endif

#if UART4_FIFO_EN == 1
	static UART_T g_tUart4;
	static uint8_t g_TxBuf4[UART4_TX_BUF_SIZE];		/* 发送缓冲区 */
	static uint8_t g_RxBuf4[UART4_RX_BUF_SIZE];		/* 接收缓冲区 */
#endif

#if UART5_FIFO_EN == 1
	static UART_T g_tUart5;
	static uint8_t g_TxBuf5[UART5_TX_BUF_SIZE];		/* 发送缓冲区 */
	static uint8_t g_RxBuf5[UART5_RX_BUF_SIZE];		/* 接收缓冲区 */
#endif

#if UART6_FIFO_EN == 1
	static UART_T g_tUart6;
	static uint8_t g_TxBuf6[UART6_TX_BUF_SIZE];		/* 发送缓冲区 */
	static uint8_t g_RxBuf6[UART6_RX_BUF_SIZE];		/* 接收缓冲区 */
#endif


void bsp_InitUart(void);
void comSendBuf(COM_PORT_E _ucPort, uint8_t *_ucaBuf, uint16_t _usLen);
void comSendChar(COM_PORT_E _ucPort, uint8_t _ucByte);
uint8_t comGetChar(COM_PORT_E _ucPort, uint8_t *_pByte);

void comClearTxFifo(COM_PORT_E _ucPort);
void comClearRxFifo(COM_PORT_E _ucPort);
void PrintfLogo(void);

#endif

/***************************** (END OF FILE) *********************************/
