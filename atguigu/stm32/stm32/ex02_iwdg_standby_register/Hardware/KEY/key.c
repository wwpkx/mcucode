/*
 * @Author: wushengran
 * @Date: 2024-09-19 15:17:44
 * @Description: 
 * 
 * Copyright (c) 2024 by atguigu, All Rights Reserved. 
 */
#include "key.h"
#include "delay.h"
#include "led.h"

// ��ʼ��
void KEY_Init(void)
{
    // 1. ����ʱ��
    RCC->APB2ENR |= RCC_APB2ENR_IOPFEN;
    RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;

    // 2. PF10 ����ģʽ���������룬CNF = 10�� MODE = 00
    GPIOF->CRH &= ~GPIO_CRH_MODE10;
    GPIOF->CRH |= GPIO_CRH_CNF10_1;
    GPIOF->CRH &= ~GPIO_CRH_CNF10_0;

    GPIOF->ODR &= ~GPIO_ODR_ODR10;

    // 3. AFIO ���Ÿ������� EXTI10 - PF10
    AFIO->EXTICR[2] |= AFIO_EXTICR3_EXTI10_PF;

    // 4. EXTI����
    // 4.1 ���������ش���
    EXTI->RTSR |= EXTI_RTSR_TR10;
    // 4.2 ������λ
    EXTI->IMR |= EXTI_IMR_MR10;

    // 5. NVIC����
    // 5.1 �������ȼ��鷽��
    NVIC_SetPriorityGrouping(3);    // ȫ��4λ������ռ���ȼ�
    NVIC_SetPriority(EXTI15_10_IRQn, 2);
    NVIC_EnableIRQ(EXTI15_10_IRQn);
}

// extern uint8_t flag;

// �жϴ������
void EXTI15_10_IRQHandler(void)
{
    // �������λ��PR ��1��
    EXTI->PR |= EXTI_PR_PR10;

    // flag = 1;
    // printf("��������...\n");
}
