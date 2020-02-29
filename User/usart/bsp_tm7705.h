/*
*********************************************************************************************************
*
*	模块名称 : TM7705 驱动模块(2通道带PGA的16位ADC)
*	文件名称 : bsp_tm7705.h
*
*	Copyright (C), 2013-2014, 安富莱电子 www.armfly.com
*
*********************************************************************************************************
*/

#ifndef _BSP_TM7705_H
#define _BSP_TM7705_H

#include "stm32f10x.h"

#define SOFT_SPI		/* 定义此行表示使用GPIO模拟SPI接口 */
//#define HARD_SPI		/* 定义此行表示使用CPU的硬件SPI接口 */

//-------------------------AD 工作模式修改
#if ad7705 == 1
/* 通道1和通道2的增益,输入缓冲，极性 */
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

#ifdef SOFT_SPI		/* 软件SPI */
/*
	TM7705模块可以直接插到STM32-V5开发板nRF24L01模块的排母接口上。

    TM7705模块   STM32F1 开发板
      SCK   ------  PA5/SPI3_SCK
      DOUT  ------  PA6/SPI3_MISO
      DIN   ------  PA7/SPI3_MOSI
      CS    ------  PC6/NRF24L01_CSN
      DRDY  ------  PC4/NRF24L01_IRQ
      RST   ------  PC5/NRF905_TX_EN/NRF24L01_CE/DAC1_OUT	(复位 RESET)
*/
//----------------------------------------------------------------------------
//-------------------修改GPIO-------------------------------------------------
//----------------------------------------------------------------------------
	/* 定义GPIO端口 */
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
	//-------------------修改GPIO-------------------------------------------------
	//----------------------------------------------------------------------------

	/* 定义口线置0和置1的宏 */
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

#ifdef HARD_SPI		/* 硬件SPI */
	;
#endif

/* 通信寄存器bit定义 */
enum
{
	/* 寄存器选择  RS2 RS1 RS0  */
	REG_COMM	= 0x00,	/* 通信寄存器 */
	REG_SETUP	= 0x10,	/* 设置寄存器 */
	REG_CLOCK	= 0x20,	/* 时钟寄存器 */
	REG_DATA	= 0x30,	/* 数据寄存器 */
	REG_ZERO_CH1	= 0x60,	/* CH1 偏移寄存器 */
	REG_FULL_CH1	= 0x70,	/* CH1 满量程寄存器 */
	REG_ZERO_CH2	= 0x61,	/* CH2 偏移寄存器 */
	REG_FULL_CH2	= 0x71,	/* CH2 满量程寄存器 */

	/* 读写操作 */
	WRITE 		= 0x00,	/* 写操作 */
	READ 		= 0x08,	/* 读操作 */

	//通道设置         AD7705通道选择      AD7706通道选择
	CH_1		= 0,	/* AIN1+  AIN1-						AIN1     COMMON */
	CH_2		= 1,	/* AIN2+  AIN2- 					AIN2     COMMON */
	CH_3		= 2,	/* AIN1-  AIN1- 					COMMON 	 COMMON	*/
	CH_4		= 3		/* AIN1-  AIN2- 					AIN#     COMMON */
};

/* 设置寄存器bit定义 */
enum
{
	MD_NORMAL		= (0 << 6),	/* 正常模式 */
	MD_CAL_SELF		= (1 << 6),	/* 自校准模式 */
	MD_CAL_ZERO		= (2 << 6),	/* 校准0刻度模式 */
	MD_CAL_FULL		= (3 << 6),	/* 校准满刻度模式 */

	GAIN_1			= (0 << 3),	/* 增益 */
	GAIN_2			= (1 << 3),	/* 增益 */
	GAIN_4			= (2 << 3),	/* 增益 */
	GAIN_8			= (3 << 3),	/* 增益 */
	GAIN_16			= (4 << 3),	/* 增益 */
	GAIN_32			= (5 << 3),	/* 增益 */
	GAIN_64			= (6 << 3),	/* 增益 */
	GAIN_128		= (7 << 3),	/* 增益 */

	/* 无论双极性还是单极性都不改变任何输入信号的状态，它只改变输出数据的代码和转换函数上的校准点 */
	BIPOLAR			= (0 << 2),	/* 双极性输入 */
	UNIPOLAR		= (1 << 2),	/* 单极性输入 */

	BUF_NO			= (0 << 1),	/* 输入无缓冲（内部缓冲器不启用) */
	BUF_EN			= (1 << 1),	/* 输入有缓冲 (启用内部缓冲器) */

	FSYNC_0			= 0,
	FSYNC_1			= 1		/* 不启用 */
};

/* 时钟寄存器bit定义 */
enum
{
	CLKDIS_0	= 0x00,		/* 时钟输出使能 （当外接晶振时，必须使能才能振荡） */
	CLKDIS_1	= 0x10,		/* 时钟禁止 （当外部提供时钟时，设置该位可以禁止MCK_OUT引脚输出时钟以省电 */

	/*
		2.4576MHz（CLKDIV=0 ）或为 4.9152MHz （CLKDIV=1 ），CLK 应置 “0”。
		1MHz （CLKDIV=0 ）或 2MHz   （CLKDIV=1 ），CLK 该位应置  “1”
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
		四十九、电子秤应用中提高TM7705 精度的方法
			当使用主时钟为 2.4576MHz 时，强烈建议将时钟寄存器设为 84H,此时数据输出更新率为10Hz,即每0.1S 输出一个新数据。
			当使用主时钟为 1MHz 时，强烈建议将时钟寄存器设为80H, 此时数据输出更新率为4Hz, 即每0.25S 输出一个新数据
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
