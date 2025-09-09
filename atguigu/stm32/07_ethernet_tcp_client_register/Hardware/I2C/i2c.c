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

    // 2. GPIO配置，PB10、PB11 通用开漏输出，MODE - 11， CNF - 01
    GPIOB->CRH |= (GPIO_CRH_MODE10 | GPIO_CRH_MODE11);
    GPIOB->CRH &= ~(GPIO_CRH_CNF10_1 | GPIO_CRH_CNF11_1);
    GPIOB->CRH |= (GPIO_CRH_CNF10_0 | GPIO_CRH_CNF11_0);
}

// 主机发出起始信号
void I2C_Start(void)
{
    // 1. 拉高SCL、SDA
    SCL_HIGH;
    SDA_HIGH;
    I2C_DELAY;

    // 2. 拉低SDA，产生下降沿
    SDA_LOW;
    I2C_DELAY;
}

// 主机发出停止信号
void I2C_Stop(void)
{
    // 1. 拉高SCL，拉低SDA
    SCL_HIGH;
    SDA_LOW;
    I2C_DELAY;

    // 2. 拉高SDA，产生上升沿
    SDA_HIGH;
    I2C_DELAY;
}

// 主机发送应答/非应答信号
void I2C_Ack(void)
{
    // 1. 拉高SDA、拉低SCL，准备翻转SDA信号
    SDA_HIGH;
    SCL_LOW;
    I2C_DELAY;

    // 2. 拉低SDA，翻转数据，输出应答信号
    SDA_LOW;
    I2C_DELAY;

    // 3. 拉高SCL，保持SDA信号，做数据采样
    SCL_HIGH;
    I2C_DELAY;

    // 4. 拉低SCL，数据采样结束
    SCL_LOW;
    I2C_DELAY;

    // 5. 拉高SDA，应答输出结束，释放SDA总线
    SDA_HIGH;
    I2C_DELAY;
}
void I2C_NAck(void)
{
    // 1. 拉高SDA、拉低SCL，准备发送响应信号
    SDA_HIGH;
    SCL_LOW;
    I2C_DELAY;

    // 2. 拉高SCL，保持SDA信号，做数据采样，当前数据没有翻转，依然是高
    SCL_HIGH;
    I2C_DELAY;

    // 3. 拉低SCL，数据采样结束
    SCL_LOW;
    I2C_DELAY;
}

// 主机从SDA读取响应信号，等待应答
uint8_t I2C_Wait4Ack(void)
{
    // 1. 拉高SDA、拉低SCL，释放SDA总线
    SDA_HIGH;
    SCL_LOW;
    I2C_DELAY;

    // 2. 拉高SCL，保持SDA信号，做数据采样
    SCL_HIGH;
    I2C_DELAY;

    // 3. 读取SDA上的电平
    uint8_t ack = READ_SDA;

    // 4. 拉低SCL，数据采样结束
    SCL_LOW;
    I2C_DELAY;

    return ack ? NACK : ACK;
}

// 主机发送一个字节
void I2C_SendByte(uint8_t byte)
{
    for (uint8_t i = 0; i < 8; i++)
    {
        // 1. 拉低SCL，等待可能的翻转SDA信号
        SCL_LOW;
        I2C_DELAY;

        // 2. 将要发送的位传到SDA上，MSB先行
        if (byte & 0x80)
        {
            SDA_HIGH;
        }
        else
        {
            SDA_LOW;
        }
        I2C_DELAY;

        // 3. 拉高SCL，保持SDA信号，让从设备做数据采样
        SCL_HIGH;
        I2C_DELAY;

        // 4. 拉低SCL，数据采样结束
        SCL_LOW;
        I2C_DELAY;

        // 5. 左移1位，为下一次发送做准备
        byte <<= 1;
    }
}

// 主机从SDA读取一个字节
uint8_t I2C_ReadByte(void)
{
    // 定义变量，用来保存读取的字节
    uint8_t data = 0;

    for (uint8_t i = 0; i < 8; i++)
    {
        // 1. 拉高SDA、拉低SCL，释放SDA总线
        SDA_HIGH;
        SCL_LOW;
        I2C_DELAY;

        // 2. 拉高SCL，保持SDA信号，做数据采样
        SCL_HIGH;
        I2C_DELAY;

        // 3. 先左移1位，为下一次接收做准备
        data <<= 1;

        // 4. 读取SDA上的电平，放到数据的最低位
        if(READ_SDA)
        {
            data |= 0x01;
        }

        // 5. 拉低SCL，数据采样结束
        SCL_LOW;
        I2C_DELAY;
    }

    return data;
}
