/*
*********************************************************************************************************
*
*	ģ������ : ������ģ�顣
*	�ļ����� : main.c
*	��    �� : V1.1
*	˵    �� : ������
*	��Ϣ˵��
	1��USART1 ����������Ϣ���  ������ 115200-0-8-1
	2��USART2 ����ģ��ModeBus DTU ͨ��485�������������Ϣ
	3��ADC ��������ʵʱ�ĵ�ѹֵ�����洢�� ȫ�ֱ���Reg�У�Reg����Ϊ�������������ļĴ���
	4��LED1 ����ָʾ����״̬��ÿ�������������һ�����ݵ�ʱ������һ��
	5��LED2 �Ƕ������Ź�ָʾ�ƣ���һ��ʱ�䲻ι����ƻ�����
	6��OLED ����II2C�ӿ�
	7��DS18b20 ���� ���߽ӿڣ���DS18b20ͨ�Ų��ɹ���LEDһֱ����˸
	8�����ñ�׼Modbus RTU ֡�ṹ������һ��GPIO����485оƬ���������״̬ת��
	9��bsp.h ��ȫ��Ӳ����ʼ���ű��������ӣ���Ҫ���ĵ�GPIO˵��Ҳ��¼�ڸ��ļ���
	10��������EEPROM���ڴ洢ϵ��������󲻶�ʧ
	11��2019.6.2 ��У׼ϵ���洢��ȫ�ֱ���reg�У���RS485��modbusЭ���ȡ
	12��CL ORP PH
	13��126 2863 2990
*
*********************************************************************************************************
*/

#include "bsp.h"


/*
*********************************************************************************************************
*	�� �� ��: main
*	����˵��: c�������
*	��    �Σ���
*	�� �� ֵ: �������(���账��)
*********************************************************************************************************
*/

// ȫ�ֱ���
                    //�Ĵ�����ַ ��̬��ַ(�����ڷ������˵�ַ)  ���豸�Ĵ����е�ֵ����
u16 Reg[]={0x0000,  //0000H  CL_H                        0000
           0x0001,  //0001H  CL_L                        005A
           0x0000,  //0002H  ORP_H                       0000
           0x0002,  //0003H  ORP_L                       0938
           0x0000,  //0004H  PH_H                        0000
           0x0003,  //0005H  PH_L                        0000
           0x0000,  //0006H  Temp_H                      0000
           0x0004,  //0007H  Temp_L                      00F8
					 0x0000,  //0008H  CL  У׼ϵ��1_H б��  8     067C
           0x0000,  //0009H	 CL  У׼ϵ��1_L       9     0000
					 0x0000,  //0008H  CL  У׼ϵ��2_H �ؾ�  10    8EC6
           0x0000,  //0009H	 CL  У׼ϵ��2_L       11    0000
           0x0000,	//000AH  ORP У׼ϵ��1_H       12    001E
					 0x0000,  //000BH  ORP У׼ϵ��1_L       13    0000
					 0x0000,  //0008H  ORP У׼ϵ��2_H       14		 5316
           0x0000,  //0009H	 ORP У׼ϵ��2_L       15    0000
					 0x0000,  //000CH  PH  У׼ϵ��1_H			 16    0FA0
					 0x0000,  //000DH  PH  У׼ϵ��1_L			 17    0000
					 0x0000,  //0008H  PH  У׼ϵ��2_H			 18    0000
           0x0000,  //0009H	 PH  У׼ϵ��2_L			 19    0000
          };	
u8 sys_state = 0; //ϵͳ�Ĺ���״̬
u8 function_key_flat = 1; //������У׼��ʱ�� ��ֹ���ܼ�
					
// I2C ����
uint8_t I2c_Buf_Write[Para_Len];
uint8_t I2c_Buf_Read[Para_Len];

#define default_para  0
					
int main(void)
{	
	//main �����ڲ�������ʼ��
	u8 key;
	//u8 last_key;
	u8 t = 0;
	u8 *str = 0;
	float *parameter;
	
	//���õ������������ʼ��
	bsp_Init();
	
	if(default_para == 0)
	{
		parameter = reload_para();
	}
	else{
		float para[Para_Num] = {1,0,1,0,CL2_K,0};
		parameter = para;
	}

	
	//���̣߳���ʱ����
	while (1)
	{	
		//task1 ι��
		bsp_Idle();		   // ����ʱִ�еĺ���,����ι��. ��bsp.c�У��ڱ��������У����ǲ�ͣ����modbus����������Ѱַ��Ϣ 
		
		//task2 modbusͨ��
		Mosbus_Event();  //����MODbus����
		
		//task3 ϵͳ״̬�л�
		//ɨ�谴����ȷ��ϵͳ����״̬
		//function_key_flat=1 ����У׼
		//function_key_flat=0 ��У׼
		if (function_key_flat) 
		{
			//����У׼�Ϳ����л����ܽ���
			key=Remote_Scan();	
			//if( (key !=0) && key != last_key) //key ֵ��Ч�Ҳ��ظ�
			if(key)
			{	   
				//last_key = key;
				switch(key)
				{
					case 24: sys_state = ORP_STATE;	OLED_Clear();					break;		   //up ����У׼PH����
					case 16: sys_state = PH_STATE ;	OLED_Clear();		  		break;		   //left ����У׼ ORP����  					
					case 90: sys_state = CL_STATE;	OLED_Clear();					break;	     //right
					case 56: sys_state = DEFAULT_STATE; OLED_Clear();	    break;       //OK
					case 152:sys_state = TEST_STATE;OLED_Clear();	        break;       //����״̬1��ֱ����ʾADC��ȡ�ĵ�ѹ
					case 104:sys_state = TEST_STATE2; OLED_Clear();       break;       //����״̬2����ʾEEPROM�д洢��ϵ��
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
		//����ϵͳ����״̬��ִ�в�ͬ����
		switch(sys_state)
		{
			case PH_STATE :  Cali_task(1);     					  			break;
			case ORP_STATE:  Cali_task(2);      								break;
			case CL_STATE:   Cali_task(3);      								break;
			case DEFAULT_STATE: Regular_task(parameter);				break; //��ʾ����ֵ
			case TIP_STATE: 		Tip_task(); 										break; //ָʾ�û�����
			case TEST_STATE:    Test_task();    								break; //ֱ����ʾ��ѹ
			case TEST_STATE2:   Test_task2(parameter);    			break; //
			case PARA_UPDATA:   parameter = reload_para();sys_state=DEFAULT_STATE; OLED_Clear(); break;
			default:  Regular_task(parameter);
		}
	}
}




