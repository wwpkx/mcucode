/*
 * @Author: wushengran
 * @Date: 2024-09-13 16:29:39
 * @Description:
 *
 * Copyright (c) 2024 by atguigu, All Rights Reserved.
 */

#include "usart.h"
#include "delay.h"
#include "led.h"
#include "key.h"

void enter_stop_mode(void);
void get_clock_freq(uint32_t *sys_clock, uint32_t *AHB_clock, uint32_t *APB2_clock, uint32_t *APB1_clock);

int main(void)
{
	// 初始化
	USART_Init();
	LED_Init();
	KEY_Init();

	printf("尚硅谷低功率实验：停机模式...\n");

	// 1. 开启LED灯，延时2s，模拟正常程序执行过程
	LED_On(LED_2);
	Delay_s(2);

	while (1)
	{
		// 2. 进入停机模式
		printf("正常代码执行完毕，3s后进入停机模式...\n");
		Delay_s(3);
		printf("进入停机模式...\n");
		Delay_ms(1);	// 引入延时，确保发送信息成功
		enter_stop_mode();

		uint32_t sys_clock, ahb_clock, apb1_clock, apb2_clock;
		get_clock_freq(&sys_clock, &ahb_clock, &apb2_clock, &apb1_clock);

		// 3. 重新进行系统初始化，设置系统时钟
		SystemInit();

		printf("重置前的时钟频率：\nsys_clock = %d, ahb_clock = %d, apb1_clock = %d, apb2_clock = %d\n\n", 
				sys_clock, ahb_clock, apb1_clock, apb2_clock);

		get_clock_freq(&sys_clock, &ahb_clock, &apb2_clock, &apb1_clock);

		printf("重置后的时钟频率：\nsys_clock = %d, ahb_clock = %d, apb1_clock = %d, apb2_clock = %d\n\n", 
				sys_clock, ahb_clock, apb1_clock, apb2_clock);

		// 4. 以下代码只有在唤醒之后才会执行
		printf("从停机模式中唤醒...\n");
		Delay_s(2);
	}
}

// 定义进入睡眠模式的函数
void enter_stop_mode(void)
{
	// 1. 设置深睡眠模式
	SCB->SCR |= SCB_SCR_SLEEPDEEP;

	// 2. 开启PWR模块时钟
	RCC->APB1ENR |= RCC_APB1ENR_PWREN;

	// 3. 配置PDDS = 0，表示停机模式
	PWR->CR &= ~PWR_CR_PDDS;

	// 4. 设置电压调节器的工作模式：低功耗模式
	PWR->CR |= PWR_CR_LPDS;

	// 5. 使用WFI指令，进入停机模式
	__WFI();
}

// 自定义函数：查询各个时钟频率（系统时钟sysclock、AHB、APB2、APB1）
void get_clock_freq(uint32_t *sys_clock, uint32_t *AHB_clock, uint32_t *APB2_clock, uint32_t *APB1_clock)
{
	// 1. 获取时钟源
	uint32_t clock_src = RCC->CFGR & RCC_CFGR_SWS;

	// 2. 根据时钟源获取系统时钟频率
	if (clock_src == RCC_CFGR_SWS_HSE)
	{
		*sys_clock = HSE_VALUE;
	}
	else if (clock_src == RCC_CFGR_SWS_HSI)
	{
		*sys_clock = HSI_VALUE;
	}
	else if (clock_src == RCC_CFGR_SWS_PLL)
	{
		// 取倍频系数相乘
		uint32_t mul = ((RCC->CFGR & RCC_CFGR_PLLMULL) >> 18) + 2;
		*sys_clock = HSE_VALUE * mul;
	}

	// 3. 根据分频系数，确定各个时钟频率
	// 3.1 AHB
	// 3.1.1 获取分频系数
	uint32_t hpre = (RCC->CFGR & RCC_CFGR_HPRE) >> 4;
	// 3.1.2 根据最高位，判断是否有分频
	if (hpre & 0x8)
	{
		// 有分频，取最后三位
		uint32_t n = hpre & 0x7;
		// 跟4做比较，选择右移n+1位或者n+2位
		if (n < 4)
		{
			*AHB_clock = *sys_clock >> (n+1);
		}
		else
		{
			*AHB_clock = *sys_clock >> (n+2);
		}
	}
	else
	{
		// 没有分频
		*AHB_clock = *sys_clock;
	}

	// 3.2 APB1
	// 3.2.1 获取分频系数
	uint32_t ppre1 = (RCC->CFGR & RCC_CFGR_PPRE1) >> 8;
	// 3.2.2 根据最高位，判断是否有分频
	if (ppre1 & 0x4)
	{
		*APB1_clock = *AHB_clock >> ((ppre1 & 0x3) + 1);
	}
	else
	{
		// 没有分频
		*APB1_clock = *AHB_clock;
	}

	// 3.3 APB2
	// 3.3.1 获取分频系数
	uint32_t ppre2 = (RCC->CFGR & RCC_CFGR_PPRE2) >> 11;
	// 3.3.2 根据最高位，判断是否有分频
	if (ppre2 & 0x4)
	{
		*APB2_clock = *AHB_clock >> ((ppre2 & 0x3) + 1);
	}
	else
	{
		// 没有分频
		*APB2_clock = *AHB_clock;
	}
}
