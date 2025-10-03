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
#include "systick.h"

int main(void)
{
	// ��ʼ��
	USART_Init();
	LED_Init();
	Systick_Init();

	printf("Hello world!\n");

	while (1)
	{
		// LED_On(LED_1);
		// Delay_ms(500);
		// LED_Off(LED_1);
		// Delay_ms(1000);
	}
}

