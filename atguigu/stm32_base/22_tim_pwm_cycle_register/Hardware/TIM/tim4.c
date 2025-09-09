/*
 * @Author: wushengran
 * @Date: 2024-09-28 14:36:37
 * @Description: 
 * 
 * Copyright (c) 2024 by atguigu, All Rights Reserved. 
 */
#include "tim4.h"

// ��ʼ��
void TIM4_Init(void)
{
    // 1. ����ʱ��
    RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;

    // 2. ����GPIO����ģʽ���������룬CNF = 01��MODE = 00
    GPIOB->CRL &= ~GPIO_CRL_MODE6;
    GPIOB->CRL &= ~GPIO_CRL_CNF6_1;
    GPIOB->CRL |= GPIO_CRL_CNF6_0;

    // 3. ��ʱ����������������
    // 3.1 Ԥ��Ƶֵ 71���õ� 1MHz���������� 1us
    TIM4->PSC = 71;

    // 3.2 �Զ���װ��ֵ 65535��������֤�����
    TIM4->ARR = 65535;

    // 3.3 ��������Ĭ������
    TIM4->CR1 &= ~TIM_CR1_DIR;

    // 4. ��ʱ�����벶�񲿷�����
    // 4.1 TI1 ѡ�� CH1 ���ŵ�ֱ�����루Ĭ�ϣ�
    TIM4->CR2 &= ~TIM_CR2_TI1S;

    // 4.2 ��ʹ�������˲�����Ĭ�ϣ�
    TIM4->CCMR1 &= ~TIM_CCMR1_IC1F;

    // 4.3 ��������أ�Ĭ�ϼ��ԣ�
    TIM4->CCER &= ~TIM_CCER_CC1P;

    // 4.4 ��ͨ��1����Ϊ���룬������Ϊֱͨ�ź����루TI1 �� IC1����CC1S = 01
    TIM4->CCMR1 &= ~TIM_CCMR1_CC1S_1;
    TIM4->CCMR1 |= TIM_CCMR1_CC1S_0;

    // 4.5 ������Ԥ��Ƶ����Ĭ�ϣ�
    TIM4->CCMR1 &= ~TIM_CCMR1_IC1PSC;

    // 4.6 ����ͨ��1ʹ��
    TIM4->CCER |= TIM_CCER_CC1E;

    // 4.7 �������벶���ж�ʹ��
    TIM4->DIER |= TIM_DIER_CC1IE;

    // 5. NVIC����
    NVIC_SetPriorityGrouping(3);
    NVIC_SetPriority(TIM4_IRQn, 3);
    NVIC_EnableIRQ(TIM4_IRQn);
}

// ����
void TIM4_Start(void)
{
    TIM4->CR1 |= TIM_CR1_CEN;
}
void TIM4_Stop(void)
{
    TIM4->CR1 &= ~TIM_CR1_CEN;
}

// // ����ȫ�ֱ�������������жϵĴ����������ص����Ĵ�����
// uint8_t count = 0;
// // �����ȡ������ֵ
// uint16_t cycle = 0;

// // �жϷ������
// void TIM4_IRQHandler(void)
// {
//     // �ж��Ƿ���ͨ��1�Ĳ����ж�
//     if (TIM4->SR & TIM_SR_CC1IF)
//     {
//         // ������жϱ�־λ
//         TIM4->SR &= ~TIM_SR_CC1IF;

//         count ++;

//         // ���ݽ����жϵĴ�����ѡ��������������߲�������ֵ
//         if (count == 1)
//         {
//             // ����CNT�����¿�ʼ����
//             TIM4->CNT = 0;
//         }
//         else if (count == 2)
//         {
//             // ֱ�Ӷ�ȡCCR1��ֵ���������ڵ�΢����
//             cycle = TIM4->CCR1 + 1;
            
//             // ����count
//             count = 0;
//         } 
//     }
// }


// �Ľ����жϷ������
void TIM4_IRQHandler(void)
{
    // �ж��Ƿ���ͨ��1�Ĳ����ж�
    if (TIM4->SR & TIM_SR_CC1IF)
    {
        // ������жϱ�־λ
        TIM4->SR &= ~TIM_SR_CC1IF;

        // ֱ������CNT
        TIM4->CNT = 0;
    }
}

// ��ȡPWM���ڣ���λΪms
double TIM4_GetPWMCycle(void)
{
    return (TIM4->CCR1 + 1) / 1000.0;
}

// ��ȡPWMƵ�ʣ���λΪHz
double TIM4_GetPWMFreq(void)
{
    return 1000000.0 / (TIM4->CCR1 + 1);
}
