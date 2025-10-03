/*
 * @Author: wushengran
 * @Date: 2024-12-21 16:52:20
 * @Description: 
 * 
 * Copyright (c) 2024 by atguigu, All Rights Reserved. 
 */
#ifndef __CAN_H
#define __CAN_H

#include "stm32f10x.h"

// ����ṹ�壬������յ��ı�����Ϣ
typedef struct
{
    uint16_t stdID;
    uint8_t data[8];
    uint8_t len;
} RxMsg;

// ��ʼ��
void CAN_Init(void);

// ���ͱ���
void CAN_SendMsg(uint16_t stdID, uint8_t * data, uint8_t len);

// ���ձ���
void CAN_ReceiveMsg(RxMsg rxMsg[], uint8_t * msgCount);

#endif
