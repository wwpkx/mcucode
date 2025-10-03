/*
 * @Author: wushengran
 * @Date: 2025-01-03 15:16:18
 * @Description: 
 * 
 * Copyright (c) 2025 by atguigu, All Rights Reserved. 
 */
#include "bkp.h"

// ��ʼ��
void BKP_Init(void)
{
    // 1. ����PWRʱ��
    RCC->APB1ENR |= RCC_APB1ENR_PWREN;

    // 2. �ſ������д����
    PWR->CR |= PWR_CR_DBP;

    // 3. ����BKPʱ��
    RCC->APB1ENR |= RCC_APB1ENR_BKPEN;

    // // 4. �����λ����������
    // RCC->BDCR |= RCC_BDCR_BDRST;

    // // 5. ���������λ
    // RCC->BDCR &= ~RCC_BDCR_BDRST;
}
