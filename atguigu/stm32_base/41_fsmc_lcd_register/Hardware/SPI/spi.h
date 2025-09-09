/*
 * @Author: wushengran
 * @Date: 2024-10-09 16:17:42
 * @Description: 
 * 
 * Copyright (c) 2024 by atguigu, All Rights Reserved. 
 */
#ifndef __SPI_H
#define __SPI_H

#include "stm32f10x.h"

// 宏定义引脚线的操作
// CS - PC13
#define CS_HIGH (GPIOC->ODR |= GPIO_ODR_ODR13)
#define CS_LOW (GPIOC->ODR &= ~GPIO_ODR_ODR13)

// 初始化
void SPI_Init(void);

// SPI通信的开启和关闭
void SPI_Start(void);
void SPI_Stop(void);

// 一个时钟周期内，主从交换一个字节数据
uint8_t SPI_SwapByte(uint8_t byte);

#endif
