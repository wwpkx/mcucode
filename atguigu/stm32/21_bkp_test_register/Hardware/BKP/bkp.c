/*
 * @Author: wushengran
 * @Date: 2025-01-03 15:16:18
 * @Description: 
 * 
 * Copyright (c) 2025 by atguigu, All Rights Reserved. 
 */
#include "bkp.h"

// 初始化
void BKP_Init(void)
{
    // 1. 开启PWR时钟
    RCC->APB1ENR |= RCC_APB1ENR_PWREN;

    // 2. 放开后备域的写保护
    PWR->CR |= PWR_CR_DBP;

    // 3. 开启BKP时钟
    RCC->APB1ENR |= RCC_APB1ENR_BKPEN;

    // // 4. 软件复位整个备份域
    // RCC->BDCR |= RCC_BDCR_BDRST;

    // // 5. 解除备份域复位
    // RCC->BDCR &= ~RCC_BDCR_BDRST;
}
