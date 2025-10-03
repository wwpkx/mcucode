/*
 * @Author: wushengran
 * @Date: 2024-12-28 11:07:55
 * @Description: 
 * 
 * Copyright (c) 2024 by atguigu, All Rights Reserved. 
 */
#include "wifi.h"

// �����ڲ�����������Wi-Fi����ģʽ
static void WIFI_STA_Mode(void);
static void WIFI_AP_Mode(void);

// ��ʼ��������WiFi����ģʽ
void WIFI_Init(WIFI_MODE mode)
{
    // 1. ESP32��ʼ��
    ESP32_Init();

    // 2. ���ù���ģʽ��IP����
    if (mode == STA)
    {
        WIFI_STA_Mode();
    }
    else if (mode == AP)
    {
        WIFI_AP_Mode();
    }

    printf("Wi-Fi ��ʼ����ɣ�\n");
}

static void WIFI_STA_Mode(void)
{
    // 1. ���ù���ģʽΪSTA
    printf("����ΪSTAģʽ...\n");
    uint8_t *cmd = "AT+CWMODE=1\r\n";
    ESP32_SendCmd(cmd, strlen((char *)cmd));

    // 2. ���ӵ�ָ����AP
    printf("����AP...\n");
    cmd = "AT+CWJAP=\"wsr\",\"1234abcd\"\r\n";
    ESP32_SendCmd(cmd, strlen((char *)cmd));

    // 3. ��ѯ����IP
    printf("����IP��ַΪ��\n");
    cmd = "AT+CIPSTA?\r\n";
    ESP32_SendCmd(cmd, strlen((char *)cmd));
}

static void WIFI_AP_Mode(void)
{
    // 1. ���ù���ģʽΪAP
    printf("����ΪAPģʽ...\n");
    uint8_t *cmd = "AT+CWMODE=2\r\n";
    ESP32_SendCmd(cmd, strlen((char *)cmd));

    // 2. ����AP����
    printf("����AP...\n");
    cmd = "AT+CWSAP=\"atguigu-esp32\",\"12345678\",5,3\r\n";
    ESP32_SendCmd(cmd, strlen((char *)cmd));

    // 3. ���ñ���IP
    printf("���ñ���IP��ַ...\n");
    cmd = "AT+CIPAP=\"192.168.8.1\"\r\n";
    ESP32_SendCmd(cmd, strlen((char *)cmd));
}

// ����TCP������
void WIFI_TCP_ServerStart(void)
{
    // 1. ����ʹ�ܶ�����
    printf("����ʹ�ܶ�����...\n");
    uint8_t *cmd = "AT+CIPMUX=1\r\n";
    ESP32_SendCmd(cmd, strlen((char *)cmd));

    // 2. ����TCP������
    printf("����TCP������...\n");
    cmd = "AT+CIPSERVER=1,8080\r\n";
    ESP32_SendCmd(cmd, strlen((char *)cmd));

    // 3. ����IPD���ݸ�ʽ����ʾ���飩
    printf("����IPD���ݸ�ʽ...\n");
    cmd = "AT+CIPDINFO=1\r\n";
    ESP32_SendCmd(cmd, strlen((char *)cmd));
}

// �������ݣ���������ID��
void WIFI_TCP_SendData(uint8_t id, uint8_t *data, uint16_t len)
{
    // 1. ���÷������ݵ�����ID�����ݳ���
    printf("���Է���������...\n");

    uint8_t sendCmd[50] = {0};
    sprintf((char *)sendCmd, "AT+CIPSEND=%d,%d\r\n", id, len);
    ESP32_SendCmd(sendCmd, strlen((char *)sendCmd));

    // 2. ��������ģʽ��ֱ��ͨ�����ڷ�������
    HAL_UART_Transmit(&huart2, data, len, 1000);
}

// ����ʱ����������IPD����
uint8_t tempBuff[1024];
uint16_t tempLen;

// ��ȡ���ݣ���������ID���Զ�IP�Ͷ˿ںţ�
void WIFI_TCP_ReadData(uint8_t rxBuff[], uint16_t *rxLen, uint8_t *id, uint8_t *ip, uint16_t *port)
{
    // 1. �Ӵ��ڽ�������
    HAL_UARTEx_ReceiveToIdle(&huart2, tempBuff, 1024, &tempLen, 1000);

    // 2. �ӽ��յ���IPD�����н�������
    // �ж��Ƿ���+IPD��ͷ�����й���
    if ( strstr((char *)tempBuff, "+IPD") != NULL )
    {
        // ����sscanf��������ʽ��ȡ���е���Ϣ
        sscanf((char *)tempBuff, "%*[\r\n]+IPD,%hhu,%hu,\"%[^\"]\",%hu",
             id, rxLen, ip, port);
        
        // �����������ݸ��Ƶ�����Ļ�����������
        char * pData = strstr((char *)tempBuff, ":") + 1;
        memcpy(rxBuff, pData, *rxLen);
    }  
}

