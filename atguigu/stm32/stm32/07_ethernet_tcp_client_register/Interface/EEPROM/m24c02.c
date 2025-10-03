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
    I2C_SendByte(W_ADDR);

    // 3. 等待应答
    uint8_t ack = I2C_Wait4Ack();

    if (ack == ACK)
    {
        // 4. 发送EEPROM内部地址
        I2C_SendByte(innerAddr);

        // 5. 等待应答
        I2C_Wait4Ack();

        // 6. 发送具体数据
        I2C_SendByte(byte);

        // 7. 等待应答
        I2C_Wait4Ack();

        // 8. 发送结束信号
        I2C_Stop();
    }

    // 加入延迟，等待写入EEPROM完成
    Delay_ms(5);
}

// 读取一个字节（随机地址读）
uint8_t M24C02_ReadByte(uint8_t innerAddr)
{
    // 1. 发出开始信号
    I2C_Start();

    // 2. 发送写地址（假写）
    I2C_SendByte(W_ADDR);

    // 3. 等待应答
    I2C_Wait4Ack();

    // 4. 发送EEPROM内部地址
    I2C_SendByte(innerAddr);

    // 5. 等待应答
    I2C_Wait4Ack();

    // 6. 再次发送开始信号
    I2C_Start();

    // 7. 发送读地址（真读）
    I2C_SendByte(R_ADDR);

    // 8. 等待应答
    I2C_Wait4Ack();

    // 9. 读取一个字节的数据
    uint8_t byte = I2C_ReadByte();

    // 10. 发送一个非应答
    I2C_NAck();

    // 11. 发送结束信号
    I2C_Stop();

    return byte;
}

// 连续写入多个字节（页写）
void M24C02_WriteBytes(uint8_t innerAddr, uint8_t *bytes, uint8_t size)
{
    // 1. 发出开始信号
    I2C_Start();

    // 2. 发送写地址
    I2C_SendByte(W_ADDR);

    // 3. 等待应答
    uint8_t ack = I2C_Wait4Ack();

    if (ack == ACK)
    {
        // 4. 发送EEPROM内部地址
        I2C_SendByte(innerAddr);

        // 5. 等待应答
        I2C_Wait4Ack();

        for (uint8_t i = 0; i < size; i++)
        {
            // 6. 循环发送具体数据
            I2C_SendByte(bytes[i]);

            // 7. 等待应答
            I2C_Wait4Ack();
        }

        // 8. 发送结束信号
        I2C_Stop();
    }

    // 加入延迟，等待写入EEPROM完成
    Delay_ms(5);
}

// 连续读取多个字节
void M24C02_ReadBytes(uint8_t innerAddr, uint8_t *bytes, uint8_t size)
{
    // 1. 发出开始信号
    I2C_Start();

    // 2. 发送写地址（假写）
    I2C_SendByte(W_ADDR);

    // 3. 等待应答
    I2C_Wait4Ack();

    // 4. 发送EEPROM内部地址
    I2C_SendByte(innerAddr);

    // 5. 等待应答
    I2C_Wait4Ack();

    // 6. 再次发送开始信号
    I2C_Start();

    // 7. 发送读地址（真读）
    I2C_SendByte(R_ADDR);

    // 8. 等待应答
    I2C_Wait4Ack();

    for (uint8_t i = 0; i < size; i++)
    {
        // 9. 循环读取一个字节的数据
        bytes[i] = I2C_ReadByte();

        // 10. 如果没有结束，发送一个应答；如果是最后一个字节，发送非应答
        if (i < size - 1)
        {
            I2C_Ack();
        }
        else
        {
            I2C_NAck();
        }
    }
    // 11. 发送结束信号
    I2C_Stop();
}
