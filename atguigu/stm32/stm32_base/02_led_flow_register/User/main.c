/*
 * @Author: wushengran
 * @Date: 2024-09-13 16:29:39
 * @Description:
 *
 * Copyright (c) 2024 by atguigu, All Rights Reserved.
 */

#include "stm32f10x.h"

// 定义延时函数
void Delay_us(uint16_t us);
void Delay_ms(uint16_t ms);
void Delay_s(uint16_t s);

int main(void)
{
	// 1. 时钟配置，打开GPIOA时钟
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;

	// 2. 工作模式配置，PA0、PA1、PA8 通用推挽输出，CNF = 00，MODE = 11
	GPIOA->CRL |= GPIO_CRL_MODE0;
	GPIOA->CRL &= ~GPIO_CRL_CNF0;
	GPIOA->CRL |= GPIO_CRL_MODE1;
	GPIOA->CRL &= ~GPIO_CRL_CNF1;
	GPIOA->CRH |= GPIO_CRH_MODE8;
	GPIOA->CRH &= ~GPIO_CRH_CNF8;

	// 3. 初始状态所有引脚输出高电平，关灯
	GPIOA->ODR |= (GPIO_ODR_ODR0 | GPIO_ODR_ODR1 | GPIO_ODR_ODR8);

	// 4. 在循环中依次点亮每个灯、延迟一段时间后关闭
	while (1)
	{
		// 点亮 LED1
		GPIOA->ODR &= ~GPIO_ODR_ODR0;
		// 延时
		Delay_ms(500);
		// 关闭 LED1
		GPIOA->ODR |= GPIO_ODR_ODR0;

		// 点亮 LED2
		GPIOA->ODR &= ~GPIO_ODR_ODR1;
		// 延时
		Delay_ms(500);
		// 关闭 LED2
		GPIOA->ODR |= GPIO_ODR_ODR1;

		// 点亮 LED3
		GPIOA->ODR &= ~GPIO_ODR_ODR8;
		// 延时
		Delay_ms(500);
		// 关闭 LED3
		GPIOA->ODR |= GPIO_ODR_ODR8;
	}
}

// 微秒级延时函数，延迟us微秒
void Delay_us(uint16_t us)
{
	// 设置系统定时器的初始计数值
	SysTick->LOAD = 72 * us;

	// 配置系统定时器
	SysTick->CTRL = 0x05;

	// 轮询等待计数值变为0，COUNTFLAG = 1
	while (!(SysTick->CTRL & SysTick_CTRL_COUNTFLAG))
	{
	}

	// 关闭定时器
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE;
}

void Delay_ms(uint16_t ms)
{
	while (ms--)
	{
		Delay_us(1000);
	}
}
void Delay_s(uint16_t s)
{
	while (s--)
	{
		Delay_ms(1000);
	}
}
