/*
 * @Author: wushengran
 * @Date: 2024-09-27 09:29:04
 * @Description: 
 * 
 * Copyright (c) 2024 by atguigu, All Rights Reserved. 
 */
#include "tim6.h"

void TIM6_Init(void)
{
    // 1. ����ʱ��
    RCC->APB1ENR |= RCC_APB1ENR_TIM6EN;

    // 2. ����Ԥ��Ƶֵ 7199����ʾ7200��Ƶ���õ�10000Hz��100us��
    TIM6->PSC = 7199;

    // 3. �����Զ���װ��ֵ 9999����ʾ 10000 �μ������õ� 1s
    TIM6->ARR = 99;

    // 4. �򿪸����ж�ʹ��
    TIM6->DIER |= TIM_DIER_UIE;

    // 5. NVIC����
    NVIC_SetPriorityGrouping(3);
    NVIC_SetPriority(TIM6_IRQn, 2);
    NVIC_EnableIRQ(TIM6_IRQn);

    // 6. ʹ�ܼ�����
    TIM6->CR1 = TIM_CR1_CEN;
}

// �жϷ������
void TIM6_IRQHandler(void)
{
    // ����жϱ�־λ
    TIM6->SR &= ~TIM_SR_UIF;

    // ÿ1s��תһ��LED2
    LED_Toggle(LED_2);
}
