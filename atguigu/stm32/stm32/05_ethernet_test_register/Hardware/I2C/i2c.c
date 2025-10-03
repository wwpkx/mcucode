/*
 * @Author: wushengran
 * @Date: 2024-09-23 14:01:27
 * @Description:
 *
 * Copyright (c) 2024 by atguigu, All Rights Reserved.
 */
#include "i2c.h"

// ��ʼ��
void I2C_Init(void)
{
    // 1. ����ʱ��
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;

    // 2. GPIO���ã�PB10��PB11 ͨ�ÿ�©�����MODE - 11�� CNF - 01
    GPIOB->CRH |= (GPIO_CRH_MODE10 | GPIO_CRH_MODE11);
    GPIOB->CRH &= ~(GPIO_CRH_CNF10_1 | GPIO_CRH_CNF11_1);
    GPIOB->CRH |= (GPIO_CRH_CNF10_0 | GPIO_CRH_CNF11_0);
}

// ����������ʼ�ź�
void I2C_Start(void)
{
    // 1. ����SCL��SDA
    SCL_HIGH;
    SDA_HIGH;
    I2C_DELAY;

    // 2. ����SDA�������½���
    SDA_LOW;
    I2C_DELAY;
}

// ��������ֹͣ�ź�
void I2C_Stop(void)
{
    // 1. ����SCL������SDA
    SCL_HIGH;
    SDA_LOW;
    I2C_DELAY;

    // 2. ����SDA������������
    SDA_HIGH;
    I2C_DELAY;
}

// ��������Ӧ��/��Ӧ���ź�
void I2C_Ack(void)
{
    // 1. ����SDA������SCL��׼����תSDA�ź�
    SDA_HIGH;
    SCL_LOW;
    I2C_DELAY;

    // 2. ����SDA����ת���ݣ����Ӧ���ź�
    SDA_LOW;
    I2C_DELAY;

    // 3. ����SCL������SDA�źţ������ݲ���
    SCL_HIGH;
    I2C_DELAY;

    // 4. ����SCL�����ݲ�������
    SCL_LOW;
    I2C_DELAY;

    // 5. ����SDA��Ӧ������������ͷ�SDA����
    SDA_HIGH;
    I2C_DELAY;
}
void I2C_NAck(void)
{
    // 1. ����SDA������SCL��׼��������Ӧ�ź�
    SDA_HIGH;
    SCL_LOW;
    I2C_DELAY;

    // 2. ����SCL������SDA�źţ������ݲ�������ǰ����û�з�ת����Ȼ�Ǹ�
    SCL_HIGH;
    I2C_DELAY;

    // 3. ����SCL�����ݲ�������
    SCL_LOW;
    I2C_DELAY;
}

// ������SDA��ȡ��Ӧ�źţ��ȴ�Ӧ��
uint8_t I2C_Wait4Ack(void)
{
    // 1. ����SDA������SCL���ͷ�SDA����
    SDA_HIGH;
    SCL_LOW;
    I2C_DELAY;

    // 2. ����SCL������SDA�źţ������ݲ���
    SCL_HIGH;
    I2C_DELAY;

    // 3. ��ȡSDA�ϵĵ�ƽ
    uint8_t ack = READ_SDA;

    // 4. ����SCL�����ݲ�������
    SCL_LOW;
    I2C_DELAY;

    return ack ? NACK : ACK;
}

// ��������һ���ֽ�
void I2C_SendByte(uint8_t byte)
{
    for (uint8_t i = 0; i < 8; i++)
    {
        // 1. ����SCL���ȴ����ܵķ�תSDA�ź�
        SCL_LOW;
        I2C_DELAY;

        // 2. ��Ҫ���͵�λ����SDA�ϣ�MSB����
        if (byte & 0x80)
        {
            SDA_HIGH;
        }
        else
        {
            SDA_LOW;
        }
        I2C_DELAY;

        // 3. ����SCL������SDA�źţ��ô��豸�����ݲ���
        SCL_HIGH;
        I2C_DELAY;

        // 4. ����SCL�����ݲ�������
        SCL_LOW;
        I2C_DELAY;

        // 5. ����1λ��Ϊ��һ�η�����׼��
        byte <<= 1;
    }
}

// ������SDA��ȡһ���ֽ�
uint8_t I2C_ReadByte(void)
{
    // ������������������ȡ���ֽ�
    uint8_t data = 0;

    for (uint8_t i = 0; i < 8; i++)
    {
        // 1. ����SDA������SCL���ͷ�SDA����
        SDA_HIGH;
        SCL_LOW;
        I2C_DELAY;

        // 2. ����SCL������SDA�źţ������ݲ���
        SCL_HIGH;
        I2C_DELAY;

        // 3. ������1λ��Ϊ��һ�ν�����׼��
        data <<= 1;

        // 4. ��ȡSDA�ϵĵ�ƽ���ŵ����ݵ����λ
        if(READ_SDA)
        {
            data |= 0x01;
        }

        // 5. ����SCL�����ݲ�������
        SCL_LOW;
        I2C_DELAY;
    }

    return data;
}
