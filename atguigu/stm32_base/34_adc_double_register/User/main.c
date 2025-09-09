/*
 * @Author: wushengran
 * @Date: 2024-09-13 16:29:39
 * @Description:
 *
 * Copyright (c) 2024 by atguigu, All Rights Reserved.
 */

#include "usart.h"
#include "adc.h"
#include "delay.h"

// ��һ������Ϊ2�����飬����ת�����
uint16_t data[2];

int main(void)
{
	// ��ʼ��
	USART_Init();
	ADC1_Init();
	ADC1_DMA_Init();

	printf("Hello world!\n");

	// ����ADת��
	ADC1_StartConvert_DMA((uint32_t)data, 2);

	while (1)
	{
		double v1 = data[0] * 3.3 / 4095;
		double v2 = data[1] * 3.3 / 4095;
		printf("V������ = %.2f, V_PC2 = %.2f\n", v1, v2);

		Delay_ms(1000);
	}
}
