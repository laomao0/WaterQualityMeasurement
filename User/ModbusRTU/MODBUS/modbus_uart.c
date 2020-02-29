#include "modbus_uart.h"
#include "modbus.h"
#include "bsp.h"


void RS485_Init()
{
    USART_InitTypeDef USART_InitStructure;
    GPIO_InitTypeDef  GPIO_InitStructure;
	 
	  // 使能GPIO管脚时钟
    RCC_APB2PeriphClockCmd(RS485_CTRL_RCC, ENABLE);
	
		// 使能usart 时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
    
	  //485收发控制引脚
    GPIO_InitStructure.GPIO_Pin = RS485_CTRL_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//GPIO_Mode_IN_FLOATING;//GPIO_Mode_AF_OD;//
    GPIO_Init(RS485_CTRL_PORT, &GPIO_InitStructure);
    RS485_RT_0; //使MAX485芯片处于接收状态
	
    //USART2_TX  
    GPIO_InitStructure.GPIO_Pin = DEBUG_USART2_TX_GPIO_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//GPIO_Mode_Out_PP;//GPIO_Mode_IN_FLOATING;//GPIO_Mode_AF_OD;//
    GPIO_Init(DEBUG_USART2_TX_GPIO_PORT, &GPIO_InitStructure);
   
    //USART2_RX	 
    GPIO_InitStructure.GPIO_Pin = DEBUG_USART2_RX_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//GPIO_Mode_IPU;//
    GPIO_Init(DEBUG_USART2_TX_GPIO_PORT, &GPIO_InitStructure);  

   //Usart2 NVIC

    USART_InitStructure.USART_BaudRate = 9600;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
		
    USART_Init(USART2, &USART_InitStructure);
    USART_ITConfig(USART2,USART_IT_RXNE,ENABLE); 
    USART_Cmd(USART2, ENABLE);
    USART_ClearFlag(USART2,USART_FLAG_TC );
}

void RS485_byte(u8 d)  //485发送一个字节
{
  
  USART_SendData(USART2, d);
	  while(USART_GetFlagStatus(USART2,USART_FLAG_TC)==RESET);
  USART_ClearFlag(USART2,USART_FLAG_TC );
   
  
}


void USART2_IRQHandler() //MODBUS字节接收中断
{
    u8 st,sbuf;
    st=USART_GetITStatus(USART2, USART_IT_RXNE);
    if(st==SET)  //
   {   		 
		  sbuf=USART2->DR;
     if( modbus.reflag==1)  //有数据包正在处理
		 {
		   return ;
		 }			 
		  modbus.rcbuf[modbus.recount++]=sbuf;
      modbus.timout=0;  
      if(modbus.recount==1)  //收到主机发来的一帧数据的第一字节
			{
			  modbus.timrun=1;  //启动定时
			}
   } 
}



