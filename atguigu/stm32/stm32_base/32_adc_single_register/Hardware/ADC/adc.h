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

// ����ADC������ת��
void ADC1_StartConvert(void);

// ����ת����ĵ�ѹֵ
double ADC1_ReadV(void);

#endif
