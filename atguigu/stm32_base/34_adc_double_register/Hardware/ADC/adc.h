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

// DMA相关的初始化
void ADC1_DMA_Init(void);

// 启动ADC并开启转换（带DMA配置和启动，源地址固定为ADC1->DR）
void ADC1_StartConvert_DMA(uint32_t destAddr, uint16_t len);

#endif
