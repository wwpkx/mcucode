/*
 * @Author: wushengran
 * @Date: 2024-09-23 14:07:07
 * @Description:
 *
 * Copyright (c) 2024 by atguigu, All Rights Reserved.
 */
#include "m24c02.h"

// ��ʼ��
void M24C02_Init(void)
{
    MX_I2C2_Init();
}

// д��һ���ֽڣ�ָ��Ҫд����ڲ���ַ
void M24C02_WriteByte(uint8_t innerAddr, uint8_t byte)
{
    HAL_I2C_Mem_Write(&hi2c2, W_ADDR, innerAddr, I2C_MEMADD_SIZE_8BIT, &byte, 1, 1000);

    // �����ӳ٣��ȴ�д��EEPROM���
    HAL_Delay(5);
}

// ��ȡһ���ֽڣ������ַ����
uint8_t M24C02_ReadByte(uint8_t innerAddr)
{
    uint8_t byte;
    HAL_I2C_Mem_Read(&hi2c2, R_ADDR, innerAddr, I2C_MEMADD_SIZE_8BIT, &byte, 1, 1000);

    return byte;
}

// ����д�����ֽڣ�ҳд��
void M24C02_WriteBytes(uint8_t innerAddr, uint8_t *bytes, uint8_t size)
{
    HAL_I2C_Mem_Write(&hi2c2, W_ADDR, innerAddr, I2C_MEMADD_SIZE_8BIT, bytes, size, 1000);

    // �����ӳ٣��ȴ�д��EEPROM���
    HAL_Delay(5);
}

// ������ȡ����ֽ�
void M24C02_ReadBytes(uint8_t innerAddr, uint8_t *bytes, uint8_t size)
{
    HAL_I2C_Mem_Read(&hi2c2, R_ADDR, innerAddr, I2C_MEMADD_SIZE_8BIT, bytes, size, 1000);
}
