/*
 * @Author: wushengran
 * @Date: 2024-09-13 16:29:39
 * @Description:
 *
 * Copyright (c) 2024 by atguigu, All Rights Reserved.
 */

#include "usart.h"
#include "eth.h"

int main(void)
{
	// 1. ��ʼ��
	USART_Init();

	printf("�й����̫��ʵ�飺��������\n");

	ETH_Init();

	printf("\n��̫����ʼ����ɣ�\n");

	while (1)
	{
	}
}

