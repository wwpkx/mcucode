/*
 * @Author: wushengran
 * @Date: 2024-12-25 16:35:51
 * @Description: 
 * 
 * Copyright (c) 2024 by atguigu, All Rights Reserved. 
 */
#ifndef __WEB_SERVER_H
#define __WEB_SERVER_H

#include "httpServer.h"
#include "led.h"
#include <string.h>

// 初始化Web服务器
void WebServer_Init(void);

// 启动Web服务器
void WebServer_Start(void);

#endif
