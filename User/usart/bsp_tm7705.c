/*
*********************************************************************************************************
*
*	ģ������ : TM7705 ����ģ��(2ͨ����PGA��16λADC)
*	�ļ����� : bsp_tm7705.c
*	��    �� : V1.0
*	˵    �� : TM7705ģ���CPU֮�����SPI�ӿڡ�����������֧��Ӳ��SPI�ӿں����SPI�ӿڡ�
*			  ͨ�����л���
*
*	�޸ļ�¼ :
*		�汾��  ����        ����     ˵��
*		V1.0    2013-10-20  armfly  ��ʽ����
*
*	Copyright (C), 2013-2014, ���������� www.armfly.com
*
*********************************************************************************************************
*/

#include "bsp.h"

static void TM7705_SyncSPI(void);
static void TM7705_Send8Bit(uint8_t _data);
static uint8_t TM7705_Recive8Bit(void);
static void TM7705_WriteByte(uint8_t _data);
static void TM7705_Write3Byte(uint32_t _data);
static uint8_t TM7705_ReadByte(void);
static uint16_t TM7705_Read2Byte(void);
static uint32_t TM7705_Read3Byte(void);
static uint8_t TM7705_WaitDRDY(void);
static void TM7705_ResetHard(void);
static void TM7705_Delay(void);

