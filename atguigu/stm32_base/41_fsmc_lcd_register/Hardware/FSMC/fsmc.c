/*
 * @Author: wushengran
 * @Date: 2024-10-13 09:07:14
 * @Description:
 *
 * Copyright (c) 2024 by atguigu, All Rights Reserved.
 */
#include "fsmc.h"

void FSMC_GPIO_Init(void);

// ��ʼ��
void FSMC_Init(void)
{
    // 1. ����ʱ��
    RCC->AHBENR |= RCC_AHBENR_FSMCEN;

    RCC->APB2ENR |= (RCC_APB2ENR_IOPDEN | RCC_APB2ENR_IOPEEN |
                     RCC_APB2ENR_IOPBEN | RCC_APB2ENR_IOPGEN);

    // 2. GPIO����ģʽ����
    FSMC_GPIO_Init();

    // 3. FSMC�Ĵ�������
    // 3.1 BCR4 - BTCR[6]
    // 3.1.1 �洢��ʹ��
    FSMC_Bank1->BTCR[6] |= FSMC_BCR4_MBKEN;

    // 3.1.2 ���ô洢�����ͣ�MTYP = 00��SRAM/ROM
    FSMC_Bank1->BTCR[6] &= ~FSMC_BCR4_MTYP;

    // 3.1.3 ��ֹFlash����
    FSMC_Bank1->BTCR[6] &= ~FSMC_BCR4_FACCEN;

    // 3.1.4 ��ַ���ݸ��ù��ܣ�������
    FSMC_Bank1->BTCR[6] &= ~FSMC_BCR4_MUXEN;

    // 3.1.5 �������߿�ȣ�MWID = 01��16λ
    FSMC_Bank1->BTCR[6] &= ~FSMC_BCR4_MWID_1;
    FSMC_Bank1->BTCR[6] |= FSMC_BCR4_MWID_0;

    // 3.1.6 ����дʹ��
    FSMC_Bank1->BTCR[6] |= FSMC_BCR4_WREN;

    // 3.2 BTR4 - BTCR[7]
    // 3.2.1 ��ַ����ʱ�� ADDSET
    FSMC_Bank1->BTCR[7] &= ~FSMC_BTR4_ADDSET;

    // 3.2.2 ���ݽ���ʱ�� DATAST
    FSMC_Bank1->BTCR[7] &= ~FSMC_BTR4_DATAST;
    FSMC_Bank1->BTCR[7] |= (71 << 8);
}

// ����GPIO���ţ�����Ϊ�������������CNF = 10��MODE = 11
void FSMC_GPIO_Init(void)
{
    // 1. ��ַ�� A10 - PG0
    // MODE = 11
    GPIOG->CRL |= GPIO_CRL_MODE0;

    // CNF = 10
    GPIOG->CRL |= GPIO_CRL_CNF0_1;
    GPIOG->CRL &= ~GPIO_CRL_CNF0_0;

    // 2. ������ D0 ~ D15
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

    // 3. ������
    // PD4 - NOE��PD5 - NWE
    GPIOD->CRL |= (GPIO_CRL_MODE4 |
                   GPIO_CRL_MODE5);
    GPIOD->CRL |= (GPIO_CRL_CNF4_1 |
                   GPIO_CRL_CNF5_1);
    GPIOD->CRL &= ~(GPIO_CRL_CNF4_0 |
                    GPIO_CRL_CNF5_0);

    // PG12 - NE4
    GPIOG->CRH |= GPIO_CRH_MODE12;
    GPIOG->CRH |= GPIO_CRH_CNF12_1;
    GPIOG->CRH &= ~GPIO_CRH_CNF12_0;

    // PB0 - LCD_BG��ͨ���������
    GPIOB->CRL |= GPIO_CRL_MODE0;
    GPIOB->CRL &= ~GPIO_CRL_CNF0;

    // PG15 - LCD_RST��ͨ���������
    GPIOG->CRH |= GPIO_CRH_MODE15;
    GPIOG->CRH &= ~GPIO_CRH_CNF15;
}
