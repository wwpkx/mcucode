/*
 * @Author: wushengran
 * @Date: 2024-09-23 14:01:33
 * @Description: 
 * 
 * Copyright (c) 2024 by atguigu, All Rights Reserved. 
 */
#ifndef __I2C_H
#define __I2C_H

#include "stm32f10x.h"
#include "usart.h"
#include "delay.h"

// 宏定义
// 将 SCL（PB10）、SDA（PB11）拉高、拉低
#define SCL_HIGH (GPIOB->ODR |= GPIO_ODR_ODR10)
#define SCL_LOW (GPIOB->ODR &= ~GPIO_ODR_ODR10)
#define SDA_HIGH (GPIOB->ODR |= GPIO_ODR_ODR11)
#define SDA_LOW (GPIOB->ODR &= ~GPIO_ODR_ODR11)

// 读取 SDA 数据
#define READ_SDA (GPIOB->IDR & GPIO_IDR_IDR11)

// 定义响应 ACK、NACK
#define ACK 0
#define NACK 1

// 定义每次操作之后的延迟（类似时钟周期）
#define I2C_DELAY Delay_us(10)

// 初始化
void I2C_Init(void);

// 主机发出起始信号
void I2C_Start(void);

// 主机发出停止信号
void I2C_Stop(void);

// 主机发送应答/非应答信号
void I2C_Ack(void);
void I2C_NAck(void);

// 主机从SDA读取响应信号，等待应答
uint8_t I2C_Wait4Ack(void);

// 主机发送一个字节
void I2C_SendByte(uint8_t byte);

// 主机从SDA读取一个字节
uint8_t I2C_ReadByte(void);

#endif
