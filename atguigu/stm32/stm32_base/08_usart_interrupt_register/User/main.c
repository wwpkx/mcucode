/*
 * @Author: wushengran
 * @Date: 2024-09-13 16:29:39
 * @Description:
 *
 * Copyright (c) 2024 by atguigu, All Rights Reserved.
 */

#include "usart.h"
#include "delay.h"
#include <string.h>

// 定义接收缓冲区和接收数据长度
uint8_t buff[100];
uint8_t size;

// 定义接收完成的标志位
uint8_t isOver;

int main(void)
{
	// 初始化
	USART_Init();

	// 发送字符串
	uint8_t * str = "Hello, World!\n";
	USART_SendString(str, strlen((char *)str));

	while (1)
	{
		if (isOver)
		{
			USART_SendString(buff, size);
			size = 0;

			isOver = 0;	// 标志位清零
		}	
	}
}

