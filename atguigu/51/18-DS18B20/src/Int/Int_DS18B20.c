#include "Int_DS18B20.h"
#include "Dri_1Wire.h"
char Int_DS18B20_GetTemperature()
{
    u8 ls_byte;
    u8 ms_byte;

    // ��ʼ��
    Dri_1Wire_Init();
    // ����ROM����
    Dri_1Wire_WriteByte(0xCC);
    // ���Ͳ�������
    Dri_1Wire_WriteByte(0x44);

    // ��ʼ��
    Dri_1Wire_Init();
    // ����ROM����
    Dri_1Wire_WriteByte(0xCC);
    // ���Ͷ�ȡ�¶�����
    Dri_1Wire_WriteByte(0xBE);

    ls_byte = Dri_1Wire_ReadByte();
    ms_byte = Dri_1Wire_ReadByte();

    return (ls_byte >> 4) | (ms_byte << 4);
}