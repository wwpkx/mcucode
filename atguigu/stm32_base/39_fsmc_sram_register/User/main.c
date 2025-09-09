/*
 * @Author: wushengran
 * @Date: 2024-09-13 16:29:39
 * @Description:
 *
 * Copyright (c) 2024 by atguigu, All Rights Reserved.
 */

#include "usart.h"
#include "fsmc.h"

// 方法1：使用关键字attribute和at指定全局变量的地址
uint8_t v1 __attribute__((at(0x68000000)));
uint8_t v2 __attribute__((at(0x68000004)));

uint16_t v3 = 30;

int main(void)
{
	// 1. 初始化
	USART_Init();
	FSMC_Init();

	printf("尚硅谷FSMC扩展SRAM实验...\n");

	v1 = 10;
	v2 = 20;

	// 增加局部变量，进行对比
	uint8_t v4 __attribute__((at(0x68000008)));
	v4 = 40;
	uint8_t v5 = 50;

	// 打印地址进行验证
	printf("v1 = %d, @%p\n", v1, &v1);
	printf("v2 = %d, @%p\n", v2, &v2);
	printf("v3 = %d, @%p\n", v3, &v3);
	printf("v4 = %d, @%p\n", v4, &v4);
	printf("v5 = %d, @%p\n", v5, &v5);

	// 方法2：定义指针
	uint8_t *p = (uint8_t *)0x68000FFF;
	*p = 100;
	printf("*p = %d, @%p\n", *p, p);

	while (1)
	{
	}
}
