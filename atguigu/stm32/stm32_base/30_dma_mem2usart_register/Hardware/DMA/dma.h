#ifndef __DMA_H
#define __DMA_H

#include "stm32f10x.h"

// 初始化
void DMA1_Init(void);

// DMA1通道1传输数据，传入参数：源地址、目的地址、数据长度
void DMA1_Transmit(uint32_t srcAddr, uint32_t destAddr, uint16_t dataLen);

#endif
