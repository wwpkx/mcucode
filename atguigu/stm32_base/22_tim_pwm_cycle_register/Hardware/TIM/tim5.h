/*
 * @Author: wushengran
 * @Date: 2024-09-27 16:12:45
 * @Description: 
 * 
 * Copyright (c) 2024 by atguigu, All Rights Reserved. 
 */
#ifndef __TIM5_H
#define __TIM5_H

#include "stm32f10x.h"

// 初始化
void TIM5_Init(void);

// 计数器的开启和关闭
void TIM5_Start(void);
void TIM5_Stop(void);

// 设置占空比，传入百分比值
void TIM5_SetDutyCycle(uint8_t duty);

#endif
