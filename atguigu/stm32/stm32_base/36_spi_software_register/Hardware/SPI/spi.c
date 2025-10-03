/*
 * @Author: wushengran
 * @Date: 2024-10-09 16:17:30
 * @Description: 
 * 
 * Copyright (c) 2024 by atguigu, All Rights Reserved. 
 */
#include "spi.h"

// ��ʼ��
void SPI_Init(void)
{
    // 1. ����ʱ��
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
    RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;

    // 2. GPIO����ģʽ
    // 2.1 CS - PC13��ͨ�����������CNF = 00��MODE = 11
    GPIOC->CRH |= GPIO_CRH_MODE13;
    GPIOC->CRH &= ~GPIO_CRH_CNF13;

    // 2.2 SCK - PA5��ͨ�����������CNF = 00��MODE = 11
    GPIOA->CRL |= GPIO_CRL_MODE5;
    GPIOA->CRL &= ~GPIO_CRL_CNF5;

    // 2.3 MOSI - PA7��ͨ�����������CNF = 00��MODE = 11
    GPIOA->CRL |= GPIO_CRL_MODE7;
    GPIOA->CRL &= ~GPIO_CRL_CNF7;

    // 2.4 MISO - PA6���������룬CNF = 01��MODE = 00
    GPIOA->CRL &= ~GPIO_CRL_MODE6;
    GPIOA->CRL &= ~GPIO_CRL_CNF6_1;
    GPIOA->CRL |= GPIO_CRL_CNF6_0;

    // 3. SCK ���ֿ���״̬��ģʽ0 - �͵�ƽ����
    SCK_LOW;

    // 4. Ƭѡ��ʼ��ѡ��
    CS_HIGH;

    // 5. ��ʱ
    SPI_DELAY;
}

// SPIͨ�ŵĿ����͹رգ�ͨ��Ƭѡ�źſ��ƣ�
void SPI_Start(void)
{
    CS_LOW;
}
void SPI_Stop(void)
{
    CS_HIGH;
}

// һ��ʱ�������ڣ����ӽ���һ���ֽ�����
uint8_t SPI_SwapByte(uint8_t byte)
{
    // ���������������
    uint8_t rByte = 0x00;

    // ��ѭ��һ�ζ�дÿһλ
    for (uint8_t i = 0; i < 8; i++)
    {
        // 1. �жϵ�ǰ���λ����MOSI������Ӧ��ƽ
        if ( byte & 0x80 )
        {
            MOSI_HIGH;
        }
        else
        {
            MOSI_LOW;
        }

        // 2. ����һλ
        byte <<= 1;
        
        // 3. ����ʱ�ӣ��ڵ�һ�������ز�������
        SCK_HIGH;
        SPI_DELAY;

        // 4. �����ƣ��ճ����λ����Ŷ�ȡ������
        rByte <<= 1;

        // 5. �� MISO �ϲ�����ȡ����
        if ( MISO_READ )
        {
            rByte |= 0x01;  // �����ߵ�ƽ�����һλ��1
        }

        // 6. ����ʱ�ӣ�Ϊ�´δ�����׼��
        SCK_LOW;
        SPI_DELAY;
    }
    
    return rByte;
}
