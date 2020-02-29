#include "modbus.h"
#include "modbus_uart.h"
#include "modbusCRC.h"
#include "bsp.h"

MODBUS modbus;
extern u16 Reg[];



/*
��Ϊ������ 9600
1λ���ݵ�ʱ��Ϊ 1000000us/9600bit/s=104us
һ���ֽ�Ϊ    104us*10λ  =1040us
���� MODBUSȷ��һ������֡��ɵ�ʱ��Ϊ   1040us*3.5=3.64ms  ->10ms
*/

void Mosbus_Init()
{
	modbus.myadd=Machine_Address;  //�����豸�ĵ�ַ
	modbus.timrun=0; //MODbus��ʱ��ֹͣ��ʱ
  RS485_Init();
}


void Modbud_fun3()  //3�Ź����봦��  ---����Ҫ��ȡ���ӻ��ļĴ���
{
  u16 Regadd;
	u16 Reglen;
	u16 byte;
	u16 i,j;
	u16 crc;
	Regadd=modbus.rcbuf[2]*256+modbus.rcbuf[3];  //�õ�Ҫ��ȡ�ļĴ������׵�ַ
	Reglen=modbus.rcbuf[4]*256+modbus.rcbuf[5];  //�õ�Ҫ��ȡ�ļĴ���������
	i=0;
	
	modbus.Sendbuf[i++]=modbus.myadd;//���豸��ַ
  modbus.Sendbuf[i++]=0x03;        //�����룬�������+128      
  byte=Reglen*2;   //Ҫ���ص������ֽ���
//modbus.Sendbuf[i++]=byte/256;  //
	modbus.Sendbuf[i++]=byte%256;
	
	for(j=0;j<Reglen;j++)
	{ 
	  modbus.Sendbuf[i++]=Reg[Regadd+j]/256;
		modbus.Sendbuf[i++]=Reg[Regadd+j]%256;		
	}
	crc=crc16(modbus.Sendbuf,i);
	modbus.Sendbuf[i++]=crc/256;  //
	modbus.Sendbuf[i++]=crc%256;
	
	RS485_RT_1;  // 485 send
	
	for(j=0;j<i;j++)
	{
	 RS485_byte(modbus.Sendbuf[j]);
	}
	
	RS485_RT_0;  // 485 chip receive
	
	//��Ӧ��֮�󣬷�����Ϣ�����ں�OLED
	//OLED_CLS();
	//OLED_P8x16Str(0,2,"     Server ");
	//OLED_P8x16Str(0,4,"     Request ");
	//printf("Receiving A Frame");
	//OLED_CLS();
	
}

void Modbud_fun6()  //6�Ź����봦�� д��Ĵ���
{
  u16 Regadd;
	u16 val;
	u16 i,crc,j;
	i=0;
  Regadd=modbus.rcbuf[2]*256+modbus.rcbuf[3];  //�õ�Ҫ�޸ĵĵ�ַ 
	val=modbus.rcbuf[4]*256+modbus.rcbuf[5];     //�޸ĺ��ֵ
	Reg[Regadd]=val;  //�޸ı��豸��Ӧ�ļĴ���
	
	//����Ϊ��Ӧ����
	
	modbus.Sendbuf[i++]=modbus.myadd;//���豸��ַ
  modbus.Sendbuf[i++]=0x06;        //������ 
  modbus.Sendbuf[i++]=Regadd/256;
	modbus.Sendbuf[i++]=Regadd%256;
	modbus.Sendbuf[i++]=val/256;
	modbus.Sendbuf[i++]=val%256;
	crc=crc16(modbus.Sendbuf,i);
	modbus.Sendbuf[i++]=crc/256;  //
	modbus.Sendbuf[i++]=crc%256;
	
	RS485_RT_1;  //
	
	for(j=0;j<i;j++)
	{
	 RS485_byte(modbus.Sendbuf[j]);
	}
	
	RS485_RT_0;  //
	
	//��Ӧ��֮�󣬷�����Ϣ�����ں�OLED
	//OLED_CLS();
	//OLED_P8x16Str(0,2,"     Server ");
	//OLED_P8x16Str(0,4,"     Modify ");
	//printf("Sending A Frame");
	//delay_ms(1000);
	//OLED_CLS();
}

void Mosbus_Event()
{
	u16 crc;
	u16 rccrc;
  if(modbus.reflag==0)  //û���յ�MODbus�����ݰ�
	{
	  return ;
	}
	
	crc= crc16(&modbus.rcbuf[0], modbus.recount-2);       //����У����
  rccrc=modbus.rcbuf[modbus.recount-2]*256 + modbus.rcbuf[modbus.recount-1];  //�յ���У����
  if(crc ==  rccrc)  //���ݰ�����CRCУ�����
	{ 
	  if(modbus.rcbuf[0] == modbus.myadd)  //ȷ�����ݰ��Ƿ��Ƿ������豸�� 
		{
		  switch(modbus.rcbuf[1])  //����������
			{
			  case 0:     break;
			  case 1:     break;
		    case 2:     break;
		    case 3:     Modbud_fun3();bsp_LedToggle(1); delay_ms(200); break;   //3�Ź����봦����תһ��LED��ָʾ�����������ڴ�������
		    case 4:     break;
		    case 5:     break;
		    case 6:     Modbud_fun6();bsp_LedOn(1); delay_ms(200); break;   //6�Ź����봦��
	      case 7:     break;			
        //....				
			}
		}
		else if(modbus.rcbuf[0] == 0)   //�㲥��ַ
		{
		
		}
	}
	
	bsp_LedOff(1);
	
	modbus.recount=0;   //
  modbus.reflag=0;	
}

















