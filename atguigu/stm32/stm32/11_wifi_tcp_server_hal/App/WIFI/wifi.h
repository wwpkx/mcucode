/*
 * @Author: wushengran
 * @Date: 2024-12-28 11:07:59
 * @Description: 
 * 
 * Copyright (c) 2024 by atguigu, All Rights Reserved. 
 */
#ifndef __WIFI_H
#define __WIFI_H

#include "esp32.h"

// ����ö�����ͣ�STAģʽ��APģʽ
typedef enum
{
    STA = 1,
    AP = 2
} WIFI_MODE;

// ��ʼ��������WiFi����ģʽ
void WIFI_Init(WIFI_MODE mode);

// ����TCP������
void WIFI_TCP_ServerStart(void);

// �������ݣ���������ID��
void WIFI_TCP_SendData(uint8_t id, uint8_t *data, uint16_t len);

// ��ȡ���ݣ���������ID���Զ�IP�Ͷ˿ںţ�
void WIFI_TCP_ReadData(uint8_t rxBuff[], uint16_t *rxLen, uint8_t *id, uint8_t *ip, uint16_t *port);

#endif
