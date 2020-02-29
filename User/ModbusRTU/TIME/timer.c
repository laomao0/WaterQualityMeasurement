#include "timer.h"
#include "modbus_uart.h"
#include "modbus.h"

void Timer2_Init()    //1ms����1�θ����¼�
{
	TIM_TimeBaseInitTypeDef timer;
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	TIM_DeInit(TIM2);
	timer.TIM_Period=1000-1;//   1ms
	timer.TIM_Prescaler=72-1;// 72M/72=1MHZ-->1us
	timer.TIM_ClockDivision=TIM_CKD_DIV1;
	timer.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2,&timer);
	
	TIM_Cmd(TIM2,ENABLE);
	
	TIM_ITConfig(TIM2, TIM_IT_Update,ENABLE);

}

void Isr_Init()
{
	NVIC_InitTypeDef  isr;
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 
	
	// ���ö�ʱ��2
	isr.NVIC_IRQChannel=TIM2_IRQn;
	isr.NVIC_IRQChannelCmd=ENABLE;
	isr.NVIC_IRQChannelPreemptionPriority=1;
	isr.NVIC_IRQChannelSubPriority=2;	
	NVIC_Init(&isr);   //	
	
	isr.NVIC_IRQChannel=USART2_IRQn;
	isr.NVIC_IRQChannelCmd=ENABLE;
	isr.NVIC_IRQChannelPreemptionPriority=1;
	isr.NVIC_IRQChannelSubPriority=1;	
	NVIC_Init(&isr);   //
}

void delay(u32 x)
{

 while(x--);
}

void TIM2_IRQHandler()//��ʱ��2���жϷ����Ӻ���  1msһ���ж�
{
  u8 st;
  st= TIM_GetFlagStatus(TIM2, TIM_FLAG_Update);	
	if(st==SET)
	{
	  TIM_ClearFlag(TIM2, TIM_FLAG_Update);
		if(modbus.timrun!=0)
		{
		  modbus.timout++; 
		  if(modbus.timout>=8)  //���ʱ��ﵽ��ʱ��
			{
				modbus.timrun=0;//�رն�ʱ��--ֹͣ��ʱ
				modbus.reflag=1;  //�յ�һ֡����
			}
		}  		
	}	
}




