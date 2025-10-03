/*
 * @Author: wushengran
 * @Date: 2024-09-13 16:29:39
 * @Description:
 *
 * Copyright (c) 2024 by atguigu, All Rights Reserved.
 */

#include "delay.h"
#include "led.h"

int main(void)
{
	// 1. 初始化
	LED_Init();

	// 2. 在循环中依次点亮每个灯、延迟一段时间后关闭
	uint16_t leds[] = {LED_1, LED_2, LED_3};
	uint8_t n = 3;

	while (1)
	{
		for (uint8_t i = 0; i < n; i++)
		{
			LED_On(leds[i]);
			Delay_ms(500);
			LED_Off(leds[i]);
		}
	}
}
