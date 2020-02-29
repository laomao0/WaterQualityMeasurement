#include "bsp.h"
#include "string.h"
extern u16 Reg[];
extern u8 sys_state;
extern u8 function_key_flat;
extern uint8_t I2c_Buf_Write[Para_Len];
extern uint8_t I2c_Buf_Read[Para_Len];

u8 cali_state =0;
float cal_x1 =0;
float cal_x2 =0;
int cal_y1 =0;
int cal_y2 =0;
unsigned char in_str[M];
/*
*********************************************************************************************************
*	函 数 名: Regular_task
*	功能说明: 读取三通道数据和温度，显示，直接显示
*	形    参: float*p 指向参数矩阵
*	返 回 值: 无
修改：需要修改的功能：在校准某一个参数时候，选择上up键位，能够回到上个状态，重新进行上一步
*********************************************************************************************************
*/
void Regular_task(float*p)
{
		uint16_t adc[P_NUm];  //三个ADC
		short tmpeture;  //温度值
		unsigned char s[10]; 
		int volt[3];
		char *state;
		/* 双通道切换采样，执行一轮实际那约 160ms */	
		adc[0] = TM7705_ReadAdc(1); //CL2	/* 执行时间 80ms */		
		adc[1] = TM7705_ReadAdc(2);	//ORP/* 执行时间 80ms */
		adc[2] = TM7705_ReadAdc(4);	//PH
		tmpeture = DS18B20_Get_Temp();				
		/* 打印采集数据 */

		/* 计算实际电压值（近似估算的），如需准确，请进行校准 */
		volt[0] = (adc[0] * 5000) / 65535; //CL2
		volt[1] = (adc[1] * 5000) / 65535; //ORP
		volt[2] = (adc[2] * 5000) / 65535; //Ph
		
		// 根据电压值，计算测量的系数的值
		float measurement[P_NUm];
		for(int no = 0; no<P_NUm; no++)
		{
			float k = *(p + no*2);
			float b = *(p + no*2 + 1);
			measurement[no] = (volt[2-no] - b)/k;
		}
		if(tmpeture<0)
		{
			tmpeture=-tmpeture;					//转为正数
		}
		
		//将采样道到的数值放到modbus的数组中
		//下面存储的是ADC采样得到的电压值
		//Reg[0] = (volt[0] & 0xFFFF0000) >> 16;
		//Reg[1] = (volt[0] & 0x0000FFFF);
		//Reg[2] = (volt[1] & 0xFFFF0000) >> 16;
		//Reg[3] = (volt[1] & 0x0000FFFF);
		//Reg[4] = (volt[2] & 0xFFFF0000) >> 16;
		//Reg[5] = (volt[2] & 0x0000FFFF);
		//Reg[6] = (tmpeture & 0xFFFF0000) >> 16;
		//Reg[7] = (tmpeture & 0x0000FFFF);
		
		//下面存储的是计算得到的数据，规则是 测到到的浮点数乘以100强制转换为16位的定点数值
		// 只是存储的实时测量值
		uint16_t tmp_ = (uint16_t) measurement[0]*Amp;
		Reg[0] = (tmp_ & 0xFFFF0000) >> 16;
		Reg[1] = (tmp_ & 0x0000FFFF);
		tmp_ = (uint16_t) measurement[1]*Amp;
		Reg[2] = (tmp_ & 0xFFFF0000) >> 16;
		Reg[3] = (tmp_ & 0x0000FFFF);
		tmp_ = (uint16_t) measurement[2]*Amp;
		Reg[4] = (tmp_ & 0xFFFF0000) >> 16;
		Reg[5] = (tmp_ & 0x0000FFFF);
		Reg[6] = (tmpeture & 0xFFFF0000) >> 16;
		Reg[7] = (tmpeture & 0x0000FFFF);
		
		//display
		if (volt[0] <= 0 || volt[0] >= 5000) state = "?"; else state = ">";
		sprintf(s, "%s PH  %0.2f",state, measurement[0]);
		OLED_ShowString(0,0,s,16);//第一行 -- 8x16的显示单元显示ASCII码
		if (volt[1] <= 0 || volt[1]  >= 5000) state = "?"; else state = ">";
		sprintf(s, "%s ORP %.0f  mv", state, measurement[1]);
		OLED_ShowString(0,2,s,16);//第一行 -- 8x16的显示单元显示ASCII码
		if (volt[2] <= 0 || volt[2]  >= 5000) state = "?"; else state = ">";
		//double cl2 = (volt3 * cl_cali_para / 400.0);
		//sprintf(s, "%s CL2 %.2f ppm",state, cl2);
		sprintf(s, "%s CL  %.2f ppm", state, measurement[2]);
		OLED_ShowString(0,4,s,16);//第一行 -- 8x16的显示单元显示ASCII码
		DS18B20_Rst();
		if (DS18B20_Check()) state = "?"; else state = ">";
		sprintf(s, "%s TEM %2d.%d  C",state,tmpeture/10,tmpeture%10);
		OLED_ShowString(0,6,s,16);//第一行 -- 8x16的显示单元显示ASCII码
		delay_ms(800);	
		sys_state = DEFAULT_STATE;			
}

