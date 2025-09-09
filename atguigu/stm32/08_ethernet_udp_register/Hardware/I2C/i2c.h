/*
 * @Author: wushengran
 * @Date: 2024-09-23 14:01:33
 * @Description: 
 * 
 * Copyright (c) 2024 by atguigu, All Rights Reserved. 
 */
#ifndef __I2C_H
#define __I2C_H

#include "stm32f10x.h"
#include "usart.h"
#include "delay.h"

// �궨��
// �� SCL��PB10����SDA��PB11�����ߡ�����
#define SCL_HIGH (GPIOB->ODR |= GPIO_ODR_ODR10)
#define SCL_LOW (GPIOB->ODR &= ~GPIO_ODR_ODR10)
#define SDA_HIGH (GPIOB->ODR |= GPIO_ODR_ODR11)
#define SDA_LOW (GPIOB->ODR &= ~GPIO_ODR_ODR11)

// ��ȡ SDA ����
#define READ_SDA (GPIOB->IDR & GPIO_IDR_IDR11)

// ������Ӧ ACK��NACK
#define ACK 0
#define NACK 1

// ����ÿ�β���֮����ӳ٣�����ʱ�����ڣ�
#define I2C_DELAY Delay_us(10)

// ��ʼ��
void I2C_Init(void);

// ����������ʼ�ź�
void I2C_Start(void);

// ��������ֹͣ�ź�
void I2C_Stop(void);

// ��������Ӧ��/��Ӧ���ź�
void I2C_Ack(void);
void I2C_NAck(void);

// ������SDA��ȡ��Ӧ�źţ��ȴ�Ӧ��
uint8_t I2C_Wait4Ack(void);

// ��������һ���ֽ�
void I2C_SendByte(uint8_t byte);

// ������SDA��ȡһ���ֽ�
uint8_t I2C_ReadByte(void);

#endif
