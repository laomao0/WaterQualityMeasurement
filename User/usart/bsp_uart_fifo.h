/*
*********************************************************************************************************
*
*	ģ������ : �����ж�+FIFO����ģ��
*	�ļ����� : bsp_uart_fifo.h
*	��    �� : V1.0
*	˵    �� : ͷ�ļ�
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
	�����Ҫ���Ĵ��ڶ�Ӧ�Ĺܽţ��������޸� bsp_uart_fifo.c�ļ��е� static void InitHardUart(void)����
*/

/* ����ʹ�ܵĴ���, 0 ��ʾ��ʹ�ܣ������Ӵ����С���� 1��ʾʹ�� */
#ifdef STM32_F1	
	/* STM32-F1�����崮�ڷ��䣺
	  --APB2����
		PA9/USART1_TX	  --- ��ӡ���Կ�
		PA10/USART1_RX
		--APB1����
		PA2/USART2_TX 
		PA3/USART2_RX 
		
		PB10/USART3_TX
		PB11/USART3_RX
		
		PC10/USART4_TX
		PC11/USART4_RX
		
		PC12/USART5_TX
		PD3/USART5_RX
	*/
	#define	UART1_FIFO_EN	1   //Usart1 �������������Դ���
	#define	UART2_FIFO_EN	0   //Usart2 ���� modbus DTU
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

	/* RS485оƬ����ʹ��GPIO, PB2 */
	#define RCC_RS485_TXEN 	RCC_AHB1Periph_GPIOB
	#define PORT_RS485_TXEN  GPIOB
	#define PIN_RS485_TXEN	 GPIO_Pin_2

	#define RS485_RX_EN()	PORT_RS485_TXEN->BSRRH = PIN_RS485_TXEN
	#define RS485_TX_EN()	PORT_RS485_TXEN->BSRRL = PIN_RS485_TXEN
#endif


/* ����˿ں� */
typedef enum
{
	COM1 = 0,	/* USART1  PA9, PA10*/
	COM2 = 1,	/* USART2, PA2, PA3 */
	COM3 = 2,	/* USART3, PB10, PB11 */
	COM4 = 3,	/* UART4, PC10, PC11 */
	COM5 = 4,	/* UART5, PC12, PD2 */
	COM6 = 5	/* USART6, PC6, PC7 STM32-F1δʹ��*/
}COM_PORT_E;

/* �����豸�ṹ�� */
typedef struct
{
	USART_TypeDef *uart;		/* STM32�ڲ������豸ָ�� */
	uint8_t *pTxBuf;			/* ���ͻ����� */
	uint8_t *pRxBuf;			/* ���ջ����� */
	uint16_t usTxBufSize;		/* ���ͻ�������С */
	uint16_t usRxBufSize;		/* ���ջ�������С */
	uint16_t usTxWrite;			/* ���ͻ�����дָ�� */
	uint16_t usTxRead;			/* ���ͻ�������ָ�� */
	uint16_t usTxCount;			/* �ȴ����͵����ݸ��� */
	
	uint16_t usRxWrite;			/* ���ջ�����дָ�� */
	uint16_t usRxRead;			/* ���ջ�������ָ�� */
	uint16_t usRxCount;			/* ��δ��ȡ�������ݸ��� */
	
	void (*SendBefor)(void); 	/* ��ʼ����֮ǰ�Ļص�����ָ�루��Ҫ����RS485�л�������ģʽ�� */
	void (*SendOver)(void); 	/* ������ϵĻص�����ָ�루��Ҫ����RS485������ģʽ�л�Ϊ����ģʽ�� */
	void (*ReciveNew)(uint8_t);	//�����յ����ݵĻص�����ָ�룬�򵥵ظ����յ��ĵ��Թ���ָ�������
}UART_T;

