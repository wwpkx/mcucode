/*
 * @Author: wushengran
 * @Date: 2024-09-20 16:23:08
 * @Description: 
 * 
 * Copyright (c) 2024 by atguigu, All Rights Reserved. 
 */
#ifndef __USART_H
#define __USART_H

#include "stm32f10x.h"

// 初始化
void USART_Init(void);

// 发送一个字符
void USART_SendChar(uint8_t ch);

// 发送字符串
void USART_SendString(uint8_t * str, uint8_t size);


#endif
