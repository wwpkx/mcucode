/*
 * @Author: wushengran
 * @Date: 2024-12-27 15:06:13
 * @Description: 
 * 
 * Copyright (c) 2024 by atguigu, All Rights Reserved. 
 */
#include "esp32.h"

// ����ȫ�ֱ�����������Ӧ�������ͳ���
uint8_t respBuff[1024];
uint16_t respLen;

// ��ʼ��
void ESP32_Init(void)
{
    // 1. ��ʼ������2
    MX_USART2_UART_Init();

    // 2. ����ATָ���λESP32
    uint8_t *cmd = "AT+RST=0\r\n";
    ESP32_SendCmd(cmd, strlen((char *)cmd));

    // 3. ������ʱ��ȷ�������ɹ�
    HAL_Delay(2000);
}

// ��������
void ESP32_SendCmd(uint8_t *cmd, uint16_t cmdLen)
{
    // �����Ӧ������
    memset(respBuff, 0, 1024);

    // ֱ��ͨ������2��������
    HAL_UART_Transmit(&huart2, cmd, cmdLen, 1000);

    // ��ȡ��Ӧ
    do
    {
        ESP32_ReadResp(respBuff, &respLen);
    } while (strstr((char *)respBuff, "OK") == NULL);

    // ����Ӧ��ӡ���
    printf("%.*s\n", respLen, respBuff);
}

// ��ȡ��Ӧ
void ESP32_ReadResp(uint8_t buff[], uint16_t *len)
{
    // ����2���ձ䳤����
    HAL_UARTEx_ReceiveToIdle(&huart2, buff, 1024, len, 1000);
}
