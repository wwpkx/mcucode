/*
 * @Author: wushengran
 * @Date: 2024-09-13 16:29:39
 * @Description:
 *
 * Copyright (c) 2024 by atguigu, All Rights Reserved.
 */

#include "usart.h"
#include "can.h"
#include <string.h>

int main(void)
{
	// 初始化
	USART_Init();
	CAN_Init();

	printf("尚硅谷CAN通讯实验：环回静默模式测试，寄存器版...\n");

	// 1. 发送三个报文
	uint16_t stdID = 0x066;
	uint8_t * data = "abcdefg";
	CAN_SendMsg(stdID, data, strlen((char *)data));

	stdID = 0x068;
	data = "123";
	CAN_SendMsg(stdID, data, strlen((char *)data));

	stdID = 0x067;
	data = "xyz";
	CAN_SendMsg(stdID, data, strlen((char *)data));

	// 2. 接收数据
	RxMsg rxMsg[3];
	uint8_t msgCount;

	CAN_ReceiveMsg(rxMsg, &msgCount);

	printf("报文接收完毕！count = %d\n", msgCount);

	// 3. 打印输出报文
	for (uint8_t i = 0; i < msgCount; i++)
	{
		printf("stdID = %#X, len = %d, data = %.*s\n",
			 rxMsg[i].stdID, rxMsg[i].len, rxMsg[i].len, rxMsg[i].data);
	}

	while (1)
	{
	}
}

