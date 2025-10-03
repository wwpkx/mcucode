/*
 * @Author: wushengran
 * @Date: 2024-09-13 16:29:39
 * @Description:
 *
 * Copyright (c) 2024 by atguigu, All Rights Reserved.
 */

#include "usart.h"
#include "delay.h"
#include "led.h"

void enter_sleep_mode(void);

int main(void)
{
	// ��ʼ��
	USART_Init();
	LED_Init();

	printf("�й�ȵ͹���ʵ�飺˯��ģʽ...\n");

	// 1. ����LED�ƣ���ʱ2s��ģ����������ִ�й���
	LED_On(LED_1);
	Delay_s(2);

	while (1)
	{
		// 2. ����˯��ģʽ
		printf("��������ִ����ϣ�3s�����˯��ģʽ...\n");
		Delay_s(3);
		printf("����˯��ģʽ");
		enter_sleep_mode();

		// 3. ���´���ֻ���ڻ���֮��Ż�ִ��
		printf("��˯��ģʽ�л���...\n");
		Delay_s(2);
	}
}

// �������˯��ģʽ�ĺ���
void enter_sleep_mode(void)
{
	// 1. ������ͨ˯��ģʽ��Ĭ�ϣ�
	SCB->SCR &= ~SCB_SCR_SLEEPDEEP;

	// 2. ʹ��WFIָ�����˯��ģʽ
	__WFI();
}
