/*
 * @Author: wushengran
 * @Date: 2024-09-13 16:29:39
 * @Description:
 *
 * Copyright (c) 2024 by atguigu, All Rights Reserved.
 */

#include "usart.h"
#include "delay.h"
#include "key.h"
#include "iwdg.h"

// 全局变量，按键标志位
uint8_t flag;

int main(void)
{
	// 初始化
	USART_Init();
	KEY_Init();
	IWDG_Init();

	printf("尚硅谷独立看门狗实验...\n");

	while (1)
	{
		// 1. 模拟程序正常执行
		printf("程序正常执行...\n");
		Delay_ms(3000);

		// 插入按键判断，如果按键按下，就延长代码执行时间
		if (flag)
		{
			Delay_ms(3000);
			flag = 0;
		}
		
		// 2. 程序正常运行结束，喂狗
		IWDG_Refresh();
		printf("程序执行结束，喂狗成功！\n");
	}
}

