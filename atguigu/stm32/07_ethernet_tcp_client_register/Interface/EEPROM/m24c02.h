#ifndef __M24C02_H
#define __M24C02_H

#include "i2c.h"

// �궨���豸�루д��ַ/����ַ��
#define W_ADDR 0xA0
#define R_ADDR 0xA1

// ��ʼ��
void M24C02_Init(void);

// д��һ���ֽڣ�ָ��Ҫд����ڲ���ַ
void M24C02_WriteByte(uint8_t innerAddr, uint8_t byte);

// ��ȡһ���ֽڣ������ַ����
uint8_t M24C02_ReadByte(uint8_t innerAddr);

// ����д�����ֽڣ�ҳд��
void M24C02_WriteBytes(uint8_t innerAddr, uint8_t * bytes, uint8_t size);

// ������ȡ����ֽ�
void M24C02_ReadBytes(uint8_t innerAddr, uint8_t * bytes, uint8_t size);

#endif
