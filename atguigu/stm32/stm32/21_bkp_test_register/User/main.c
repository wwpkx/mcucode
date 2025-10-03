/*
 * @Author: wushengran
 * @Date: 2024-09-13 16:29:39
 * @Description:
 *
 * Copyright (c) 2024 by atguigu, All Rights Reserved.
 */

#include "usart.h"
#include "delay.h"
#include "key.h"
#include "bkp.h"

int main(void)
{
	// 初始化
	USART_Init();
	KEY_Init();
	BKP_Init();

	printf("尚硅谷备份寄存器实验...\n");

	// 向备份数据寄存器写入数值
	// BKP->DR1 = 9999;

	while (1)
	{
	}
}
