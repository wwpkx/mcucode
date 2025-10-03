#include "Int_EEPROM.h"
#include "Dri_IIC.h"
#include "Com_Util.h"
#define DEV_ADDR  0xA0
#define PAGE_SIZE 16

void Int_EEPROM_WritePage(u8 addr, u8 bytes[], u8 len)
{
    u8 i;
    Dri_IIC_Start();
    Dri_IIC_SendByte(DEV_ADDR);
    Dri_IIC_ReceiveAck();
    Dri_IIC_SendByte(addr);
    Dri_IIC_ReceiveAck();
    for (i = 0; i < len; i++) {
        Dri_IIC_SendByte(bytes[i]);
        Dri_IIC_ReceiveAck();
    }
    Dri_IIC_Stop();
    Com_Util_Delay1ms(5);
}

// �ݹ�д��
// void Int_EEPROM_WriteBytes(u8 addr, u8 bytes[], u8 len)
// {
//     u8 page_remain = PAGE_SIZE - addr % PAGE_SIZE;
//     if (len > page_remain) {
//         // ��ǰҳ�ռ䲻��
//         // д����ǰҳ
//         Int_EEPROM_WritePage(addr, bytes, page_remain);
//         // дʣ������
//         Int_EEPROM_WriteBytes(addr + page_remain, bytes + page_remain, len - page_remain);
//     } else {
//         // ��ǰҳ�ռ����
//         Int_EEPROM_WritePage(addr, bytes, len);
//     }
// }

// ѭ��д��
void Int_EEPROM_WriteBytes(u8 addr, u8 bytes[], u8 len)
{
    while (1) {
        u8 page_remain = PAGE_SIZE - addr % PAGE_SIZE;
        if (len > page_remain) {
            // ��ǰҳ�ռ䲻��
            // д����ǰҳ
            Int_EEPROM_WritePage(addr, bytes, page_remain);
            // д��ʣ������
            addr += page_remain;
            bytes += page_remain;
            len -= page_remain;
        } else {
            // ��ǰҳ�ռ����
            Int_EEPROM_WritePage(addr, bytes, len);
            break;
        }
    }
}

void Int_EEPROM_ReadBytes(u8 addr, u8 bytes[], u8 len)
{
    u8 i;
    Dri_IIC_Start();
    Dri_IIC_SendByte(DEV_ADDR);
    Dri_IIC_ReceiveAck();
    Dri_IIC_SendByte(addr);
    Dri_IIC_ReceiveAck();
    Dri_IIC_Start();
    Dri_IIC_SendByte(DEV_ADDR + 1);
    Dri_IIC_ReceiveAck();
    for (i = 0; i < len; i++) {
        bytes[i] = Dri_IIC_ReceiveByte();
        Dri_IIC_SendAck(i == len - 1 ? 1 : 0);
    }
    Dri_IIC_Stop();
}
