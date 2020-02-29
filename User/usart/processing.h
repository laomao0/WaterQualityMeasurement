/*
*********************************************************************************************************
*
*	模块名称 : BSP模块
*	文件名称 : bsp.h
*	说    明 : 进行数据处理
*
*	Copyright (C), 2019, shenwang@sjtu.edu.cn
*
*********************************************************************************************************
*/

#ifndef __PROCESSING_H
#define __PROCESSING_H 
#include "sys.h"   
#include "bsp.h"

//定义系统状态
#define PARA_UPDATA   7 //系数更新
#define TEST_STATE2   6
#define TEST_STATE    5
#define TIP_STATE     4  //UI 告知指令
#define PH_STATE 			3  //校准
#define ORP_STATE 		2//校准
#define CL_STATE 			1//校准
#define DEFAULT_STATE 0//默认状态-测量与显示

#define CALI_0 0 
#define CALI_1 1
#define CALI_2 2
#define CALI_3 3
#define CALI_4 4
#define CALI_5 5

#define CL2_K 400

#define Para_Len M*2*3  //校准的参数的数量，三个参数，每个参数有2个系数，一个系数32位（4byte）
#define M 10
#define  EEP_Firstpage      0x00
#define Para_Num 6  // 每一个有两个参数，一共6个系数
#define P_NUm 3  //PH ORP CL2
#define Amp 10  // float 转为 int的放大倍数

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
