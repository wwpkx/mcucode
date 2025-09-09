/*
 * @Author: wushengran
 * @Date: 2024-09-13 16:29:39
 * @Description:
 *
 * Copyright (c) 2024 by atguigu, All Rights Reserved.
 */

#include "usart.h"
#include "delay.h"

int main(void)
{
	// 初始化
	USART_Init();

	printf("Hello world!\n");

	int a = 0;

	while (1)
	{
		// 每隔1s打印一次a的值
		printf("a = %d\n", a++);

		Delay_ms(1000);
	}
}

