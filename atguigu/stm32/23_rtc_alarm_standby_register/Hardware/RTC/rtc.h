#ifndef __RTC_H
#define __RTC_H

#include "stm32f10x.h"

// ��ʼ��
void RTC_Init(void);

// �������ӣ�s��֮����
void RTC_SetAlarm(uint32_t s);

#endif
