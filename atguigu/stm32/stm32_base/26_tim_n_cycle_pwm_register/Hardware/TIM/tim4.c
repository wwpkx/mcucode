/*
 * @Author: wushengran
 * @Date: 2024-09-28 14:36:37
 * @Description: 
 * 
 * Copyright (c) 2024 by atguigu, All Rights Reserved. 
 */
#include "tim4.h"

// 初始化
void TIM4_Init(void)
{
    // 1. 开启时钟
    RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;

    // 2. 配置GPIO工作模式，浮空输入，CNF = 01，MODE = 00
    GPIOB->CRL &= ~GPIO_CRL_MODE6;
    GPIOB->CRL &= ~GPIO_CRL_CNF6_1;
    GPIOB->CRL |= GPIO_CRL_CNF6_0;

    // 3. 定时器计数器部分配置
    // 3.1 预分频值 71，得到 1MHz，计数周期 1us
    TIM4->PSC = 71;

    // 3.2 自动重装载值 65535，尽量保证不溢出
    TIM4->ARR = 65535;

    // 3.3 计数方向，默认向上
    TIM4->CR1 &= ~TIM_CR1_DIR;

    // 4. 定时器输入捕获部分配置
    // 4.1 TI1 选择 CH1 引脚的直接输入（默认）
    TIM4->CR2 &= ~TIM_CR2_TI1S;

    // 4.2 不使用输入滤波器（默认）
    TIM4->CCMR1 &= ~TIM_CCMR1_IC1F;

    // 4.3 检测上升沿（默认极性）
    TIM4->CCER &= ~TIM_CCER_CC1P;
    TIM4->CCER |= TIM_CCER_CC2P;

    // 4.4 将通道1配置为输入，并设置为直通信号输入（TI1 → IC1），CC1S = 01
    TIM4->CCMR1 &= ~TIM_CCMR1_CC1S_1;
    TIM4->CCMR1 |= TIM_CCMR1_CC1S_0;

    // 将通道2配置为输入，并设置为间接信号输入（TI1 → IC2），CC2S = 10
    TIM4->CCMR1 |= TIM_CCMR1_CC2S_1;
    TIM4->CCMR1 &= ~TIM_CCMR1_CC2S_0;

    // 4.5 不开启预分频器（默认）
    TIM4->CCMR1 &= ~TIM_CCMR1_IC1PSC;
    TIM4->CCMR1 &= ~TIM_CCMR1_IC2PSC;

    // 4.6 配置触发输入信号源，TS = 101
    TIM4->SMCR |= TIM_SMCR_TS_2;
    TIM4->SMCR &= ~TIM_SMCR_TS_1;
    TIM4->SMCR |= TIM_SMCR_TS_0;

    // 4.7 配置从模式：复位模式 SMS = 100
    TIM4->SMCR |= TIM_SMCR_SMS_2;
    TIM4->SMCR &= ~TIM_SMCR_SMS_1;
    TIM4->SMCR &= ~TIM_SMCR_SMS_0;

    // 4.8 开启通道1、2使能
    TIM4->CCER |= TIM_CCER_CC1E;
    TIM4->CCER |= TIM_CCER_CC2E;
}

// 开关
void TIM4_Start(void)
{
    TIM4->CR1 |= TIM_CR1_CEN;
}
void TIM4_Stop(void)
{
    TIM4->CR1 &= ~TIM_CR1_CEN;
}

// 获取PWM周期，单位为ms
double TIM4_GetPWMCycle(void)
{
    return (TIM4->CCR1 + 1) / 1000.0;
}

// 获取PWM频率，单位为Hz
double TIM4_GetPWMFreq(void)
{
    return 1000000.0 / (TIM4->CCR1 + 1);
}

// 获取PWM占空比
double TIM4_GetPWMDuty(void)
{
    return (TIM4->CCR2 + 1) * 1.0 /(TIM4->CCR1 + 1);
}
