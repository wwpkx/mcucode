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
    MX_I2C2_Init();
}

// 写入一个字节，指定要写入的内部地址
void M24C02_WriteByte(uint8_t innerAddr, uint8_t byte)
{
    HAL_I2C_Mem_Write(&hi2c2, W_ADDR, innerAddr, I2C_MEMADD_SIZE_8BIT, &byte, 1, 1000);

    // 加入延迟，等待写入EEPROM完成
    HAL_Delay(5);
}

// 读取一个字节（随机地址读）
uint8_t M24C02_ReadByte(uint8_t innerAddr)
{
    uint8_t byte;
    HAL_I2C_Mem_Read(&hi2c2, R_ADDR, innerAddr, I2C_MEMADD_SIZE_8BIT, &byte, 1, 1000);

    return byte;
}

// 连续写入多个字节（页写）
void M24C02_WriteBytes(uint8_t innerAddr, uint8_t *bytes, uint8_t size)
{
    HAL_I2C_Mem_Write(&hi2c2, W_ADDR, innerAddr, I2C_MEMADD_SIZE_8BIT, bytes, size, 1000);

    // 加入延迟，等待写入EEPROM完成
    HAL_Delay(5);
}

// 连续读取多个字节
void M24C02_ReadBytes(uint8_t innerAddr, uint8_t *bytes, uint8_t size)
{
    HAL_I2C_Mem_Read(&hi2c2, R_ADDR, innerAddr, I2C_MEMADD_SIZE_8BIT, bytes, size, 1000);
}
