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
// �������ִ�з��ؽ�� OK - �ɹ���FAIL - ��ʱʧ��
#define OK 0
#define FAIL 1

// ��ʼ��
void I2C_Init(void);

// ����������ʼ�ź�
uint8_t I2C_Start(void);

// ��������ֹͣ�ź�
void I2C_Stop(void);

// ��������Ӧ��/��Ӧ���ź�
void I2C_Ack(void);
void I2C_NAck(void);

// ���������豸��ַ
uint8_t I2C_SendAddr(uint8_t addr);

// ��������һ���ֽ�
uint8_t I2C_SendByte(uint8_t byte);

// ������SDA��ȡһ���ֽ�
uint8_t I2C_ReadByte(void);

#endif
