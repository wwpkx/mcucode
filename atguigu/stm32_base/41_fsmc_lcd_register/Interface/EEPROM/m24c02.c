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
    I2C_Init();
}

// д��һ���ֽڣ�ָ��Ҫд����ڲ���ַ
void M24C02_WriteByte(uint8_t innerAddr, uint8_t byte)
{
    // 1. ������ʼ�ź�
    I2C_Start();

    // 2. ����д��ַ
    I2C_SendAddr(W_ADDR);

    // 3. ����EEPROM�ڲ���ַ
    I2C_SendByte(innerAddr);

    // 4. ���;�������
    I2C_SendByte(byte);

    // 5. ���ͽ����ź�
    I2C_Stop();

    // �����ӳ٣��ȴ�д��EEPROM���
    Delay_ms(5);
}

// ��ȡһ���ֽڣ������ַ����
uint8_t M24C02_ReadByte(uint8_t innerAddr)
{
    // 1. ������ʼ�ź�
    I2C_Start();

    // 2. ����д��ַ����д��
    I2C_SendAddr(W_ADDR);

    // 4. ����EEPROM�ڲ���ַ
    I2C_SendByte(innerAddr);

    // 6. �ٴη��Ϳ�ʼ�ź�
    I2C_Start();

    // 7. ���Ͷ���ַ�������
    I2C_SendAddr(R_ADDR);

    // 10. ��ǰ���ã��ڽ��յ���һ�����ݺ󣬷���һ����Ӧ��
    I2C_NAck();

    // 11. �������շ�������ɺ󣬷��ͽ����ź�
    I2C_Stop();

    // 9. ��ȡһ���ֽڵ�����
    uint8_t byte = I2C_ReadByte();

    return byte;
}

// ����д�����ֽڣ�ҳд��
void M24C02_WriteBytes(uint8_t innerAddr, uint8_t *bytes, uint8_t size)
{
    // 1. ������ʼ�ź�
    I2C_Start();

    // 2. ����д��ַ
    I2C_SendAddr(W_ADDR);

    // 4. ����EEPROM�ڲ���ַ
    I2C_SendByte(innerAddr);

    for (uint8_t i = 0; i < size; i++)
    {
        // 6. ѭ�����;�������
        I2C_SendByte(bytes[i]);
    }

    // 8. ���ͽ����ź�
    I2C_Stop();

    // �����ӳ٣��ȴ�д��EEPROM���
    Delay_ms(5);
}

// ������ȡ����ֽ�
void M24C02_ReadBytes(uint8_t innerAddr, uint8_t *bytes, uint8_t size)
{
    // 1. ������ʼ�ź�
    I2C_Start();

    // 2. ����д��ַ����д��
    I2C_SendAddr(W_ADDR);

    // 4. ����EEPROM�ڲ���ַ
    I2C_SendByte(innerAddr);

    // 6. �ٴη��Ϳ�ʼ�ź�
    I2C_Start();

    // 7. ���Ͷ���ַ�������
    I2C_SendAddr(R_ADDR);

    for (uint8_t i = 0; i < size; i++)
    {
        // 10. ���û�н��������÷���һ��Ӧ����������һ���ֽڣ����÷��ͷ�Ӧ��
        if (i < size - 1)
        {
            I2C_Ack();
        }
        else
        {
            I2C_NAck();

            // ���÷���ֹͣ�ź�
            I2C_Stop();
        }

        // 9. ѭ����ȡһ���ֽڵ�����
        bytes[i] = I2C_ReadByte();
    }
    // // 11. ���ͽ����ź�
    // I2C_Stop();
}
