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
	// ��ʼ��
	USART_Init();
	RTC_Init();

	printf("�й��RTCʵ�飺RTCʵʱʱ��...\n");

	// ����һ�ε�ǰ��ʱ���
	// RTC_SetTimestamp(1736160789);

	DateTime dateTime;

	while (1)
	{
		// ÿ��1s��ȡ��ǰʱ���ӡ���һ��
		RTC_GetDateTime(&dateTime);

		printf("%04d��%02d��%02d�� %02d:%02d:%02d\n",
			dateTime.year, dateTime.month, dateTime.day, dateTime.hour, dateTime.minute, dateTime.second);
		
		Delay_ms(1000);
	}
}

