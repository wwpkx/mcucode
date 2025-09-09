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

// ����LED��
void LED_On(uint16_t led);
void LED_Off(uint16_t led);

// ��תLED��״̬
void LED_Toggle(uint16_t led);

// ��������LED�ƵĿ���
void LED_OnAll(uint16_t leds[], uint8_t size);
void LED_OffAll(uint16_t leds[], uint8_t size);

#endif
