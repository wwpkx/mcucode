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

// ������ջ������ͽ������ݳ���
uint8_t buff[100];
uint8_t size;

// ���������ɵı�־λ
uint8_t isOver;

int main(void)
{
	// ��ʼ��
	USART_Init();

	// �����ַ���
	uint8_t * str = "Hello, World!\n";
	USART_SendString(str, strlen((char *)str));

	while (1)
	{
		if (isOver)
		{
			USART_SendString(buff, size);
			size = 0;

			isOver = 0;	// ��־λ����
		}	
	}
}

