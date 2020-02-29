/*
*********************************************************************************************************
*
*	模块名称 : 主程序模块。
*	文件名称 : main.c
*	版    本 : V1.1
*	说    明 : 主程序
*	信息说明
	1、USART1 用作调试信息输出  波特率 115200-0-8-1
	2、USART2 用作模拟ModeBus DTU 通过485向服务器发送信息
	3、ADC 持续测量实时的电压值，并存储在 全局变量Reg中，Reg即作为发送至服务器的寄存器
	4、LED1 用于指示网络状态，每当向服务器发送一组数据的时候则闪一下
	5、LED2 是独立看门狗指示灯，当一段时间不喂狗则灯会亮起
	6、OLED 采用II2C接口
	7、DS18b20 采用 单线接口，若DS18b20通信不成功则LED一直会闪烁
	8、采用标准Modbus RTU 帧结构，并留一个GPIO控制485芯片发送与接收状态转换
	9、bsp.h 是全局硬件初始化脚本，换板子，需要更改的GPIO说明也记录在该文件中
	10、增加了EEPROM用于存储系数，掉电后不丢失
	11、2019.6.2 将校准系数存储在全局变量reg中，供RS485经modbus协议获取
	12、CL ORP PH
	13、126 2863 2990
*
*********************************************************************************************************
*/

#include "bsp.h"


/*
*********************************************************************************************************
*	函 数 名: main
*	功能说明: c程序入口
*	形    参：无
*	返 回 值: 错误代码(无需处理)
*********************************************************************************************************
*/

// 全局变量
                    //寄存器地址 组态地址(设置在服务器端地址)  本设备寄存器中的值含义
u16 Reg[]={0x0000,  //0000H  CL_H                        0000
           0x0001,  //0001H  CL_L                        005A
           0x0000,  //0002H  ORP_H                       0000
           0x0002,  //0003H  ORP_L                       0938
           0x0000,  //0004H  PH_H                        0000
           0x0003,  //0005H  PH_L                        0000
           0x0000,  //0006H  Temp_H                      0000
           0x0004,  //0007H  Temp_L                      00F8
					 0x0000,  //0008H  CL  校准系数1_H 斜率  8     067C
           0x0000,  //0009H	 CL  校准系数1_L       9     0000
					 0x0000,  //0008H  CL  校准系数2_H 截距  10    8EC6
           0x0000,  //0009H	 CL  校准系数2_L       11    0000
           0x0000,	//000AH  ORP 校准系数1_H       12    001E
					 0x0000,  //000BH  ORP 校准系数1_L       13    0000
					 0x0000,  //0008H  ORP 校准系数2_H       14		 5316
           0x0000,  //0009H	 ORP 校准系数2_L       15    0000
					 0x0000,  //000CH  PH  校准系数1_H			 16    0FA0
					 0x0000,  //000DH  PH  校准系数1_L			 17    0000
					 0x0000,  //0008H  PH  校准系数2_H			 18    0000
           0x0000,  //0009H	 PH  校准系数2_L			 19    0000
          };	
u8 sys_state = 0; //系统的工作状态
u8 function_key_flat = 1; //控制在校准的时候 禁止功能键
					
// I2C 变量
uint8_t I2c_Buf_Write[Para_Len];
uint8_t I2c_Buf_Read[Para_Len];

#define default_para  0
					
int main(void)
{	
	//main 函数内部变量初始化
	u8 key;
	//u8 last_key;
	u8 t = 0;
	u8 *str = 0;
	float *parameter;
	
	//所用到的所有外设初始化
	bsp_Init();
	
	if(default_para == 0)
	{
		parameter = reload_para();
	}
	else{
		float para[Para_Num] = {1,0,1,0,CL2_K,0};
		parameter = para;
	}

	
	//单线程，分时程序
	while (1)
	{	
		//task1 喂狗
		bsp_Idle();		   // 空闲时执行的函数,比如喂狗. 在bsp.c中，在本次任务中，就是不停监听modbus主机发来的寻址信息 
		
		//task2 modbus通信
		Mosbus_Event();  //处理MODbus数据
		
		//task3 系统状态切换
		//扫描按键，确定系统工作状态
		//function_key_flat=1 不在校准
		//function_key_flat=0 在校准
		if (function_key_flat) 
		{
			//不在校准就可以切换功能界面
			key=Remote_Scan();	
			//if( (key !=0) && key != last_key) //key 值有效且不重复
			if(key)
			{	   
				//last_key = key;
				switch(key)
				{
					case 24: sys_state = ORP_STATE;	OLED_Clear();					break;		   //up 进行校准PH交互
					case 16: sys_state = PH_STATE ;	OLED_Clear();		  		break;		   //left 进行校准 ORP交互  					
					case 90: sys_state = CL_STATE;	OLED_Clear();					break;	     //right
					case 56: sys_state = DEFAULT_STATE; OLED_Clear();	    break;       //OK
					case 152:sys_state = TEST_STATE;OLED_Clear();	        break;       //测试状态1，直接显示ADC读取的电压
					case 104:sys_state = TEST_STATE2; OLED_Clear();       break;       //测试状态2，显示EEPROM中存储的系数
					default: sys_state = TIP_STATE;  OLED_Clear();
				}	
			}else delay_ms(10);	  
			t++;
			if(t==20)
			{
				t=0;
				bsp_LedToggle(2);
			}
		}

		//task4 
		//按照系统工作状态，执行不同任务
		switch(sys_state)
		{
			case PH_STATE :  Cali_task(1);     					  			break;
			case ORP_STATE:  Cali_task(2);      								break;
			case CL_STATE:   Cali_task(3);      								break;
			case DEFAULT_STATE: Regular_task(parameter);				break; //显示测量值
			case TIP_STATE: 		Tip_task(); 										break; //指示用户操作
			case TEST_STATE:    Test_task();    								break; //直接显示电压
			case TEST_STATE2:   Test_task2(parameter);    			break; //
			case PARA_UPDATA:   parameter = reload_para();sys_state=DEFAULT_STATE; OLED_Clear(); break;
			default:  Regular_task(parameter);
		}
	}
}




