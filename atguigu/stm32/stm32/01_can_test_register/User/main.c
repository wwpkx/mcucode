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

	printf("�й��CANͨѶʵ�飺���ؾ�Ĭģʽ���ԣ��Ĵ�����...\n");

	// 1. ������������
	uint16_t stdID = 0x066;
	uint8_t * data = "abcdefg";
	CAN_SendMsg(stdID, data, strlen((char *)data));

	stdID = 0x068;
	data = "123";
	CAN_SendMsg(stdID, data, strlen((char *)data));

	stdID = 0x067;
	data = "xyz";
	CAN_SendMsg(stdID, data, strlen((char *)data));

	// 2. ��������
	RxMsg rxMsg[3];
	uint8_t msgCount;

	CAN_ReceiveMsg(rxMsg, &msgCount);

	printf("���Ľ�����ϣ�count = %d\n", msgCount);

	// 3. ��ӡ�������
	for (uint8_t i = 0; i < msgCount; i++)
	{
		printf("stdID = %#X, len = %d, data = %.*s\n",
			 rxMsg[i].stdID, rxMsg[i].len, rxMsg[i].len, rxMsg[i].data);
	}

	while (1)
	{
	}
}

