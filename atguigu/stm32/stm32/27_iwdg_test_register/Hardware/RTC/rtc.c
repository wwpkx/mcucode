/*
 * @Author: wushengran
 * @Date: 2025-01-03 16:22:29
 * @Description:
 *
 * Copyright (c) 2025 by atguigu, All Rights Reserved.
 */
#include "rtc.h"

// 初始化
void RTC_Init(void)
{
    // 1. 后备域统一配置
    // 1.1 开启PWR时钟
    RCC->APB1ENR |= RCC_APB1ENR_PWREN;

    // 1.2 放开后备域的写保护
    PWR->CR |= PWR_CR_DBP;

    // // 1.3 软件复位整个备份域
    // RCC->BDCR |= RCC_BDCR_BDRST;

    // // 1.4 解除备份域复位
    // RCC->BDCR &= ~RCC_BDCR_BDRST;

    // 2. 配置RTC时钟源以及开启RTC
    // 2.1 开启RTC时钟
    RCC->BDCR |= RCC_BDCR_RTCEN;

    // 2.2 打开LSE并等待启动完成
    RCC->BDCR |= RCC_BDCR_LSEON;
    while (!(RCC->BDCR & RCC_BDCR_LSERDY))
    {
    }

    // 2.3 选择LSE作为RTC的时钟源
    RCC->BDCR &= ~RCC_BDCR_RTCSEL;
    RCC->BDCR |= RCC_BDCR_RTCSEL_0;

    // 3. RTC寄存器的配置
    // 3.1 查询RTOFF位，直到变为1
    while (!(RTC->CRL & RTC_CRL_RTOFF))
    {
    }

    // 3.2 进入配置模式
    RTC->CRL |= RTC_CRL_CNF;

    // 3.3 设置预分频系数 32767，产生秒脉冲
    RTC->PRLH = 0;
    RTC->PRLL = 0x7fff;

    // 3.4 退出配置模式
    RTC->CRL &= ~RTC_CRL_CNF;

    // 3.5 查询RTOFF位，直到变为1
    while (!(RTC->CRL & RTC_CRL_RTOFF))
    {
    }
}

// 设置闹钟，s秒之后唤醒
void RTC_SetAlarm(uint32_t s)
{
    // 0. 先清除闹钟标志
    RTC->CRL &= ~RTC_CRL_ALRF;

    // 1. 查询RTOFF位，直到变为1
    while (!(RTC->CRL & RTC_CRL_RTOFF))
    {
    }

    // 2. 进入配置模式
    RTC->CRL |= RTC_CRL_CNF;

    // 3. 设置寄存器
    // 3.1 计数器 CNT = 0
    RTC->CNTH = 0;
    RTC->CNTL = 0;

    // 3.2 闹钟 ALR = s - 1
    s -= 1;
    RTC->ALRH = (s >> 16) & 0xffff;
    RTC->ALRL = (s >> 0) & 0xffff;

    // 4. 退出配置模式
    RTC->CRL &= ~RTC_CRL_CNF;

    // 5. 查询RTOFF位，直到变为1
    while (!(RTC->CRL & RTC_CRL_RTOFF))
    {
    }
}

// 设置当前时间（UNIX时间戳）
void RTC_SetTimestamp(uint32_t ts)
{
    // 1. 查询RTOFF位，直到变为1
    while (!(RTC->CRL & RTC_CRL_RTOFF))
    {
    }

    // 2. 进入配置模式
    RTC->CRL |= RTC_CRL_CNF;

    // 3. 设置CNT寄存器
    RTC->CNTH = (ts >> 16) & 0xffff;
    RTC->CNTL = (ts >> 0) & 0xffff;

    // 4. 退出配置模式
    RTC->CRL &= ~RTC_CRL_CNF;

    // 5. 查询RTOFF位，直到变为1
    while (!(RTC->CRL & RTC_CRL_RTOFF))
    {
    }
}

// 获取日历时间（年月日时分秒）
void RTC_GetDateTime(DateTime *dateTime)
{
    // 1. 等待寄存器同步
    while ( !(RTC->CRL & RTC_CRL_RSF) )
    {}
    
    // 2. 读取当前计数值（秒数）
    uint32_t second = RTC->CNTH << 16 | RTC->CNTL;

    // 3. 将秒数转换成tm结构体对象
    struct tm* ptm = localtime(&second);

    // 4. 基于tm构建自定义的结构体对象
    dateTime->year = ptm->tm_year + 1900;
    dateTime->month = ptm->tm_mon + 1;
    dateTime->day = ptm->tm_mday;
    dateTime->hour = ptm->tm_hour;
    dateTime->minute = ptm->tm_min;
    dateTime->second = ptm->tm_sec;
}
