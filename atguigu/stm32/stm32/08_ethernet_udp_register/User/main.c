/*
 * @Author: wushengran
 * @Date: 2024-09-13 16:29:39
 * @Description:
 *
 * Copyright (c) 2024 by atguigu, All Rights Reserved.
 */

#include "usart.h"
#include "eth.h"
#include "udp.h"

// 定义全局变量，接收数据缓冲区和长度
uint8_t rxBuff[1024];
uint16_t rxLen;

// 对端IP和端口号
uint8_t srcIP[4];
uint16_t srcPort;

int main(void)
{
	// 1. 初始化
	USART_Init();

	printf("尚硅谷以太网实验：UDP传输数据\n");

	ETH_Init();

	printf("\n以太网初始化完成！\n");

	while (1)
	{
		UDP_Start();
		UDP_RecvData(rxBuff, &rxLen, srcIP, &srcPort);

		// 判断如果长度大于0，表示读取到数据，就原样发回去
		if (rxLen > 0)
		{
			printf("收到数据：%.*s\n", rxLen, rxBuff);

			UDP_SendData(rxBuff, rxLen, srcIP, srcPort);

			// 数据长度清0
			rxLen = 0;
		}
	}
}

