/*
*********************************************************************************************************
*
*	ģ������ : LEDָʾ������ģ��
*	�ļ����� : bsp_led.h
*	��    �� : V1.2
*	˵    �� : ͷ�ļ�
*
*	Copyright (C), 2019 shenwang@sjtu.edu.cn
*
*********************************************************************************************************
*/

#ifndef __BSP_LED_H
#define __BSP_LED_H

#include "stm32f10x.h"

/** the macro definition to trigger the led on or off 
  * 1 - off
  *0 - on
  */
#define ON  0
#define OFF 1

/*
	�ó���������STM32-F1

	�����������Ӳ�������޸�GPIO����� IsKeyDown1 - IsKeyDown8 ����

	����û���LEDָʾ�Ƹ���С��4�������Խ������LEDȫ������Ϊ�͵�1��LEDһ��������Ӱ�������
*/
	
	/* ����LED���ӵ�GPIO�˿�, �û�ֻ��Ҫ�޸�����Ĵ��뼴�ɸı���Ƶ�LED���� */
	// R-��ɫ	
	
//----------------------------------------------------------------------------
//-------------------�޸�GPIO-------------------------------------------------
//----------------------------------------------------------------------------
#ifdef STM32_F1
	//led1
	#define LED1_GPIO_PORT    	GPIOA			              /* GPIO�˿� */
	#define GPIO_PORT_LED1      GPIOA
	#define LED1_GPIO_CLK 	    RCC_APB2Periph_GPIOA		/* GPIO�˿�ʱ�� */
	#define LED1_GPIO_PIN				GPIO_Pin_4			        /* ���ӵ�SCLʱ���ߵ�GPIO */
	#define GPIO_PIN_LED1				GPIO_Pin_4
	//led2
	#define LED2_GPIO_PORT    	GPIOB			              /* GPIO�˿� */
	#define GPIO_PORT_LED2  		GPIOB
	#define LED2_GPIO_CLK 	    RCC_APB2Periph_GPIOB		/* GPIO�˿�ʱ�� */
	#define LED2_GPIO_PIN				GPIO_Pin_9			        /* ���ӵ�SCLʱ���ߵ�GPIO */
	#define GPIO_PIN_LED2				GPIO_Pin_9
	//led2
	#define LED3_GPIO_PORT    	GPIOB			              /* GPIO�˿� */
	#define GPIO_PORT_LED3  		GPIOB
	#define LED3_GPIO_CLK 	    RCC_APB2Periph_GPIOB		/* GPIO�˿�ʱ�� */
	#define LED3_GPIO_PIN				GPIO_Pin_9			        /* ���ӵ�SCLʱ���ߵ�GPIO */
	#define GPIO_PIN_LED3				GPIO_Pin_9
	#define RCC_ALL_LED 				RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOA	/* �����ڶ�Ӧ��RCCʱ�� */
		//δʹ��LED4 ��LED 4 ����Ϊ LED1
	#define GPIO_PORT_LED4  		GPIOA
	#define GPIO_PIN_LED4				GPIO_Pin_4
#endif
//----------------------------------------------------------------------------
//-------------------�޸�GPIO-------------------------------------------------
//----------------------------------------------------------------------------



/* ʹ�ñ�׼�Ĺ̼������IO*/
#define LED1(a)	if (a)	\
					GPIO_SetBits(LED1_GPIO_PORT,LED1_GPIO_PIN);\
					else		\
					GPIO_ResetBits(LED1_GPIO_PORT,LED1_GPIO_PIN)

#define LED2(a)	if (a)	\
					GPIO_SetBits(LED2_GPIO_PORT,LED2_GPIO_PIN);\
					else		\
					GPIO_ResetBits(LED2_GPIO_PORT,LED2_GPIO_PIN)

#define LED3(a)	if (a)	\
					GPIO_SetBits(LED3_GPIO_PORT,LED3_GPIO_PIN);\
					else		\
					GPIO_ResetBits(LED3_GPIO_PORT,LED3_GPIO_PIN)
					
/* ֱ�Ӳ����Ĵ����ķ�������IO */
#define	digitalHi(p,i)		 {p->BSRR=i;}	 //���Ϊ�ߵ�ƽ		
#define digitalLo(p,i)		 {p->BRR=i;}	 //����͵�ƽ
#define digitalToggle(p,i) {p->ODR ^=i;} //�����ת״̬

/* �������IO�ĺ� */
#define LED1_TOGGLE		 digitalToggle(LED1_GPIO_PORT,LED1_GPIO_PIN)
#define LED1_OFF		   digitalHi(LED1_GPIO_PORT,LED1_GPIO_PIN)
#define LED1_ON			   digitalLo(LED1_GPIO_PORT,LED1_GPIO_PIN)

#define LED2_TOGGLE		 digitalToggle(LED2_GPIO_PORT,LED2_GPIO_PIN)
#define LED2_OFF		   digitalHi(LED2_GPIO_PORT,LED2_GPIO_PIN)
#define LED2_ON			   digitalLo(LED2_GPIO_PORT,LED2_GPIO_PIN)

#define LED3_TOGGLE		 digitalToggle(LED3_GPIO_PORT,LED3_GPIO_PIN)
#define LED3_OFF		   digitalHi(LED3_GPIO_PORT,LED3_GPIO_PIN)
#define LED3_ON			   digitalLo(LED3_GPIO_PORT,LED3_GPIO_PIN)

/* ������ɫ������߼��÷�ʹ��PWM�ɻ��ȫ����ɫ,��Ч������ */

//��
#define LED_RED  \
					LED1_ON;\
					LED2_OFF\
					LED3_OFF

//��
#define LED_GREEN		\
					LED1_OFF;\
					LED2_ON\
					LED3_OFF

//��
#define LED_BLUE	\
					LED1_OFF;\
					LED2_OFF\
					LED3_ON

					
//��(��+��)					
#define LED_YELLOW	\
					LED1_ON;\
					LED2_ON\
					LED3_OFF
//��(��+��)
#define LED_PURPLE	\
					LED1_ON;\
					LED2_OFF\
					LED3_ON

//��(��+��)
#define LED_CYAN \
					LED1_OFF;\
					LED2_ON\
					LED3_ON
					
//��(��+��+��)
#define LED_WHITE	\
					LED1_ON;\
					LED2_ON\
					LED3_ON
					
//��(ȫ���ر�)
#define LED_RGBOFF	\
					LED1_OFF;\
					LED2_OFF\
					LED3_OFF

/* ���ⲿ���õĺ������� */
void bsp_InitLed(void);
void bsp_LedOn(uint8_t _no);
void bsp_LedOff(uint8_t _no);
void bsp_LedToggle(uint8_t _no);
uint8_t bsp_IsLedOn(uint8_t _no);

#endif

/***************************** (END OF FILE) *********************************/
