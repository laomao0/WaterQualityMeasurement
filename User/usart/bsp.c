/*
*********************************************************************************************************
*
*	模块名称 : BSP模块(For STM32F4XX)
*	文件名称 : bsp.c
*	版    本 : V1.1
*	说    明 : 这是硬件底层驱动程序的主文件。每个c文件可以 #include "bsp.h" 来包含所有的外设驱动模块。
*			   bsp = Borad surport packet 板级支持包
*	修改记录 :
*		版本号  日期        作者     说明
*		V1.0    2013-03-01  armfly   正式发布
*		V1.1    2013-06-20  armfly   规范注释，添加必要说明
*		V1.2    2019-05-03  shenwang  添加针对ad ds18b20
*
*********************************************************************************************************
*/

#include "bsp.h"
/*
*********************************************************************************************************
*	函 数 名: bsp_Init
*	功能说明: 初始化所有的硬件设备。该函数配置CPU寄存器和外设的寄存器并初始化一些全局变量。只需要调用一次
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_Init(void)
{
	/*
		由于ST固件库的启动文件已经执行了CPU系统时钟的初始化，所以不必再次重复配置系统时钟。
		启动文件配置了CPU主时钟频率、内部Flash访问速度和可选的外部SRAM FSMC初始化。

		系统时钟缺省配置为72MHz，如果需要更改，可以修改 system_stm32f4xx.c 文件
	
		中断组2  主优先级0-3 子优先级0-3
		中断优先级  中断源    抢占   子优先级
								usart1     1        3
								usart2     1        1
								tim2       1        2
								tim3       1        3
							
	*/
	//初始化调试用的usart1
	bsp_InitUart(); 	/* 初始化串口1 */
	
	//初始化按键（暂时没用到）
	//bsp_InitKey();		/* 初始化按键变量（必须在 bsp_InitTimer() 之前调用） */
	
	
	bsp_InitTimer();	/* 初始化系统滴答定时器，用于实现精准定时 */
	
	//初始化指示灯
	bsp_InitLed();
	
	Remote_Init();			//红外接收初始化	
	
	/* I2C 外设初(AT24C02)始化 */
	I2C_EE_Init();
	
	//初始化显示OLED部分
	//在首次上电时，oled的初始化时间比单片机要长
	//所以会出现单片机已经初始化完成，但是oled还没初始化完成的情况
	//此时单片机刷新oled屏幕会导致屏幕乱码，所以需要延时一下
	delay_ms(2000);   
	OLED_Init();			//初始化OLED  
	
	//展示设备信息
	//display
	PrintfLogo();		/* 打印例程Logo到串口1 */
	OLED_Clear();
	OLED_ShowCHinese(0+18,0,0);//蓝
	OLED_ShowCHinese(18+18,0,1);//宇
	OLED_ShowCHinese(36+18,0,2);//水
	OLED_ShowCHinese(54+18,0,3);//处
	OLED_ShowCHinese(72+18,0,4);//理
	
	OLED_ShowCHinese(0,4,5);//健
	OLED_ShowCHinese(18,4,6);//康
	OLED_ShowCHinese(36,4,7);//水
	OLED_ShowCHinese(54,4,8);//质
	OLED_ShowCHinese(72,4,9);//创
	OLED_ShowCHinese(90,4,10);//造
	OLED_ShowCHinese(108,4,11);//者
	delay_ms(2000);
	OLED_Clear();
	OLED_ShowString(0,0, " Water Analyzer",16);  
	OLED_ShowString(0,3, "       V1",16);  
	OLED_ShowString(0,6, "    PWT Co.Ltd.",16);  
	OLED_ShowString(0,6," www.shlanyu.cn",16);  
	delay_ms(2000);
  
	//初始化AD部分
	bsp_InitTM7705();			/* 初始化配置TM7705 */

	/* 针对不同的应用程序，添加需要的底层驱动模块初始化函数 */
	
	//初始化AD，若使用ad7706，需要按照bsp.h中的说明修改
	// 通道1 AD第一路
	uint8_t ad_state = TM7705_CalibSelf(1);		/* 自校准。执行时间较长，约180ms */		
	if(ad_state == 1)
	{
		bsp_LedOn(2);
		OLED_Clear();
		OLED_ShowString(0,0, "   Channel 1 ERROR",16);
		OLED_ShowString(0,2, "    ADC ERR",16);
		OLED_ShowString(0,4, "  Not Connected",16);
		OLED_ShowString(0,7, "   System wi1l reset",16);
		delay_ms(1000);
	}
	// 通道2 AD第二路
	ad_state = TM7705_CalibSelf(2);		/* 自校准。执行时间较长，约180ms */
	if(ad_state == 1)
	{
		bsp_LedOn(2);
		OLED_Clear();
		OLED_ShowString(0,0, "   Channel 2 ERROR",16);
		OLED_ShowString(0,2, "    ADC ERR",16);
		OLED_ShowString(0,4, "  Not Connected",16);
		OLED_ShowString(0,7, "   System wi1l reset",16);
		delay_ms(1000);
	}
	// 通道3 不考虑使用
	// 通道4 AD第三路
	ad_state = TM7705_CalibSelf(4);		/* 自校准。执行时间较长，约180ms */
	if(ad_state == 1)
	{
		bsp_LedOn(2);
		OLED_Clear();
		OLED_ShowString(0,0, "   Channel 3 ERROR",16);
		OLED_ShowString(0,2, "    ADC ERR",16);
		OLED_ShowString(0,4, "  Not Connected",16);
		OLED_ShowString(0,7, "   System wi1l reset",16);
		delay_ms(1000);
	}
	//DS18b20初始化
	//注意 若DS18b20没有通信成功，则程序会尝试连接18b20几次在这里，会让灯亮
	for( int i=0; i<= 3; i++)
	{
		if (DS18B20_Init())
		{
			bsp_LedOn(2);
			OLED_Clear();
			OLED_ShowString(0,0, "   Temp ERROR",16);
			OLED_ShowString(0,3, "     DS18B20",16);
			OLED_ShowString(0,6, "  Not Connected",16);
			delay_ms(2000);
		}
		else
		{
			break;
		}
	}
	bsp_LedOff(2);
	
	OLED_PrintLogo(); //OLED 展示公司信息，产品信息
	
	//为了防止网络读取到未初始化的值，应该将modbus放在最后初始化
	// modbus 部分初始化--用到TIM2，USART2
	Timer2_Init();   //初始化定时器TIM2 用于modbus协议的延时
  Mosbus_Init();   //初始化modbus组件
	Isr_Init();      //初始化

	Reset_wdd();         //初始化看门狗	，若此时AD失效，则系统会一直重启
}