/*
*********************************************************************************************************
*	�� �� ��: bsp_InitTM7705
*	����˵��: ����STM32��GPIO��SPI�ӿڣ��������� TM7705
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_InitTM7705(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

#ifdef SOFT_SPI		/* ���SPI */
	CS_1();
	SCK_1();
	DI_1();	
	
	/* ��GPIOʱ�� */
	RCC_APB2PeriphClockCmd(RCC_SCK | RCC_DIN | RCC_DOUT | RCC_CS | RCC_DRDY | RCC_RESET, ENABLE);
	
	/* ���ü����������IO */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		/* ��Ϊ����� */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	/* IO������ٶ� */
	
	//GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;		/* ��Ϊ����� */
	//GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		/* ��Ϊ����ģʽ */
	//GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;	/* ���������費ʹ�� */
	//GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	/* IO������ٶ� */

	GPIO_InitStructure.GPIO_Pin = PIN_SCK;
	GPIO_Init(PORT_SCK, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = PIN_DIN;
	GPIO_Init(PORT_DIN, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = PIN_CS;
	GPIO_Init(PORT_CS, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = PIN_RESET;
	GPIO_Init(PORT_RESET, &GPIO_InitStructure);

	/* ����GPIOΪ��������ģʽ(ʵ����CPU��λ���������״̬) */
	//GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;		/* ��Ϊ����� */
	//GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		/* ��Ϊ����ģʽ */
	//GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;	/* �������������� */
	//GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	/* IO������ٶ� */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 

	GPIO_InitStructure.GPIO_Pin = PIN_DOUT;
	GPIO_Init(PORT_DOUT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = PIN_DRDY;
	GPIO_Init(PORT_DRDY, &GPIO_InitStructure);
	

#endif

	bsp_DelayMS(10);
	
	TM7705_ResetHard();		/* Ӳ����λ */
	
	/*
		�ڽӿ����ж�ʧ������£������DIN �ߵ�ƽ��д�����������㹻����ʱ�䣨���� 32������ʱ�����ڣ���
		TM7705 ����ص�Ĭ��״̬��
	*/	
	bsp_DelayMS(5);

	TM7705_SyncSPI();		/* ͬ��SPI�ӿ�ʱ�� */

	bsp_DelayMS(5);

	/* ����ʱ�ӼĴ��� */
	TM7705_WriteByte(REG_CLOCK | WRITE | CH_1);			/* ��дͨ�żĴ�������һ����дʱ�ӼĴ��� */
	
	TM7705_WriteByte(CLKDIS_0 | CLK_4_9152M | FS_50HZ);	/* ˢ������50Hz */
	//TM7705_WriteByte(CLKDIS_0 | CLK_4_9152M | FS_500HZ);	/* ˢ������500Hz */
	
	/* ÿ���ϵ����һ����У׼ */
	TM7705_CalibSelf(1);	/* �ڲ���У׼ CH1 */
	bsp_DelayMS(5);
}

/*
*********************************************************************************************************
*	�� �� ��: TM7705_Delay
*	����˵��: CLK֮����ӳ٣�ʱ���ӳ�. ����STM32F407  168M��Ƶ
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void TM7705_Delay(void)
{
	uint16_t i;

	for (i = 0; i < 5; i++);
}

/*
*********************************************************************************************************
*	�� �� ��: TM7705_ResetHard
*	����˵��: Ӳ����λ TM7705оƬ
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void TM7705_ResetHard(void)
{
	RESET_1();
	bsp_DelayMS(1);	
	RESET_0();
	bsp_DelayMS(2);
	RESET_1();
	bsp_DelayMS(1);
}

/*
*********************************************************************************************************
*	�� �� ��: TM7705_SyncSPI
*	����˵��: ͬ��TM7705оƬSPI�ӿ�ʱ��
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void TM7705_SyncSPI(void)
{
	/* AD7705���нӿ�ʧ�����临λ����λ��Ҫ��ʱ500us�ٷ��� */
	CS_0();
	TM7705_Send8Bit(0xFF);
	TM7705_Send8Bit(0xFF);
	TM7705_Send8Bit(0xFF);
	TM7705_Send8Bit(0xFF);	
	CS_1();
}

/*
*********************************************************************************************************
*	�� �� ��: TM7705_Send8Bit
*	����˵��: ��SPI���߷���8��bit���ݡ� ����CS���ơ�
*	��    ��: _data : ����
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void TM7705_Send8Bit(uint8_t _data)
{
	uint8_t i;

	for(i = 0; i < 8; i++)
	{
		if (_data & 0x80)
		{
			DI_1();
		}
		else
		{
			DI_0();
		}
		SCK_0();
		_data <<= 1;
		TM7705_Delay();
		SCK_1();	
		TM7705_Delay();		
	}
}

/*
*********************************************************************************************************
*	�� �� ��: TM7705_Recive8Bit
*	����˵��: ��SPI���߽���8��bit���ݡ� ����CS���ơ�
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static uint8_t TM7705_Recive8Bit(void)
{
	uint8_t i;
	uint8_t read = 0;

	for (i = 0; i < 8; i++)
	{
		SCK_0();
		TM7705_Delay();		
		read = read<<1;
		if (DO_IS_HIGH())
		{
			read++;
		}
		SCK_1();		
		TM7705_Delay();
	}
	return read;
}

/*
*********************************************************************************************************
*	�� �� ��: TM7705_WriteByte
*	����˵��: д��1���ֽڡ���CS����
*	��    ��: _data ����Ҫд�������
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void TM7705_WriteByte(uint8_t _data)
{
	CS_0();
	TM7705_Send8Bit(_data);
	CS_1();
}

/*
*********************************************************************************************************
*	�� �� ��: TM7705_Write3Byte
*	����˵��: д��3���ֽڡ���CS����
*	��    ��: _data ����Ҫд�������
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void TM7705_Write3Byte(uint32_t _data)
{
	CS_0();
	TM7705_Send8Bit((_data >> 16) & 0xFF);
	TM7705_Send8Bit((_data >> 8) & 0xFF);
	TM7705_Send8Bit(_data);
	CS_1();
}

/*
*********************************************************************************************************
*	�� �� ��: TM7705_ReadByte
*	����˵��: ��ADоƬ��ȡһ���֣�16λ��
*	��    ��: ��
*	�� �� ֵ: ��ȡ���֣�16λ��
*********************************************************************************************************
*/
static uint8_t TM7705_ReadByte(void)
{
	uint8_t read;

	CS_0();
	read = TM7705_Recive8Bit();
	CS_1();
	
	return read;
}

/*
*********************************************************************************************************
*	�� �� ��: TM7705_Read2Byte
*	����˵��: ��2�ֽ�����
*	��    ��: ��
*	�� �� ֵ: ��ȡ�����ݣ�16λ��
*********************************************************************************************************
*/
static uint16_t TM7705_Read2Byte(void)
{
	uint16_t read;

	CS_0();
	read = TM7705_Recive8Bit(); //��ȡ��8λ
	read <<= 8;
	read += TM7705_Recive8Bit();//��ȡ��8λ
	CS_1();

	return read;
}

/*
*********************************************************************************************************
*	�� �� ��: TM7705_Read3Byte
*	����˵��: ��3�ֽ�����
*	��    ��: ��
*	�� �� ֵ: ��ȡ�������ݣ�24bit) ��8λ�̶�Ϊ0.
*********************************************************************************************************
*/
static uint32_t TM7705_Read3Byte(void)
{
	uint32_t read;

	CS_0();
	read = TM7705_Recive8Bit();
	read <<= 8;
	read += TM7705_Recive8Bit();
	read <<= 8;
	read += TM7705_Recive8Bit();
	CS_1();
	return read;
}

/*
*********************************************************************************************************
*	�� �� ��: TM7705_WaitDRDY
*	����˵��: �ȴ��ڲ�������ɡ� ��У׼ʱ��ϳ�����Ҫ�ȴ���
*	��    ��: ��
*	�� �� ֵ: 1 ��ʱ 0-�ɹ�
*********************************************************************************************************
*/
static uint8_t TM7705_WaitDRDY(void)
{
	uint32_t i;

	for (i = 0; i < 4000000; i++)
	{
		if (DRDY_IS_LOW())
		{
			return 0;
		}
	}
	if (i >= 4000000)
	{
		printf("TM7705_WaitDRDY() Time Out ...\r\n");		/* �������. �����Ŵ� */
		delay_ms(1);
		return 1;
	}
	return 1;
}

/*
*********************************************************************************************************
*	�� �� ��: TM7705_WriteReg
*	����˵��: дָ���ļĴ���
*	��    ��:  _RegID : �Ĵ���ID
*			  _RegValue : �Ĵ���ֵ�� ����8λ�ļĴ�����ȡ32λ�βεĵ�8bit
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void TM7705_WriteReg(uint8_t _RegID, uint32_t _RegValue)
{
	uint8_t bits;

	switch (_RegID)
	{
		case REG_COMM:		/* ͨ�żĴ��� */		
		case REG_SETUP:		/* ���üĴ��� 8bit */
		case REG_CLOCK:		/* ʱ�ӼĴ��� 8bit */
			bits = 8;
			break;

		case REG_ZERO_CH1:	/* CH1 ƫ�ƼĴ��� 24bit */
		case REG_FULL_CH1:	/* CH1 �����̼Ĵ��� 24bit */
		case REG_ZERO_CH2:	/* CH2 ƫ�ƼĴ��� 24bit */
		case REG_FULL_CH2:	/* CH2 �����̼Ĵ��� 24bit*/
			bits = 24;
			break;

		case REG_DATA:		/* ���ݼĴ��� 16bit */
		default:
			return;
	}

	TM7705_WriteByte(_RegID | WRITE);	/* дͨ�żĴ���, ָ����һ����д��������ָ��д�ĸ��Ĵ��� */

	if (bits == 8)
	{
		TM7705_WriteByte((uint8_t)_RegValue);
	}
	else	/* 24bit */
	{
		TM7705_Write3Byte(_RegValue);
	}
}

