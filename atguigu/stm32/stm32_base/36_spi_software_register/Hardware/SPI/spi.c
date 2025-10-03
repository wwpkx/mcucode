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

    // 2. GPIO工作模式
    // 2.1 CS - PC13，通用推挽输出，CNF = 00，MODE = 11
    GPIOC->CRH |= GPIO_CRH_MODE13;
    GPIOC->CRH &= ~GPIO_CRH_CNF13;

    // 2.2 SCK - PA5，通用推挽输出，CNF = 00，MODE = 11
    GPIOA->CRL |= GPIO_CRL_MODE5;
    GPIOA->CRL &= ~GPIO_CRL_CNF5;

    // 2.3 MOSI - PA7，通用推挽输出，CNF = 00，MODE = 11
    GPIOA->CRL |= GPIO_CRL_MODE7;
    GPIOA->CRL &= ~GPIO_CRL_CNF7;

    // 2.4 MISO - PA6，浮空输入，CNF = 01，MODE = 00
    GPIOA->CRL &= ~GPIO_CRL_MODE6;
    GPIOA->CRL &= ~GPIO_CRL_CNF6_1;
    GPIOA->CRL |= GPIO_CRL_CNF6_0;

    // 3. SCK 保持空闲状态，模式0 - 低电平空闲
    SCK_LOW;

    // 4. 片选初始不选中
    CS_HIGH;

    // 5. 延时
    SPI_DELAY;
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
    // 定义变量接收数据
    uint8_t rByte = 0x00;

    // 用循环一次读写每一位
    for (uint8_t i = 0; i < 8; i++)
    {
        // 1. 判断当前最高位，将MOSI拉至对应电平
        if ( byte & 0x80 )
        {
            MOSI_HIGH;
        }
        else
        {
            MOSI_LOW;
        }

        // 2. 左移一位
        byte <<= 1;
        
        // 3. 拉高时钟，在第一个上升沿采样数据
        SCK_HIGH;
        SPI_DELAY;

        // 4. 先左移，空出最低位来存放读取的数据
        rByte <<= 1;

        // 5. 在 MISO 上采样读取数据
        if ( MISO_READ )
        {
            rByte |= 0x01;  // 读到高电平，最后一位置1
        }

        // 6. 拉低时钟，为下次传输做准备
        SCK_LOW;
        SPI_DELAY;
    }
    
    return rByte;
}
