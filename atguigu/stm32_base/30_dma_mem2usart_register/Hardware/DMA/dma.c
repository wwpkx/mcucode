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
    // 2.1 ���䷽��DIR = 1���Ӵ洢������������
    DMA1_Channel4->CCR |= DMA_CCR4_DIR;

    // 2.2 �������ݿ�ȣ�λ������00 - 8λ
    DMA1_Channel4->CCR &= ~DMA_CCR4_PSIZE;
    DMA1_Channel4->CCR &= ~DMA_CCR4_MSIZE;

    // 2.3 �洢����ַ�����������ַ������
    DMA1_Channel4->CCR &= ~DMA_CCR4_PINC;
    DMA1_Channel4->CCR |= DMA_CCR4_MINC;

    // 2.4 ������������ж�
    DMA1_Channel4->CCR |= DMA_CCR4_TCIE;

    // 2.5 ��������TX��DMAʹ��
    USART1->CR3 |= USART_CR3_DMAT;

    // ��ѭ������
    DMA1_Channel4->CCR |= DMA_CCR1_CIRC;

    // 3. NVIC����
    NVIC_SetPriorityGrouping(3);
    NVIC_SetPriority(DMA1_Channel4_IRQn, 3);
    NVIC_EnableIRQ(DMA1_Channel4_IRQn);
}

// DMA1ͨ��1�������ݣ����������Դ��ַ��Ŀ�ĵ�ַ�����ݳ���
void DMA1_Transmit(uint32_t srcAddr, uint32_t destAddr, uint16_t dataLen)
{
    // 1. ��Դ��ַд��洢����ַ�Ĵ���
    DMA1_Channel4->CMAR = srcAddr;

    // 2. ��Ŀ�ĵ�ַд�������ַ�Ĵ���
    DMA1_Channel4->CPAR = destAddr;

    // 3. �����ݳ���д�� CNDTR
    DMA1_Channel4->CNDTR = dataLen;

    // 4. ����ͨ��ʹ�ܣ���ʼ��������
    DMA1_Channel4->CCR |= DMA_CCR4_EN;
}


// �жϷ������
void DMA1_Channel4_IRQHandler(void)
{
    // �ж��Ƿ��Ǵ�������жϣ�TCIF��
    if ( DMA1->ISR & DMA_ISR_TCIF4 )
    {
        // ����жϱ�־λ
        DMA1->IFCR |= DMA_IFCR_CTCIF4;

        // �ر�ͨ��
        // DMA1_Channel4->CCR &= ~DMA_CCR4_EN;
    }
}
