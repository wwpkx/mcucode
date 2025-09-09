/*
 * @Author: wushengran
 * @Date: 2025-01-03 16:22:29
 * @Description:
 *
 * Copyright (c) 2025 by atguigu, All Rights Reserved.
 */
#include "rtc.h"

// ��ʼ��
void RTC_Init(void)
{
    // 1. ����ͳһ����
    // 1.1 ����PWRʱ��
    RCC->APB1ENR |= RCC_APB1ENR_PWREN;

    // 1.2 �ſ������д����
    PWR->CR |= PWR_CR_DBP;

    // // 1.3 �����λ����������
    // RCC->BDCR |= RCC_BDCR_BDRST;

    // // 1.4 ���������λ
    // RCC->BDCR &= ~RCC_BDCR_BDRST;

    // 2. ����RTCʱ��Դ�Լ�����RTC
    // 2.1 ����RTCʱ��
    RCC->BDCR |= RCC_BDCR_RTCEN;

    // 2.2 ��LSE���ȴ��������
    RCC->BDCR |= RCC_BDCR_LSEON;
    while (!(RCC->BDCR & RCC_BDCR_LSERDY))
    {
    }

    // 2.3 ѡ��LSE��ΪRTC��ʱ��Դ
    RCC->BDCR &= ~RCC_BDCR_RTCSEL;
    RCC->BDCR |= RCC_BDCR_RTCSEL_0;

    // 3. RTC�Ĵ���������
    // 3.1 ��ѯRTOFFλ��ֱ����Ϊ1
    while (!(RTC->CRL & RTC_CRL_RTOFF))
    {
    }

    // 3.2 ��������ģʽ
    RTC->CRL |= RTC_CRL_CNF;

    // 3.3 ����Ԥ��Ƶϵ�� 32767������������
    RTC->PRLH = 0;
    RTC->PRLL = 0x7fff;

    // 3.4 �˳�����ģʽ
    RTC->CRL &= ~RTC_CRL_CNF;

    // 3.5 ��ѯRTOFFλ��ֱ����Ϊ1
    while (!(RTC->CRL & RTC_CRL_RTOFF))
    {
    }
}

// �������ӣ�s��֮����
void RTC_SetAlarm(uint32_t s)
{
    // 0. ��������ӱ�־
    RTC->CRL &= ~RTC_CRL_ALRF;

    // 1. ��ѯRTOFFλ��ֱ����Ϊ1
    while (!(RTC->CRL & RTC_CRL_RTOFF))
    {
    }

    // 2. ��������ģʽ
    RTC->CRL |= RTC_CRL_CNF;

    // 3. ���üĴ���
    // 3.1 ������ CNT = 0
    RTC->CNTH = 0;
    RTC->CNTL = 0;

    // 3.2 ���� ALR = s - 1
    s -= 1;
    RTC->ALRH = (s >> 16) & 0xffff;
    RTC->ALRL = (s >> 0) & 0xffff;

    // 4. �˳�����ģʽ
    RTC->CRL &= ~RTC_CRL_CNF;

    // 5. ��ѯRTOFFλ��ֱ����Ϊ1
    while (!(RTC->CRL & RTC_CRL_RTOFF))
    {
    }
}

// ���õ�ǰʱ�䣨UNIXʱ�����
void RTC_SetTimestamp(uint32_t ts)
{
    // 1. ��ѯRTOFFλ��ֱ����Ϊ1
    while (!(RTC->CRL & RTC_CRL_RTOFF))
    {
    }

    // 2. ��������ģʽ
    RTC->CRL |= RTC_CRL_CNF;

    // 3. ����CNT�Ĵ���
    RTC->CNTH = (ts >> 16) & 0xffff;
    RTC->CNTL = (ts >> 0) & 0xffff;

    // 4. �˳�����ģʽ
    RTC->CRL &= ~RTC_CRL_CNF;

    // 5. ��ѯRTOFFλ��ֱ����Ϊ1
    while (!(RTC->CRL & RTC_CRL_RTOFF))
    {
    }
}

// ��ȡ����ʱ�䣨������ʱ���룩
void RTC_GetDateTime(DateTime *dateTime)
{
    // 1. �ȴ��Ĵ���ͬ��
    while ( !(RTC->CRL & RTC_CRL_RSF) )
    {}
    
    // 2. ��ȡ��ǰ����ֵ��������
    uint32_t second = RTC->CNTH << 16 | RTC->CNTL;

    // 3. ������ת����tm�ṹ�����
    struct tm* ptm = localtime(&second);

    // 4. ����tm�����Զ���Ľṹ�����
    dateTime->year = ptm->tm_year + 1900;
    dateTime->month = ptm->tm_mon + 1;
    dateTime->day = ptm->tm_mday;
    dateTime->hour = ptm->tm_hour;
    dateTime->minute = ptm->tm_min;
    dateTime->second = ptm->tm_sec;
}
