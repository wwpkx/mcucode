#include "Int_DigitalTube.h"
#include <STC89C5xRC.H>

// ����0-9�ı���
static u8 s_codes[10] = {
    0x3F, // 0
    0x06, // 1
    0x5B, // 2
    0x4F, // 3
    0x66, // 4
    0x6D, // 5
    0x7D, // 6
    0x07, // 7
    0x7F, // 8
    0x6F  // 9
};

static u8 s_buffer[8];

void Int_DigitalTube_Init()
{
    SMG_EN = 0;
    LED_EN = 0;
}

/**
 * @brief �ڲ��������������ĳһλ��ʾ�ض�����
 *
 * @param position Ƭѡ, ������[0-7]
 * @param num_code ��ʾ��Ҫ�����ֱ���
 */
static void Int_DigitalTube_DisplaySingle(u8 position, u8 num_code)
{
    P0 = 0x00;
    // λѡ��P15 P14 P13
    position <<= 3;
    P1 &= 0xC7;
    P1 |= position;

    // ��ѡ��P0
    P0 = num_code;
}

void Int_DigitalTube_DisplayNum(u32 num)
{
    u8 i;
    for (i = 0; i < 8; i++) {
        s_buffer[i] = 0x00;
    }

    if (num == 0) {
        s_buffer[7] = s_codes[0];
        return;
    }

    i = 7;
    while (num > 0) {
        s_buffer[i] = s_codes[num % 10];
        num /= 10;
        i--;
    }
}

void Int_DigitalTube_Refresh()
{
    u8 i;
    for (i = 0; i < 8; i++) {
        Int_DigitalTube_DisplaySingle(i, s_buffer[i]);
        Com_Util_Delay1ms(1);
    }
}
