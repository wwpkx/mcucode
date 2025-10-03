/*
 * @Author: wushengran
 * @Date: 2024-09-13 16:29:39
 * @Description:
 *
 * Copyright (c) 2024 by atguigu, All Rights Reserved.
 */

#include "usart.h"
#include "eth.h"
#include "tcp.h"

// ����ȫ�ֱ������������ݻ������ͳ���
uint8_t rxBuff[1024];
uint16_t rxLen;

int main(void)
{
	// 1. ��ʼ��
	USART_Init();

	printf("�й����̫��ʵ�飺��������\n");

	ETH_Init();

	printf("\n��̫����ʼ����ɣ�\n");

	while (1)
	{
		TCP_ServerStart();
		TCP_RecvData(rxBuff, &rxLen);

		// �ж�������ȴ���0����ʾ��ȡ�����ݣ���ԭ������ȥ
		if (rxLen > 0)
		{
			printf("�յ����ݣ�%.*s\n", rxLen, rxBuff);

			TCP_SendData(rxBuff, rxLen);

			// ���ݳ�����0
			rxLen = 0;
		}
	}
}

