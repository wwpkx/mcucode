/*
 * @Author: wushengran
 * @Date: 2024-09-13 16:29:39
 * @Description:
 *
 * Copyright (c) 2024 by atguigu, All Rights Reserved.
 */

#include "usart.h"
#include "dma.h"

// 定义全局常量，保存在 ROM 中
const uint8_t src[] = {10, 20, 30, 40};

// 定义全局变量，保存在 RAM 中
uint8_t dest[4];

// 定义全局变量，表示数据传输完成
uint8_t isOver;

int main(void)
{
	// 初始化
	USART_Init();
	DMA1_Init();

	printf("Hello world!\n");

	// 打印常量和变量地址
	printf("src = %p, dest = %p\n", src, dest);

	// 调用函数传输数据
	DMA1_Transmit((uint32_t)src, (uint32_t)dest, 4);

	// 等待传输完成，打印变量
	while (!isOver)
	{
	}

	for (uint8_t i = 0; i < 4; i++)
	{
		printf("%d\t", dest[i]);
	}

	while (1)
	{
	}
}
