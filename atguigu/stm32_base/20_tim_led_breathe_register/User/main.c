/*
 * @Author: wushengran
 * @Date: 2024-09-13 16:29:39
 * @Description:
 *
 * Copyright (c) 2024 by atguigu, All Rights Reserved.
 */

#include "usart.h"
#include "delay.h"
#include "tim5.h"

int main(void)
{
	// ��ʼ��
	USART_Init();
	TIM5_Init();

	printf("Hello world!\n");

	// ������ʱ��
	TIM5_Start();

	// ������������浱ǰ��ռ�ձ�
	uint8_t duty = 0;
	// ����ռ�ձȱ仯����Ͳ���
	uint8_t dir = 0;	// 0 - ����1 - ��С
	uint8_t step = 1;

	while (1)
	{
		// �жϷ��򣬾����µ�ռ�ձ�
		if (dir == 0)
		{
			// ռ�ձ�����
			duty += step;
			// �������99�ͷ�ת����
			if (duty >= 99)
			{
				dir = 1;
			}
		}
		else
		{
			// ռ�ձ�����
			duty -= step;
			// �������0�ͷ�ת����
			if (duty <= 0)
			{
				dir = 0;
			}
		}
		
		TIM5_SetDutyCycle(duty);

		Delay_ms(10);
	}
}

