/*
 * @Author: wushengran
 * @Date: 2024-09-13 16:29:39
 * @Description:
 *
 * Copyright (c) 2024 by atguigu, All Rights Reserved.
 */

#include "usart.h"
#include "delay.h"
#include "tim1.h"

int main(void)
{
	// ��ʼ��
	USART_Init();
	TIM1_Init();

	printf("Hello world!\n");

	// ������ʱ��
	TIM1_Start();

	while (1)
	{
	}
}

