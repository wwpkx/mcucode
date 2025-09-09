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

int main(void)
{
	// ��ʼ��
	USART_Init();

	// ���͵����ַ�
	USART_SendChar('a');
	USART_SendChar('t');
	USART_SendChar('\n');

	// �����ַ���
	uint8_t * str = "Hello, World!\n";
	USART_SendString(str, strlen((char *)str));

	while (1)
	{
		// // ��ͣ�����ַ�
		// USART_SendChar('x');
		// USART_SendChar('\n');

		// // ÿ���ӳ�1s
		// Delay_ms(1000);

		// // ����Сд�ַ���ת���ɴ�д����ȥ
		// uint8_t ch = USART_ReceiveChar();
		// USART_SendChar(ch - 32);

		USART_ReceiveString(buff, &size);
		USART_SendString(buff, size);
	}
}

