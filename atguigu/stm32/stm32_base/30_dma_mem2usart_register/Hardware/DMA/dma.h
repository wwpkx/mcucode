#ifndef __DMA_H
#define __DMA_H

#include "stm32f10x.h"

// ��ʼ��
void DMA1_Init(void);

// DMA1ͨ��1�������ݣ����������Դ��ַ��Ŀ�ĵ�ַ�����ݳ���
void DMA1_Transmit(uint32_t srcAddr, uint32_t destAddr, uint16_t dataLen);

#endif
