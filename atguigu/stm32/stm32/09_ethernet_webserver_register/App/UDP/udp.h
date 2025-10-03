/*
 * @Author: wushengran
 * @Date: 2024-12-25 11:27:12
 * @Description: 
 * 
 * Copyright (c) 2024 by atguigu, All Rights Reserved. 
 */
#ifndef __UDP_H
#define __UDP_H

#include "eth.h"
#include "socket.h"

// �궨��
#define SN 0

// ����UDPģʽsocket
void UDP_Start(void);

// �������ݣ���Ҫ����Զ�̵�IP�Ͷ˿ں�
void UDP_RecvData(uint8_t buff[], uint16_t *len, uint8_t *srcIP, uint16_t *srcPort);

// ��������
void UDP_SendData(uint8_t data[], uint16_t len, uint8_t *dstIP, uint16_t dstPort);

#endif
