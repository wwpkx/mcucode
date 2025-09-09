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

// 宏定义
#define SN 0

// 启动UDP模式socket
void UDP_Start(void);

// 接收数据，需要增加远程的IP和端口号
void UDP_RecvData(uint8_t buff[], uint16_t *len, uint8_t *srcIP, uint16_t *srcPort);

// 发送数据
void UDP_SendData(uint8_t data[], uint16_t len, uint8_t *dstIP, uint16_t dstPort);

#endif
