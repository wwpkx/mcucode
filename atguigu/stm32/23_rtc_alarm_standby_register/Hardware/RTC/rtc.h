#ifndef __RTC_H
#define __RTC_H

#include "stm32f10x.h"

// 初始化
void RTC_Init(void);

// 设置闹钟，s秒之后唤醒
void RTC_SetAlarm(uint32_t s);

#endif
