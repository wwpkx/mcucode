#include "Dri_UART.h"
#include <STC89C5xRC.H>
#include "Com_Util.h"
#include "Dri_Timer0.h"

static bit s_is_sending = 0; // 0��δ�ڷ��� 1�����ڷ���
static char s_buffer[16];
static u8 s_index = 0;

static u8 s_idle_count   = 0;
static bit s_is_complete = 0;

void Dri_UART_Timer0Callback()
{
    s_idle_count++;
    if (s_index > 0 && s_idle_count >= 10) {
        // ���ݽ������
        s_is_complete = 1;
    }
}

void Dri_UART_Init()
{
    // 1.���ڹ���ģʽ
    SM0 = 0;
    SM1 = 1;

    // 2. ������
    // 2.1 SMOD
    PCON &= 0x7F;
    // 2.2��ʱ��1
    // 2.2.1 ����ģʽ
    TMOD &= 0x0F;
    TMOD |= 0x20;
    // 2.2.2 ��ʼֵ
    TL1 = 253;
    TH1 = 253;
    // 2.2.3 ������ʱ��
    TR1 = 1;

    // 3.���������������
    REN = 1;
    SM2 = 0;

    // 4.�����ж��������
    EA = 1;
    ES = 1;
    RI = 0;
    TI = 0;

    // 5.ע����м�⺯��
    Dri_Timer0_RegisterCallback(Dri_UART_Timer0Callback);
}

void Dri_UART_SendChar(char c)
{
    while (s_is_sending == 1);
    s_is_sending = 1;
    SBUF         = c;
}

void Dri_UART_SendStr(char *str)
{
    while (*str != 0) {
        Dri_UART_SendChar(*str);
        str++;
    }
}

bit Dri_UART_RecevieStr(char *str)
{
    if (s_is_complete == 1) {
        u8 i;
        for (i = 0; i < s_index; i++) {
            str[i] = s_buffer[i];
        }
        str[i] = '\0';
        s_is_complete = 0;
        s_index       = 0;
        return 1;
    }
    return 0;
}

void Dri_UART_Handler() interrupt 4
{
    if (RI == 1) {
        s_buffer[s_index++] = SBUF;
        s_idle_count        = 0;
        RI                  = 0;
    }

    if (TI == 1) {
        s_is_sending = 0;
        TI           = 0;
    }
}