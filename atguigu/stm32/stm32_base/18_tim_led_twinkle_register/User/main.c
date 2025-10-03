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
#include "tim6.h"

int main(void)
{
	// ≥ı ºªØ
	USART_Init();
	LED_Init();
	TIM6_Init();

	printf("Hello world!\n");

	while (1)
	{
	}
}

