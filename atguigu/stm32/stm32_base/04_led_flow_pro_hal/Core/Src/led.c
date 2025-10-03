/*
 * @Author: wushengran
 * @Date: 2024-09-18 09:17:17
 * @Description:
 *
 * Copyright (c) 2024 by atguigu, All Rights Reserved.
 */
#include "led.h"

// 开关LED灯
void LED_On(uint16_t led)
{
    HAL_GPIO_WritePin(LED1_GPIO_Port, led, GPIO_PIN_RESET);
}
void LED_Off(uint16_t led)
{
    HAL_GPIO_WritePin(LED1_GPIO_Port, led, GPIO_PIN_SET);
}

// 翻转LED灯状态
void LED_Toggle(uint16_t led)
{
    // // 根据IDR对应位的值，判断当前LED状态
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

// 控制所有LED灯的开关
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
