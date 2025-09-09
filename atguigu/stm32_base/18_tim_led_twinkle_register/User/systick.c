/*
 * @Author: wushengran
 * @Date: 2024-09-25 15:14:30
 * @Description: 
 * 
 * Copyright (c) 2024 by atguigu, All Rights Reserved. 
 */
#include "systick.h"

void Systick_Init(void)
{
    // 1. ���ó�ʼ���ص�ֵ��1ms ����һ���жϣ�72000 ��ʱ������
    SysTick->LOAD = 72000 - 1;

    // 2. ����ʱ��Դ
    SysTick->CTRL |= SysTick_CTRL_CLKSOURCE;

    // 3. ʹ���ж�
    SysTick->CTRL |= SysTick_CTRL_TICKINT;

    // 4. ������ʱ��
    SysTick->CTRL |= SysTick_CTRL_ENABLE;
}

// ��������������жϴ���
uint16_t count = 0;

void SysTick_Handler(void)
{
    count++;

    // ������˵� 1000 ���жϣ�����1s����תLED1
    if (count == 1000)
    {
        LED_Toggle(LED_1);
        count = 0;
    }
}
