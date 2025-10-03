#include "Dri_Timer0.h"
#include <STC89C5xRC.H>
#include "Com_Util.h"
#include <STDIO.H>
#define MAX_CALLBACK_COUNT 4

static Timer0_Callback s_callbacks[MAX_CALLBACK_COUNT];

void Dri_Timer0_Init()
{
    u8 i;
    // 1.�����ж�
    EA  = 1;
    ET0 = 1;

    // 2.����ģʽ��GATE=0��C/T=0��M1=0��M0=1
    TMOD &= 0xF0;
    TMOD |= 0x01;

    // 3.��ʼֵ
    TL0 = 64614;
    TH0 = 64614 >> 8;

    // 4.����
    TR0 = 1;

    // 5.��ʼ������ָ����
    for (i = 0; i < MAX_CALLBACK_COUNT; i++) {
        s_callbacks[i] = NULL;
    }
}

bit Dri_Timer0_RegisterCallback(Timer0_Callback callback)
{
    u8 i;
    // ȷ�������ظ�ע��
    for (i = 0; i < MAX_CALLBACK_COUNT; i++) {
        if (s_callbacks[i] == callback) {
            return 1;
        }
    }

    // Ѱ�ҿ�λ
    for (i = 0; i < MAX_CALLBACK_COUNT; i++) {
        if (s_callbacks[i] == NULL) {
            s_callbacks[i] = callback;
            return 1;
        }
    }

    return 0;
}

bit Dri_Timer0_DeregisterCallback(Timer0_Callback callback)
{
    u8 i;
    for (i = 0; i < MAX_CALLBACK_COUNT; i++) {
        if (s_callbacks[i] == callback) {
            s_callbacks[i] = NULL;
            return 1;
        }
    }

    return 0;
}

void Dri_Timer0_Handler() interrupt 1
{
    u8 i;
    // 1.����װ�س�ʼֵ
    TL0 = 64614;
    TH0 = 64614 >> 8;

    // 2.��ѯ����ָ������
    for (i = 0; i < MAX_CALLBACK_COUNT; i++) {
        if (s_callbacks[i] != NULL) {
            s_callbacks[i]();
        }
    }
}