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
#include "rtc.h"

void enter_standby_mode(void);

int main(void)
{
	// ��ʼ��
	USART_Init();
	LED_Init();
	RTC_Init();

	// �ж��Ƿ��б�־λ������о�ֱ�����
	if (PWR->CSR & PWR_CSR_SBF)
	{
		printf("�Ӵ���ģʽ���ѣ�\n");
		PWR->CR |= PWR_CR_CSBF;
	}
	if (PWR->CSR & PWR_CSR_WUF)
	{
		printf("�����˻����¼���\n");
		PWR->CR |= PWR_CR_CWUF;
	}

	printf("�й��RTCʵ�飺���ӻ��Ѵ���ģʽ...\n");

	// 1. ����LED�ƣ���ʱ2s��ģ����������ִ�й���
	LED_On(LED_2);
	Delay_s(2);

	while (1)
	{
		// 2. �������ģʽ
		printf("��������ִ����ϣ�3s��������ģʽ...\n");
		Delay_s(3);
		printf("�������ģʽ��5s�����ӻ���...\n");
		Delay_ms(1);	// ������ʱ��ȷ��������Ϣ�ɹ�

		// 3. �ڽ������ģʽǰ��������
		RTC_SetAlarm(5);
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

	// 3. ����PDDS = 1����ʾ����ģʽ
	PWR->CR |= PWR_CR_PDDS;

	// 5. ʹ��WFIָ��������ģʽ
	__WFI();
}
