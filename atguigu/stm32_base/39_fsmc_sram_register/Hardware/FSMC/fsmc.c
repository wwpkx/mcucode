/*
 * @Author: wushengran
 * @Date: 2024-10-13 09:07:14
 * @Description:
 *
 * Copyright (c) 2024 by atguigu, All Rights Reserved.
 */
#include "fsmc.h"

void FSMC_GPIO_Init(void);

// 初始化
void FSMC_Init(void)
{
    // 1. 开启时钟
    RCC->AHBENR |= RCC_AHBENR_FSMCEN;

    RCC->APB2ENR |= (RCC_APB2ENR_IOPDEN | RCC_APB2ENR_IOPEEN |
                     RCC_APB2ENR_IOPFEN | RCC_APB2ENR_IOPGEN);

    // 2. GPIO工作模式配置
    FSMC_GPIO_Init();

    // 3. FSMC寄存器配置
    // 3.1 BCR3 - BTCR[4]
    // 3.1.1 存储块使能
    FSMC_Bank1->BTCR[4] |= FSMC_BCR3_MBKEN;

    // 3.1.2 设置存储器类型，MTYP = 00，SRAM/ROM
    FSMC_Bank1->BTCR[4] &= ~FSMC_BCR3_MTYP;

    // 3.1.3 禁止Flash访问
    FSMC_Bank1->BTCR[4] &= ~FSMC_BCR3_FACCEN;

    // 3.1.4 地址数据复用功能：不开启
    FSMC_Bank1->BTCR[4] &= ~FSMC_BCR3_MUXEN;

    // 3.1.5 数据总线宽度，MWID = 01，16位
    FSMC_Bank1->BTCR[4] &= ~FSMC_BCR3_MWID_1;
    FSMC_Bank1->BTCR[4] |= FSMC_BCR3_MWID_0;

    // 3.1.6 开启写使能
    FSMC_Bank1->BTCR[4] |= FSMC_BCR3_WREN;

    // 3.2 BTR - BTCR[5]
    // 3.2.1 地址建立时间 ADDSET
    FSMC_Bank1->BTCR[5] &= ~FSMC_BTR3_ADDSET;

    // 3.2.2 数据建立时间 DATAST
    FSMC_Bank1->BTCR[5] &= ~FSMC_BTR3_DATAST;
    FSMC_Bank1->BTCR[5] |= (71 << 8);
}

