/*
*********************************************************************************************************
*
*	模块名称 : LED指示灯驱动模块
*	文件名称 : bsp_led.c
*	版    本 : V1.2
*	说    明 : 驱动LED指示灯
*
*	修改记录 :
*		版本号  日期        作者     说明
*		V1.0    2013-02-01 armfly  正式发布
*   V1.2    2019-04-05 shenwang

*
*********************************************************************************************************
*/

#include "bsp.h"
#include "bsp_led.h"



/*
*********************************************************************************************************
*	函 数 名: bsp_InitLed
*	功能说明: 配置LED指示灯相关的GPIO,  该函数被 bsp_Init() 调用。
*	形    参:  无
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_InitLed(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* 打开GPIO时钟 */
	RCC_APB2PeriphClockCmd(RCC_ALL_LED, ENABLE);

	/*
		配置所有的LED指示灯GPIO为推挽输出模式
		由于将GPIO设置为输出时，GPIO输出寄存器的值缺省是0，因此会驱动LED点亮.
		这是我不希望的，因此在改变GPIO为输出前，先关闭LED指示灯
	*/
	
	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_LED1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		/* 设为输出口 */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	/* IO口最大速度 */
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
*	函 数 名: bsp_LedOn
*	功能说明: 点亮指定的LED指示灯。
*	形    参:  _no : 指示灯序号，范围 1 - 4
*	返 回 值: 无
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
*	函 数 名: bsp_LedOff
*	功能说明: 熄灭指定的LED指示灯。
*	形    参:  _no : 指示灯序号，范围 1 - 4
*	返 回 值: 无
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
*	函 数 名: bsp_LedToggle
*	功能说明: 翻转指定的LED指示灯。
*	形    参:  _no : 指示灯序号，范围 1 - 4
*	返 回 值: 按键代码
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
*	函 数 名: bsp_IsLedOn
*	功能说明: 判断LED指示灯是否已经点亮。
*	形    参:  _no : 指示灯序号，范围 1 - 4
*	返 回 值: 1表示已经点亮，0表示未点亮
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
	// 断言错误时执行的代码
	LED1_ON;
}

/***************************** (END OF FILE) *********************************/
