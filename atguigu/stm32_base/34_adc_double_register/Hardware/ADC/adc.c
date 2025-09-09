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

    // PC2 - 模拟输入，CNF = 00， MODE = 00
    GPIOC->CRL &= ~(GPIO_CRL_MODE2 | GPIO_CRL_CNF2);

    // 3. ADC配置
    // 3.1 启用扫描模式
    ADC1->CR1 |= ADC_CR1_SCAN;

    // 3.2 启用连续转换模式
    ADC1->CR2 |= ADC_CR2_CONT;

    // 3.3 数据对其方式：0 - 右对齐
    ADC1->CR2 &= ~ADC_CR2_ALIGN;

    // 3.4 设置采样时间，010 - 13.5个ADC时钟周期
    ADC1->SMPR1 &= ~ADC_SMPR1_SMP10;
    ADC1->SMPR1 |= ADC_SMPR1_SMP10_1;
    ADC1->SMPR1 &= ~ADC_SMPR1_SMP12;

    // 3.5 配置通道组序列
    // 3.5.1 配置序列中通道个数，L = 0001，2个通道
    ADC1->SQR1 &= ~ADC_SQR1_L;
    ADC1->SQR1 |= ADC_SQR1_L_0;

    // 3.5.2 将通道号10写入 SQ1（在SQR3里）
    ADC1->SQR3 &= ~ADC_SQR3_SQ1;
    ADC1->SQR3 |= 10 << 0;      // 最后5位（第一个通道号）置为10

    // 将通道号12写入 SQ2（在SQR3里）
    ADC1->SQR3 &= ~ADC_SQR3_SQ2;
    ADC1->SQR3 |= 12 << 5;      // 左移5位（第二个通道号）置为12

    // 3.6 选择软件触发方式，111
    // ADC1->CR2 |= ADC_CR2_EXTTRIG;
    // ADC1->CR2 |= ADC_CR2_EXTSEL;
}

// DMA相关的初始化
void ADC1_DMA_Init(void)
{
    // 1. 开启时钟
    RCC->AHBENR |= RCC_AHBENR_DMA1EN;

    // 2. DMA传输方向，DIR = 0，从外设读取
    DMA1_Channel1->CCR &= ~DMA_CCR1_DIR;

    // 3. 数据宽度：01 - 16位
    DMA1_Channel1->CCR &= ~DMA_CCR1_PSIZE_1;
    DMA1_Channel1->CCR |= DMA_CCR1_PSIZE_0;

    DMA1_Channel1->CCR &= ~DMA_CCR1_MSIZE_1;
    DMA1_Channel1->CCR |= DMA_CCR1_MSIZE_0;

    // 4. 地址是否自增：外设不增，存储器自增
    DMA1_Channel1->CCR &= ~DMA_CCR1_PINC;
    DMA1_Channel1->CCR |= DMA_CCR1_MINC;

    // 5. 开启循环模式
    DMA1_Channel1->CCR |= DMA_CCR1_CIRC;

    // 6. ADC1开启DMA功能
    ADC1->CR2 |= ADC_CR2_DMA;
}

// 启动ADC并开启转换
void ADC1_StartConvert_DMA(uint32_t destAddr, uint16_t len)
{
    // 0. DMA配置并开启通道1
    DMA1_Channel1->CPAR = (uint32_t)&(ADC1->DR);
    DMA1_Channel1->CMAR = destAddr;
    DMA1_Channel1->CNDTR = len;

    DMA1_Channel1->CCR |= DMA_CCR1_EN;

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