/* ���崮�ڲ����ʺ�FIFO��������С����Ϊ���ͻ������ͽ��ջ�����, ֧��ȫ˫�� */
#if UART1_FIFO_EN == 1

	//----------------------------------------------------------------------------
	//-------------------�޸�GPIO-------------------------------------------------
	//----------------------------------------------------------------------------
		#define UART1_BAUD						115200
		#define UART1_TX_BUF_SIZE			1*1024
		#define UART1_RX_BUF_SIZE			1*1024
		
			// ����1-USART1
		#define  DEBUG_USARTx                   USART1
		#define  DEBUG_USART_CLK                RCC_APB2Periph_USART1
		#define  DEBUG_USART_APBxClkCmd         RCC_APB2PeriphClockCmd
		#define  DEBUG_USART_BAUDRATE           UART1_BAUD

		// USART GPIO ���ź궨��
		#define  DEBUG_USART_GPIO_CLK           (RCC_APB2Periph_GPIOA)
		#define  DEBUG_USART_GPIO_APBxClkCmd    RCC_APB2PeriphClockCmd
				
		#define  DEBUG_USART_TX_GPIO_PORT       GPIOA   
		#define  DEBUG_USART_TX_GPIO_PIN        GPIO_Pin_9
		#define  DEBUG_USART_RX_GPIO_PORT       GPIOA
		#define  DEBUG_USART_RX_GPIO_PIN        GPIO_Pin_10

		#define  DEBUG_USART_IRQ                USART1_IRQn
		#define  DEBUG_USART_IRQHandler         USART1_IRQHandler
	//----------------------------------------------------------------------------
	//-------------------�޸�GPIO-------------------------------------------------
	//----------------------------------------------------------------------------
	
#endif

#if UART2_FIFO_EN == 1
	#define UART2_BAUD			9600
	#define UART2_TX_BUF_SIZE	100
	#define UART2_RX_BUF_SIZE 100
	
	// ����2-USART2
	#define  DEBUG_USART2                    USART2
	#define  DEBUG_USART2_CLK                RCC_APB1Periph_USART2
	#define  DEBUG_USART2_APBxClkCmd         RCC_APB1PeriphClockCmd
	#define  DEBUG_USART2_BAUDRATE           UART2_BAUD

	//// USART GPIO ���ź궨��
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
	// ����3-USART3
	//#define  DEBUG_USARTx                   USART3
	//#define  DEBUG_USART_CLK                RCC_APB1Periph_USART3
	//#define  DEBUG_USART_APBxClkCmd         RCC_APB1PeriphClockCmd
	//#define  DEBUG_USART_BAUDRATE           115200

	//// USART GPIO ���ź궨��
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
	// ����4-UART4
	//#define  DEBUG_USARTx                   UART4
	//#define  DEBUG_USART_CLK                RCC_APB1Periph_UART4
	//#define  DEBUG_USART_APBxClkCmd         RCC_APB1PeriphClockCmd
	//#define  DEBUG_USART_BAUDRATE           115200

	//// USART GPIO ���ź궨��
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
	// ����5-UART5
	//#define  DEBUG_USARTx                   UART5
	//#define  DEBUG_USART_CLK                RCC_APB1Periph_UART5
	//#define  DEBUG_USART_APBxClkCmd         RCC_APB1PeriphClockCmd
	//#define  DEBUG_USART_BAUDRATE           115200

	//// USART GPIO ���ź궨��
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

/* ����ÿ�����ڽṹ����� */
#if UART1_FIFO_EN == 1
	static UART_T g_tUart1;
	static uint8_t g_TxBuf1[UART1_TX_BUF_SIZE];		/* ���ͻ����� */
	static uint8_t g_RxBuf1[UART1_RX_BUF_SIZE];		/* ���ջ����� */
#endif

#if UART2_FIFO_EN == 1
	static UART_T g_tUart2;
	static uint8_t g_TxBuf2[UART2_TX_BUF_SIZE];		/* ���ͻ����� */
	static uint8_t g_RxBuf2[UART2_RX_BUF_SIZE];		/* ���ջ����� */
#endif

#if UART3_FIFO_EN == 1
	static UART_T g_tUart3;
	static uint8_t g_TxBuf3[UART3_TX_BUF_SIZE];		/* ���ͻ����� */
	static uint8_t g_RxBuf3[UART3_RX_BUF_SIZE];		/* ���ջ����� */
#endif

#if UART4_FIFO_EN == 1
	static UART_T g_tUart4;
	static uint8_t g_TxBuf4[UART4_TX_BUF_SIZE];		/* ���ͻ����� */
	static uint8_t g_RxBuf4[UART4_RX_BUF_SIZE];		/* ���ջ����� */
#endif

#if UART5_FIFO_EN == 1
	static UART_T g_tUart5;
	static uint8_t g_TxBuf5[UART5_TX_BUF_SIZE];		/* ���ͻ����� */
	static uint8_t g_RxBuf5[UART5_RX_BUF_SIZE];		/* ���ջ����� */
#endif

#if UART6_FIFO_EN == 1
	static UART_T g_tUart6;
	static uint8_t g_TxBuf6[UART6_TX_BUF_SIZE];		/* ���ͻ����� */
	static uint8_t g_RxBuf6[UART6_RX_BUF_SIZE];		/* ���ջ����� */
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
