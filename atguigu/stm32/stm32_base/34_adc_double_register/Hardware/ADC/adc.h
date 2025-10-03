/*
 * @Author: wushengran
 * @Date: 2024-10-08 16:18:04
 * @Description: 
 * 
 * Copyright (c) 2024 by atguigu, All Rights Reserved. 
 */
#ifndef __ADC_H
#define __ADC_H

#include "stm32f10x.h"

// ��ʼ��
void ADC1_Init(void);

// DMA��صĳ�ʼ��
void ADC1_DMA_Init(void);

// ����ADC������ת������DMA���ú�������Դ��ַ�̶�ΪADC1->DR��
void ADC1_StartConvert_DMA(uint32_t destAddr, uint16_t len);

#endif
