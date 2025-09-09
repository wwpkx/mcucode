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

	printf("尚硅谷CAN通讯实验：双机收发测试 - 接收节点，寄存器版...\n");

	RxMsg rxMsg[3];
	uint8_t msgCount;

	while (1)
	{
		// 接收数据
		CAN_ReceiveMsg(rxMsg, &msgCount);

		// printf("报文接收完毕！count = %d\n", msgCount);

		// 3. 打印输出报文
		for (uint8_t i = 0; i < msgCount; i++)
		{
			printf("stdID = %#X, len = %d, data = %.*s\n",
				   rxMsg[i].stdID, rxMsg[i].len, rxMsg[i].len, rxMsg[i].data);
		}
	}
}
