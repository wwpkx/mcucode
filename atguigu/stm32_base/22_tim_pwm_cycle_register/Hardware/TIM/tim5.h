/*
 * @Author: wushengran
 * @Date: 2024-09-27 16:12:45
 * @Description: 
 * 
 * Copyright (c) 2024 by atguigu, All Rights Reserved. 
 */
#ifndef __TIM5_H
#define __TIM5_H

#include "stm32f10x.h"

// ��ʼ��
void TIM5_Init(void);

// �������Ŀ����͹ر�
void TIM5_Start(void);
void TIM5_Stop(void);

// ����ռ�ձȣ�����ٷֱ�ֵ
void TIM5_SetDutyCycle(uint8_t duty);

#endif
