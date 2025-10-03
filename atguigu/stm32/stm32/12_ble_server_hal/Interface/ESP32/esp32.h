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

// 初始化
void ESP32_Init(void);

// 发送命令
void ESP32_SendCmd(uint8_t *cmd, uint16_t cmdLen);

// 读取响应
void ESP32_ReadResp(uint8_t buff[], uint16_t *len);

#endif
