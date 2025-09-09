/*
 * @Author: wushengran
 * @Date: 2024-09-23 14:01:27
 * @Description:
 *
 * Copyright (c) 2024 by atguigu, All Rights Reserved.
 */
#include "i2c.h"

// 初始化
void I2C_Init(void)
{
    // 1. 配置时钟
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
    RCC->APB1ENR |= RCC_APB1ENR_I2C2EN;

    // 2. GPIO配置，PB10、PB11 复用开漏输出，MODE - 11， CNF - 11
    GPIOB->CRH |= (GPIO_CRH_MODE10 | GPIO_CRH_MODE11 | GPIO_CRH_CNF10 | GPIO_CRH_CNF11);

    // 3. I2C 配置
    // 3.1 硬件工作模式配置
    I2C2->CR1 &= ~I2C_CR1_SMBUS; // I2C模式
    I2C2->CCR &= ~I2C_CCR_FS;    // 标准模式

    // 3.2 时钟频率 36MHz
    I2C2->CR2 |= 36 << 0;

    // 3.3 高电平时间：5us - 180个时钟周期；100kbps
    I2C2->CCR |= 180 << 0;

    // 3.4 上升沿时间 1us - 36个时钟周期
    I2C2->TRISE |= 37;

    // 3.5 使能I2C模块
    I2C2->CR1 |= I2C_CR1_PE;
}

// 主机发出起始信号
uint8_t I2C_Start(void)
{
    // 产生起始信号
    I2C2->CR1 |= I2C_CR1_START;

    // 等待起始信号发出
    uint16_t timeout = 0xffff; // 超时时间
    while ((I2C2->SR1 & I2C_SR1_SB) == 0 && timeout)
    {
        timeout--;
    }
    return timeout ? OK : FAIL;
}

// 主机设置收发完数据之后发出停止信号
void I2C_Stop(void)
{
    I2C2->CR1 |= I2C_CR1_STOP;
}

// 主机设置发送应答/非应答信号
void I2C_Ack(void)
{
    I2C2->CR1 |= I2C_CR1_ACK;
}
void I2C_NAck(void)
{
    I2C2->CR1 &= ~I2C_CR1_ACK;
}

// 主机发送设备地址
uint8_t I2C_SendAddr(uint8_t addr)
{
    // 将要发送的地址写入DR，自动发送
    I2C2->DR = addr;

    // 等待地址成功发出（等待应答）
    uint16_t timeout = 0xffff; // 超时时间
    while ((I2C2->SR1 & I2C_SR1_ADDR) == 0 && timeout)
    {
        timeout--;
    }

    I2C2->SR2; // 清除 ADDR 标志位

    return timeout ? OK : FAIL;
}

// 主机发送一个字节
uint8_t I2C_SendByte(uint8_t byte)
{
    // 将要发送的数据写入DR，自动发送
    I2C2->DR = byte;

    // 等待字节发送完成（等待应答）
    uint16_t timeout = 0xffff; // 超时时间
    while ((I2C2->SR1 & I2C_SR1_BTF) == 0 && timeout)
    {
        timeout--;
    }

    I2C2->DR; // 清除 BTF 标志位

    return timeout ? OK : FAIL;
}

// 主机从SDA读取一个字节
uint8_t I2C_ReadByte(void)
{
    // 等待字节接收完成
    uint16_t timeout = 0xffff; // 超时时间
    while ((I2C2->SR1 & I2C_SR1_RXNE) == 0 && timeout)
    {
        timeout--;
    }

    return timeout ? I2C2->DR : FAIL;
}
