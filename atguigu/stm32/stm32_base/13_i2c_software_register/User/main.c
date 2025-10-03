/*
 * @Author: wushengran
 * @Date: 2024-09-13 16:29:39
 * @Description:
 *
 * Copyright (c) 2024 by atguigu, All Rights Reserved.
 */

#include "usart.h"
#include "m24c02.h"
#include <string.h>

int main(void)
{
	// 1. ��ʼ��
	USART_Init();
	M24C02_Init();

	printf("�й��I2C���ģ��ʵ��...\n");

	// 2. д�뵥���ַ�
	M24C02_WriteByte(0x00, 'a');
	M24C02_WriteByte(0x01, 'b');
	M24C02_WriteByte(0x02, 'c');

	// 3. ��ȡ�ַ�
	uint8_t byte1 = M24C02_ReadByte(0x00);
	uint8_t byte2 = M24C02_ReadByte(0x01);
	uint8_t byte3 = M24C02_ReadByte(0x02);

	// 4. ���������ӡ
	printf("%c\n%c\n%c\n", byte1, byte2, byte3);

	// 5. ҳд
	M24C02_WriteBytes(0x00, "123456", 6);

	// 6. ��ȡ����ַ�
	uint8_t buff[50] = {0};
	M24C02_ReadBytes(0x00, buff, 6);

	// 7. ���������ӡ
	printf("%s\n", buff);

	// �������������0��
	memset(buff, 0, sizeof(buff));

	// 8. ���ȳ���16��ҳд
	M24C02_WriteBytes(0x05, "1234567890abcdefghijk", 21);
	M24C02_ReadBytes(0x00, buff, 21);

	// 7. ���������ӡ
	printf("%s\n", buff);

	while (1)
	{
	}
}

