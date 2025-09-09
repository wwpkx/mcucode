/*
 * @Author: wushengran
 * @Date: 2024-09-13 16:29:39
 * @Description:
 *
 * Copyright (c) 2024 by atguigu, All Rights Reserved.
 */

#include "usart.h"
#include "adc.h"
#include "delay.h"

int main(void)
{
	// 初始化
	USART_Init();
	ADC1_Init();

	printf("Hello world!\n");

	// 开启AD转换
	ADC1_StartConvert();

	while (1)
	{
		printf("V = %.2f\n", ADC1_ReadV());

		Delay_ms(1000);
	}
}
