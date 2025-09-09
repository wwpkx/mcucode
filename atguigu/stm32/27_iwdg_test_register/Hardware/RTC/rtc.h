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

// 自定义日历时间结构体类型
typedef struct
{
    uint16_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
} DateTime;

// 初始化
void RTC_Init(void);

// 设置闹钟，s秒之后唤醒
void RTC_SetAlarm(uint32_t s);

// 设置当前时间（UNIX时间戳）
void RTC_SetTimestamp(uint32_t ts);

// 获取日历时间（年月日时分秒）
void RTC_GetDateTime(DateTime * dateTime);

#endif
