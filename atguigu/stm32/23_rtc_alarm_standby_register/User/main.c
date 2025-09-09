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
#include "rtc.h"

void enter_standby_mode(void);

int main(void)
{
	// 初始化
	USART_Init();
	LED_Init();
	RTC_Init();

	// 判断是否有标志位，如果有就直接清除
	if (PWR->CSR & PWR_CSR_SBF)
	{
		printf("从待机模式唤醒！\n");
		PWR->CR |= PWR_CR_CSBF;
	}
	if (PWR->CSR & PWR_CSR_WUF)
	{
		printf("发生了唤醒事件！\n");
		PWR->CR |= PWR_CR_CWUF;
	}

	printf("尚硅谷RTC实验：闹钟唤醒待机模式...\n");

	// 1. 开启LED灯，延时2s，模拟正常程序执行过程
	LED_On(LED_2);
	Delay_s(2);

	while (1)
	{
		// 2. 进入待机模式
		printf("正常代码执行完毕，3s后进入待机模式...\n");
		Delay_s(3);
		printf("进入待机模式，5s后闹钟唤醒...\n");
		Delay_ms(1);	// 引入延时，确保发送信息成功

		// 3. 在进入待机模式前设置闹钟
		RTC_SetAlarm(5);
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

	// 3. 配置PDDS = 1，表示待机模式
	PWR->CR |= PWR_CR_PDDS;

	// 5. 使用WFI指令，进入待机模式
	__WFI();
}
