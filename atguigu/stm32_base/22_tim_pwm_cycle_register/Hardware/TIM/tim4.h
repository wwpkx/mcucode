/*
 * @Author: wushengran
 * @Date: 2024-09-28 14:36:44
 * @Description: 
 * 
 * Copyright (c) 2024 by atguigu, All Rights Reserved. 
 */
#ifndef __TIM4_H
#define __TIM4_H

#include "stm32f10x.h"

// ��ʼ��
void TIM4_Init(void);

// ����
void TIM4_Start(void);
void TIM4_Stop(void);

// ��ȡPWM���ں�Ƶ��
double TIM4_GetPWMCycle(void);
double TIM4_GetPWMFreq(void);

#endif
