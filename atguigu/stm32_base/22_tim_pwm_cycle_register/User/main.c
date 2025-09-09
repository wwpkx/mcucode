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
#include "tim4.h"

int main(void)
{
	// 初始化
	USART_Init();
	TIM5_Init();
	TIM4_Init();

	printf("Hello world!\n");

	// 开启定时器
	TIM5_Start();
	TIM4_Start();

	TIM5_SetDutyCycle(50);

	while (1)
	{
		// 输出当前检测的周期和频率
		printf("T = %.2f ms, f = %.2f Hz\n", TIM4_GetPWMCycle(), TIM4_GetPWMFreq());
		Delay_ms(1000);
	}
}

