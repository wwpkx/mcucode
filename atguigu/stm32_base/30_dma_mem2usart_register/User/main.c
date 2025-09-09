/*
 * @Author: wushengran
 * @Date: 2024-09-13 16:29:39
 * @Description:
 *
 * Copyright (c) 2024 by atguigu, All Rights Reserved.
 */

#include "usart.h"
#include "dma.h"
#include "delay.h"

// 定义全局变量，要发送的数据
uint8_t src[] = {'a', 'b', 'c', 'd', 'e'};

int main(void)
{
	// 初始化
	USART_Init();
	DMA1_Init();

	printf("Hello world!\n");

	Delay_ms(1);

	// 调用函数传输数据
	DMA1_Transmit((uint32_t)src, (uint32_t)&(USART1->DR), 5);

	while (1)
	{
	}
}
