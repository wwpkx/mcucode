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
    RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;

    // 2. GPIO����ģʽ
    // 2.1 CS - PC13��ͨ�����������CNF = 00��MODE = 11
    GPIOC->CRH |= GPIO_CRH_MODE13;
    GPIOC->CRH &= ~GPIO_CRH_CNF13;

    // 2.2 SCK - PA5���������������CNF = 10��MODE = 11
    GPIOA->CRL |= GPIO_CRL_MODE5;
    GPIOA->CRL |= GPIO_CRL_CNF5_1;
    GPIOA->CRL &= ~GPIO_CRL_CNF5_0;

    // 2.3 MOSI - PA7���������������CNF = 10��MODE = 11
    GPIOA->CRL |= GPIO_CRL_MODE7;
    GPIOA->CRL |= GPIO_CRL_CNF7_1;
    GPIOA->CRL &= ~GPIO_CRL_CNF7_0;

    // 2.4 MISO - PA6���������룬CNF = 01��MODE = 00
    GPIOA->CRL &= ~GPIO_CRL_MODE6;
    GPIOA->CRL &= ~GPIO_CRL_CNF6_1;
    GPIOA->CRL |= GPIO_CRL_CNF6_0;

    // 3. SPIģ������
    // 3.1 ����ģʽ�������
    // 3.1.1 ����Ϊ��ģʽ
    SPI1->CR1 |= SPI_CR1_MSTR;

    // 3.1.2 ѡ���������Ƭѡ��������Ϊ�ߵ�ƽ
    SPI1->CR1 |= SPI_CR1_SSM;
    SPI1->CR1 |= SPI_CR1_SSI;

    // 3.1.2 SPI����ģʽ����Ϊģʽ0��CPOL = 0��CPHA = 0
    SPI1->CR1 &= ~(SPI_CR1_CPOL | SPI_CR1_CPHA);

    // 3.2 ���ò����ʣ�ʱ�ӷ�Ƶϵ������001 - 4��Ƶ��18MHz
    SPI1->CR1 &= ~SPI_CR1_BR;
    SPI1->CR1 |= SPI_CR1_BR_0;

    // 3.3 ����֡��ʽ�������
    // 3.3.1 ����֡���ȣ�0 - 8λ
    SPI1->CR1 &= ~SPI_CR1_DFF;

    // 3.3.2 ���ݴ���˳��0 - MSB����
    SPI1->CR1 &= ~SPI_CR1_LSBFIRST;

    // 3.4 ����SPIʹ��
    SPI1->CR1 |= SPI_CR1_SPE;
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
    // 1. ��Ҫ���͵�����д�뷢�ͻ�����
    // 1.1 �ȴ����ͻ�����Ϊ�գ�TXE = 1
    while ((SPI1->SR & SPI_SR_TXE) == 0 )
    {}
    
    // 1.2 ������д��DR
    SPI1->DR = byte;

    // 2. ��ȡ���յ����ݲ�����
    // 2.1 �ȴ����ջ������ǿգ�RXNE = 1
    while ( (SPI1->SR & SPI_SR_RXNE) == 0 )
    {}
    
    // 2.2 ��DR�����ݷ���
    return (uint8_t)(SPI1->DR & 0xff);
}
