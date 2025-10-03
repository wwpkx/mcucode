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

    // 4.4 将通道1配置为输入，并设置为直通信号输入（TI1 → IC1），CC1S = 01
    TIM4->CCMR1 &= ~TIM_CCMR1_CC1S_1;
    TIM4->CCMR1 |= TIM_CCMR1_CC1S_0;

    // 4.5 不开启预分频器（默认）
    TIM4->CCMR1 &= ~TIM_CCMR1_IC1PSC;

    // 4.6 开启通道1使能
    TIM4->CCER |= TIM_CCER_CC1E;

    // 4.7 开启输入捕获中断使能
    TIM4->DIER |= TIM_DIER_CC1IE;

    // 5. NVIC配置
    NVIC_SetPriorityGrouping(3);
    NVIC_SetPriority(TIM4_IRQn, 3);
    NVIC_EnableIRQ(TIM4_IRQn);
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

// // 定义全局变量，保存进入中断的次数（上升沿到来的次数）
// uint8_t count = 0;
// // 保存获取的周期值
// uint16_t cycle = 0;

// // 中断服务程序
// void TIM4_IRQHandler(void)
// {
//     // 判断是否是通道1的捕获中断
//     if (TIM4->SR & TIM_SR_CC1IF)
//     {
//         // 先清除中断标志位
//         TIM4->SR &= ~TIM_SR_CC1IF;

//         count ++;

//         // 根据进入中断的次数，选择清零计数器或者捕获周期值
//         if (count == 1)
//         {
//             // 清零CNT，重新开始计数
//             TIM4->CNT = 0;
//         }
//         else if (count == 2)
//         {
//             // 直接读取CCR1的值，就是周期的微秒数
//             cycle = TIM4->CCR1 + 1;
            
//             // 清零count
//             count = 0;
//         } 
//     }
// }


// 改进版中断服务程序
void TIM4_IRQHandler(void)
{
    // 判断是否是通道1的捕获中断
    if (TIM4->SR & TIM_SR_CC1IF)
    {
        // 先清除中断标志位
        TIM4->SR &= ~TIM_SR_CC1IF;

        // 直接清零CNT
        TIM4->CNT = 0;
    }
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