/*
*********************************************************************************************************
*	函 数 名: Regular_task
*	功能说明: 读取三通道数据和温度，显示，直接显示
*	形    参: 无
*	返 回 值: 无
修改：需要修改的功能：在校准某一个参数时候，选择上up键位，能够回到上个状态，重新进行上一步
*********************************************************************************************************
*/
void Test_task(void)
{
		char cali_key_;
		uint16_t adc1, adc2, adc3;  //三个ADC
		short tmpeture;  //温度值
		unsigned char s[10]; 
		int volt1, volt2, volt3;
		char *state;
		/* 双通道切换采样，执行一轮实际那约 160ms */			
		adc1 = TM7705_ReadAdc(1);	/*CL 执行时间 80ms */		
		adc2 = TM7705_ReadAdc(2);	/*ORP 执行时间 80ms */
		adc3 = TM7705_ReadAdc(4);	//PH
		tmpeture = DS18B20_Get_Temp();				
		/* 打印采集数据 */

		/* 计算实际电压值（近似估算的），如需准确，请进行校准 */
		volt1 = (adc1 * 5000) / 65535;
		volt2 = (adc2 * 5000) / 65535;
		volt3 = (adc3 * 5000) / 65535;
		if(tmpeture<0)
		{
			tmpeture=-tmpeture;					//转为正数
		}
		//display
		if (volt1 <= 0 || volt1 >= 5000) state = "?"; else state = "o";
		sprintf(s, "%s CL  %3d  mv",state, volt1);
		OLED_ShowString(0,0,s,16);//第一行 -- 8x16的显示单元显示ASCII码
		if (volt2 <= 0 || volt2  >= 5000) state = "?"; else state = "o";
		sprintf(s, "%s ORP %3d  mv", state, volt2);
		OLED_ShowString(0,2,s,16);//第一行 -- 8x16的显示单元显示ASCII码
		if (volt3 <= 0 || volt3  >= 5000) state = "?"; else state = "o";
		//double cl2 = (volt3 * cl_cali_para / 400.0);
		//sprintf(s, "%s CL2 %.2f ppm",state, cl2);
		sprintf(s, "%s PH  %3d  mv", state, volt3);
		OLED_ShowString(0,4,s,16);//第一行 -- 8x16的显示单元显示ASCII码
		DS18B20_Rst();
		if (DS18B20_Check()) state = "?"; else state = "o";
		sprintf(s, "%s TEM %2d.%d  C",state,tmpeture/10,tmpeture%10);
		OLED_ShowString(0,6,s,16);//第一行 -- 8x16的显示单元显示ASCII码
		
		delay_ms(600);
		cali_key_ = Input_key();
		if(cali_key_ != 'x'){ //没有任何输入的情况
			if(cali_key_ == 'o') //输入了ok 代表推出测试模式
					{
						delay_ms(500);
						OLED_Clear();
						sys_state = DEFAULT_STATE;	
					}
		}
	  
}


