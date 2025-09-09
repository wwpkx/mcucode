/*
 * @Author: wushengran
 * @Date: 2024-09-18 09:13:00
 * @Description: 
 * 
 * Copyright (c) 2024 by atguigu, All Rights Reserved. 
 */
#include "delay.h"

// ΢�뼶��ʱ�������ӳ�us΢��
void Delay_us(uint16_t us)
{
	// ����ϵͳ��ʱ���ĳ�ʼ����ֵ
	SysTick->LOAD = 72 * us;

	// ������ֵ����
	SysTick->VAL = 0;

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
