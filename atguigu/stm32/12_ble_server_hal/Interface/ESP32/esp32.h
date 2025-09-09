/*
 * @Author: wushengran
 * @Date: 2024-12-27 15:06:18
 * @Description: 
 * 
 * Copyright (c) 2024 by atguigu, All Rights Reserved. 
 */
#ifndef __ESP32_H
#define __ESP32_H

#include "usart.h"
#include <string.h>

// ��ʼ��
void ESP32_Init(void);

// ��������
void ESP32_SendCmd(uint8_t *cmd, uint16_t cmdLen);

// ��ȡ��Ӧ
void ESP32_ReadResp(uint8_t buff[], uint16_t *len);

#endif
