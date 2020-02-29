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
*	�� �� ��: Regular_task
*	����˵��: ��ȡ��ͨ�����ݺ��¶ȣ���ʾ��ֱ����ʾ
*	��    ��: float*p ָ���������
*	�� �� ֵ: ��
�޸ģ���Ҫ�޸ĵĹ��ܣ���У׼ĳһ������ʱ��ѡ����up��λ���ܹ��ص��ϸ�״̬�����½�����һ��
*********************************************************************************************************
*/
void Regular_task(float*p)
{
		uint16_t adc[P_NUm];  //����ADC
		short tmpeture;  //�¶�ֵ
		unsigned char s[10]; 
		int volt[3];
		char *state;
		/* ˫ͨ���л�������ִ��һ��ʵ����Լ 160ms */	
		adc[0] = TM7705_ReadAdc(1); //CL2	/* ִ��ʱ�� 80ms */		
		adc[1] = TM7705_ReadAdc(2);	//ORP/* ִ��ʱ�� 80ms */
		adc[2] = TM7705_ReadAdc(4);	//PH
		tmpeture = DS18B20_Get_Temp();				
		/* ��ӡ�ɼ����� */

		/* ����ʵ�ʵ�ѹֵ�����ƹ���ģ�������׼ȷ�������У׼ */
		volt[0] = (adc[0] * 5000) / 65535; //CL2
		volt[1] = (adc[1] * 5000) / 65535; //ORP
		volt[2] = (adc[2] * 5000) / 65535; //Ph
		
		// ���ݵ�ѹֵ�����������ϵ����ֵ
		float measurement[P_NUm];
		for(int no = 0; no<P_NUm; no++)
		{
			float k = *(p + no*2);
			float b = *(p + no*2 + 1);
			measurement[no] = (volt[2-no] - b)/k;
		}
		if(tmpeture<0)
		{
			tmpeture=-tmpeture;					//תΪ����
		}
		
		//��������������ֵ�ŵ�modbus��������
		//����洢����ADC�����õ��ĵ�ѹֵ
		//Reg[0] = (volt[0] & 0xFFFF0000) >> 16;
		//Reg[1] = (volt[0] & 0x0000FFFF);
		//Reg[2] = (volt[1] & 0xFFFF0000) >> 16;
		//Reg[3] = (volt[1] & 0x0000FFFF);
		//Reg[4] = (volt[2] & 0xFFFF0000) >> 16;
		//Reg[5] = (volt[2] & 0x0000FFFF);
		//Reg[6] = (tmpeture & 0xFFFF0000) >> 16;
		//Reg[7] = (tmpeture & 0x0000FFFF);
		
		//����洢���Ǽ���õ������ݣ������� �⵽���ĸ���������100ǿ��ת��Ϊ16λ�Ķ�����ֵ
		// ֻ�Ǵ洢��ʵʱ����ֵ
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
		OLED_ShowString(0,0,s,16);//��һ�� -- 8x16����ʾ��Ԫ��ʾASCII��
		if (volt[1] <= 0 || volt[1]  >= 5000) state = "?"; else state = ">";
		sprintf(s, "%s ORP %.0f  mv", state, measurement[1]);
		OLED_ShowString(0,2,s,16);//��һ�� -- 8x16����ʾ��Ԫ��ʾASCII��
		if (volt[2] <= 0 || volt[2]  >= 5000) state = "?"; else state = ">";
		//double cl2 = (volt3 * cl_cali_para / 400.0);
		//sprintf(s, "%s CL2 %.2f ppm",state, cl2);
		sprintf(s, "%s CL  %.2f ppm", state, measurement[2]);
		OLED_ShowString(0,4,s,16);//��һ�� -- 8x16����ʾ��Ԫ��ʾASCII��
		DS18B20_Rst();
		if (DS18B20_Check()) state = "?"; else state = ">";
		sprintf(s, "%s TEM %2d.%d  C",state,tmpeture/10,tmpeture%10);
		OLED_ShowString(0,6,s,16);//��һ�� -- 8x16����ʾ��Ԫ��ʾASCII��
		delay_ms(800);	
		sys_state = DEFAULT_STATE;			
}