/*
*********************************************************************************************************
*	�� �� ��: TM7705_ReadReg
*	����˵��: дָ���ļĴ���
*	��    ��:  _RegID : �Ĵ���ID
*			  _RegValue : �Ĵ���ֵ�� ����8λ�ļĴ�����ȡ32λ�βεĵ�8bit
*	�� �� ֵ: �����ļĴ���ֵ�� ����8λ�ļĴ�����ȡ32λ�βεĵ�8bit
*********************************************************************************************************
*/
uint32_t TM7705_ReadReg(uint8_t _RegID)
{
	uint8_t bits;
	uint32_t read;

	switch (_RegID)
	{
		case REG_COMM:		/* ͨ�żĴ��� */
		case REG_SETUP:		/* ���üĴ��� 8bit */
		case REG_CLOCK:		/* ʱ�ӼĴ��� 8bit */
			bits = 8;
			break;

		case REG_ZERO_CH1:	/* CH1 ƫ�ƼĴ��� 24bit */
		case REG_FULL_CH1:	/* CH1 �����̼Ĵ��� 24bit */
		case REG_ZERO_CH2:	/* CH2 ƫ�ƼĴ��� 24bit */
		case REG_FULL_CH2:	/* CH2 �����̼Ĵ��� 24bit*/
			bits = 24;
			break;

		case REG_DATA:		/* ���ݼĴ��� 16bit */
		default:
			return 0xFFFFFFFF;
	}

	TM7705_WriteByte(_RegID | READ);	/* дͨ�żĴ���, ָ����һ����д��������ָ��д�ĸ��Ĵ��� */

	if (bits == 16)
	{
		read = TM7705_Read2Byte();
	}
	else if (bits == 8)
	{
		read = TM7705_ReadByte();
	}
	else	/* 24bit */
	{
		read = TM7705_Read3Byte();
	}
	return read;
}

