/*
 * @Author: wushengran
 * @Date: 2024-10-09 16:17:42
 * @Description: 
 * 
 * Copyright (c) 2024 by atguigu, All Rights Reserved. 
 */
#ifndef __SPI_H
#define __SPI_H

#include "stm32f10x.h"

// �궨�������ߵĲ���
// CS - PC13
#define CS_HIGH (GPIOC->ODR |= GPIO_ODR_ODR13)
#define CS_LOW (GPIOC->ODR &= ~GPIO_ODR_ODR13)

// ��ʼ��
void SPI_Init(void);

// SPIͨ�ŵĿ����͹ر�
void SPI_Start(void);
void SPI_Stop(void);

// һ��ʱ�������ڣ����ӽ���һ���ֽ�����
uint8_t SPI_SwapByte(uint8_t byte);

#endif
