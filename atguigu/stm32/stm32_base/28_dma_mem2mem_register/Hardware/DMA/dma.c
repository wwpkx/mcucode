/*
 * @Author: wushengran
 * @Date: 2024-09-30 14:09:56
 * @Description: 
 * 
 * Copyright (c) 2024 by atguigu, All Rights Reserved. 
 */
#include "dma.h"

// ��ʼ��
void DMA1_Init(void)
{
    // 1. ����ʱ��
    RCC->AHBENR |= RCC_AHBENR_DMA1EN;

    // 2. DMA�������
    // 2.1 ���䷽��DIR = 0�������赽�洢������ROM�������裬�洢�����洢����
    DMA1_Channel1->CCR &= ~DMA_CCR1_DIR;
    DMA1_Channel1->CCR |= DMA_CCR1_MEM2MEM;

    // 2.2 �������ݿ�ȣ�λ������00 - 8λ
    DMA1_Channel1->CCR &= ~DMA_CCR1_PSIZE;
    DMA1_Channel1->CCR &= ~DMA_CCR1_MSIZE;

    // 2.3 ����ʹ洢����ַ����
    DMA1_Channel1->CCR |= DMA_CCR1_PINC;
    DMA1_Channel1->CCR |= DMA_CCR1_MINC;

    // 2.4 ������������ж�
    DMA1_Channel1->CCR |= DMA_CCR1_TCIE;

    // 3. NVIC����
    NVIC_SetPriorityGrouping(3);
    NVIC_SetPriority(DMA1_Channel1_IRQn, 3);
    NVIC_EnableIRQ(DMA1_Channel1_IRQn);
}

// DMA1ͨ��1�������ݣ����������Դ��ַ��Ŀ�ĵ�ַ�����ݳ���
void DMA1_Transmit(uint32_t srcAddr, uint32_t destAddr, uint16_t dataLen)
{
    // 1. ��Դ��ַд�������ַ�Ĵ���
    DMA1_Channel1->CPAR = srcAddr;

    // 2. ��Ŀ�ĵ�ַд��洢����ַ�Ĵ���
    DMA1_Channel1->CMAR = destAddr;

    // 3. �����ݳ���д�� CNDTR
    DMA1_Channel1->CNDTR = dataLen;

    // 4. ����ͨ��ʹ�ܣ���ʼ��������
    DMA1_Channel1->CCR |= DMA_CCR1_EN;
}

extern uint8_t isOver;

// �жϷ������
void DMA1_Channel1_IRQHandler(void)
{
    // �ж��Ƿ��Ǵ�������жϣ�TCIF��
    if ( DMA1->ISR & DMA_ISR_TCIF1 )
    {
        // ����жϱ�־λ
        DMA1->IFCR |= DMA_IFCR_CTCIF1;

        // �ر�ͨ��
        DMA1_Channel1->CCR &= ~DMA_CCR1_EN;

        isOver = 1;
    }
}
