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

    // 3. ADC����
    // 3.1 ����ɨ��ģʽ��ֻ��1��ת��ͨ����
    ADC1->CR1 &= ~ADC_CR1_SCAN;

    // 3.2 ��������ת��ģʽ
    ADC1->CR2 |= ADC_CR2_CONT;

    // 3.3 ���ݶ��䷽ʽ��0 - �Ҷ���
    ADC1->CR2 &= ~ADC_CR2_ALIGN;

    // 3.4 ���ò���ʱ�䣬010 - 13.5��ADCʱ������
    ADC1->SMPR1 &= ~ADC_SMPR1_SMP10;
    ADC1->SMPR1 |= ADC_SMPR1_SMP10_1;

    // 3.5 ����ͨ��������
    // 3.5.1 ����������ͨ��������L = 0000��1��ͨ��
    ADC1->SQR1 &= ~ADC_SQR1_L;

    // 3.5.2 ��ͨ����10д�� SQ1����SQR3�
    ADC1->SQR3 &= ~ADC_SQR3_SQ1;
    ADC1->SQR3 |= 10 << 0;      // ���5λ����һ��ͨ���ţ���Ϊ10

    // 3.6 ѡ�����������ʽ��111
    // ADC1->CR2 |= ADC_CR2_EXTTRIG;
    // ADC1->CR2 |= ADC_CR2_EXTSEL;
}

// ����ADC������ת��
void ADC1_StartConvert(void)
{
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

// ����ת����ĵ�ѹֵ
double ADC1_ReadV(void)
{
    return ADC1->DR * 3.3 / 4095;
}