/*
*********************************************************************************************************
*	函 数 名: Cali_task
*	功能说明: 标准校准任务
* 					PH ORP CL2连接的ADC的通道分别是 2,1,0
*	形    参: 无
*	返 回 值: no = 1，校正PH， no= 2，校正ORP， no=3 校正余氯

*********************************************************************************************************
*/
void Cali_task(int no)
{
	char *three_name [3] = {"PH", "ORP", "CL2"};
	char *three_range [3] = {"1--14", "0-4000", "0-800"}; //设置三个参数的取值范围
	char * typical_value [3] = { "7.00#", "2000#", "400#"};
	char * unit[3] = {" ", "(mv)", "(ppm)"};
	int adc_c[3] = {4,2,1};
	//PH范围 1-14 ORP范围 0-40000 mv 余氯范围 0-20 ppm
	uint16_t minimum[3] = {1,  0, 0}; 
	uint16_t maximum[3] = {14, 4000, 800};
	char cali_key;
	uint16_t adc1;
	int volt1;
	unsigned char s[10]; 
	char cali_key_;
	char string_cali_key[1];
	float k; //系数斜率
	float b; //系数截距
	unsigned char info_str[10]; //专门用来绘制oled图片的
	
	switch(cali_state)
	{
		case CALI_0:
		{
				sprintf(info_str, "       %s   ",three_name[no-1]); //校正的式第no个参数
				OLED_ShowString(0,0,info_str,16);
				OLED_ShowString(0,2,"   Calibration  ",16);
				OLED_ShowString(0,4,"   For example  ",16);
				sprintf(info_str,"   %s:%s%s ",three_name[no-1],typical_value[no-1], unit[no-1]);
				OLED_ShowString(0,6,info_str,16);
				delay_ms(4000);		
				function_key_flat = 0; //进入校准状态，禁止功能键的作用
				cali_state = CALI_1; //状态转移
				OLED_Clear(); 
				break;
		}			

		case CALI_1: //进入输入数字状态
		{
				if(no == 3 && CL_Single_Cali == 1)
				{
					  OLED_ShowString(0,1,"   Input Slope",16);
					 	sprintf(info_str,"   %s:",three_name[no-1]);
						OLED_ShowString(0,4,info_str,16);
						OLED_ShowString(60,4,in_str,16);
				}
				else
				{
						sprintf(info_str," %s:",three_name[no-1]);
						OLED_ShowString(0,0,info_str,16);
						OLED_ShowString(60,0,in_str,16);
				}

				delay_ms(600);
				cali_key_ = Input_key();
		
				if(cali_key_ != 'x'){ //没有任何输入的情况
					
					if(cali_key_ == 'l')  //left用作消除输入
					{
						OLED_Clear();
						u8 cali_len = strlen(in_str);
						if(cali_len)
							in_str[cali_len-1] = 0x00;
					}
					else if(cali_key_ == '#')  //输入结束
					{
							//判断是否输入了正确长度的数字
						  u8 cali_len = strlen(in_str);
							if(cali_len >= 1)
							{
								cal_x1 = atof(in_str);
								if( cal_x1 <= maximum[no-1] && cal_x1 >= minimum[no-1])  //限制范围测量
								{
									for(u8 i =0;i<=cali_len;i++)
									{
										in_str[i] = 0x00;
									}
									if( no == 3 && CL_Single_Cali == 1)
										cali_state = CALI_5; //因为氯只需要单点校准，所以直接进入状态5
									else
									{
										cali_state = CALI_2; //进入下一状态
									}
									
									delay_ms(500);
									OLED_Clear();
								}
								else{
									OLED_Clear();
									OLED_ShowString(0,0,"   Please Input",16);
									OLED_ShowString(0,2,"   Right Range ",16);
									sprintf(info_str,"    %s %s",three_name[no-1], three_range[no-1]);
									OLED_ShowString(0,5,info_str,16);
									delay_ms(2000);
									OLED_Clear();
								}
							}else
							{
									OLED_Clear();
									OLED_ShowString(0,0,"   Please Input",16);
									OLED_ShowString(0,2,"     A Number  ",16);
									sprintf(info_str,"    %s %s",three_name[no-1], three_range[no-1]);
									OLED_ShowString(0,5,info_str,16);
									delay_ms(2000);
									OLED_Clear();
							
							}
					}
					else if(cali_key_ == 'o') //输入了ok 代表推出校准
					{
							delay_ms(500);
							OLED_Clear();
							cali_state = CALI_0;
							function_key_flat = 1; //开启功能选择
							sys_state = DEFAULT_STATE;
							
					}
					else if(cali_key_ == 'u') //输入了up 回到上一步
					{
						delay_ms(500);
						OLED_Clear();
						//输入的是u，表示回退，不做任何处理
					}
					else{
						string_cali_key[0] = cali_key_;
						sprintf(in_str, "%s%s",in_str, string_cali_key);
					}

				} 
				break;
		}
				
		case CALI_2: //改状态，校准当前的
		{
				adc1 = TM7705_ReadAdc(adc_c[no-1]);	/* 执行时间 80ms */	
				volt1 = (adc1 * 5000) / 65535; //计算第no个参数的ADC采样值
				adc1 = TM7705_ReadAdc(adc_c[no-1]);	/* 执行时间 80ms */	
				volt1 = (adc1 * 5000) / 65535;
		    //plt
				sprintf(s, "%.2f #",cal_x1);
				sprintf(info_str," %s:",three_name[no-1]);
				OLED_ShowString(0,0,info_str,16);
				OLED_ShowString(60,0,s,16);
				sprintf(s, "%3d", volt1); //第二行
				OLED_ShowString(0,2," Volt:",16);
				OLED_ShowString(60,2,s,16);
				//待数据稳定后输出确认
				delay_ms(1000);
				cali_key_ = Input_key();
				if(cali_key_ != 'x'){ //没有任何输入的情况
					if(cali_key_ == '#')
					{
						cali_state = CALI_3; //进入下一状态
					  cal_y1 = volt1;
						delay_ms(500);
						OLED_Clear();
					}
					else if(cali_key_ == 'o') //输入了ok 代表推出校准
					{
						delay_ms(500);
						OLED_Clear();
						cali_state = CALI_0;
						function_key_flat = 1; //开启功能选择
						sys_state = DEFAULT_STATE;
					}
					else if(cali_key_ == 'u') //输入了up 回到上一步
					{
						delay_ms(500);
						OLED_Clear();
						cali_state = CALI_1;
						for(int i=0;i<=M-1;i++)
						{
							in_str[i] = 0x00;  //清空输入
						}				
					}
		
				}
				break;
		
		}

		case CALI_3:
		{
			//plt
				sprintf(s, "%.2f #",cal_x1);
				sprintf(info_str," %s:",three_name[no-1]);
				OLED_ShowString(0,0,info_str,16);
				OLED_ShowString(60,0,s,16);
				sprintf(s, "%3d #", cal_y1); //第二行
				OLED_ShowString(0,2," Volt:",16);
				OLED_ShowString(60,2,s,16);
				sprintf(info_str," %s:",three_name[no-1]);
				OLED_ShowString(0,4,info_str,16);
				OLED_ShowString(60,4,in_str,16);
				delay_ms(500);
				cali_key_ = Input_key();
				if(cali_key_ != 'x'){ //没有任何输入的情况
					if(cali_key_ == 'l')  //left用作消除输入
					{
						OLED_Clear();
						u8 cali_len = strlen(in_str);
						if(cali_len)
							in_str[cali_len-1] = 0x00;
					}else if(cali_key_ == '#')  //输入结束
					{
							u8 cali_len = strlen(in_str);
							if(cali_len >= 1)
							{
								cal_x2 = atof(in_str);
								if( cal_x2 <= maximum[no-1] && cal_x1 >= minimum[no-1])  //限制范围测量
								{
									for(u8 i =0;i<=cali_len;i++)
									{
										in_str[i] = 0x00;
									}
										cali_state = CALI_4; //进入下一状态
										delay_ms(500);
										OLED_Clear();
								  }
								  else
									{
										OLED_Clear();
										OLED_ShowString(0,0,"   Please Input",16);
										OLED_ShowString(0,2,"   Right Range ",16);
										sprintf(info_str,"    %s %s",three_name[no-1], three_range[no-1]);
										OLED_ShowString(0,5,info_str,16);
										delay_ms(2000);
										OLED_Clear();
								  }
							}
							else
							{
									OLED_Clear();
									OLED_ShowString(0,0,"   Please Input",16);
									OLED_ShowString(0,2,"     A Number  ",16);
									sprintf(info_str,"    %s %s",three_name[no-1], three_range[no-1]);
									OLED_ShowString(0,5,info_str,16);
									delay_ms(2000);
									OLED_Clear();
							
							}
					}
					else if(cali_key_ == 'o') //输入了ok 代表推出校准
					{
							delay_ms(500);
							OLED_Clear();
							cali_state = CALI_0;
							function_key_flat = 1; //开启功能选择
							sys_state = DEFAULT_STATE;
					}
					else if(cali_key_ == 'u') //输入了up 回到上一步
					{
						delay_ms(500);
						OLED_Clear();
						cali_state = CALI_2;
						for(int i=0;i<=M-1;i++)
						{
							in_str[i] = 0x00;  //清空输入
						}				
					}
					else{
						string_cali_key[0] = cali_key_;
						sprintf(in_str, "%s%s",in_str, string_cali_key);
					}

				} 
				break;
		}
				
		case CALI_4:
		{
				adc1 = TM7705_ReadAdc(adc_c[no-1]);	/* 执行时间 80ms */	
				volt1 = (adc1 * 5000) / 65535;
				adc1 = TM7705_ReadAdc(adc_c[no-1]);	/* 执行时间 80ms */	
				volt1 = (adc1 * 5000) / 65535;
				//plt
				sprintf(s, "%.2f #",cal_x1);
				sprintf(info_str," %s:",three_name[no-1]);
				OLED_ShowString(0,0,info_str,16);
				OLED_ShowString(60,0,s,16);
				sprintf(s, "%3d #", cal_y1); //第二行
				OLED_ShowString(0,2," Volt:",16);
				OLED_ShowString(60,2,s,16);
		    sprintf(s, "%.2f #",cal_x2);
				sprintf(info_str," %s:",three_name[no-1]);
				OLED_ShowString(0,4,info_str,16);
				OLED_ShowString(60,4,s,16);
				sprintf(s, "%3d", volt1); //第二行
				OLED_ShowString(0,6," Volt:",16);
				OLED_ShowString(60,6,s,16);
				//待数据稳定后输出确认
				delay_ms(1000);
				cali_key_ = Input_key();
				if(cali_key_ != 'x'){ //没有任何输入的情况
					if(cali_key_ == '#')
					{
						cali_state = CALI_5; //进入下一状态
					  cal_y2 = volt1;
						delay_ms(500);
						OLED_Clear();
					}
					else if(cali_key_ == 'o') //输入了ok 代表推出校准
					{
							delay_ms(500);
							OLED_Clear();
							cali_state = CALI_0;
							function_key_flat = 1; //开启功能选择
							sys_state = DEFAULT_STATE;
					}
					else if(cali_key_ == 'u') //输入了up 回到上一步
					{
						delay_ms(500);
						OLED_Clear();
						cali_state = CALI_3;
						for(int i=0;i<=M-1;i++)
						{
							in_str[i] = 0x00;  //清空输入
						}				
					}
				}
				break;
		
		}
				
		case CALI_5:  //完成了校准，进行系数计算并存储在eeprom中
		{
				//plt
				sprintf(s, "%.2f #",cal_x1);
				sprintf(info_str," %s:",three_name[no-1]);
				OLED_ShowString(0,0,info_str,16);
				OLED_ShowString(60,0,s,16);
				sprintf(s, "%3d #", cal_y1); //第二行
				OLED_ShowString(0,2," Volt:",16);
				OLED_ShowString(60,2,s,16);
		    sprintf(s, "%.2f #",cal_x2);
				sprintf(info_str," %s:",three_name[no-1]);
				OLED_ShowString(0,4,info_str,16);
				OLED_ShowString(60,4,s,16);
				sprintf(s, "%3d #", cal_y2); //第二行
				OLED_ShowString(0,6," Volt:",16);
				OLED_ShowString(60,6,s,16);
				delay_ms(3000);
				
				//进入系数计算和存储
				OLED_Clear();
				OLED_ShowString(0,0,"      Finish  ",16);
				OLED_ShowString(0,2,"   Calibration ",16);
				OLED_ShowString(0,4,"       .... ",16);
		    delay_ms(2000);
				for(u8 i =0;i<=9;i++)
				{
				in_str[i] = 0x00;
				}
				//计算系数并将其存储到eeprom中
				if( no == 3 && CL_Single_Cali == 1)
				{
					k = cal_x1;
					b = 0;
				}
					else
				{
					k = (cal_y2 - cal_y1)/(cal_x2 - cal_x1);
					b = (cal_y1 * cal_x2 - cal_y2 * cal_x1)/(cal_x2 - cal_x1);
				}
				
				//为了避免 重新校准 单个参数 导致 整个参数被抹掉
				//先读取现有的参数值，存储在校准矩阵中，
				//再去校准新的参数
				//读取eprom
				I2C_EE_BufferRead(I2c_Buf_Read, EEP_Firstpage,Para_Len); 
				for( int i = 0; i < Para_Len; i++)
				{ 
				   I2c_Buf_Write[i] = I2c_Buf_Read[i];
				}
				
			
				//用字符串存储第一个系数
				for(int j =0;j<M;j++)
						in_str[j]=0x00;
				for(int i = 0; i<M;i++)
				{
					int indx = (no-1)*2*M+i;
					sprintf(in_str, "%.4f",k);
					I2c_Buf_Write[indx] = in_str[i];
				}
				//用字符串存储第二个系数
				for(int j =0;j<M;j++)
						in_str[j]=0x00;
				for(int i = M; i<2*M;i++)
				{
					int indx = (no-1)*2*M+i;
					sprintf(in_str, "%.4f",b);
					I2c_Buf_Write[indx] = in_str[i-M];
				}
				for(int j =0;j<M;j++)
					in_str[j]=0x00;
				
				//写入rom
				I2C_EE_BufferWrite(I2c_Buf_Write, EEP_Firstpage,Para_Len);
				
				//读取eprom
				I2C_EE_BufferRead(I2c_Buf_Read, EEP_Firstpage,Para_Len); 
				
				//check 读写正确与否
				for(int i=0; i <Para_Len; i++ )
				{
						if(I2c_Buf_Read[i] != I2c_Buf_Write[i])
						{
								OLED_Clear();
								OLED_ShowString(0,0,"  EEPROM ",16);
								OLED_ShowString(0,2,"  ERROR ",16);
								delay_ms(2000);
								OLED_Clear();
						}
				}
				
				//校正完成回到主界面
				OLED_Clear();
				cali_state = CALI_0;
				function_key_flat = 1;  //开启功能选择
				sys_state = PARA_UPDATA;//进入 系数更新的状态
				cal_x1 = 0;
				cal_x2 = 0;
				cal_y1 =0;
				cal_y2 =0;
		}
				
	}

}

