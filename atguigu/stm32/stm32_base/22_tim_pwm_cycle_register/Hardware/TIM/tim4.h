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

// 初始化
void TIM4_Init(void);

// 开关
void TIM4_Start(void);
void TIM4_Stop(void);

// 获取PWM周期和频率
double TIM4_GetPWMCycle(void);
double TIM4_GetPWMFreq(void);

#endif
