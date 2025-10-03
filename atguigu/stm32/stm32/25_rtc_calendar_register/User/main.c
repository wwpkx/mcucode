/*
 * @Author: wushengran
 * @Date: 2024-09-13 16:29:39
 * @Description:
 *
 * Copyright (c) 2024 by atguigu, All Rights Reserved.
 */

#include "usart.h"
#include "delay.h"
#include "rtc.h"

int main(void)
{
	// 初始化
	USART_Init();
	RTC_Init();

	printf("尚硅谷RTC实验：RTC实时时钟...\n");

	// 设置一次当前的时间戳
	// RTC_SetTimestamp(1736160789);

	DateTime dateTime;

	while (1)
	{
		// 每隔1s获取当前时间打印输出一次
		RTC_GetDateTime(&dateTime);

		printf("%04d年%02d月%02d日 %02d:%02d:%02d\n",
			dateTime.year, dateTime.month, dateTime.day, dateTime.hour, dateTime.minute, dateTime.second);
		
		Delay_ms(1000);
	}
}

