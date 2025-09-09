/*
 * @Author: wushengran
 * @Date: 2024-09-29 16:18:30
 * @Description: 
 * 
 * Copyright (c) 2024 by atguigu, All Rights Reserved. 
 */
#include "tim1.h"

// ��ʼ��
void TIM1_Init(void)
{
    // 1. ����ʱ��
    RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;

    // 2. ����GPIO����ģʽ��PA8 �������������CNF = 10��MODE = 11
    GPIOA->CRH |= GPIO_CRH_MODE8;
    GPIOA->CRH |= GPIO_CRH_CNF8_1;
    GPIOA->CRH &= ~GPIO_CRH_CNF8_0;

    // 3. ��ʱ��ʱ����������
    // 3.1 Ԥ��Ƶֵ 7199���õ� 10000Hz���������� 100us��0.1ms��
    TIM1->PSC = 7199;

    // 3.2 �Զ���װ��ֵ 4999��5000 �μ���֮����������������� 0.5s��Ƶ�� 2Hz
    TIM1->ARR = 4999;

    // 3.3 ��������
    TIM1->CR1 &= ~TIM_CR1_DIR;

    // 3.4 �ظ�����ֵ 4����ʾ 5 �����֮����������¼�
    TIM1->RCR = 4;

    // 4. ��ʱ������Ƚϲ�������
    // 4.1 ͨ��1����Ϊ���
    TIM1->CCMR1 &= ~TIM_CCMR1_CC1S;

    // 4.2 ����Ϊ PWMģʽ1��OC1M = 110
    TIM1->CCMR1 |= TIM_CCMR1_OC1M_2;
    TIM1->CCMR1 |= TIM_CCMR1_OC1M_1;
    TIM1->CCMR1 &= ~TIM_CCMR1_OC1M_0;

    // 4.3 ���� CCR ��ֵ��50% ռ�ձ�
    TIM1->CCR1 = 2500;

    // 4.4 ����ͨ��ʹ��
    TIM1->CCER |= TIM_CCER_CC1E;

    // ���ļ���
    TIM1->CCER |= TIM_CCER_CC1P;
    // ���ÿ���״̬��ƽΪ��
    TIM1->CR2 |= TIM_CR2_OIS1;

    // 4.5 ���������ʹ��
    TIM1->BDTR |= TIM_BDTR_MOE;

    // 4.6 ��ֹһ�����Ϳ��ٽ����жϣ��� UG λ���������¼��������� URS
    TIM1->CR1 |= TIM_CR1_URS;
    TIM1->EGR |= TIM_EGR_UG;

    // 4.7 ���������ж�
    TIM1->DIER |= TIM_DIER_UIE;

    // 5. NVIC����
    NVIC_SetPriorityGrouping(3);
    NVIC_SetPriority(TIM1_UP_IRQn, 3);
    NVIC_EnableIRQ(TIM1_UP_IRQn);
}

// ���ض�ʱ��
void TIM1_Start(void)
{
    TIM1->CR1 |= TIM_CR1_CEN;
}
void TIM1_Stop(void)
{
    TIM1->CR1 &= ~TIM_CR1_CEN;
}

// �жϴ������
void TIM1_UP_IRQHandler(void)
{
    // �����־λ
    TIM1->SR &= ~TIM_SR_UIF;

    // �ر���ʹ�ܣ��������ģʽ
    TIM1->BDTR &= ~TIM_BDTR_MOE;

    // ͣ����ʱ��
    TIM1_Stop();
}
