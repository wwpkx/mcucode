/*
 * @Author: wushengran
 * @Date: 2024-10-08 16:17:59
 * @Description: 
 * 
 * Copyright (c) 2024 by atguigu, All Rights Reserved. 
 */
#include "adc.h"

// 初始化
void ADC1_Init(void)
{
    // 1. 时钟配置
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
    // ADC的时钟频率不能超过14MHz，需要做预分频（10 - 6分频）
    RCC->CFGR |= RCC_CFGR_ADCPRE_DIV6;

    // RCC->APB2ENR | RCC_APB2ENR_IOPCEN;

    // 2. GPIO工作模式：PC0 - 模拟输入，CNF = 00， MODE = 00
    GPIOC->CRL &= ~(GPIO_CRL_MODE0 | GPIO_CRL_CNF0);

    // 3. ADC配置
    // 3.1 禁用扫描模式（只有1个转换通道）
    ADC1->CR1 &= ~ADC_CR1_SCAN;

    // 3.2 启用连续转换模式
    ADC1->CR2 |= ADC_CR2_CONT;

    // 3.3 数据对其方式：0 - 右对齐
    ADC1->CR2 &= ~ADC_CR2_ALIGN;

    // 3.4 设置采样时间，010 - 13.5个ADC时钟周期
    ADC1->SMPR1 &= ~ADC_SMPR1_SMP10;
    ADC1->SMPR1 |= ADC_SMPR1_SMP10_1;

    // 3.5 配置通道组序列
    // 3.5.1 配置序列中通道个数，L = 0000，1个通道
    ADC1->SQR1 &= ~ADC_SQR1_L;

    // 3.5.2 将通道号10写入 SQ1（在SQR3里）
    ADC1->SQR3 &= ~ADC_SQR3_SQ1;
    ADC1->SQR3 |= 10 << 0;      // 最后5位（第一个通道号）置为10

    // 3.6 选择软件触发方式，111
    // ADC1->CR2 |= ADC_CR2_EXTTRIG;
    // ADC1->CR2 |= ADC_CR2_EXTSEL;
}

// 启动ADC并开启转换
void ADC1_StartConvert(void)
{
    // 1. 上电，将ADC从断电模式唤醒
    ADC1->CR2 |= ADC_CR2_ADON;

    // 2. 执行校准并等待完成
    ADC1->CR2 |= ADC_CR2_CAL;
    while (ADC1->CR2 & ADC_CR2_CAL)
    {}

    // 3. 软件方式开启AD转换
    // ADC1->CR2 |= ADC_CR2_SWSTART;
    ADC1->CR2 |= ADC_CR2_ADON;
    
    // 4. 等待转换完成
    while ((ADC1->SR & ADC_SR_EOC) == 0)
    {}
}

// 返回转换后的电压值
double ADC1_ReadV(void)
{
    return ADC1->DR * 3.3 / 4095;
}
