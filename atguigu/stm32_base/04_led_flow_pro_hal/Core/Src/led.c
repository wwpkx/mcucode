/*
 * @Author: wushengran
 * @Date: 2024-09-18 09:17:17
 * @Description:
 *
 * Copyright (c) 2024 by atguigu, All Rights Reserved.
 */
#include "led.h"

// ����LED��
void LED_On(uint16_t led)
{
    HAL_GPIO_WritePin(LED1_GPIO_Port, led, GPIO_PIN_RESET);
}
void LED_Off(uint16_t led)
{
    HAL_GPIO_WritePin(LED1_GPIO_Port, led, GPIO_PIN_SET);
}

// ��תLED��״̬
void LED_Toggle(uint16_t led)
{
    // // ����IDR��Ӧλ��ֵ���жϵ�ǰLED״̬
    // if (HAL_GPIO_ReadPin(LED1_GPIO_Port, led) == 0)
    // {
    //     LED_Off(led);
    // }
    // else
    // {
    //     LED_On(led);
    // }
    HAL_GPIO_TogglePin(LED1_GPIO_Port, led);
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
