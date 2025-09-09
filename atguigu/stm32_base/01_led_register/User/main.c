/*
 * @Author: wushengran
 * @Date: 2024-09-13 16:29:39
 * @Description: 
 * 
 * Copyright (c) 2024 by atguigu, All Rights Reserved. 
 */
 
#include "stm32f10x.h"

int main(void)
{
	/*
	// 1. 时钟配置，开启GPIOA时钟
	*(uint32_t *)(0x40021000 + 0x18) = 4;
	
	// 2. 配置PA0为输出
	// *(uint32_t *)(0x40010800 + 0x00) = 3;
	// 配置PA1为输出
	*(uint32_t *)(0x40010800 + 0x00) = 0x33;
	// 配置PA8为输出
	*(uint32_t *)(0x40010800 + 0x04) = 0x3;
	
	// 3. PA0输出低电平
	// *(uint32_t *)(0x40010800 + 0x0C) = 0xfffe;
	// PA1输出低电平
	// *(uint32_t *)(0x40010800 + 0x0C) = 0xfffd;
	// PA8输出低电平
	// *(uint32_t *)(0x40010800 + 0x0C) = 0xfeff;
	
	*(uint32_t *)(0x40010800 + 0x0C) = 0xfefc;
	*/
	
	// 改进1：使用宏定义改写寄存器地址
	
	// 1. 时钟配置，开启GPIOA时钟
	// RCC->APB2ENR = 4;
	
	// 2. 配置PA0为输出
	// GPIOA->CRL = 3;
	
	// 3. PA0输出低电平
	// GPIOA->ODR = 0xfffe;
	
	/*
	// 改进2：使用位运算实现只改变某一位的值
	
	// 1. 时钟配置，开启GPIOA时钟
	RCC->APB2ENR |= (1 << 2);
	
	// 2. 配置PA0为输出
	GPIOA->CRL |= (1 << 0);
	GPIOA->CRL |= (1 << 1);
	GPIOA->CRL &= ~(1 << 2);
	GPIOA->CRL &= ~(1 << 3);
	
	// 3. PA0输出低电平
	GPIOA->ODR &= ~(1 << 0);
	*/
	
	// 改进3：使用宏定义改写每一位的表示
	
	// 1. 时钟配置，开启GPIOA时钟
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
	
	// 2. 配置PA0为输出
	GPIOA->CRL |= GPIO_CRL_MODE0;
	GPIOA->CRL &= ~GPIO_CRL_CNF0;
	
	// 3. PA0输出低电平
	GPIOA->ODR &= ~GPIO_ODR_ODR0;
	
	while(1)
	{}
}
