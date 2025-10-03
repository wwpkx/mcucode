/*
 * @Author: wushengran
 * @Date: 2024-09-18 09:17:17
 * @Description:
 *
 * Copyright (c) 2024 by atguigu, All Rights Reserved.
 */
#include "led.h"

// ��ʼ��
void LED_Init(void)
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
    LED_Off(LED_1);
    LED_Off(LED_2);
    LED_Off(LED_3);
}

// ����LED��
void LED_On(uint16_t led)
{
    GPIOA->ODR &= ~led;
}
void LED_Off(uint16_t led)
{
    GPIOA->ODR |= led;
}

// ��תLED��״̬
void LED_Toggle(uint16_t led)
{
    // ����IDR��Ӧλ��ֵ���жϵ�ǰLED״̬
    if ((GPIOA->IDR & led) == 0)
    {
        LED_Off(led);
    }
    else
    {
        LED_On(led);
    }
}

// ��������LED�ƵĿ���
void LED_OnAll(uint16_t leds[], uint8_t size)
{
    for (uint8_t i = 0; i < size; i++)
    {
        LED_On(leds[i]);
    }
}
void LED_OffAll(uint16_t leds[], uint8_t size)
{
    for (uint8_t i = 0; i < size; i++)
    {
        LED_Off(leds[i]);
    }
}