// 所有GPIO引脚，配置为复用推挽输出，CNF = 10，MODE = 11
void FSMC_GPIO_Init(void)
{
    // 1. 地址线 A0 ~ A18
    // MODE = 11
    GPIOF->CRL |= (GPIO_CRL_MODE0 |
                   GPIO_CRL_MODE1 |
                   GPIO_CRL_MODE2 |
                   GPIO_CRL_MODE3 |
                   GPIO_CRL_MODE4 |
                   GPIO_CRL_MODE5);

    GPIOF->CRH |= (GPIO_CRH_MODE12 |
                   GPIO_CRH_MODE13 |
                   GPIO_CRH_MODE14 |
                   GPIO_CRH_MODE15);

    GPIOG->CRL |= (GPIO_CRL_MODE0 |
                   GPIO_CRL_MODE1 |
                   GPIO_CRL_MODE2 |
                   GPIO_CRL_MODE3 |
                   GPIO_CRL_MODE4 |
                   GPIO_CRL_MODE5);

    GPIOD->CRH |= (GPIO_CRH_MODE11 |
                   GPIO_CRH_MODE12 |
                   GPIO_CRH_MODE13);

    // CNF = 10
    GPIOF->CRL |= (GPIO_CRL_CNF0_1 |
                   GPIO_CRL_CNF1_1 |
                   GPIO_CRL_CNF2_1 |
                   GPIO_CRL_CNF3_1 |
                   GPIO_CRL_CNF4_1 |
                   GPIO_CRL_CNF5_1);
    GPIOF->CRL &= ~(
        GPIO_CRL_CNF0_0 |
        GPIO_CRL_CNF1_0 |
        GPIO_CRL_CNF2_0 |
        GPIO_CRL_CNF3_0 |
        GPIO_CRL_CNF4_0 |
        GPIO_CRL_CNF5_0);

    GPIOF->CRH |= (GPIO_CRH_CNF12_1 |
                   GPIO_CRH_CNF13_1 |
                   GPIO_CRH_CNF14_1 |
                   GPIO_CRH_CNF15_1);
    GPIOF->CRH &= ~(
        GPIO_CRH_CNF12_0 |
        GPIO_CRH_CNF13_0 |
        GPIO_CRH_CNF14_0 |
        GPIO_CRH_CNF15_0);

    GPIOG->CRL |= (GPIO_CRL_CNF0_1 |
                   GPIO_CRL_CNF1_1 |
                   GPIO_CRL_CNF2_1 |
                   GPIO_CRL_CNF3_1 |
                   GPIO_CRL_CNF4_1 |
                   GPIO_CRL_CNF5_1);
    GPIOG->CRL &= ~(
        GPIO_CRL_CNF0_0 |
        GPIO_CRL_CNF1_0 |
        GPIO_CRL_CNF2_0 |
        GPIO_CRL_CNF3_0 |
        GPIO_CRL_CNF4_0 |
        GPIO_CRL_CNF5_0);

    GPIOD->CRH |= (GPIO_CRH_CNF11_1 |
                   GPIO_CRH_CNF12_1 |
                   GPIO_CRH_CNF13_1);
    GPIOD->CRH &= ~(
        GPIO_CRH_CNF11_0 |
        GPIO_CRH_CNF12_0 |
        GPIO_CRH_CNF13_0);

    // 2. 数据线 D0 ~ D15
    // MODE = 11
    GPIOD->CRL |= (GPIO_CRL_MODE0 |
                   GPIO_CRL_MODE1);

    GPIOD->CRH |= (GPIO_CRH_MODE8 |
                   GPIO_CRH_MODE9 |
                   GPIO_CRH_MODE10 |
                   GPIO_CRH_MODE14 |
                   GPIO_CRH_MODE15);

    GPIOE->CRL |= (GPIO_CRL_MODE7);

    GPIOE->CRH |= (GPIO_CRH_MODE8 |
                   GPIO_CRH_MODE9 |
                   GPIO_CRH_MODE10 |
                   GPIO_CRH_MODE11 |
                   GPIO_CRH_MODE12 |
                   GPIO_CRH_MODE13 |
                   GPIO_CRH_MODE14 |
                   GPIO_CRH_MODE15);

    // CNF = 10
    GPIOD->CRL |= (GPIO_CRL_CNF0_1 |
                   GPIO_CRL_CNF1_1);
    GPIOD->CRL &= ~(GPIO_CRL_CNF0_0 |
                    GPIO_CRL_CNF1_0);

    GPIOD->CRH |= (GPIO_CRH_CNF8_1 |
                   GPIO_CRH_CNF9_1 |
                   GPIO_CRH_CNF10_1 |
                   GPIO_CRH_CNF14_1 |
                   GPIO_CRH_CNF15_1);
    GPIOD->CRH &= ~(GPIO_CRH_CNF8_0 |
                    GPIO_CRH_CNF9_0 |
                    GPIO_CRH_CNF10_0 |
                    GPIO_CRH_CNF14_0 |
                    GPIO_CRH_CNF15_0);

    GPIOE->CRL |= (GPIO_CRL_CNF7_1);
    GPIOE->CRL &= ~(GPIO_CRL_CNF7_0);

    GPIOE->CRH |= (GPIO_CRH_CNF8_1 |
                   GPIO_CRH_CNF9_1 |
                   GPIO_CRH_CNF10_1 |
                   GPIO_CRH_CNF11_1 |
                   GPIO_CRH_CNF12_1 |
                   GPIO_CRH_CNF13_1 |
                   GPIO_CRH_CNF14_1 |
                   GPIO_CRH_CNF15_1);
    GPIOE->CRH &= ~(GPIO_CRH_CNF8_0 |
                    GPIO_CRH_CNF9_0 |
                    GPIO_CRH_CNF10_0 |
                    GPIO_CRH_CNF11_0 |
                    GPIO_CRH_CNF12_0 |
                    GPIO_CRH_CNF13_0 |
                    GPIO_CRH_CNF14_0 |
                    GPIO_CRH_CNF15_0);

    // 3. 控制线
    // PD4 - NOE，PD5 - NWE
    GPIOD->CRL |= (GPIO_CRL_MODE4 |
                   GPIO_CRL_MODE5);
    GPIOD->CRL |= (GPIO_CRL_CNF4_1 |
                   GPIO_CRL_CNF5_1);
    GPIOD->CRL &= ~(GPIO_CRL_CNF4_0 |
                    GPIO_CRL_CNF5_0);

    // PG10 - NE3
    GPIOG->CRH |= GPIO_CRH_MODE10;
    GPIOG->CRH |= GPIO_CRH_CNF10_1;
    GPIOG->CRH &= ~GPIO_CRH_CNF10_0;

    // PE0, PE1 - NBL
    GPIOE->CRL |= (GPIO_CRL_MODE0 |
                   GPIO_CRL_MODE1);
    GPIOE->CRL |= (GPIO_CRL_CNF0_1 |
                   GPIO_CRL_CNF1_1);
    GPIOE->CRL &= ~(GPIO_CRL_CNF0_0 |
                    GPIO_CRL_CNF1_0);
}
