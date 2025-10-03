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

// 宏定义
#define SN 0

#define CLIENT 0
#define SERVER 1
#define ROLE CLIENT

// 启动TCP服务器
void TCP_ServerStart(void);

// 启动TCP客户端
void TCP_ClientStart(void);

// 接收数据
void TCP_RecvData(uint8_t buff[], uint16_t *len);

// 发送数据
void TCP_SendData(uint8_t data[], uint16_t len);

#endif