/*
*********************************************************************************************************
*	�� �� ��: Regular_task
*	����˵��: ��ȡ��ͨ�����ݺ��¶ȣ���ʾ��ֱ����ʾ
*	��    ��: ��
*	�� �� ֵ: ��
�޸ģ���Ҫ�޸ĵĹ��ܣ���У׼ĳһ������ʱ��ѡ����up��λ���ܹ��ص��ϸ�״̬�����½�����һ��
*********************************************************************************************************
*/
void Test_task(void)
{
		char cali_key_;
		uint16_t adc1, adc2, adc3;  //����ADC
		short tmpeture;  //�¶�ֵ
		unsigned char s[10]; 
		int volt1, volt2, volt3;
		char *state;
		/* ˫ͨ���л�������ִ��һ��ʵ����Լ 160ms */			
		adc1 = TM7705_ReadAdc(1);	/*CL ִ��ʱ�� 80ms */		
		adc2 = TM7705_ReadAdc(2);	/*ORP ִ��ʱ�� 80ms */
		adc3 = TM7705_ReadAdc(4);	//PH
		tmpeture = DS18B20_Get_Temp();				
		/* ��ӡ�ɼ����� */

		/* ����ʵ�ʵ�ѹֵ�����ƹ���ģ�������׼ȷ�������У׼ */
		volt1 = (adc1 * 5000) / 65535;
		volt2 = (adc2 * 5000) / 65535;
		volt3 = (adc3 * 5000) / 65535;
		if(tmpeture<0)
		{
			tmpeture=-tmpeture;					//תΪ����
		}
		//display
		if (volt1 <= 0 || volt1 >= 5000) state = "?"; else state = "o";
		sprintf(s, "%s CL  %3d  mv",state, volt1);
		OLED_ShowString(0,0,s,16);//��һ�� -- 8x16����ʾ��Ԫ��ʾASCII��
		if (volt2 <= 0 || volt2  >= 5000) state = "?"; else state = "o";
		sprintf(s, "%s ORP %3d  mv", state, volt2);
		OLED_ShowString(0,2,s,16);//��һ�� -- 8x16����ʾ��Ԫ��ʾASCII��
		if (volt3 <= 0 || volt3  >= 5000) state = "?"; else state = "o";
		//double cl2 = (volt3 * cl_cali_para / 400.0);
		//sprintf(s, "%s CL2 %.2f ppm",state, cl2);
		sprintf(s, "%s PH  %3d  mv", state, volt3);
		OLED_ShowString(0,4,s,16);//��һ�� -- 8x16����ʾ��Ԫ��ʾASCII��
		DS18B20_Rst();
		if (DS18B20_Check()) state = "?"; else state = "o";
		sprintf(s, "%s TEM %2d.%d  C",state,tmpeture/10,tmpeture%10);
		OLED_ShowString(0,6,s,16);//��һ�� -- 8x16����ʾ��Ԫ��ʾASCII��
		
		delay_ms(600);
		cali_key_ = Input_key();
		if(cali_key_ != 'x'){ //û���κ���������
			if(cali_key_ == 'o') //������ok �����Ƴ�����ģʽ
					{
						delay_ms(500);
						OLED_Clear();
						sys_state = DEFAULT_STATE;	
					}
		}
	  
}


