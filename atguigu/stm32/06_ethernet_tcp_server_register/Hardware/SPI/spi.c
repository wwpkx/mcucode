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
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
    RCC->APB2ENR |= RCC_APB2ENR_IOPDEN;
    RCC->APB1ENR |= RCC_APB1ENR_SPI2EN;

    // 2. GPIO����ģʽ
    // 2.1 CS - PD3��ͨ�����������CNF = 00��MODE = 11
    GPIOD->CRL |= GPIO_CRL_MODE3;
    GPIOD->CRL &= ~GPIO_CRL_CNF3;

    // 2.2 SCK - PB13���������������CNF = 10��MODE = 11
    GPIOB->CRH |= GPIO_CRH_MODE13;
    GPIOB->CRH |= GPIO_CRH_CNF13_1;
    GPIOB->CRH &= ~GPIO_CRH_CNF13_0;

    // 2.3 MOSI - PB15���������������CNF = 10��MODE = 11
    GPIOB->CRH |= GPIO_CRH_MODE15;
    GPIOB->CRH |= GPIO_CRH_CNF15_1;
    GPIOB->CRH &= ~GPIO_CRH_CNF15_0;

    // 2.4 MISO - PB14���������룬CNF = 01��MODE = 00
    GPIOB->CRH &= ~GPIO_CRH_MODE14;
    GPIOB->CRH &= ~GPIO_CRH_CNF14_1;
    GPIOB->CRH |= GPIO_CRH_CNF14_0;

    // 3. SPIģ������
    // 3.1 ����ģʽ�������
    // 3.1.1 ����Ϊ��ģʽ
    SPI2->CR1 |= SPI_CR1_MSTR;

    // 3.1.2 ѡ���������Ƭѡ��������Ϊ�ߵ�ƽ
    SPI2->CR1 |= SPI_CR1_SSM;
    SPI2->CR1 |= SPI_CR1_SSI;

    // 3.1.2 SPI����ģʽ����Ϊģʽ0��CPOL = 0��CPHA = 0
    SPI2->CR1 &= ~(SPI_CR1_CPOL | SPI_CR1_CPHA);

    // 3.2 ���ò����ʣ�ʱ�ӷ�Ƶϵ������000 - 2��Ƶ��18MHz
    SPI2->CR1 &= ~SPI_CR1_BR;
    // SPI2->CR1 |= SPI_CR1_BR_0;

    // 3.3 ����֡��ʽ�������
    // 3.3.1 ����֡���ȣ�0 - 8λ
    SPI2->CR1 &= ~SPI_CR1_DFF;

    // 3.3.2 ���ݴ���˳��0 - MSB����
    SPI2->CR1 &= ~SPI_CR1_LSBFIRST;

    // 3.4 ����SPIʹ��
    SPI2->CR1 |= SPI_CR1_SPE;
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
    while ((SPI2->SR & SPI_SR_TXE) == 0 )
    {}
    
    // 1.2 ������д��DR
    SPI2->DR = byte;

    // 2. ��ȡ���յ����ݲ�����
    // 2.1 �ȴ����ջ������ǿգ�RXNE = 1
    while ( (SPI2->SR & SPI_SR_RXNE) == 0 )
    {}
    
    // 2.2 ��DR�����ݷ���
    return (uint8_t)(SPI2->DR & 0xff);
}
