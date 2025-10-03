/*
 * @Author: wushengran
 * @Date: 2024-09-20 16:23:02
 * @Description:
 *
 * Copyright (c) 2024 by atguigu, All Rights Reserved.
 */
#include "usart.h"

// 初始化
void USART_Init(void)
{
    // 1. 开启时钟
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;

    // 2. GPIO 工作模式
    // 2.1 PA9 - TX，复用推挽输出，CNF = 10，MODE = 11
    GPIOA->CRH |= GPIO_CRH_MODE9;
    GPIOA->CRH |= GPIO_CRH_CNF9_1;
    GPIOA->CRH &= ~GPIO_CRH_CNF9_0;
    // 2.2 PA10 - RX，浮空输入，CNF = 01，MODE = 00
    GPIOA->CRH &= ~GPIO_CRH_MODE10;
    GPIOA->CRH &= ~GPIO_CRH_CNF10_1;
    GPIOA->CRH |= GPIO_CRH_CNF10_0;

    // 3. 串口模块配置
    // 3.1 设置波特率 115200
    USART1->BRR = 0x271;

    // 3.2 使能串口和收发模块
    USART1->CR1 |= USART_CR1_UE;
    USART1->CR1 |= (USART_CR1_TE | USART_CR1_RE);

    // 3.3 配置数据帧的格式
    USART1->CR1 &= ~USART_CR1_M;    // 长度为 8 位
    USART1->CR1 &= ~USART_CR1_PCE;  // 不使用校验位
    USART1->CR2 &= ~USART_CR2_STOP; // 1 位停止位

    // 3.4 使能串口接收中断
    USART1->CR1 |= USART_CR1_RXNEIE;
    USART1->CR1 |= USART_CR1_IDLEIE;

    // 4. 配置NVIC
    NVIC_SetPriorityGrouping(3);
    NVIC_SetPriority(USART1_IRQn, 2);
    NVIC_EnableIRQ(USART1_IRQn);
}

// 发送一个字符
void USART_SendChar(uint8_t ch)
{
    // 判断 TDR 是否为空，必须等待 TDR 为空才能继续发送
    while ((USART1->SR & USART_SR_TXE) == 0)
    {
    }

    // 将要发送的数据写入TDR
    USART1->DR = ch;
}


// 发送字符串
void USART_SendString(uint8_t *str, uint8_t size)
{
    for (uint8_t i = 0; i < size; i++)
    {
        USART_SendChar(str[i]);
    }
}

// 引入外部变量
extern uint8_t buff[100];
extern uint8_t size;
extern uint8_t isOver;

// 中断服务程序
void USART1_IRQHandler(void)
{
    // 判断是 RXNE = 1 还是 IDLE = 1
    if ( USART1->SR & USART_SR_RXNE )
    {
        // 如果 RXNE = 1，表示接收到一个字符
        buff[size] = USART1->DR;
        size ++;
    }
    else if ( USART1->SR & USART_SR_IDLE )
    {
        USART1->DR;
        // 如果 IDLE = 1，表示检测到空闲帧，字符串接收完毕
        // USART_SendString(buff, size);
        // size = 0;

        isOver = 1;
    }
}
