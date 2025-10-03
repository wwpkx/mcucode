/*
 * @Author: wushengran
 * @Date: 2025-01-06 16:17:25
 * @Description: 
 * 
 * Copyright (c) 2025 by atguigu, All Rights Reserved. 
 */
#include "iwdg.h"

// 初始化
void IWDG_Init(void)
{
    // 1. 开启看门狗
    IWDG->KR = 0xCCCC;

    // 2. 允许访问寄存器
    IWDG->KR = 0x5555;

    // 3. 设置预分频系数 64 - PR = 100
    IWDG->PR = 4;

    // 4. 设置重装载值，2499
    IWDG->RLR = 2499;

    // 5. 更新计数器的值（喂狗）
    IWDG_Refresh();
}

// 喂狗（刷新计数器的值）
void IWDG_Refresh(void)
{
    IWDG->KR = 0xAAAA;
}
