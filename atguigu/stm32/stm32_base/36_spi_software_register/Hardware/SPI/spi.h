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
#include "delay.h"

// �궨�������ߵĲ���
// CS - PC13
#define CS_HIGH (GPIOC->ODR |= GPIO_ODR_ODR13)
#define CS_LOW (GPIOC->ODR &= ~GPIO_ODR_ODR13)

// SCK - PA5
#define SCK_HIGH (GPIOA->ODR |= GPIO_ODR_ODR5)
#define SCK_LOW (GPIOA->ODR &= ~GPIO_ODR_ODR5)

// MOSI - PA7
#define MOSI_HIGH (GPIOA->ODR |= GPIO_ODR_ODR7)
#define MOSI_LOW (GPIOA->ODR &= ~GPIO_ODR_ODR7)

// MISO - PA6����ȡ������
#define MISO_READ ( GPIOA->IDR & GPIO_IDR_IDR6 )

// �����׼�ӳ�ʱ��
#define SPI_DELAY Delay_us(5)

// ��ʼ��
void SPI_Init(void);

// SPIͨ�ŵĿ����͹ر�
void SPI_Start(void);
void SPI_Stop(void);

// һ��ʱ�������ڣ����ӽ���һ���ֽ�����
uint8_t SPI_SwapByte(uint8_t byte);

#endif