/*
*********************************************************************************************************
*	函 数 名: Tip_task
*	功能说明: 告知用户一些功能建议
*	形    参: 无
*	返 回 值: 无
*******
*/
void Test_task2(float*p)
{
		char *three_name [3] = {"PH", "ORP", "CL2"};
		unsigned char s[10]; 
		float measurement[P_NUm];
		for(int no = 0; no<P_NUm; no++)
		{
			float k = *(p + no*2);
			float b = *(p + no*2 + 1);
			sprintf(s, " %s:%.1f %.0f",three_name[no],k,b); //第二行
			OLED_ShowString(0,no*2,s,16);
		}
	delay_ms(5000); //延时2秒之后回到基本测量状态
	OLED_Clear();
	sys_state = DEFAULT_STATE;
}



/*
*********************************************************************************************************
*	函 数 名: Tip_task
*	功能说明: 告知用户一些功能建议
*	形    参: 无
*	返 回 值: 无

*********************************************************************************************************
*/
void Tip_task(void)
{
	OLED_ShowString(0,0,"     Press ",16);
	OLED_ShowString(0,2,"   Controller",16);
	OLED_ShowString(0,4,"      For ",16);
	OLED_ShowString(0,6,"   Calibration",16);
	delay_ms(2000);
	OLED_Clear();
	OLED_ShowString(0,0," Left  -> PH  ",16);
	OLED_ShowString(0,2," Up    -> ORP ",16);
	OLED_ShowString(0,4," Right -> PH  ",16);
	OLED_ShowString(0,6," OK    -> Home",16);
	delay_ms(5000); //延时2秒之后回到基本测量状态
	OLED_Clear();
	sys_state = DEFAULT_STATE;
}

