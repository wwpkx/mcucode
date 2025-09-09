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
#include "iwdg.h"

void enter_standby_mode(void);

int main(void)
{
	// ��ʼ��
	USART_Init();
	LED_Init();
	IWDG_Init();

	printf("�й��IWDGʵ�飺IWDG���Ѵ���ģʽ...\n");

	// 1. ����LED�ƣ���ʱ2s��ģ����������ִ�й���
	LED_On(LED_2);
	Delay_s(2);

	while (1)
	{
		// 2. �������ģʽ
		printf("�������ģʽ���ȴ�IWDG����...\n");
		Delay_ms(1);	// ������ʱ��ȷ��������Ϣ�ɹ�

		enter_standby_mode();

		// 4. ���´��벻��ִ��
		printf("�Ӵ���ģʽ�л���...\n");
		Delay_s(2);
	}
}

// ����������ģʽ�ĺ���
void enter_standby_mode(void)
{
	// 1. ������˯��ģʽ
	SCB->SCR |= SCB_SCR_SLEEPDEEP;

	RCC->APB1ENR |= RCC_APB1ENR_PWREN;

	// 3. ����PDDS = 1����ʾ����ģʽ
	PWR->CR |= PWR_CR_PDDS;

	// 5. ʹ��WFIָ��������ģʽ
	__WFI();
}
