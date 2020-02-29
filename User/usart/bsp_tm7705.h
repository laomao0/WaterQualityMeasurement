/*
*********************************************************************************************************
*
*	ģ������ : TM7705 ����ģ��(2ͨ����PGA��16λADC)
*	�ļ����� : bsp_tm7705.h
*
*	Copyright (C), 2013-2014, ���������� www.armfly.com
*
*********************************************************************************************************
*/

#ifndef _BSP_TM7705_H
#define _BSP_TM7705_H

#include "stm32f10x.h"

#define SOFT_SPI		/* ������б�ʾʹ��GPIOģ��SPI�ӿ� */
//#define HARD_SPI		/* ������б�ʾʹ��CPU��Ӳ��SPI�ӿ� */

//-------------------------AD ����ģʽ�޸�
#if ad7705 == 1
/* ͨ��1��ͨ��2������,���뻺�壬���� */
	#define __CH1_GAIN_BIPOLAR_BUF	(GAIN_1 | UNIPOLAR | BUF_EN)
	#define __CH2_GAIN_BIPOLAR_BUF	(GAIN_1 | UNIPOLAR | BUF_EN)
	#define __CH3_GAIN_BIPOLAR_BUF	(GAIN_1 | UNIPOLAR | BUF_EN)
	#define __CH4_GAIN_BIPOLAR_BUF	(GAIN_1 | UNIPOLAR | BUF_EN)
#else
	#define __CH1_GAIN_BIPOLAR_BUF	(GAIN_1 | BIPOLAR | BUF_EN)
	#define __CH2_GAIN_BIPOLAR_BUF	(GAIN_1 | BIPOLAR | BUF_EN)
	#define __CH3_GAIN_BIPOLAR_BUF	(GAIN_1 | BIPOLAR | BUF_EN)
	#define __CH4_GAIN_BIPOLAR_BUF	(GAIN_1 | BIPOLAR | BUF_EN)
#endif



#if !defined(SOFT_SPI) && !defined(HARD_SPI)
 	#error "Please define SPI Interface mode : SOFT_SPI or HARD_SPI"
#endif

#ifdef SOFT_SPI		/* ���SPI */
/*
	TM7705ģ�����ֱ�Ӳ嵽STM32-V5������nRF24L01ģ�����ĸ�ӿ��ϡ�

    TM7705ģ��   STM32F1 ������
      SCK   ------  PA5/SPI3_SCK
      DOUT  ------  PA6/SPI3_MISO
      DIN   ------  PA7/SPI3_MOSI
      CS    ------  PC6/NRF24L01_CSN
      DRDY  ------  PC4/NRF24L01_IRQ
      RST   ------  PC5/NRF905_TX_EN/NRF24L01_CE/DAC1_OUT	(��λ RESET)
*/
//----------------------------------------------------------------------------
//-------------------�޸�GPIO-------------------------------------------------
//----------------------------------------------------------------------------
	/* ����GPIO�˿� */
	#define RCC_SCK 	RCC_APB2Periph_GPIOB
	#define PORT_SCK	GPIOB
	#define PIN_SCK		GPIO_Pin_12

	#define RCC_DIN 	RCC_APB2Periph_GPIOB
	#define PORT_DIN	GPIOB
	#define PIN_DIN		GPIO_Pin_14

	#define RCC_DOUT 	RCC_APB2Periph_GPIOB
	#define PORT_DOUT	GPIOB
	#define PIN_DOUT	GPIO_Pin_15

	#define RCC_CS 		RCC_APB2Periph_GPIOB
	#define PORT_CS		GPIOB
	#define PIN_CS		GPIO_Pin_13

	#define RCC_DRDY 	RCC_APB2Periph_GPIOA
	#define PORT_DRDY	GPIOA
	#define PIN_DRDY	GPIO_Pin_8

	#define RCC_RESET 	RCC_APB2Periph_GPIOB
	#define PORT_RESET	GPIOB
	#define PIN_RESET	GPIO_Pin_5
	//----------------------------------------------------------------------------
	//-------------------�޸�GPIO-------------------------------------------------
	//----------------------------------------------------------------------------

	/* ���������0����1�ĺ� */
	#define RESET_0()	GPIO_ResetBits(PORT_RESET, PIN_RESET)
	#define RESET_1()	GPIO_SetBits(PORT_RESET, PIN_RESET)

	#define CS_0()		GPIO_ResetBits(PORT_CS, PIN_CS)
	#define CS_1()		GPIO_SetBits(PORT_CS, PIN_CS)

	#define SCK_0()		GPIO_ResetBits(PORT_SCK, PIN_SCK)
	#define SCK_1()		GPIO_SetBits(PORT_SCK, PIN_SCK)

	#define DI_0()		GPIO_ResetBits(PORT_DIN, PIN_DIN)
	#define DI_1()		GPIO_SetBits(PORT_DIN, PIN_DIN)

	#define DO_IS_HIGH()	(GPIO_ReadInputDataBit(PORT_DOUT, PIN_DOUT) == Bit_SET)

	#define DRDY_IS_LOW()	(GPIO_ReadInputDataBit(PORT_DRDY, PIN_DRDY) == Bit_RESET)
#endif

