/*
 * @Author: wushengran
 * @Date: 2025-01-06 10:23:40
 * @Description: 
 * 
 * Copyright (c) 2025 by atguigu, All Rights Reserved. 
 */
#ifndef __RTC_H
#define __RTC_H

#include "stm32f10x.h"
#include <time.h>

// �Զ�������ʱ��ṹ������
typedef struct
{
    uint16_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
} DateTime;

// ��ʼ��
void RTC_Init(void);

// �������ӣ�s��֮����
void RTC_SetAlarm(uint32_t s);

// ���õ�ǰʱ�䣨UNIXʱ�����
void RTC_SetTimestamp(uint32_t ts);

// ��ȡ����ʱ�䣨������ʱ���룩
void RTC_GetDateTime(DateTime * dateTime);

#endif
