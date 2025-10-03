/*
 * @Author: wushengran
 * @Date: 2024-10-08 16:17:59
 * @Description: 
 * 
 * Copyright (c) 2024 by atguigu, All Rights Reserved. 
 */
#include "adc.h"

// ��ʼ��
void ADC1_Init(void)
{
    // 1. ʱ������
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
    // ADC��ʱ��Ƶ�ʲ��ܳ���14MHz����Ҫ��Ԥ��Ƶ��10 - 6��Ƶ��
    RCC->CFGR |= RCC_CFGR_ADCPRE_DIV6;

    // RCC->APB2ENR | RCC_APB2ENR_IOPCEN;

    // 2. GPIO����ģʽ��PC0 - ģ�����룬CNF = 00�� MODE = 00
    GPIOC->CRL &= ~(GPIO_CRL_MODE0 | GPIO_CRL_CNF0);

    // PC2 - ģ�����룬CNF = 00�� MODE = 00
    GPIOC->CRL &= ~(GPIO_CRL_MODE2 | GPIO_CRL_CNF2);

    // 3. ADC����
    // 3.1 ����ɨ��ģʽ
    ADC1->CR1 |= ADC_CR1_SCAN;

    // 3.2 ��������ת��ģʽ
    ADC1->CR2 |= ADC_CR2_CONT;

    // 3.3 ���ݶ��䷽ʽ��0 - �Ҷ���
    ADC1->CR2 &= ~ADC_CR2_ALIGN;

    // 3.4 ���ò���ʱ�䣬010 - 13.5��ADCʱ������
    ADC1->SMPR1 &= ~ADC_SMPR1_SMP10;
    ADC1->SMPR1 |= ADC_SMPR1_SMP10_1;
    ADC1->SMPR1 &= ~ADC_SMPR1_SMP12;

    // 3.5 ����ͨ��������
    // 3.5.1 ����������ͨ��������L = 0001��2��ͨ��
    ADC1->SQR1 &= ~ADC_SQR1_L;
    ADC1->SQR1 |= ADC_SQR1_L_0;

    // 3.5.2 ��ͨ����10д�� SQ1����SQR3�
    ADC1->SQR3 &= ~ADC_SQR3_SQ1;
    ADC1->SQR3 |= 10 << 0;      // ���5λ����һ��ͨ���ţ���Ϊ10

    // ��ͨ����12д�� SQ2����SQR3�
    ADC1->SQR3 &= ~ADC_SQR3_SQ2;
    ADC1->SQR3 |= 12 << 5;      // ����5λ���ڶ���ͨ���ţ���Ϊ12

    // 3.6 ѡ�����������ʽ��111
    // ADC1->CR2 |= ADC_CR2_EXTTRIG;
    // ADC1->CR2 |= ADC_CR2_EXTSEL;
}

// DMA��صĳ�ʼ��
void ADC1_DMA_Init(void)
{
    // 1. ����ʱ��
    RCC->AHBENR |= RCC_AHBENR_DMA1EN;

    // 2. DMA���䷽��DIR = 0���������ȡ
    DMA1_Channel1->CCR &= ~DMA_CCR1_DIR;

    // 3. ���ݿ�ȣ�01 - 16λ
    DMA1_Channel1->CCR &= ~DMA_CCR1_PSIZE_1;
    DMA1_Channel1->CCR |= DMA_CCR1_PSIZE_0;

    DMA1_Channel1->CCR &= ~DMA_CCR1_MSIZE_1;
    DMA1_Channel1->CCR |= DMA_CCR1_MSIZE_0;

    // 4. ��ַ�Ƿ����������費�����洢������
    DMA1_Channel1->CCR &= ~DMA_CCR1_PINC;
    DMA1_Channel1->CCR |= DMA_CCR1_MINC;

    // 5. ����ѭ��ģʽ
    DMA1_Channel1->CCR |= DMA_CCR1_CIRC;

    // 6. ADC1����DMA����
    ADC1->CR2 |= ADC_CR2_DMA;
}

// ����ADC������ת��
void ADC1_StartConvert_DMA(uint32_t destAddr, uint16_t len)
{
    // 0. DMA���ò�����ͨ��1
    DMA1_Channel1->CPAR = (uint32_t)&(ADC1->DR);
    DMA1_Channel1->CMAR = destAddr;
    DMA1_Channel1->CNDTR = len;

    DMA1_Channel1->CCR |= DMA_CCR1_EN;

    // 1. �ϵ磬��ADC�Ӷϵ�ģʽ����
    ADC1->CR2 |= ADC_CR2_ADON;

    // 2. ִ��У׼���ȴ����
    ADC1->CR2 |= ADC_CR2_CAL;
    while (ADC1->CR2 & ADC_CR2_CAL)
    {}

    // 3. �����ʽ����ADת��
    // ADC1->CR2 |= ADC_CR2_SWSTART;
    ADC1->CR2 |= ADC_CR2_ADON;
    
    // 4. �ȴ�ת�����
    while ((ADC1->SR & ADC_SR_EOC) == 0)
    {}
}