#ifdef HARD_SPI		/* Ӳ��SPI */
	;
#endif

/* ͨ�żĴ���bit���� */
enum
{
	/* �Ĵ���ѡ��  RS2 RS1 RS0  */
	REG_COMM	= 0x00,	/* ͨ�żĴ��� */
	REG_SETUP	= 0x10,	/* ���üĴ��� */
	REG_CLOCK	= 0x20,	/* ʱ�ӼĴ��� */
	REG_DATA	= 0x30,	/* ���ݼĴ��� */
	REG_ZERO_CH1	= 0x60,	/* CH1 ƫ�ƼĴ��� */
	REG_FULL_CH1	= 0x70,	/* CH1 �����̼Ĵ��� */
	REG_ZERO_CH2	= 0x61,	/* CH2 ƫ�ƼĴ��� */
	REG_FULL_CH2	= 0x71,	/* CH2 �����̼Ĵ��� */

	/* ��д���� */
	WRITE 		= 0x00,	/* д���� */
	READ 		= 0x08,	/* ������ */

	//ͨ������         AD7705ͨ��ѡ��      AD7706ͨ��ѡ��
	CH_1		= 0,	/* AIN1+  AIN1-						AIN1     COMMON */
	CH_2		= 1,	/* AIN2+  AIN2- 					AIN2     COMMON */
	CH_3		= 2,	/* AIN1-  AIN1- 					COMMON 	 COMMON	*/
	CH_4		= 3		/* AIN1-  AIN2- 					AIN#     COMMON */
};

/* ���üĴ���bit���� */
enum
{
	MD_NORMAL		= (0 << 6),	/* ����ģʽ */
	MD_CAL_SELF		= (1 << 6),	/* ��У׼ģʽ */
	MD_CAL_ZERO		= (2 << 6),	/* У׼0�̶�ģʽ */
	MD_CAL_FULL		= (3 << 6),	/* У׼���̶�ģʽ */

	GAIN_1			= (0 << 3),	/* ���� */
	GAIN_2			= (1 << 3),	/* ���� */
	GAIN_4			= (2 << 3),	/* ���� */
	GAIN_8			= (3 << 3),	/* ���� */
	GAIN_16			= (4 << 3),	/* ���� */
	GAIN_32			= (5 << 3),	/* ���� */
	GAIN_64			= (6 << 3),	/* ���� */
	GAIN_128		= (7 << 3),	/* ���� */

	/* ����˫���Ի��ǵ����Զ����ı��κ������źŵ�״̬����ֻ�ı�������ݵĴ����ת�������ϵ�У׼�� */
	BIPOLAR			= (0 << 2),	/* ˫�������� */
	UNIPOLAR		= (1 << 2),	/* ���������� */

	BUF_NO			= (0 << 1),	/* �����޻��壨�ڲ�������������) */
	BUF_EN			= (1 << 1),	/* �����л��� (�����ڲ�������) */

	FSYNC_0			= 0,
	FSYNC_1			= 1		/* ������ */
};

/* ʱ�ӼĴ���bit���� */
enum
{
	CLKDIS_0	= 0x00,		/* ʱ�����ʹ�� ������Ӿ���ʱ������ʹ�ܲ����񵴣� */
	CLKDIS_1	= 0x10,		/* ʱ�ӽ�ֹ �����ⲿ�ṩʱ��ʱ�����ø�λ���Խ�ֹMCK_OUT�������ʱ����ʡ�� */

	/*
		2.4576MHz��CLKDIV=0 ����Ϊ 4.9152MHz ��CLKDIV=1 ����CLK Ӧ�� ��0����
		1MHz ��CLKDIV=0 ���� 2MHz   ��CLKDIV=1 ����CLK ��λӦ��  ��1��
	*/
	CLK_4_9152M = 0x08,
	CLK_2_4576M = 0x00,
	CLK_1M 		= 0x04,
	CLK_2M 		= 0x0C,

	FS_50HZ		= 0x00,
	FS_60HZ		= 0x01,
	FS_250HZ	= 0x02,
	FS_500HZ	= 0x04,

	/*
		��ʮ�š����ӳ�Ӧ�������TM7705 ���ȵķ���
			��ʹ����ʱ��Ϊ 2.4576MHz ʱ��ǿ�ҽ��齫ʱ�ӼĴ�����Ϊ 84H,��ʱ�������������Ϊ10Hz,��ÿ0.1S ���һ�������ݡ�
			��ʹ����ʱ��Ϊ 1MHz ʱ��ǿ�ҽ��齫ʱ�ӼĴ�����Ϊ80H, ��ʱ�������������Ϊ4Hz, ��ÿ0.25S ���һ��������
	*/
	ZERO_0		= 0x00,
	ZERO_1		= 0x80
};


void bsp_InitTM7705(void);
uint8_t TM7705_CalibSelf(uint8_t _ch);
void TM7705_SytemCalibZero(uint8_t _ch);
void TM7705_SytemCalibFull(uint8_t _ch);
uint16_t TM7705_ReadAdc(uint8_t _ch);

void TM7705_WriteReg(uint8_t _RegID, uint32_t _RegValue);
uint32_t TM7705_ReadReg(uint8_t _RegID);

#endif

/*****************************(END OF FILE) *********************************/
