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

void enter_sleep_mode(void);

int main(void)
{
	// 初始化
	USART_Init();
	LED_Init();

	printf("尚硅谷低功率实验：睡眠模式...\n");

	// 1. 开启LED灯，延时2s，模拟正常程序执行过程
	LED_On(LED_1);
	Delay_s(2);

	while (1)
	{
		// 2. 进入睡眠模式
		printf("正常代码执行完毕，3s后进入睡眠模式...\n");
		Delay_s(3);
		printf("进入睡眠模式");
		enter_sleep_mode();

		// 3. 以下代码只有在唤醒之后才会执行
		printf("从睡眠模式中唤醒...\n");
		Delay_s(2);
	}
}

// 定义进入睡眠模式的函数
void enter_sleep_mode(void)
{
	// 1. 设置普通睡眠模式（默认）
	SCB->SCR &= ~SCB_SCR_SLEEPDEEP;

	// 2. 使用WFI指令，进入睡眠模式
	__WFI();
}
