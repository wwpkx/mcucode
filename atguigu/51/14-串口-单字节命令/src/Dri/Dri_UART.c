#include "Dri_UART.h"
#include <STC89C5xRC.H>

static bit s_is_sending = 0; // 0��δ�ڷ��� 1�����ڷ���
static char s_buffer    = 0;

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
    EA  = 1;
    ES  = 1;
    ET1 = 1;
    RI  = 0;
    TI  = 0;

    // PS = 1;
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

bit Dri_UART_RecevieChar(char *c)
{
    if (s_buffer) {
        *c       = s_buffer;
        s_buffer = 0;
        return 1;
    } else {
        return 0;
    }
}

void Dri_UART_Handler() interrupt 4
{
    if (RI == 1) {
        s_buffer = SBUF;
        RI       = 0;
    }

    if (TI == 1) {
        s_is_sending = 0;
        TI           = 0;
    }
}