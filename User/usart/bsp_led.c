/*
*********************************************************************************************************
*
*	ģ������ : LEDָʾ������ģ��
*	�ļ����� : bsp_led.c
*	��    �� : V1.2
*	˵    �� : ����LEDָʾ��
*
*	�޸ļ�¼ :
*		�汾��  ����        ����     ˵��
*		V1.0    2013-02-01 armfly  ��ʽ����
*   V1.2    2019-04-05 shenwang

*
*********************************************************************************************************
*/

#include "bsp.h"
#include "bsp_led.h"



/*
*********************************************************************************************************
*	�� �� ��: bsp_InitLed
*	����˵��: ����LEDָʾ����ص�GPIO,  �ú����� bsp_Init() ���á�
*	��    ��:  ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_InitLed(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* ��GPIOʱ�� */
	RCC_APB2PeriphClockCmd(RCC_ALL_LED, ENABLE);

	/*
		�������е�LEDָʾ��GPIOΪ�������ģʽ
		���ڽ�GPIO����Ϊ���ʱ��GPIO����Ĵ�����ֵȱʡ��0����˻�����LED����.
		�����Ҳ�ϣ���ģ�����ڸı�GPIOΪ���ǰ���ȹر�LEDָʾ��
	*/
	
	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_LED1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		/* ��Ϊ����� */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	/* IO������ٶ� */
	GPIO_Init(GPIO_PORT_LED1, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_LED2;
	GPIO_Init(GPIO_PORT_LED2, &GPIO_InitStructure);

	//GPIO_InitStructure.GPIO_Pin = GPIO_PIN_LED3;
	//GPIO_Init(GPIO_PORT_LED3, &GPIO_InitStructure);

	////GPIO_InitStructure.GPIO_Pin = GPIO_PIN_LED4;
	//GPIO_Init(GPIO_PORT_LED4, &GPIO_InitStructure);

	bsp_LedOn(1);
	bsp_LedOn(2);
	bsp_LedToggle(1);
	bsp_LedToggle(2);
	bsp_LedToggle(1);
	bsp_LedToggle(2);
	//bsp_LedOn(3);
	//bsp_LedOn(4);
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_LedOn
*	����˵��: ����ָ����LEDָʾ�ơ�
*	��    ��:  _no : ָʾ����ţ���Χ 1 - 4
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_LedOn(uint8_t _no)
{
	_no--;

	if (_no == 0)
	{
		digitalLo(GPIO_PORT_LED1,GPIO_PIN_LED1);
	}
	else if (_no == 1)
	{
		digitalLo(GPIO_PORT_LED2,GPIO_PIN_LED2);
	}
	else if (_no == 2)
	{
		digitalLo(GPIO_PORT_LED3,GPIO_PIN_LED3);
	}
	else if (_no == 3)
	{
		digitalLo(GPIO_PORT_LED4,GPIO_PIN_LED4);
	}
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_LedOff
*	����˵��: Ϩ��ָ����LEDָʾ�ơ�
*	��    ��:  _no : ָʾ����ţ���Χ 1 - 4
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_LedOff(uint8_t _no)
{

	if (_no == 1)
	{
		digitalHi(GPIO_PORT_LED1,GPIO_PIN_LED1);
	}
	else if (_no == 2)
	{
		digitalHi(GPIO_PORT_LED2,GPIO_PIN_LED2);
	}
	else if (_no == 3)
	{
		digitalHi(GPIO_PORT_LED3,GPIO_PIN_LED3);
	}
	else if (_no == 4)
	{
		digitalHi(GPIO_PORT_LED4,GPIO_PIN_LED4);
	}
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_LedToggle
*	����˵��: ��תָ����LEDָʾ�ơ�
*	��    ��:  _no : ָʾ����ţ���Χ 1 - 4
*	�� �� ֵ: ��������
*********************************************************************************************************
*/
void bsp_LedToggle(uint8_t _no)
{
	if (_no == 1)
	{
		digitalToggle(GPIO_PORT_LED1,GPIO_PIN_LED1);
	}
	else if (_no == 2)
	{
		digitalToggle(GPIO_PORT_LED2,GPIO_PIN_LED2);
	}
	else if (_no == 3)
	{
		digitalToggle(GPIO_PORT_LED3,GPIO_PIN_LED3);
	}
	else if (_no == 4)
	{
		digitalToggle(GPIO_PORT_LED4,GPIO_PIN_LED4);
	}
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_IsLedOn
*	����˵��: �ж�LEDָʾ���Ƿ��Ѿ�������
*	��    ��:  _no : ָʾ����ţ���Χ 1 - 4
*	�� �� ֵ: 1��ʾ�Ѿ�������0��ʾδ����
*********************************************************************************************************
*/
uint8_t bsp_IsLedOn(uint8_t _no)
{
	if (_no == 1)
	{
		if ((GPIO_PORT_LED1->ODR & GPIO_PIN_LED1) != 0)
		{
			return 1;
		}
		return 0;
	}
	else if (_no == 2)
	{
		if ((GPIO_PORT_LED2->ODR & GPIO_PIN_LED2) != 0)
		{
			return 1;
		}
		return 0;
	}
	else if (_no == 3)
	{
		if ((GPIO_PORT_LED3->ODR & GPIO_PIN_LED3) != 0)
		{
			return 1;
		}
		return 0;
	}
	else if (_no == 4)
	{
		if ((GPIO_PORT_LED4->ODR & GPIO_PIN_LED4) != 0)
		{
			return 1;
		}
		return 0;
	}

	return 0;
}

void assert_failed(uint8_t* file, uint32_t line)
{
	// ���Դ���ʱִ�еĴ���
	LED1_ON;
}

/***************************** (END OF FILE) *********************************/
