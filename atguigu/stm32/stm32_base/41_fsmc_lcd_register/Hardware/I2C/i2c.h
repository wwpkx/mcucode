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
// 定义操作执行返回结果 OK - 成功，FAIL - 超时失败
#define OK 0
#define FAIL 1

// 初始化
void I2C_Init(void);

// 主机发出起始信号
uint8_t I2C_Start(void);

// 主机发出停止信号
void I2C_Stop(void);

// 主机发送应答/非应答信号
void I2C_Ack(void);
void I2C_NAck(void);

// 主机发送设备地址
uint8_t I2C_SendAddr(uint8_t addr);

// 主机发送一个字节
uint8_t I2C_SendByte(uint8_t byte);

// 主机从SDA读取一个字节
uint8_t I2C_ReadByte(void);

#endif
