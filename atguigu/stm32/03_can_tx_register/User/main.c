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
	// ��ʼ��
	USART_Init();
	CAN_Init();

	printf("�й��CANͨѶʵ�飺˫���շ����� - ���ͽڵ㣬�Ĵ�����...\n");

	// ���ͱ���
	uint16_t stdID = 0x066;
	uint8_t * data = "abc";

	// ���巢�����ݻ��������ڻ�����Ϣ�Ļ�����ƴ��һ������
	uint8_t buffer[10];
	uint32_t i = 0;

	while (1)
	{
		// ƴ��Ҫ���͵ı�����Ϣ
		sprintf((char *)buffer, "%s %d", data, ++i);

		CAN_SendMsg(stdID, buffer, strlen((char *)buffer));

		printf("���ķ������...\n");

		// ÿ��1s����һ�α���
		Delay_ms(1000);
	}
}

