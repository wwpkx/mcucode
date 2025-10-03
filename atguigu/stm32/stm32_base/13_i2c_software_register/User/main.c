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
	// 1. 初始化
	USART_Init();
	M24C02_Init();

	printf("尚硅谷I2C软件模拟实验...\n");

	// 2. 写入单个字符
	M24C02_WriteByte(0x00, 'a');
	M24C02_WriteByte(0x01, 'b');
	M24C02_WriteByte(0x02, 'c');

	// 3. 读取字符
	uint8_t byte1 = M24C02_ReadByte(0x00);
	uint8_t byte2 = M24C02_ReadByte(0x01);
	uint8_t byte3 = M24C02_ReadByte(0x02);

	// 4. 串口输出打印
	printf("%c\n%c\n%c\n", byte1, byte2, byte3);

	// 5. 页写
	M24C02_WriteBytes(0x00, "123456", 6);

	// 6. 读取多个字符
	uint8_t buff[50] = {0};
	M24C02_ReadBytes(0x00, buff, 6);

	// 7. 串口输出打印
	printf("%s\n", buff);

	// 清除缓冲区（清0）
	memset(buff, 0, sizeof(buff));

	// 8. 长度超出16的页写
	M24C02_WriteBytes(0x05, "1234567890abcdefghijk", 21);
	M24C02_ReadBytes(0x00, buff, 21);

	// 7. 串口输出打印
	printf("%s\n", buff);

	while (1)
	{
	}
}

