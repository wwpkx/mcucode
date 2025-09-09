/*
 * @Author: wushengran
 * @Date: 2024-12-24 14:37:53
 * @Description:
 *
 * Copyright (c) 2024 by atguigu, All Rights Reserved.
 */
#include "eth.h"
#include "delay.h"

// ����W5500��IP��ַ��MAC��ַ��������������ص�ַ
uint8_t ip[4] = {192, 168, 44, 222};
uint8_t mac[6] = {110, 120, 130, 140, 150, 160};
uint8_t submask[4] = {255, 255, 255, 0};
uint8_t gateway[4] = {192, 168, 44, 1};

// ��λW5500
static void ETH_Reset(void);
// ���� MAC ��ַ
static void ETH_SetMac(void);
// ���� IP ��ַ���������غ��������룩
static void ETH_SetIP(void);

// ��ʼ��
void ETH_Init(void)
{
    // 0. SPI ��ʼ��
    SPI_Init();

    // 1. ע���Զ���ص�����
    user_register_function();

    // 2. ��λW5500
    ETH_Reset();

    // 3. ���� MAC ��ַ
    ETH_SetMac();

    // 4. ���� IP ��ַ���������غ��������룩
    ETH_SetIP();
}

// ��λW5500
static void ETH_Reset(void)
{
    // 1. ����RST����-PG7
    // 1.1 ����ʱ��
    RCC->APB2ENR |= RCC_APB2ENR_IOPGEN;

    // 1.2 ���ù���ģʽ��ͨ�����������MODE - 11��CNF - 00
    GPIOG->CRL |= GPIO_CRL_MODE7;
    GPIOG->CRL &= ~GPIO_CRL_CNF7;

    // 2. ����RST������500us����
    GPIOG->ODR &= ~GPIO_ODR_ODR7;

    Delay_us(800);

    GPIOG->ODR |= GPIO_ODR_ODR7;

    printf("W5500 ��λ��ɣ�\n");
}

// ���� MAC ��ַ
static void ETH_SetMac(void)
{
    printf("��ʼ���� MAC ��ַ:\n");
    setSHAR(mac);
    printf("MAC ��ַ�������: %X-%X-%X-%X-%X-%X\n", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
}

// ���� IP ��ַ���������غ��������룩
static void ETH_SetIP(void)
{
    printf("��ʼ���� IP ��ַ:\n");

    // ����IP
    setSIPR(ip);
    // ������������
    setSUBR(submask);
    // �������ص�ַ
    setGAR(gateway);

    printf("IP ��ַ�������: %d.%d.%d.%d\n", ip[0], ip[1], ip[2], ip[3]);
}