/*
*********************************************************************************************************
*	�� �� ��: Cali_task
*	����˵��: ��׼У׼����
* 					PH ORP CL2���ӵ�ADC��ͨ���ֱ��� 2,1,0
*	��    ��: ��
*	�� �� ֵ: no = 1��У��PH�� no= 2��У��ORP�� no=3 У������

*********************************************************************************************************
*/
void Cali_task(int no)
{
	char *three_name [3] = {"PH", "ORP", "CL2"};
	char *three_range [3] = {"1--14", "0-4000", "0-800"}; //��������������ȡֵ��Χ
	char * typical_value [3] = { "7.00#", "2000#", "400#"};
	char * unit[3] = {" ", "(mv)", "(ppm)"};
	int adc_c[3] = {4,2,1};
	//PH��Χ 1-14 ORP��Χ 0-40000 mv ���ȷ�Χ 0-20 ppm
	uint16_t minimum[3] = {1,  0, 0}; 
	uint16_t maximum[3] = {14, 4000, 800};
	char cali_key;
	uint16_t adc1;
	int volt1;
	unsigned char s[10]; 
	char cali_key_;
	char string_cali_key[1];
	float k; //ϵ��б��
	float b; //ϵ���ؾ�
	unsigned char info_str[10]; //ר����������oledͼƬ��
	
	switch(cali_state)
	{
		case CALI_0:
		{
				sprintf(info_str, "       %s   ",three_name[no-1]); //У����ʽ��no������
				OLED_ShowString(0,0,info_str,16);
				OLED_ShowString(0,2,"   Calibration  ",16);
				OLED_ShowString(0,4,"   For example  ",16);
				sprintf(info_str,"   %s:%s%s ",three_name[no-1],typical_value[no-1], unit[no-1]);
				OLED_ShowString(0,6,info_str,16);
				delay_ms(4000);		
				function_key_flat = 0; //����У׼״̬����ֹ���ܼ�������
				cali_state = CALI_1; //״̬ת��
				OLED_Clear(); 
				break;
		}			

		case CALI_1: //������������״̬
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
		
				if(cali_key_ != 'x'){ //û���κ���������
					
					if(cali_key_ == 'l')  //left������������
					{
						OLED_Clear();
						u8 cali_len = strlen(in_str);
						if(cali_len)
							in_str[cali_len-1] = 0x00;
					}
					else if(cali_key_ == '#')  //�������
					{
							//�ж��Ƿ���������ȷ���ȵ�����
						  u8 cali_len = strlen(in_str);
							if(cali_len >= 1)
							{
								cal_x1 = atof(in_str);
								if( cal_x1 <= maximum[no-1] && cal_x1 >= minimum[no-1])  //���Ʒ�Χ����
								{
									for(u8 i =0;i<=cali_len;i++)
									{
										in_str[i] = 0x00;
									}
									if( no == 3 && CL_Single_Cali == 1)
										cali_state = CALI_5; //��Ϊ��ֻ��Ҫ����У׼������ֱ�ӽ���״̬5
									else
									{
										cali_state = CALI_2; //������һ״̬
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
					else if(cali_key_ == 'o') //������ok �����Ƴ�У׼
					{
							delay_ms(500);
							OLED_Clear();
							cali_state = CALI_0;
							function_key_flat = 1; //��������ѡ��
							sys_state = DEFAULT_STATE;
							
					}
					else if(cali_key_ == 'u') //������up �ص���һ��
					{
						delay_ms(500);
						OLED_Clear();
						//�������u����ʾ���ˣ������κδ���
					}
					else{
						string_cali_key[0] = cali_key_;
						sprintf(in_str, "%s%s",in_str, string_cali_key);
					}

				} 
				break;
		}
				
		case CALI_2: //��״̬��У׼��ǰ��
		{
				adc1 = TM7705_ReadAdc(adc_c[no-1]);	/* ִ��ʱ�� 80ms */	
				volt1 = (adc1 * 5000) / 65535; //�����no��������ADC����ֵ
				adc1 = TM7705_ReadAdc(adc_c[no-1]);	/* ִ��ʱ�� 80ms */	
				volt1 = (adc1 * 5000) / 65535;
		    //plt
				sprintf(s, "%.2f #",cal_x1);
				sprintf(info_str," %s:",three_name[no-1]);
				OLED_ShowString(0,0,info_str,16);
				OLED_ShowString(60,0,s,16);
				sprintf(s, "%3d", volt1); //�ڶ���
				OLED_ShowString(0,2," Volt:",16);
				OLED_ShowString(60,2,s,16);
				//�������ȶ������ȷ��
				delay_ms(1000);
				cali_key_ = Input_key();
				if(cali_key_ != 'x'){ //û���κ���������
					if(cali_key_ == '#')
					{
						cali_state = CALI_3; //������һ״̬
					  cal_y1 = volt1;
						delay_ms(500);
						OLED_Clear();
					}
					else if(cali_key_ == 'o') //������ok �����Ƴ�У׼
					{
						delay_ms(500);
						OLED_Clear();
						cali_state = CALI_0;
						function_key_flat = 1; //��������ѡ��
						sys_state = DEFAULT_STATE;
					}
					else if(cali_key_ == 'u') //������up �ص���һ��
					{
						delay_ms(500);
						OLED_Clear();
						cali_state = CALI_1;
						for(int i=0;i<=M-1;i++)
						{
							in_str[i] = 0x00;  //�������
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
				sprintf(s, "%3d #", cal_y1); //�ڶ���
				OLED_ShowString(0,2," Volt:",16);
				OLED_ShowString(60,2,s,16);
				sprintf(info_str," %s:",three_name[no-1]);
				OLED_ShowString(0,4,info_str,16);
				OLED_ShowString(60,4,in_str,16);
				delay_ms(500);
				cali_key_ = Input_key();
				if(cali_key_ != 'x'){ //û���κ���������
					if(cali_key_ == 'l')  //left������������
					{
						OLED_Clear();
						u8 cali_len = strlen(in_str);
						if(cali_len)
							in_str[cali_len-1] = 0x00;
					}else if(cali_key_ == '#')  //�������
					{
							u8 cali_len = strlen(in_str);
							if(cali_len >= 1)
							{
								cal_x2 = atof(in_str);
								if( cal_x2 <= maximum[no-1] && cal_x1 >= minimum[no-1])  //���Ʒ�Χ����
								{
									for(u8 i =0;i<=cali_len;i++)
									{
										in_str[i] = 0x00;
									}
										cali_state = CALI_4; //������һ״̬
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
					else if(cali_key_ == 'o') //������ok �����Ƴ�У׼
					{
							delay_ms(500);
							OLED_Clear();
							cali_state = CALI_0;
							function_key_flat = 1; //��������ѡ��
							sys_state = DEFAULT_STATE;
					}
					else if(cali_key_ == 'u') //������up �ص���һ��
					{
						delay_ms(500);
						OLED_Clear();
						cali_state = CALI_2;
						for(int i=0;i<=M-1;i++)
						{
							in_str[i] = 0x00;  //�������
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
				adc1 = TM7705_ReadAdc(adc_c[no-1]);	/* ִ��ʱ�� 80ms */	
				volt1 = (adc1 * 5000) / 65535;
				adc1 = TM7705_ReadAdc(adc_c[no-1]);	/* ִ��ʱ�� 80ms */	
				volt1 = (adc1 * 5000) / 65535;
				//plt
				sprintf(s, "%.2f #",cal_x1);
				sprintf(info_str," %s:",three_name[no-1]);
				OLED_ShowString(0,0,info_str,16);
				OLED_ShowString(60,0,s,16);
				sprintf(s, "%3d #", cal_y1); //�ڶ���
				OLED_ShowString(0,2," Volt:",16);
				OLED_ShowString(60,2,s,16);
		    sprintf(s, "%.2f #",cal_x2);
				sprintf(info_str," %s:",three_name[no-1]);
				OLED_ShowString(0,4,info_str,16);
				OLED_ShowString(60,4,s,16);
				sprintf(s, "%3d", volt1); //�ڶ���
				OLED_ShowString(0,6," Volt:",16);
				OLED_ShowString(60,6,s,16);
				//�������ȶ������ȷ��
				delay_ms(1000);
				cali_key_ = Input_key();
				if(cali_key_ != 'x'){ //û���κ���������
					if(cali_key_ == '#')
					{
						cali_state = CALI_5; //������һ״̬
					  cal_y2 = volt1;
						delay_ms(500);
						OLED_Clear();
					}
					else if(cali_key_ == 'o') //������ok �����Ƴ�У׼
					{
							delay_ms(500);
							OLED_Clear();
							cali_state = CALI_0;
							function_key_flat = 1; //��������ѡ��
							sys_state = DEFAULT_STATE;
					}
					else if(cali_key_ == 'u') //������up �ص���һ��
					{
						delay_ms(500);
						OLED_Clear();
						cali_state = CALI_3;
						for(int i=0;i<=M-1;i++)
						{
							in_str[i] = 0x00;  //�������
						}				
					}
				}
				break;
		
		}
				
		case CALI_5:  //�����У׼������ϵ�����㲢�洢��eeprom��
		{
				//plt
				sprintf(s, "%.2f #",cal_x1);
				sprintf(info_str," %s:",three_name[no-1]);
				OLED_ShowString(0,0,info_str,16);
				OLED_ShowString(60,0,s,16);
				sprintf(s, "%3d #", cal_y1); //�ڶ���
				OLED_ShowString(0,2," Volt:",16);
				OLED_ShowString(60,2,s,16);
		    sprintf(s, "%.2f #",cal_x2);
				sprintf(info_str," %s:",three_name[no-1]);
				OLED_ShowString(0,4,info_str,16);
				OLED_ShowString(60,4,s,16);
				sprintf(s, "%3d #", cal_y2); //�ڶ���
				OLED_ShowString(0,6," Volt:",16);
				OLED_ShowString(60,6,s,16);
				delay_ms(3000);
				
				//����ϵ������ʹ洢
				OLED_Clear();
				OLED_ShowString(0,0,"      Finish  ",16);
				OLED_ShowString(0,2,"   Calibration ",16);
				OLED_ShowString(0,4,"       .... ",16);
		    delay_ms(2000);
				for(u8 i =0;i<=9;i++)
				{
				in_str[i] = 0x00;
				}
				//����ϵ��������洢��eeprom��
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
				
				//Ϊ�˱��� ����У׼ �������� ���� ����������Ĩ��
				//�ȶ�ȡ���еĲ���ֵ���洢��У׼�����У�
				//��ȥУ׼�µĲ���
				//��ȡeprom
				I2C_EE_BufferRead(I2c_Buf_Read, EEP_Firstpage,Para_Len); 
				for( int i = 0; i < Para_Len; i++)
				{ 
				   I2c_Buf_Write[i] = I2c_Buf_Read[i];
				}
				
			
				//���ַ����洢��һ��ϵ��
				for(int j =0;j<M;j++)
						in_str[j]=0x00;
				for(int i = 0; i<M;i++)
				{
					int indx = (no-1)*2*M+i;
					sprintf(in_str, "%.4f",k);
					I2c_Buf_Write[indx] = in_str[i];
				}
				//���ַ����洢�ڶ���ϵ��
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
				
				//д��rom
				I2C_EE_BufferWrite(I2c_Buf_Write, EEP_Firstpage,Para_Len);
				
				//��ȡeprom
				I2C_EE_BufferRead(I2c_Buf_Read, EEP_Firstpage,Para_Len); 
				
				//check ��д��ȷ���
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
				
				//У����ɻص�������
				OLED_Clear();
				cali_state = CALI_0;
				function_key_flat = 1;  //��������ѡ��
				sys_state = PARA_UPDATA;//���� ϵ�����µ�״̬
				cal_x1 = 0;
				cal_x2 = 0;
				cal_y1 =0;
				cal_y2 =0;
		}
				
	}

}

/*
*********************************************************************************************************
*	�� �� ��: Tip_task
*	����˵��: ��֪�û�һЩ���ܽ���
*	��    ��: ��
*	�� �� ֵ: ��
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
			sprintf(s, " %s:%.1f %.0f",three_name[no],k,b); //�ڶ���
			OLED_ShowString(0,no*2,s,16);
		}
	delay_ms(5000); //��ʱ2��֮��ص���������״̬
	OLED_Clear();
	sys_state = DEFAULT_STATE;
}



/*
*********************************************************************************************************
*	�� �� ��: Tip_task
*	����˵��: ��֪�û�һЩ���ܽ���
*	��    ��: ��
*	�� �� ֵ: ��

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
	delay_ms(5000); //��ʱ2��֮��ص���������״̬
	OLED_Clear();
	sys_state = DEFAULT_STATE;
}

/*
*********************************************************************************************************
*	�� �� ��: Input_key
*	����˵��: ɨ����ⰴ��
*	��    ��: ��
*	�� �� ֵ: ��

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
				case 104:key_value='.';				break;		   //* 			С����
				case 152:key_value='0';				break;	     //0
				case 176:key_value='#';				break;	     //#  		�������
				case 24: key_value='u';			break;		   //up	 	δʹ�� 
				case 16: key_value='l';		    break;		   //left   �������					
				case 56: key_value='o';			  break;	     //ok			�Ƴ�У׼ �ص����˵�
				//case 90: key_value='r';		  break;     //right  
				default: key_value = 'x';    //x����δ����
	}
	
	return key_value;
}

/*
*********************************************************************************************************
*	�� �� ��: parameter_init
*	����˵��: ��ʼ��У׼ϵ��
*	��    ��: no �ڼ������� 1-ph 2-orp 3-cl2
*	�� �� ֵ: ��

*********************************************************************************************************
*/
float* parameter_init(int no)
{				
	static float para[2];
	char string_cali_key[1];
	//��ȡeprom
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
*	�� �� ��: reload_para
*	����˵��: ���¶�ȡУ׼ϵ��
*	��    ��: no �ڼ������� 1-ph 2-orp 3-cl2
*	�� �� ֵ: ��

*********************************************************************************************************
*/
float* reload_para(void)
{
	u8 base_reg = 8;
	//��EE prom��ȡ����
	static float para[Para_Num];
	float *p;
	for(int no = 1; no<=P_NUm; no++)
	{
		p = parameter_init(no);
		for(int j =0; j<2; j++)
		{
			para[(no-1)*2+j] = *(p+j);
			
			// ����������Reg������в���
			// ��һ��������б�� �ڶ��������ǽؾ�
			int16_t tmp_ = (int16_t) para[(no-1)*2+j]*Amp;
			Reg[base_reg+(no-1)*4 +j*2] =     (tmp_ & 0xFFFF0000) >> 16;
			Reg[base_reg+(no-1)*4 +j*2 + 1] = (tmp_ & 0x0000FFFF);
		}
	}
	return para;
}