/*
*********************************************************************************************************
*	函 数 名: Input_key
*	功能说明: 扫描红外按键
*	形    参: 无
*	返 回 值: 无

*********************************************************************************************************
*/
char Input_key(void)
{
	char key_value;
	u8 key =Remote_Scan();
	delay_ms(100);
	switch(key)
	{		   
				case 162:key_value='1';				break;	     //1    
				case 98: key_value='2';				break;	     //2
				case 2:  key_value='5';				break;		   //5
				case 226:key_value='3';				break;		   //3  
				case 194:key_value='6';				break;	     //6
				case 34: key_value='4';				break;		   //4
				case 224:key_value='7';				break;		   //7
				case 168:key_value='8';				break;		   //8
				case 144:key_value='9';				break;		   //9
				case 104:key_value='.';				break;		   //* 			小数点
				case 152:key_value='0';				break;	     //0
				case 176:key_value='#';				break;	     //#  		输入完成
				case 24: key_value='u';			break;		   //up	 	未使用 
				case 16: key_value='l';		    break;		   //left   输入回退					
				case 56: key_value='o';			  break;	     //ok			推出校准 回到主菜单
				//case 90: key_value='r';		  break;     //right  
				default: key_value = 'x';    //x代表未输入
	}
	
	return key_value;
}

