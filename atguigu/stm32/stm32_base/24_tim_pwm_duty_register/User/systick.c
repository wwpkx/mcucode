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
    // 1. 设置初始加载的值，1ms 产生一个中断，72000 个时钟周期
    SysTick->LOAD = 72000 - 1;

    // 2. 配置时钟源
    SysTick->CTRL |= SysTick_CTRL_CLKSOURCE;

    // 3. 使能中断
    SysTick->CTRL |= SysTick_CTRL_TICKINT;

    // 4. 开启定时器
    SysTick->CTRL |= SysTick_CTRL_ENABLE;
}

// 定义变量，保存中断次数
uint16_t cnt = 0;

void SysTick_Handler(void)
{
    cnt++;

    // 如果到了第 1000 次中断，就是1s，翻转LED1
    if (cnt == 1000)
    {
        LED_Toggle(LED_1);
        cnt = 0;
    }
}
