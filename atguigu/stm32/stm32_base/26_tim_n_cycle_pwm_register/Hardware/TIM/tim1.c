/*
 * @Author: wushengran
 * @Date: 2024-09-29 16:18:30
 * @Description: 
 * 
 * Copyright (c) 2024 by atguigu, All Rights Reserved. 
 */
#include "tim1.h"

// 初始化
void TIM1_Init(void)
{
    // 1. 开启时钟
    RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;

    // 2. 配置GPIO工作模式，PA8 复用推挽输出，CNF = 10，MODE = 11
    GPIOA->CRH |= GPIO_CRH_MODE8;
    GPIOA->CRH |= GPIO_CRH_CNF8_1;
    GPIOA->CRH &= ~GPIO_CRH_CNF8_0;

    // 3. 定时器时基部分配置
    // 3.1 预分频值 7199，得到 10000Hz，计数周期 100us（0.1ms）
    TIM1->PSC = 7199;

    // 3.2 自动重装载值 4999，5000 次计数之后产生溢出，溢出周期 0.5s，频率 2Hz
    TIM1->ARR = 4999;

    // 3.3 计数方向
    TIM1->CR1 &= ~TIM_CR1_DIR;

    // 3.4 重复计数值 4，表示 5 次溢出之后产生更新事件
    TIM1->RCR = 4;

    // 4. 定时器输出比较部分配置
    // 4.1 通道1配置为输出
    TIM1->CCMR1 &= ~TIM_CCMR1_CC1S;

    // 4.2 配置为 PWM模式1，OC1M = 110
    TIM1->CCMR1 |= TIM_CCMR1_OC1M_2;
    TIM1->CCMR1 |= TIM_CCMR1_OC1M_1;
    TIM1->CCMR1 &= ~TIM_CCMR1_OC1M_0;

    // 4.3 配置 CCR 的值，50% 占空比
    TIM1->CCR1 = 2500;

    // 4.4 开启通道使能
    TIM1->CCER |= TIM_CCER_CC1E;

    // 更改极性
    TIM1->CCER |= TIM_CCER_CC1P;
    // 设置空闲状态电平为高
    TIM1->CR2 |= TIM_CR2_OIS1;

    // 4.5 开启主输出使能
    TIM1->BDTR |= TIM_BDTR_MOE;

    // 4.6 防止一启动就快速进入中断，用 UG 位产生更新事件，并配置 URS
    TIM1->CR1 |= TIM_CR1_URS;
    TIM1->EGR |= TIM_EGR_UG;

    // 4.7 开启更新中断
    TIM1->DIER |= TIM_DIER_UIE;

    // 5. NVIC配置
    NVIC_SetPriorityGrouping(3);
    NVIC_SetPriority(TIM1_UP_IRQn, 3);
    NVIC_EnableIRQ(TIM1_UP_IRQn);
}

// 开关定时器
void TIM1_Start(void)
{
    TIM1->CR1 |= TIM_CR1_CEN;
}
void TIM1_Stop(void)
{
    TIM1->CR1 &= ~TIM_CR1_CEN;
}

// 中断处理程序
void TIM1_UP_IRQHandler(void)
{
    // 清除标志位
    TIM1->SR &= ~TIM_SR_UIF;

    // 关闭主使能，进入空闲模式
    TIM1->BDTR &= ~TIM_BDTR_MOE;

    // 停掉定时器
    TIM1_Stop();
}
