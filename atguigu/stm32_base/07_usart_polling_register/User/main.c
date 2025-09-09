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

int main(void)
{
	// 初始化
	USART_Init();

	// 发送单个字符
	USART_SendChar('a');
	USART_SendChar('t');
	USART_SendChar('\n');

	// 发送字符串
	uint8_t * str = "Hello, World!\n";
	USART_SendString(str, strlen((char *)str));

	while (1)
	{
		// // 不停发送字符
		// USART_SendChar('x');
		// USART_SendChar('\n');

		// // 每次延迟1s
		// Delay_ms(1000);

		// // 接收小写字符并转换成大写发回去
		// uint8_t ch = USART_ReceiveChar();
		// USART_SendChar(ch - 32);

		USART_ReceiveString(buff, &size);
		USART_SendString(buff, size);
	}
}

