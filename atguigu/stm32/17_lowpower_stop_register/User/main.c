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
#include "key.h"

void enter_stop_mode(void);
void get_clock_freq(uint32_t *sys_clock, uint32_t *AHB_clock, uint32_t *APB2_clock, uint32_t *APB1_clock);

int main(void)
{
	// ��ʼ��
	USART_Init();
	LED_Init();
	KEY_Init();

	printf("�й�ȵ͹���ʵ�飺ͣ��ģʽ...\n");

	// 1. ����LED�ƣ���ʱ2s��ģ����������ִ�й���
	LED_On(LED_2);
	Delay_s(2);

	while (1)
	{
		// 2. ����ͣ��ģʽ
		printf("��������ִ����ϣ�3s�����ͣ��ģʽ...\n");
		Delay_s(3);
		printf("����ͣ��ģʽ...\n");
		Delay_ms(1);	// ������ʱ��ȷ��������Ϣ�ɹ�
		enter_stop_mode();

		uint32_t sys_clock, ahb_clock, apb1_clock, apb2_clock;
		get_clock_freq(&sys_clock, &ahb_clock, &apb2_clock, &apb1_clock);

		// 3. ���½���ϵͳ��ʼ��������ϵͳʱ��
		SystemInit();

		printf("����ǰ��ʱ��Ƶ�ʣ�\nsys_clock = %d, ahb_clock = %d, apb1_clock = %d, apb2_clock = %d\n\n", 
				sys_clock, ahb_clock, apb1_clock, apb2_clock);

		get_clock_freq(&sys_clock, &ahb_clock, &apb2_clock, &apb1_clock);

		printf("���ú��ʱ��Ƶ�ʣ�\nsys_clock = %d, ahb_clock = %d, apb1_clock = %d, apb2_clock = %d\n\n", 
				sys_clock, ahb_clock, apb1_clock, apb2_clock);

		// 4. ���´���ֻ���ڻ���֮��Ż�ִ��
		printf("��ͣ��ģʽ�л���...\n");
		Delay_s(2);
	}
}

// �������˯��ģʽ�ĺ���
void enter_stop_mode(void)
{
	// 1. ������˯��ģʽ
	SCB->SCR |= SCB_SCR_SLEEPDEEP;

	// 2. ����PWRģ��ʱ��
	RCC->APB1ENR |= RCC_APB1ENR_PWREN;

	// 3. ����PDDS = 0����ʾͣ��ģʽ
	PWR->CR &= ~PWR_CR_PDDS;

	// 4. ���õ�ѹ�������Ĺ���ģʽ���͹���ģʽ
	PWR->CR |= PWR_CR_LPDS;

	// 5. ʹ��WFIָ�����ͣ��ģʽ
	__WFI();
}

// �Զ��庯������ѯ����ʱ��Ƶ�ʣ�ϵͳʱ��sysclock��AHB��APB2��APB1��
void get_clock_freq(uint32_t *sys_clock, uint32_t *AHB_clock, uint32_t *APB2_clock, uint32_t *APB1_clock)
{
	// 1. ��ȡʱ��Դ
	uint32_t clock_src = RCC->CFGR & RCC_CFGR_SWS;

	// 2. ����ʱ��Դ��ȡϵͳʱ��Ƶ��
	if (clock_src == RCC_CFGR_SWS_HSE)
	{
		*sys_clock = HSE_VALUE;
	}
	else if (clock_src == RCC_CFGR_SWS_HSI)
	{
		*sys_clock = HSI_VALUE;
	}
	else if (clock_src == RCC_CFGR_SWS_PLL)
	{
		// ȡ��Ƶϵ�����
		uint32_t mul = ((RCC->CFGR & RCC_CFGR_PLLMULL) >> 18) + 2;
		*sys_clock = HSE_VALUE * mul;
	}

	// 3. ���ݷ�Ƶϵ����ȷ������ʱ��Ƶ��
	// 3.1 AHB
	// 3.1.1 ��ȡ��Ƶϵ��
	uint32_t hpre = (RCC->CFGR & RCC_CFGR_HPRE) >> 4;
	// 3.1.2 �������λ���ж��Ƿ��з�Ƶ
	if (hpre & 0x8)
	{
		// �з�Ƶ��ȡ�����λ
		uint32_t n = hpre & 0x7;
		// ��4���Ƚϣ�ѡ������n+1λ����n+2λ
		if (n < 4)
		{
			*AHB_clock = *sys_clock >> (n+1);
		}
		else
		{
			*AHB_clock = *sys_clock >> (n+2);
		}
	}
	else
	{
		// û�з�Ƶ
		*AHB_clock = *sys_clock;
	}

	// 3.2 APB1
	// 3.2.1 ��ȡ��Ƶϵ��
	uint32_t ppre1 = (RCC->CFGR & RCC_CFGR_PPRE1) >> 8;
	// 3.2.2 �������λ���ж��Ƿ��з�Ƶ
	if (ppre1 & 0x4)
	{
		*APB1_clock = *AHB_clock >> ((ppre1 & 0x3) + 1);
	}
	else
	{
		// û�з�Ƶ
		*APB1_clock = *AHB_clock;
	}

	// 3.3 APB2
	// 3.3.1 ��ȡ��Ƶϵ��
	uint32_t ppre2 = (RCC->CFGR & RCC_CFGR_PPRE2) >> 11;
	// 3.3.2 �������λ���ж��Ƿ��з�Ƶ
	if (ppre2 & 0x4)
	{
		*APB2_clock = *AHB_clock >> ((ppre2 & 0x3) + 1);
	}
	else
	{
		// û�з�Ƶ
		*APB2_clock = *AHB_clock;
	}
}
