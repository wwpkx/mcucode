/*
 * @Author: wushengran
 * @Date: 2024-10-08 16:18:04
 * @Description: 
 * 
 * Copyright (c) 2024 by atguigu, All Rights Reserved. 
 */
#ifndef __ADC_H
#define __ADC_H

#include "stm32f10x.h"

// 初始化
void ADC1_Init(void);

// 启动ADC并开启转换
void ADC1_StartConvert(void);

// 返回转换后的电压值
double ADC1_ReadV(void);

#endif
