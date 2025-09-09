#include "tim5.h"

// 初始化
void TIM5_Init(void)
{
    // 1. 开启时钟
    RCC->APB1ENR |= RCC_APB1ENR_TIM5EN;
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;

    // 2. GPIO配置，复用推挽输出，CNF = 10， MODE = 11
    GPIOA->CRL |= GPIO_CRL_MODE1;
    GPIOA->CRL |= GPIO_CRL_CNF1_1;
    GPIOA->CRL &= ~GPIO_CRL_CNF1_0;

    // 3. 定时器时基配置
    // 3.1 预分频值 7199，得到 10000Hz，计数周期 0.1ms
    TIM5->PSC = 7199;

    // 3.2 自动重装载值 99，每100次计数产生一次溢出，T = 10ms，f = 100Hz
    TIM5->ARR = 99;

    // 3.3 计数方向，默认向上
    TIM5->CR1 &= ~TIM_CR1_DIR;

    // 4. 定时器输出比较部分配置
    // 4.1 将通道2配置为输出模式，CC2S = 00
    TIM5->CCMR1 &= ~TIM_CCMR1_CC2S;

    // 4.2 配置通道2的输出比较模式：OC2M = 110，PWM模式1
    TIM5->CCMR1 |= TIM_CCMR1_OC2M_2;
    TIM5->CCMR1 |= TIM_CCMR1_OC2M_1;
    TIM5->CCMR1 &= ~TIM_CCMR1_OC2M_0; 

    // 4.3 配置通道的极性
    TIM5->CCER &= ~TIM_CCER_CC2P;

    // 4.4 使能输出通道
    TIM5->CCER |= TIM_CCER_CC2E;
}

// 计数器的开启和关闭
void TIM5_Start(void)
{
    TIM5->CR1 |= TIM_CR1_CEN;
}

void TIM5_Stop(void)
{
    TIM5->CR1 &= ~TIM_CR1_CEN;
}

// 设置占空比，传入百分比值
void TIM5_SetDutyCycle(uint8_t duty)
{
    TIM5->CCR2 = duty;
}
