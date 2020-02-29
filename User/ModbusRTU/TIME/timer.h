#ifndef _timer_H
#define _timer_H

#include "stm32f10x_conf.h"
void Timer2_Init(void);
void Isr_Init(void);
void delay(u32 x);

#endif
