/*
 * @Author: wushengran
 * @Date: 2024-09-13 16:29:39
 * @Description:
 *
 * Copyright (c) 2024 by atguigu, All Rights Reserved.
 */

#include "usart.h"
#include "eth.h"
#include "web_server.h"

int main(void)
{
	// 1. 初始化
	USART_Init();

	printf("尚硅谷以太网实验：UDP传输数据\n");

	ETH_Init();

	printf("\n以太网初始化完成！\n");

	WebServer_Init();

	while (1)
	{
		WebServer_Start();
	}
}

