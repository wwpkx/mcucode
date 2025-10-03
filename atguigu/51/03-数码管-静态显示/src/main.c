#include <STC89C5xRC.H>
#define SMG_EN P36
#define LED_EN P34
typedef unsigned char u8;

// ����0-9�ı���
static u8 codes[10] = {
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

/**
 * @brief �ڲ��������������ĳһλ��ʾ�ض�����
 *
 * @param position Ƭѡ, ������[0-7]
 * @param num_code ��ʾ��Ҫ�����ֱ���
 */
void DigitalTube_DisplaySingle(u8 position, u8 num_code)
{
    // λѡ��P15 P14 P13
    position <<= 3;
    P1 &= 0xC7;
    P1 |= position;

    // ��ѡ��P0
    P0 = num_code;
}

void main()
{
    SMG_EN = 0;
    LED_EN = 0;
    DigitalTube_DisplaySingle(7, codes[6]);
    while (1) {
    }
}