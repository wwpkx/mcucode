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
    RCC->APB1ENR |= RCC_APB1ENR_I2C2EN;

    // 2. GPIO���ã�PB10��PB11 ���ÿ�©�����MODE - 11�� CNF - 11
    GPIOB->CRH |= (GPIO_CRH_MODE10 | GPIO_CRH_MODE11 | GPIO_CRH_CNF10 | GPIO_CRH_CNF11);

    // 3. I2C ����
    // 3.1 Ӳ������ģʽ����
    I2C2->CR1 &= ~I2C_CR1_SMBUS; // I2Cģʽ
    I2C2->CCR &= ~I2C_CCR_FS;    // ��׼ģʽ

    // 3.2 ʱ��Ƶ�� 36MHz
    I2C2->CR2 |= 36 << 0;

    // 3.3 �ߵ�ƽʱ�䣺5us - 180��ʱ�����ڣ�100kbps
    I2C2->CCR |= 180 << 0;

    // 3.4 ������ʱ�� 1us - 36��ʱ������
    I2C2->TRISE |= 37;

    // 3.5 ʹ��I2Cģ��
    I2C2->CR1 |= I2C_CR1_PE;
}

// ����������ʼ�ź�
uint8_t I2C_Start(void)
{
    // ������ʼ�ź�
    I2C2->CR1 |= I2C_CR1_START;

    // �ȴ���ʼ�źŷ���
    uint16_t timeout = 0xffff; // ��ʱʱ��
    while ((I2C2->SR1 & I2C_SR1_SB) == 0 && timeout)
    {
        timeout--;
    }
    return timeout ? OK : FAIL;
}

// ���������շ�������֮�󷢳�ֹͣ�ź�
void I2C_Stop(void)
{
    I2C2->CR1 |= I2C_CR1_STOP;
}

// �������÷���Ӧ��/��Ӧ���ź�
void I2C_Ack(void)
{
    I2C2->CR1 |= I2C_CR1_ACK;
}
void I2C_NAck(void)
{
    I2C2->CR1 &= ~I2C_CR1_ACK;
}

// ���������豸��ַ
uint8_t I2C_SendAddr(uint8_t addr)
{
    // ��Ҫ���͵ĵ�ַд��DR���Զ�����
    I2C2->DR = addr;

    // �ȴ���ַ�ɹ��������ȴ�Ӧ��
    uint16_t timeout = 0xffff; // ��ʱʱ��
    while ((I2C2->SR1 & I2C_SR1_ADDR) == 0 && timeout)
    {
        timeout--;
    }

    I2C2->SR2; // ��� ADDR ��־λ

    return timeout ? OK : FAIL;
}

// ��������һ���ֽ�
uint8_t I2C_SendByte(uint8_t byte)
{
    // ��Ҫ���͵�����д��DR���Զ�����
    I2C2->DR = byte;

    // �ȴ��ֽڷ�����ɣ��ȴ�Ӧ��
    uint16_t timeout = 0xffff; // ��ʱʱ��
    while ((I2C2->SR1 & I2C_SR1_BTF) == 0 && timeout)
    {
        timeout--;
    }

    I2C2->DR; // ��� BTF ��־λ

    return timeout ? OK : FAIL;
}

// ������SDA��ȡһ���ֽ�
uint8_t I2C_ReadByte(void)
{
    // �ȴ��ֽڽ������
    uint16_t timeout = 0xffff; // ��ʱʱ��
    while ((I2C2->SR1 & I2C_SR1_RXNE) == 0 && timeout)
    {
        timeout--;
    }

    return timeout ? I2C2->DR : FAIL;
}
