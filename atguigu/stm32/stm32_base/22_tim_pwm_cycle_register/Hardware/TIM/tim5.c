#include "tim5.h"

// ��ʼ��
void TIM5_Init(void)
{
    // 1. ����ʱ��
    RCC->APB1ENR |= RCC_APB1ENR_TIM5EN;
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;

    // 2. GPIO���ã��������������CNF = 10�� MODE = 11
    GPIOA->CRL |= GPIO_CRL_MODE1;
    GPIOA->CRL |= GPIO_CRL_CNF1_1;
    GPIOA->CRL &= ~GPIO_CRL_CNF1_0;

    // 3. ��ʱ��ʱ������
    // 3.1 Ԥ��Ƶֵ 7199���õ� 10000Hz���������� 0.1ms
    TIM5->PSC = 7199;

    // 3.2 �Զ���װ��ֵ 99��ÿ100�μ�������һ�������T = 10ms��f = 100Hz
    TIM5->ARR = 99;

    // 3.3 ��������Ĭ������
    TIM5->CR1 &= ~TIM_CR1_DIR;

    // 4. ��ʱ������Ƚϲ�������
    // 4.1 ��ͨ��2����Ϊ���ģʽ��CC2S = 00
    TIM5->CCMR1 &= ~TIM_CCMR1_CC2S;

    // 4.2 ����ͨ��2������Ƚ�ģʽ��OC2M = 110��PWMģʽ1
    TIM5->CCMR1 |= TIM_CCMR1_OC2M_2;
    TIM5->CCMR1 |= TIM_CCMR1_OC2M_1;
    TIM5->CCMR1 &= ~TIM_CCMR1_OC2M_0; 

    // 4.3 ����ͨ���ļ���
    TIM5->CCER &= ~TIM_CCER_CC2P;

    // 4.4 ʹ�����ͨ��
    TIM5->CCER |= TIM_CCER_CC2E;
}

// �������Ŀ����͹ر�
void TIM5_Start(void)
{
    TIM5->CR1 |= TIM_CR1_CEN;
}

void TIM5_Stop(void)
{
    TIM5->CR1 &= ~TIM_CR1_CEN;
}

// ����ռ�ձȣ�����ٷֱ�ֵ
void TIM5_SetDutyCycle(uint8_t duty)
{
    TIM5->CCR2 = duty;
}
