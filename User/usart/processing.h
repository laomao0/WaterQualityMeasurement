/*
*********************************************************************************************************
*
*	ģ������ : BSPģ��
*	�ļ����� : bsp.h
*	˵    �� : �������ݴ���
*
*	Copyright (C), 2019, shenwang@sjtu.edu.cn
*
*********************************************************************************************************
*/

#ifndef __PROCESSING_H
#define __PROCESSING_H 
#include "sys.h"   
#include "bsp.h"

//����ϵͳ״̬
#define PARA_UPDATA   7 //ϵ������
#define TEST_STATE2   6
#define TEST_STATE    5
#define TIP_STATE     4  //UI ��ָ֪��
#define PH_STATE 			3  //У׼
#define ORP_STATE 		2//У׼
#define CL_STATE 			1//У׼
#define DEFAULT_STATE 0//Ĭ��״̬-��������ʾ

#define CALI_0 0 
#define CALI_1 1
#define CALI_2 2
#define CALI_3 3
#define CALI_4 4
#define CALI_5 5

#define CL2_K 400

#define Para_Len M*2*3  //У׼�Ĳ���������������������ÿ��������2��ϵ����һ��ϵ��32λ��4byte��
#define M 10
#define  EEP_Firstpage      0x00
#define Para_Num 6  // ÿһ��������������һ��6��ϵ��
#define P_NUm 3  //PH ORP CL2
#define Amp 10  // float תΪ int�ķŴ���

#define CL_Single_Cali 1  //CL2 single step cali
 

void Regular_task(float*);
void Tip_task(void);
void Cali_task(int);
void Test_task(void);
char Input_key(void);
void Test_task2(float*);
float* reload_para(void);
float* parameter_init(int);


#endif
