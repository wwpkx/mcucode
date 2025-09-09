/*
 * @Author: wushengran
 * @Date: 2024-09-13 16:29:39
 * @Description:
 *
 * Copyright (c) 2024 by atguigu, All Rights Reserved.
 */

#include "usart.h"
#include "dma.h"

// ����ȫ�ֳ����������� ROM ��
const uint8_t src[] = {10, 20, 30, 40};

// ����ȫ�ֱ����������� RAM ��
uint8_t dest[4];

// ����ȫ�ֱ�������ʾ���ݴ������
uint8_t isOver;

int main(void)
{
	// ��ʼ��
	USART_Init();
	DMA1_Init();

	printf("Hello world!\n");

	// ��ӡ�����ͱ�����ַ
	printf("src = %p, dest = %p\n", src, dest);

	// ���ú�����������
	DMA1_Transmit((uint32_t)src, (uint32_t)dest, 4);

	// �ȴ�������ɣ���ӡ����
	while (!isOver)
	{
	}

	for (uint8_t i = 0; i < 4; i++)
	{
		printf("%d\t", dest[i]);
	}

	while (1)
	{
	}
}
