#include "Int_DS18B20.h"
#include "Dri_1Wire.h"
char Int_DS18B20_GetTemperature()
{
    u8 ls_byte;
    u8 ms_byte;

    // ³õÊ¼»¯
    Dri_1Wire_Init();
    // ·¢ËÍROMÃüÁî
    Dri_1Wire_WriteByte(0xCC);
    // ·¢ËÍ²âÎÂÃüÁî
    Dri_1Wire_WriteByte(0x44);

    // ³õÊ¼»¯
    Dri_1Wire_Init();
    // ·¢ËÍROMÃüÁî
    Dri_1Wire_WriteByte(0xCC);
    // ·¢ËÍ¶ÁÈ¡ÎÂ¶ÈÃüÁî
    Dri_1Wire_WriteByte(0xBE);

    ls_byte = Dri_1Wire_ReadByte();
    ms_byte = Dri_1Wire_ReadByte();

    return (ls_byte >> 4) | (ms_byte << 4);
}