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

// ����ȫ�ֱ������������ݻ������ͳ���
uint8_t rxBuff[1024];
uint16_t rxLen;

// �Զ�IP�Ͷ˿ں�
uint8_t srcIP[4];
uint16_t srcPort;

int main(void)
{
	// 1. ��ʼ��
	USART_Init();

	printf("�й����̫��ʵ�飺UDP��������\n");

	ETH_Init();

	printf("\n��̫����ʼ����ɣ�\n");

	while (1)
	{
		UDP_Start();
		UDP_RecvData(rxBuff, &rxLen, srcIP, &srcPort);

		// �ж�������ȴ���0����ʾ��ȡ�����ݣ���ԭ������ȥ
		if (rxLen > 0)
		{
			printf("�յ����ݣ�%.*s\n", rxLen, rxBuff);

			UDP_SendData(rxBuff, rxLen, srcIP, srcPort);

			// ���ݳ�����0
			rxLen = 0;
		}
	}
}

