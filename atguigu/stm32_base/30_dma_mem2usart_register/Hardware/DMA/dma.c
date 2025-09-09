/*
 * @Author: wushengran
 * @Date: 2024-09-30 14:09:56
 * @Description: 
 * 
 * Copyright (c) 2024 by atguigu, All Rights Reserved. 
 */
#include "dma.h"

// 初始化
void DMA1_Init(void)
{
    // 1. 开启时钟
    RCC->AHBENR |= RCC_AHBENR_DMA1EN;

    // 2. DMA相关配置
    // 2.1 传输方向，DIR = 1，从存储器到串口外设
    DMA1_Channel4->CCR |= DMA_CCR4_DIR;

    // 2.2 传输数据宽度（位数），00 - 8位
    DMA1_Channel4->CCR &= ~DMA_CCR4_PSIZE;
    DMA1_Channel4->CCR &= ~DMA_CCR4_MSIZE;

    // 2.3 存储器地址自增，外设地址不自增
    DMA1_Channel4->CCR &= ~DMA_CCR4_PINC;
    DMA1_Channel4->CCR |= DMA_CCR4_MINC;

    // 2.4 开启传输完成中断
    DMA1_Channel4->CCR |= DMA_CCR4_TCIE;

    // 2.5 开启串口TX的DMA使能
    USART1->CR3 |= USART_CR3_DMAT;

    // 打开循环传输
    DMA1_Channel4->CCR |= DMA_CCR1_CIRC;

    // 3. NVIC配置
    NVIC_SetPriorityGrouping(3);
    NVIC_SetPriority(DMA1_Channel4_IRQn, 3);
    NVIC_EnableIRQ(DMA1_Channel4_IRQn);
}

// DMA1通道1传输数据，传入参数：源地址、目的地址、数据长度
void DMA1_Transmit(uint32_t srcAddr, uint32_t destAddr, uint16_t dataLen)
{
    // 1. 将源地址写入存储器地址寄存器
    DMA1_Channel4->CMAR = srcAddr;

    // 2. 将目的地址写入外设地址寄存器
    DMA1_Channel4->CPAR = destAddr;

    // 3. 将数据长度写入 CNDTR
    DMA1_Channel4->CNDTR = dataLen;

    // 4. 开启通道使能，开始传输数据
    DMA1_Channel4->CCR |= DMA_CCR4_EN;
}


// 中断服务程序
void DMA1_Channel4_IRQHandler(void)
{
    // 判断是否是传输完成中断（TCIF）
    if ( DMA1->ISR & DMA_ISR_TCIF4 )
    {
        // 清除中断标志位
        DMA1->IFCR |= DMA_IFCR_CTCIF4;

        // 关闭通道
        // DMA1_Channel4->CCR &= ~DMA_CCR4_EN;
    }
}
