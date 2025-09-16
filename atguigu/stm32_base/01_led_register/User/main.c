/*
 * @Author: wushengran
 * @Date: 2024-09-13 16:29:39
 * @Description: 
 * 
 * Copyright (c) 2024 by atguigu, All Rights Reserved. 
 */
 
 #include "stm32f10x.h"

 int main(void)
 {
	 /*
	 // 1. ʱ�����ã�����GPIOAʱ��
	 *(uint32_t *)(0x40021000 + 0x18) = 4;
	 
	 // 2. ����PA0Ϊ���
	 // *(uint32_t *)(0x40010800 + 0x00) = 3;
	 // ����PA1Ϊ���
	 *(uint32_t *)(0x40010800 + 0x00) = 0x33;
	 // ����PA8Ϊ���
	 *(uint32_t *)(0x40010800 + 0x04) = 0x3;
	 
	 // 3. PA0����͵�ƽ
	 // *(uint32_t *)(0x40010800 + 0x0C) = 0xfffe;
	 // PA1����͵�ƽ
	 // *(uint32_t *)(0x40010800 + 0x0C) = 0xfffd;
	 // PA8����͵�ƽ
	 // *(uint32_t *)(0x40010800 + 0x0C) = 0xfeff;
	 
	 *(uint32_t *)(0x40010800 + 0x0C) = 0xfefc;
	 */
	 
	 // �Ľ�1��ʹ�ú궨���д�Ĵ�����ַ
	 
	 // 1. ʱ�����ã�����GPIOAʱ��
	 // RCC->APB2ENR = 4;
	 
	 // 2. ����PA0Ϊ���
	 // GPIOA->CRL = 3;
	 
	 // 3. PA0����͵�ƽ
	 // GPIOA->ODR = 0xfffe;
	 
	 /*
	 // �Ľ�2��ʹ��λ����ʵ��ֻ�ı�ĳһλ��ֵ
	 
	 // 1. ʱ�����ã�����GPIOAʱ��
	 RCC->APB2ENR |= (1 << 2);
	 
	 // 2. ����PA0Ϊ���
	 GPIOA->CRL |= (1 << 0);
	 GPIOA->CRL |= (1 << 1);
	 GPIOA->CRL &= ~(1 << 2);
	 GPIOA->CRL &= ~(1 << 3);
	 
	 // 3. PA0����͵�ƽ
	 GPIOA->ODR &= ~(1 << 0);
	 */
	 
	 // �Ľ�3��ʹ�ú궨���дÿһλ�ı�ʾ
	 
	 // 1. ʱ�����ã�����GPIOAʱ��
	 RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
	 
	 // 2. ����PA0Ϊ���
	 GPIOA->CRL |= GPIO_CRL_MODE0;
	 GPIOA->CRL &= ~GPIO_CRL_CNF0;
	 
	 // 3. PA0����͵�ƽ
	 GPIOA->ODR &= ~GPIO_ODR_ODR0;
	 
	 while(1)
	 {}
 }
 