/*
*********************************************************************************************************
*	�� �� ��: TM7705_CalibSelf
*	����˵��: ������У׼. �ڲ��Զ��̽�AIN+ AIN-У׼0λ���ڲ��̽ӵ�Vref У׼��λ���˺���ִ�й��̽ϳ���
*			  ʵ��Լ 180ms
*	��    ��:  _ch : ADCͨ����1��2
*	�� �� ֵ: 1 error 0-�ɹ�
*********************************************************************************************************
*/
uint8_t TM7705_CalibSelf(uint8_t _ch)
{
	uint8_t a;
	if (_ch == 1)
	{
		/* ��У׼CH1 */
		TM7705_WriteByte(REG_SETUP | WRITE | CH_1);	/* дͨ�żĴ�������һ����д���üĴ�����ͨ��1 */		
		TM7705_WriteByte(MD_CAL_SELF | __CH1_GAIN_BIPOLAR_BUF | FSYNC_0);/* ������У׼ */
		a =  TM7705_WaitDRDY();	/* �ȴ��ڲ�������� --- ʱ��ϳ���Լ180ms */
	}
	else if (_ch == 2)
	{
		/* ��У׼CH2 */
		TM7705_WriteByte(REG_SETUP | WRITE | CH_2);	/* дͨ�żĴ�������һ����д���üĴ�����ͨ��2 */
		TM7705_WriteByte(MD_CAL_SELF | __CH2_GAIN_BIPOLAR_BUF | FSYNC_0);	/* ������У׼ */
		a =  TM7705_WaitDRDY();	/* �ȴ��ڲ��������  --- ʱ��ϳ���Լ180ms */
	}
	else if (_ch == 3)
	{
		TM7705_WriteByte(REG_SETUP | WRITE | CH_3);	/* дͨ�żĴ�������һ����д���üĴ�����ͨ��2 */
		TM7705_WriteByte(MD_CAL_SELF | __CH3_GAIN_BIPOLAR_BUF | FSYNC_0);	/* ������У׼ */
		a =  TM7705_WaitDRDY();	/* �ȴ��ڲ��������  --- ʱ��ϳ���Լ180ms */
	}
		else if (_ch == 4)
	{
		TM7705_WriteByte(REG_SETUP | WRITE | CH_4);	/* дͨ�żĴ�������һ����д���üĴ�����ͨ��2 */
		TM7705_WriteByte(MD_CAL_SELF | __CH4_GAIN_BIPOLAR_BUF | FSYNC_0);	/* ������У׼ */
		a =  TM7705_WaitDRDY();	/* �ȴ��ڲ��������  --- ʱ��ϳ���Լ180ms */
	}
	return a;
}

