/*
 * @Author: wushengran
 * @Date: 2024-09-18 09:17:25
 * @Description: 
 * 
 * Copyright (c) 2024 by atguigu, All Rights Reserved. 
 */
#ifndef __LED_H
#define __LED_H

#include "gpio.h"

// 开关LED灯
void LED_On(uint16_t led);
void LED_Off(uint16_t led);

// 翻转LED灯状态
void LED_Toggle(uint16_t led);

// 控制所有LED灯的开关
void LED_OnAll(uint16_t leds[], uint8_t size);
void LED_OffAll(uint16_t leds[], uint8_t size);

#endif
