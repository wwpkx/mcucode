/*
 * @Author: wushengran
 * @Date: 2024-09-13 16:29:39
 * @Description:
 *
 * Copyright (c) 2024 by atguigu, All Rights Reserved.
 */

#include "delay.h"
#include "usart.h"
#include "can.h"
#include <string.h>

int main(void)
{
	// 初始化
	USART_Init();
	CAN_Init();

	printf("尚硅谷CAN通讯实验：双机收发测试 - 发送节点，寄存器版...\n");

	// 发送报文
	uint16_t stdID = 0x066;
	uint8_t * data = "abc";

	// 定义发送数据缓冲区，在基本信息的基础上拼接一个数字
	uint8_t buffer[10];
	uint32_t i = 0;

	while (1)
	{
		// 拼接要发送的报文信息
		sprintf((char *)buffer, "%s %d", data, ++i);

		CAN_SendMsg(stdID, buffer, strlen((char *)buffer));

		printf("报文发送完毕...\n");

		// 每隔1s发送一次报文
		Delay_ms(1000);
	}
}

