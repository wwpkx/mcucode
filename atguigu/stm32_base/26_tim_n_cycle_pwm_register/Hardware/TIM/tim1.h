/*
 * @Author: wushengran
 * @Date: 2024-09-29 16:18:35
 * @Description: 
 * 
 * Copyright (c) 2024 by atguigu, All Rights Reserved. 
 */
#ifndef __TIM1_H
#define __TIM1_H

#include "stm32f10x.h"

// 初始化
void TIM1_Init(void);

// 开关定时器
void TIM1_Start(void);
void TIM1_Stop(void);

#endif
