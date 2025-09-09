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
#include "iwdg.h"

void enter_standby_mode(void);

int main(void)
{
	// 初始化
	USART_Init();
	LED_Init();
	IWDG_Init();

	printf("尚硅谷IWDG实验：IWDG唤醒待机模式...\n");

	// 1. 开启LED灯，延时2s，模拟正常程序执行过程
	LED_On(LED_2);
	Delay_s(2);

	while (1)
	{
		// 2. 进入待机模式
		printf("进入待机模式，等待IWDG唤醒...\n");
		Delay_ms(1);	// 引入延时，确保发送信息成功

		enter_standby_mode();

		// 4. 以下代码不会执行
		printf("从待机模式中唤醒...\n");
		Delay_s(2);
	}
}

// 定义进入待机模式的函数
void enter_standby_mode(void)
{
	// 1. 设置深睡眠模式
	SCB->SCR |= SCB_SCR_SLEEPDEEP;

	RCC->APB1ENR |= RCC_APB1ENR_PWREN;

	// 3. 配置PDDS = 1，表示待机模式
	PWR->CR |= PWR_CR_PDDS;

	// 5. 使用WFI指令，进入待机模式
	__WFI();
}
