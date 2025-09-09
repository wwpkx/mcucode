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
#include "iwdg.h"

// ȫ�ֱ�����������־λ
uint8_t flag;

int main(void)
{
	// ��ʼ��
	USART_Init();
	KEY_Init();
	IWDG_Init();

	printf("�й�ȶ������Ź�ʵ��...\n");

	while (1)
	{
		// 1. ģ���������ִ��
		printf("��������ִ��...\n");
		Delay_ms(3000);

		// ���밴���жϣ�����������£����ӳ�����ִ��ʱ��
		if (flag)
		{
			Delay_ms(3000);
			flag = 0;
		}
		
		// 2. �����������н�����ι��
		IWDG_Refresh();
		printf("����ִ�н�����ι���ɹ���\n");
	}
}