/*
*********************************************************************************************************
*	函 数 名: bsp_RunPer10ms
*	功能说明: 该函数每隔10ms被Systick中断调用1次。详见 bsp_timer.c的定时中断服务程序。一些需要周期性处理
*			的事务可以放在此函数。比如：按键扫描、蜂鸣器鸣叫控制等。
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_RunPer10ms(void)
{
	//bsp_KeyScan();		/* 按键扫描 */
}

/*
*********************************************************************************************************
*	函 数 名: bsp_RunPer1ms
*	功能说明: 该函数每隔1ms被Systick中断调用1次。详见 bsp_timer.c的定时中断服务程序。一些需要周期性处理的
*			事务可以放在此函数。比如：触摸坐标扫描。
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_RunPer1ms(void)
{
	//TOUCH_Scan();	/* 触摸扫描 */
}

/*
*********************************************************************************************************
*	函 数 名: bsp_Idle
*	功能说明: 空闲时执行的函数。一般主程序在for和while循环程序体中需要插入 CPU_IDLE() 宏来调用本函数。
*			 本函数缺省为空操作。用户可以添加喂狗、设置CPU进入休眠模式的功能。
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_Idle(void)
{
	/* --- 喂狗 */
	IWDG_Feed();
	

	/* --- 让CPU进入休眠，由Systick定时中断唤醒或者其他中断唤醒 */

	/* 对于 emWin 图形库，可以插入图形库需要的轮询函数 */
	//GUI_Exec();

	/* 对于 uIP 协议实现，可以插入uip轮询函数 */
}

/***************************** 安富莱电子 www.armfly.com (END OF FILE) *********************************/
