#ifndef _modbus_
#define _modbus_

#include "stm32f10x_conf.h"

//���������ַ
#define Machine_Address  4

//----------------------------------------------------------------------------
//-------------------�޸�GPIO-------------------------------------------------
//----------------------------------------------------------------------------
#define RS485_CTRL_PORT 		GPIOA
#define RS485_CTRL_PIN 			GPIO_Pin_1
#define RS485_CTRL_RCC 			(RCC_APB2Periph_GPIOA)

#define UART2_BAUD			9600

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
//----------------------------------------------------------------------------
//-------------------�޸�GPIO-------------------------------------------------
//----------------------------------------------------------------------------

#define RS485_RT_1 GPIO_SetBits(RS485_CTRL_PORT, RS485_CTRL_PIN)     //485����״̬
#define RS485_RT_0 GPIO_ResetBits(RS485_CTRL_PORT, RS485_CTRL_PIN)   //485�ý���״̬

typedef struct
{
 u8 myadd;//���豸�ĵ�ַ
 u8 rcbuf[100]; //MODBUS���ջ�����
 u16 timout;//MODbus�����ݶ���ʱ��	
 u8 recount;//MODbus�˿��Ѿ��յ������ݸ���
 u8 timrun;//MODbus��ʱ���Ƿ��ʱ�ı�־
 u8  reflag;//�յ�һ֡���ݵı�־
 u8 Sendbuf[100]; //MODbus���ͻ�����	

}MODBUS;


extern MODBUS modbus;
void Mosbus_Init(void);
void Mosbus_Event(void);
	

#endif

