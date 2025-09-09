/*
 * @Author: wushengran
 * @Date: 2024-09-13 16:29:39
 * @Description:
 *
 * Copyright (c) 2024 by atguigu, All Rights Reserved.
 */

#include "usart.h"
#include "delay.h"
#include "key.h"
#include "bkp.h"

int main(void)
{
	// ��ʼ��
	USART_Init();
	KEY_Init();
	BKP_Init();

	printf("�й�ȱ��ݼĴ���ʵ��...\n");

	// �򱸷����ݼĴ���д����ֵ
	// BKP->DR1 = 9999;

	while (1)
	{
	}
}
