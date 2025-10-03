/*
 * @Author: wushengran
 * @Date: 2024-12-30 15:15:53
 * @Description: 
 * 
 * Copyright (c) 2024 by atguigu, All Rights Reserved. 
 */
#include "ble.h"

// ��ʼ��
void BLE_Init(void)
{
    // 1. ��ʼ��ESP32
    ESP32_Init();

    // 2. ��������ģ���ɫ��2-server
    printf("����ģ���ɫΪ��������...\n");
    uint8_t *cmd = "AT+BLEINIT=2\r\n";
    ESP32_SendCmd(cmd, strlen((char *)cmd));

    // 3. ����˴�������
    printf("����˴�������...\n");
    cmd = "AT+BLEGATTSSRVCRE\r\n";
    ESP32_SendCmd(cmd, strlen((char *)cmd));

    // 4. ����˿�������
    printf("����˿�������...\n");
    cmd = "AT+BLEGATTSSRVSTART\r\n";
    ESP32_SendCmd(cmd, strlen((char *)cmd));

    // 5. ��ȡ�����MAC��ַ
    printf("��ȡ�����MAC��ַ...\n");
    cmd = "AT+BLEADDR?\r\n";
    ESP32_SendCmd(cmd, strlen((char *)cmd));

    // 6. ���������㲥����
    printf("���������㲥����...\n");
    cmd = "AT+BLEADVPARAM=50,50,0,0,7,0\r\n";
    ESP32_SendCmd(cmd, strlen((char *)cmd));

    // 7. ���ù㲥����
    printf("���ù㲥����...\n");
    cmd = "AT+BLEADVDATAEX=\"atguigu-ble\",\"A123\",\"0102030405\",1\r\n";
    ESP32_SendCmd(cmd, strlen((char *)cmd));

    // 8. ��ʼ�㲥
    printf("��ʼ�㲥...\n");
    cmd = "AT+BLEADVSTART\r\n";
    ESP32_SendCmd(cmd, strlen((char *)cmd));

    // 9. ���� BLE SPP����
    printf("���� BLE SPP ����...\n");
    cmd = "AT+BLESPPCFG=1,1,7,1,5\r\n";
    ESP32_SendCmd(cmd, strlen((char *)cmd));

    // 10. ������͸��ģʽ�£����ӷ����仯ʱ�ᴫ��һ����ʾ��Ϣ
    printf("����ϵͳ��ʾ��Ϣ...\n");
    cmd = "AT+SYSMSG=4\r\n";
    ESP32_SendCmd(cmd, strlen((char *)cmd));
}

// ��������
void BLE_SendData(uint8_t txBuff[], uint16_t txLen)
{
    // ֱ��ͨ�����ڷ�������
    HAL_UART_Transmit(&huart2, txBuff, txLen, 1000);
}

// �Զ��庯�����ж��Ƿ������ӱ仯��Ϣ��������򷵻�1��û�з���0
static uint8_t BLE_IsConnChanged(uint8_t msg[]);

// ���գ���ȡ������
void BLE_ReadData(uint8_t rxBuff[], uint16_t *rxLen)
{
    // �Ӵ��ڽ������ݻ�״̬�ı���Ϣ
    HAL_UARTEx_ReceiveToIdle(&huart2, rxBuff, 1024, rxLen, 1000);

    // ����ǿ����ݣ�rxLen = 0����ֱ�ӷ���
    if (*rxLen == 0)
    {
        return;
    }

    // ��������ӱ仯��Ϣ��ִ����Ӧ��SPP������������len����ʾ������������
    if ( BLE_IsConnChanged(rxBuff) )
    {
        *rxLen = 0;
    }
}

static uint8_t BLE_IsConnChanged(uint8_t msg[])
{
    // 1. �����BLE�������ӣ��ͽ���SPP͸��ģʽ
    if (strstr((char *)msg, "+BLECONN:") != NULL)
    {
        printf("��BLE�ͻ������ӣ����Ͻ���SPPģʽ...\n");
        uint8_t * cmd = "AT+BLESPP\r\n";
        ESP32_SendCmd(cmd, strlen((char *)cmd));

        return 1;
    }

    // 2. �����BLE�Ͽ����ӣ����˳�͸��ģʽ
    else if (strstr((char *)msg, "+BLEDISCONN:") != NULL)
    {
        printf("BLE�ͻ��˶Ͽ����ӣ������˳�SPPģʽ...\n");
        
        // ֱ�Ӵ��ڷ��� +++ �˳�͸��ģʽ
        HAL_UART_Transmit(&huart2, "+++", 3, 1000);

        // ��ʱ�ȴ�
        HAL_Delay(2000);

        return 1;
    }

    // 3. ������������͵����ӱ仯��Wi-Fi���������κδ�������1
    else if(strstr((char *)msg, "WIFI CONNECTED") != NULL
        || strstr((char *)msg, "WIFI GOT IP") != NULL
        || strstr((char *)msg, "+DIST_STA_IP:") != NULL)
    {
        return 1;
    }
    
    // 4. �������Ĭ������������
    return 0;
}