/*
*********************************************************************************************************
*	�� �� ��: TM7705_SytemCalibZero
*	����˵��: ����ϵͳУ׼��λ. �뽫AIN+ AIN-�̽Ӻ�ִ�иú�����У׼Ӧ������������Ʋ�����У׼������
*			 ִ����Ϻ󡣿���ͨ�� TM7705_ReadReg(REG_ZERO_CH1) ��  TM7705_ReadReg(REG_ZERO_CH2) ��ȡУ׼������
*	��    ��: _ch : ADCͨ����1��2
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void TM7705_SytemCalibZero(uint8_t _ch)
{
	if (_ch == 1)
	{
		/* У׼CH1 */
		TM7705_WriteByte(REG_SETUP | WRITE | CH_1);	/* дͨ�żĴ�������һ����д���üĴ�����ͨ��1 */
		TM7705_WriteByte(MD_CAL_ZERO | __CH1_GAIN_BIPOLAR_BUF | FSYNC_0);/* ������У׼ */
		TM7705_WaitDRDY();	/* �ȴ��ڲ�������� */
	}
	else if (_ch == 2)
	{
		/* У׼CH2 */
		TM7705_WriteByte(REG_SETUP | WRITE | CH_2);	/* дͨ�żĴ�������һ����д���üĴ�����ͨ��1 */
		TM7705_WriteByte(MD_CAL_ZERO | __CH2_GAIN_BIPOLAR_BUF | FSYNC_0);	/* ������У׼ */
		TM7705_WaitDRDY();	/* �ȴ��ڲ�������� */
	}
}

/*
*********************************************************************************************************
*	�� �� ��: TM7705_SytemCalibFull
*	����˵��: ����ϵͳУ׼��λ. �뽫AIN+ AIN-����������ѹԴ��ִ�иú�����У׼Ӧ������������Ʋ�����У׼������
*			 ִ����Ϻ󡣿���ͨ�� TM7705_ReadReg(REG_FULL_CH1) ��  TM7705_ReadReg(REG_FULL_CH2) ��ȡУ׼������
*	��    ��:  _ch : ADCͨ����1��2
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void TM7705_SytemCalibFull(uint8_t _ch)
{
	if (_ch == 1)
	{
		/* У׼CH1 */
		TM7705_WriteByte(REG_SETUP | WRITE | CH_1);	/* дͨ�żĴ�������һ����д���üĴ�����ͨ��1 */
		TM7705_WriteByte(MD_CAL_FULL | __CH1_GAIN_BIPOLAR_BUF | FSYNC_0);/* ������У׼ */
		TM7705_WaitDRDY();	/* �ȴ��ڲ�������� */
	}
	else if (_ch == 2)
	{
		/* У׼CH2 */
		TM7705_WriteByte(REG_SETUP | WRITE | CH_2);	/* дͨ�żĴ�������һ����д���üĴ�����ͨ��1 */
		TM7705_WriteByte(MD_CAL_FULL | __CH2_GAIN_BIPOLAR_BUF | FSYNC_0);	/* ������У׼ */
		TM7705_WaitDRDY();	/* �ȴ��ڲ�������� */
	}
}

/*
*********************************************************************************************************
*	�� �� ��: TM7705_ReadAdc1
*	����˵��: ��ͨ��1��2��ADC����
*	��    ��: ��
*	�� �� ֵ: ��
* �޸ģ����TM7706��ȡ��ͨ���Ĵ���
*********************************************************************************************************
*/
uint16_t TM7705_ReadAdc(uint8_t _ch)
{
	uint8_t i;
	uint16_t read = 0;
	
	/* Ϊ�˱���ͨ���л���ɶ���ʧЧ����2�� */
	for (i = 0; i < 2; i++)
	{
		TM7705_WaitDRDY();		/* �ȴ�DRDY����Ϊ0 */		

		if (_ch == 1)
		{                          // дͨ�żĴ���
			TM7705_WriteByte(0x38); // 0(011���ݼĴ���) (1��)(0��������)(00�� ��1��ͨ��) 
		}
		else if (_ch == 2)
		{                          // дͨ�żĴ���
			TM7705_WriteByte(0x39); //// 0(011���ݼĴ���) (1��)(0��������)(01�� ��2��ͨ��) 
		}
		else if (_ch == 3)
		{                          // дͨ�żĴ���
			TM7705_WriteByte(0x3A); //// 0(011���ݼĴ���) (1��)(0��������)(10�� ��3��ͨ��)  ������ͨ��ͨ������
		}
		else if (_ch == 4)
		{                          // дͨ�żĴ���
			TM7705_WriteByte(0x3B); //// 0(011���ݼĴ���) (1��)(0��������)(11�� ��4��ͨ��)  ���ĸ�ͨ��
		}
		read = TM7705_Read2Byte();
	}
	return read;	
}

/***************************** ���������� www.armfly.com (END OF FILE) *********************************/
