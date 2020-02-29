#include "modbus_uart.h"
#include "modbus.h"
#include "bsp.h"


void RS485_Init()
{
    USART_InitTypeDef USART_InitStructure;
    GPIO_InitTypeDef  GPIO_InitStructure;
	 
	  // ʹ��GPIO�ܽ�ʱ��
    RCC_APB2PeriphClockCmd(RS485_CTRL_RCC, ENABLE);
	
		// ʹ��usart ʱ��
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
    
	  //485�շ���������
    GPIO_InitStructure.GPIO_Pin = RS485_CTRL_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//GPIO_Mode_IN_FLOATING;//GPIO_Mode_AF_OD;//
    GPIO_Init(RS485_CTRL_PORT, &GPIO_InitStructure);
    RS485_RT_0; //ʹMAX485оƬ���ڽ���״̬
	
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

void RS485_byte(u8 d)  //485����һ���ֽ�
{
  
  USART_SendData(USART2, d);
	  while(USART_GetFlagStatus(USART2,USART_FLAG_TC)==RESET);
  USART_ClearFlag(USART2,USART_FLAG_TC );
   
  
}


void USART2_IRQHandler() //MODBUS�ֽڽ����ж�
{
    u8 st,sbuf;
    st=USART_GetITStatus(USART2, USART_IT_RXNE);
    if(st==SET)  //
   {   		 
		  sbuf=USART2->DR;
     if( modbus.reflag==1)  //�����ݰ����ڴ���
		 {
		   return ;
		 }			 
		  modbus.rcbuf[modbus.recount++]=sbuf;
      modbus.timout=0;  
      if(modbus.recount==1)  //�յ�����������һ֡���ݵĵ�һ�ֽ�
			{
			  modbus.timrun=1;  //������ʱ
			}
   } 
}



