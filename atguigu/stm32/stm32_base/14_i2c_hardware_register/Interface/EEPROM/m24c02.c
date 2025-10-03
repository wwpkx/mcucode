/*
 * @Author: wushengran
 * @Date: 2024-09-23 14:07:07
 * @Description:
 *
 * Copyright (c) 2024 by atguigu, All Rights Reserved.
 */
#include "m24c02.h"

// 初始化
void M24C02_Init(void)
{
    I2C_Init();
}

// 写入一个字节，指定要写入的内部地址
void M24C02_WriteByte(uint8_t innerAddr, uint8_t byte)
{
    // 1. 发出开始信号
    I2C_Start();

    // 2. 发送写地址
    I2C_SendAddr(W_ADDR);

    // 3. 发送EEPROM内部地址
    I2C_SendByte(innerAddr);

    // 4. 发送具体数据
    I2C_SendByte(byte);

    // 5. 发送结束信号
    I2C_Stop();

    // 加入延迟，等待写入EEPROM完成
    Delay_ms(5);
}

// 读取一个字节（随机地址读）
uint8_t M24C02_ReadByte(uint8_t innerAddr)
{
    // 1. 发出开始信号
    I2C_Start();

    // 2. 发送写地址（假写）
    I2C_SendAddr(W_ADDR);

    // 4. 发送EEPROM内部地址
    I2C_SendByte(innerAddr);

    // 6. 再次发送开始信号
    I2C_Start();

    // 7. 发送读地址（真读）
    I2C_SendAddr(R_ADDR);

    // 10. 提前设置，在接收到下一个数据后，发送一个非应答
    I2C_NAck();

    // 11. 设置在收发数据完成后，发送结束信号
    I2C_Stop();

    // 9. 读取一个字节的数据
    uint8_t byte = I2C_ReadByte();

    return byte;
}

// 连续写入多个字节（页写）
void M24C02_WriteBytes(uint8_t innerAddr, uint8_t *bytes, uint8_t size)
{
    // 1. 发出开始信号
    I2C_Start();

    // 2. 发送写地址
    I2C_SendAddr(W_ADDR);

    // 4. 发送EEPROM内部地址
    I2C_SendByte(innerAddr);

    for (uint8_t i = 0; i < size; i++)
    {
        // 6. 循环发送具体数据
        I2C_SendByte(bytes[i]);
    }

    // 8. 发送结束信号
    I2C_Stop();

    // 加入延迟，等待写入EEPROM完成
    Delay_ms(5);
}

// 连续读取多个字节
void M24C02_ReadBytes(uint8_t innerAddr, uint8_t *bytes, uint8_t size)
{
    // 1. 发出开始信号
    I2C_Start();

    // 2. 发送写地址（假写）
    I2C_SendAddr(W_ADDR);

    // 4. 发送EEPROM内部地址
    I2C_SendByte(innerAddr);

    // 6. 再次发送开始信号
    I2C_Start();

    // 7. 发送读地址（真读）
    I2C_SendAddr(R_ADDR);

    for (uint8_t i = 0; i < size; i++)
    {
        // 10. 如果没有结束，设置发送一个应答；如果是最后一个字节，设置发送非应答
        if (i < size - 1)
        {
            I2C_Ack();
        }
        else
        {
            I2C_NAck();

            // 设置发送停止信号
            I2C_Stop();
        }

        // 9. 循环读取一个字节的数据
        bytes[i] = I2C_ReadByte();
    }
    // // 11. 发送结束信号
    // I2C_Stop();
}
