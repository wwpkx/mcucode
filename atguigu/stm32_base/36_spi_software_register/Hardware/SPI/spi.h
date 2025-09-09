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
#include "delay.h"

// 宏定义引脚线的操作
// CS - PC13
#define CS_HIGH (GPIOC->ODR |= GPIO_ODR_ODR13)
#define CS_LOW (GPIOC->ODR &= ~GPIO_ODR_ODR13)

// SCK - PA5
#define SCK_HIGH (GPIOA->ODR |= GPIO_ODR_ODR5)
#define SCK_LOW (GPIOA->ODR &= ~GPIO_ODR_ODR5)

// MOSI - PA7
#define MOSI_HIGH (GPIOA->ODR |= GPIO_ODR_ODR7)
#define MOSI_LOW (GPIOA->ODR &= ~GPIO_ODR_ODR7)

// MISO - PA6，读取数据线
#define MISO_READ ( GPIOA->IDR & GPIO_IDR_IDR6 )

// 定义标准延迟时间
#define SPI_DELAY Delay_us(5)

// 初始化
void SPI_Init(void);

// SPI通信的开启和关闭
void SPI_Start(void);
void SPI_Stop(void);

// 一个时钟周期内，主从交换一个字节数据
uint8_t SPI_SwapByte(uint8_t byte);

#endif
