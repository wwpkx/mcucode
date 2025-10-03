/*
 * @Author: wushengran
 * @Date: 2025-01-06 16:17:25
 * @Description: 
 * 
 * Copyright (c) 2025 by atguigu, All Rights Reserved. 
 */
#include "iwdg.h"

// ��ʼ��
void IWDG_Init(void)
{
    // 1. �������Ź�
    IWDG->KR = 0xCCCC;

    // 2. ������ʼĴ���
    IWDG->KR = 0x5555;

    // 3. ����Ԥ��Ƶϵ�� 64 - PR = 100
    IWDG->PR = 4;

    // 4. ������װ��ֵ��2499
    IWDG->RLR = 2499;

    // 5. ���¼�������ֵ��ι����
    IWDG_Refresh();
}

// ι����ˢ�¼�������ֵ��
void IWDG_Refresh(void)
{
    IWDG->KR = 0xAAAA;
}
