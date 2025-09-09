/*
 * @Author: wushengran
 * @Date: 2024-09-13 16:29:39
 * @Description:
 *
 * Copyright (c) 2024 by atguigu, All Rights Reserved.
 */

#include "usart.h"
#include "w25q32.h"

int main(void)
{
	// 1. ��ʼ��
	USART_Init();
	W25Q32_Init();

	printf("�й��SPI���ģ��ʵ��...\n");

	// 2. ��ȡJEDEC ID
	uint8_t mID = 0;
	uint16_t dID = 0;

	W25Q32_ReadID(&mID, &dID);
	printf("mid = %#x, did = %#x\n", mID, dID);

	// 3. д��ǰ����������������һ���Σ�
	W25Q32_EraseSector(0, 0);

	// 4. ҳд����һ��ҳ��
	W25Q32_WritePage(0, 0, 0, "12345678", 8);

	// 5. ��ȡ
	uint8_t buff[10] = {0};
	W25Q32_Read(0x3, buff, 8);

	printf("buff: %s\n", buff);

	while (1)
	{
	}
}

