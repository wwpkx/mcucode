/*
 * @Author: wushengran
 * @Date: 2024-10-09 16:17:30
 * @Description: 
 * 
 * Copyright (c) 2024 by atguigu, All Rights Reserved. 
 */
#include "spi.h"

// 初始化
void SPI_Init(void)
{
    // 1. 开启时钟
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
    RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
    RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;

    // 2. GPIO工作模式
    // 2.1 CS - PC13，通用推挽输出，CNF = 00，MODE = 11
    GPIOC->CRH |= GPIO_CRH_MODE13;
    GPIOC->CRH &= ~GPIO_CRH_CNF13;

    // 2.2 SCK - PA5，复用推挽输出，CNF = 10，MODE = 11
    GPIOA->CRL |= GPIO_CRL_MODE5;
    GPIOA->CRL |= GPIO_CRL_CNF5_1;
    GPIOA->CRL &= ~GPIO_CRL_CNF5_0;

    // 2.3 MOSI - PA7，复用推挽输出，CNF = 10，MODE = 11
    GPIOA->CRL |= GPIO_CRL_MODE7;
    GPIOA->CRL |= GPIO_CRL_CNF7_1;
    GPIOA->CRL &= ~GPIO_CRL_CNF7_0;

    // 2.4 MISO - PA6，浮空输入，CNF = 01，MODE = 00
    GPIOA->CRL &= ~GPIO_CRL_MODE6;
    GPIOA->CRL &= ~GPIO_CRL_CNF6_1;
    GPIOA->CRL |= GPIO_CRL_CNF6_0;

    // 3. SPI模块配置
    // 3.1 工作模式相关配置
    // 3.1.1 设置为主模式
    SPI1->CR1 |= SPI_CR1_MSTR;

    // 3.1.2 选择软件控制片选，并保持为高电平
    SPI1->CR1 |= SPI_CR1_SSM;
    SPI1->CR1 |= SPI_CR1_SSI;

    // 3.1.2 SPI工作模式设置为模式0，CPOL = 0，CPHA = 0
    SPI1->CR1 &= ~(SPI_CR1_CPOL | SPI_CR1_CPHA);

    // 3.2 设置波特率（时钟分频系数），001 - 4分频，18MHz
    SPI1->CR1 &= ~SPI_CR1_BR;
    SPI1->CR1 |= SPI_CR1_BR_0;

    // 3.3 数据帧格式相关配置
    // 3.3.1 数据帧长度，0 - 8位
    SPI1->CR1 &= ~SPI_CR1_DFF;

    // 3.3.2 数据传输顺序，0 - MSB优先
    SPI1->CR1 &= ~SPI_CR1_LSBFIRST;

    // 3.4 开启SPI使能
    SPI1->CR1 |= SPI_CR1_SPE;
}

// SPI通信的开启和关闭（通过片选信号控制）
void SPI_Start(void)
{
    CS_LOW;
}
void SPI_Stop(void)
{
    CS_HIGH;
}

// 一个时钟周期内，主从交换一个字节数据
uint8_t SPI_SwapByte(uint8_t byte)
{
    // 1. 将要发送的数据写入发送缓冲区
    // 1.1 等待发送缓冲区为空，TXE = 1
    while ((SPI1->SR & SPI_SR_TXE) == 0 )
    {}
    
    // 1.2 将数据写入DR
    SPI1->DR = byte;

    // 2. 读取接收的数据并返回
    // 2.1 等待接收缓冲区非空，RXNE = 1
    while ( (SPI1->SR & SPI_SR_RXNE) == 0 )
    {}
    
    // 2.2 将DR的数据返回
    return (uint8_t)(SPI1->DR & 0xff);
}
