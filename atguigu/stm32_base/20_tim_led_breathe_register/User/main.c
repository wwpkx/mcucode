/*
 * @Author: wushengran
 * @Date: 2024-09-13 16:29:39
 * @Description:
 *
 * Copyright (c) 2024 by atguigu, All Rights Reserved.
 */

#include "usart.h"
#include "delay.h"
#include "tim5.h"

int main(void)
{
	// 初始化
	USART_Init();
	TIM5_Init();

	printf("Hello world!\n");

	// 开启定时器
	TIM5_Start();

	// 定义变量，保存当前的占空比
	uint8_t duty = 0;
	// 定义占空比变化方向和步长
	uint8_t dir = 0;	// 0 - 增大，1 - 减小
	uint8_t step = 1;

	while (1)
	{
		// 判断方向，决定新的占空比
		if (dir == 0)
		{
			// 占空比增大
			duty += step;
			// 如果超过99就反转方向
			if (duty >= 99)
			{
				dir = 1;
			}
		}
		else
		{
			// 占空比增大
			duty -= step;
			// 如果超过0就反转方向
			if (duty <= 0)
			{
				dir = 0;
			}
		}
		
		TIM5_SetDutyCycle(duty);

		Delay_ms(10);
	}
}