/*
*********************************************************************************************************
*	函 数 名: parameter_init
*	功能说明: 初始化校准系数
*	形    参: no 第几个参数 1-ph 2-orp 3-cl2
*	返 回 值: 无

*********************************************************************************************************
*/
float* parameter_init(int no)
{				
	static float para[2];
	char string_cali_key[1];
	//读取eprom
	I2C_EE_BufferRead(I2c_Buf_Read, EEP_Firstpage,Para_Len); 				

	//combine
	for(int j =0;j<M;j++)in_str[j]=0x00;
	for(int i = 0; i<M;i++)
	{
		int indx = (no-1)*2*M+i;
		string_cali_key[0] = I2c_Buf_Read[indx];
		sprintf(in_str, "%s%s",in_str, string_cali_key);
	}
	para[0] = atof(in_str);
	
	for(int j =0;j<M;j++)in_str[j]=0x00;
	for(int i = M; i<2*M;i++)
	{
		int indx = (no-1)*2*M+i;
		string_cali_key[0] = I2c_Buf_Read[indx];
		sprintf(in_str, "%s%s",in_str, string_cali_key);
	}
	para[1] = atof(in_str);
	for(int j =0;j<M;j++)in_str[j]=0x00;

	return para;
}

/*
*********************************************************************************************************
*	函 数 名: reload_para
*	功能说明: 重新读取校准系数
*	形    参: no 第几个参数 1-ph 2-orp 3-cl2
*	返 回 值: 无

*********************************************************************************************************
*/
float* reload_para(void)
{
	u8 base_reg = 8;
	//从EE prom读取参数
	static float para[Para_Num];
	float *p;
	for(int no = 1; no<=P_NUm; no++)
	{
		p = parameter_init(no);
		for(int j =0; j<2; j++)
		{
			para[(no-1)*2+j] = *(p+j);
			
			// 将参数传入Reg矩阵进行测试
			// 第一个参数是斜率 第二个参数是截距
			int16_t tmp_ = (int16_t) para[(no-1)*2+j]*Amp;
			Reg[base_reg+(no-1)*4 +j*2] =     (tmp_ & 0xFFFF0000) >> 16;
			Reg[base_reg+(no-1)*4 +j*2 + 1] = (tmp_ & 0x0000FFFF);
		}
	}
	return para;
}