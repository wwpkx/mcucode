/*
 * @Author: wushengran
 * @Date: 2024-09-13 16:29:39
 * @Description:
 *
 * Copyright (c) 2024 by atguigu, All Rights Reserved.
 */

#include "stm32f10x.h"

// ������ʱ����
void Delay_us(uint16_t us);
void Delay_ms(uint16_t ms);
void Delay_s(uint16_t s);

int main(void)
{
	// 1. ʱ�����ã���GPIOAʱ��
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;

	// 2. ����ģʽ���ã�PA0��PA1��PA8 ͨ�����������CNF = 00��MODE = 11
	GPIOA->CRL |= GPIO_CRL_MODE0;
	GPIOA->CRL &= ~GPIO_CRL_CNF0;
	GPIOA->CRL |= GPIO_CRL_MODE1;
	GPIOA->CRL &= ~GPIO_CRL_CNF1;
	GPIOA->CRH |= GPIO_CRH_MODE8;
	GPIOA->CRH &= ~GPIO_CRH_CNF8;

	// 3. ��ʼ״̬������������ߵ�ƽ���ص�
	GPIOA->ODR |= (GPIO_ODR_ODR0 | GPIO_ODR_ODR1 | GPIO_ODR_ODR8);

	// 4. ��ѭ�������ε���ÿ���ơ��ӳ�һ��ʱ���ر�
	while (1)
	{
		// ���� LED1
		GPIOA->ODR &= ~GPIO_ODR_ODR0;
		// ��ʱ
		Delay_ms(500);
		// �ر� LED1
		GPIOA->ODR |= GPIO_ODR_ODR0;

		// ���� LED2
		GPIOA->ODR &= ~GPIO_ODR_ODR1;
		// ��ʱ
		Delay_ms(500);
		// �ر� LED2
		GPIOA->ODR |= GPIO_ODR_ODR1;

		// ���� LED3
		GPIOA->ODR &= ~GPIO_ODR_ODR8;
		// ��ʱ
		Delay_ms(500);
		// �ر� LED3
		GPIOA->ODR |= GPIO_ODR_ODR8;
	}
}

// ΢�뼶��ʱ�������ӳ�us΢��
void Delay_us(uint16_t us)
{
	// ����ϵͳ��ʱ���ĳ�ʼ����ֵ
	SysTick->LOAD = 72 * us;

	// ����ϵͳ��ʱ��
	SysTick->CTRL = 0x05;

	// ��ѯ�ȴ�����ֵ��Ϊ0��COUNTFLAG = 1
	while (!(SysTick->CTRL & SysTick_CTRL_COUNTFLAG))
	{
	}

	// �رն�ʱ��
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE;
}

void Delay_ms(uint16_t ms)
{
	while (ms--)
	{
		Delay_us(1000);
	}
}
void Delay_s(uint16_t s)
{
	while (s--)
	{
		Delay_ms(1000);
	}
}
