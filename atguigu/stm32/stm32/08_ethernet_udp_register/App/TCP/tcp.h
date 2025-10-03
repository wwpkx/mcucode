/*
 * @Author: wushengran
 * @Date: 2024-12-25 11:27:12
 * @Description: 
 * 
 * Copyright (c) 2024 by atguigu, All Rights Reserved. 
 */
#ifndef __TCP_H
#define __TCP_H

#include "eth.h"
#include "socket.h"

// �궨��
#define SN 0

#define CLIENT 0
#define SERVER 1
#define ROLE CLIENT

// ����TCP������
void TCP_ServerStart(void);

// ����TCP�ͻ���
void TCP_ClientStart(void);

// ��������
void TCP_RecvData(uint8_t buff[], uint16_t *len);

// ��������
void TCP_SendData(uint8_t data[], uint16_t len);

#endif
