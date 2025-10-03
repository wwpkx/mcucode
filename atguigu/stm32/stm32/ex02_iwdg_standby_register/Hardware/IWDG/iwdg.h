#ifndef __IWDG_H
#define __IWDG_H

#include "stm32f10x.h"

// 初始化
void IWDG_Init(void);

// 喂狗（刷新计数器的值）
void IWDG_Refresh(void);

#endif
