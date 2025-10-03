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
	// ��ʼ��
	USART_Init();
	CAN_Init();

	printf("�й��CANͨѶʵ�飺˫���շ����� - ���սڵ㣬�Ĵ�����...\n");

	RxMsg rxMsg[3];
	uint8_t msgCount;

	while (1)
	{
		// ��������
		CAN_ReceiveMsg(rxMsg, &msgCount);

		// printf("���Ľ�����ϣ�count = %d\n", msgCount);

		// 3. ��ӡ�������
		for (uint8_t i = 0; i < msgCount; i++)
		{
			printf("stdID = %#X, len = %d, data = %.*s\n",
				   rxMsg[i].stdID, rxMsg[i].len, rxMsg[i].len, rxMsg[i].data);
		}
	}
}
