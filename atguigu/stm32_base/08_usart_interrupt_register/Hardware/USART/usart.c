/*
 * @Author: wushengran
 * @Date: 2024-09-20 16:23:02
 * @Description:
 *
 * Copyright (c) 2024 by atguigu, All Rights Reserved.
 */
#include "usart.h"

// ��ʼ��
void USART_Init(void)
{
    // 1. ����ʱ��
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;

    // 2. GPIO ����ģʽ
    // 2.1 PA9 - TX���������������CNF = 10��MODE = 11
    GPIOA->CRH |= GPIO_CRH_MODE9;
    GPIOA->CRH |= GPIO_CRH_CNF9_1;
    GPIOA->CRH &= ~GPIO_CRH_CNF9_0;
    // 2.2 PA10 - RX���������룬CNF = 01��MODE = 00
    GPIOA->CRH &= ~GPIO_CRH_MODE10;
    GPIOA->CRH &= ~GPIO_CRH_CNF10_1;
    GPIOA->CRH |= GPIO_CRH_CNF10_0;

    // 3. ����ģ������
    // 3.1 ���ò����� 115200
    USART1->BRR = 0x271;

    // 3.2 ʹ�ܴ��ں��շ�ģ��
    USART1->CR1 |= USART_CR1_UE;
    USART1->CR1 |= (USART_CR1_TE | USART_CR1_RE);

    // 3.3 ��������֡�ĸ�ʽ
    USART1->CR1 &= ~USART_CR1_M;    // ����Ϊ 8 λ
    USART1->CR1 &= ~USART_CR1_PCE;  // ��ʹ��У��λ
    USART1->CR2 &= ~USART_CR2_STOP; // 1 λֹͣλ

    // 3.4 ʹ�ܴ��ڽ����ж�
    USART1->CR1 |= USART_CR1_RXNEIE;
    USART1->CR1 |= USART_CR1_IDLEIE;

    // 4. ����NVIC
    NVIC_SetPriorityGrouping(3);
    NVIC_SetPriority(USART1_IRQn, 2);
    NVIC_EnableIRQ(USART1_IRQn);
}

// ����һ���ַ�
void USART_SendChar(uint8_t ch)
{
    // �ж� TDR �Ƿ�Ϊ�գ�����ȴ� TDR Ϊ�ղ��ܼ�������
    while ((USART1->SR & USART_SR_TXE) == 0)
    {
    }

    // ��Ҫ���͵�����д��TDR
    USART1->DR = ch;
}


// �����ַ���
void USART_SendString(uint8_t *str, uint8_t size)
{
    for (uint8_t i = 0; i < size; i++)
    {
        USART_SendChar(str[i]);
    }
}

// �����ⲿ����
extern uint8_t buff[100];
extern uint8_t size;
extern uint8_t isOver;

// �жϷ������
void USART1_IRQHandler(void)
{
    // �ж��� RXNE = 1 ���� IDLE = 1
    if ( USART1->SR & USART_SR_RXNE )
    {
        // ��� RXNE = 1����ʾ���յ�һ���ַ�
        buff[size] = USART1->DR;
        size ++;
    }
    else if ( USART1->SR & USART_SR_IDLE )
    {
        USART1->DR;
        // ��� IDLE = 1����ʾ��⵽����֡���ַ����������
        // USART_SendString(buff, size);
        // size = 0;

        isOver = 1;
    }
}
