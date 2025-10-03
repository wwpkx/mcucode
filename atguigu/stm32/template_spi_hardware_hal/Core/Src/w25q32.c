/*
 * @Author: wushengran
 * @Date: 2024-10-09 16:18:21
 * @Description:
 *
 * Copyright (c) 2024 by atguigu, All Rights Reserved.
 */
#include "w25q32.h"

// ��ʼ��
void W25Q32_Init(void)
{
    MX_SPI1_Init();
}

// ��ȡJEDEC ID�����в���
void W25Q32_ReadID(uint8_t *pMID, uint16_t *pDID)
{
    // 1. ����SPIͨ�ţ�Ƭѡʹ��
    SPI_Start();

    // 2. ���� 9fh ָ��
    SPI_SwapByte(0x9f);

    // 3. ���ζ�ȡ�����ֽڵ�ID
    // 3.1 ��һ���ֽڣ�������ID
    *pMID = SPI_SwapByte(0xff);

    // 3.2 �������ֽڣ��豸ID
    *pDID = 0;
    *pDID |= SPI_SwapByte(0xff) << 8;
    *pDID |= SPI_SwapByte(0xff) & 0xff;

    // 4. �ر�SPIͨ��
    SPI_Stop();
}

// ����дʹ�ܺ͹ر�ʹ��
void W25Q32_WriteEnable(void)
{
    SPI_Start();
    SPI_SwapByte(0x06);
    SPI_Stop();
}
void W25Q32_WriteDisable(void)
{
    SPI_Start();
    SPI_SwapByte(0x04);
    SPI_Stop();
}

// �ȴ�ֱ��״̬��ΪBusy
void W25Q32_Wait4NotBusy(void)
{
    // 1. ��Ƭѡ
    SPI_Start();

    // 2. ����ָ�ֻ��Ҫ����һ�Σ��ͻ᲻ͣ���յ�״̬�Ĵ�����ֵ
    SPI_SwapByte(0x05);

    // 3. �ȴ����յ�������ĩλ��BUSY��Ϊ0
    while (SPI_SwapByte(0xff) & 0x01)
    {
    }

    // 4. �ر�Ƭѡ
    SPI_Stop();
}

// �β����������źͶκ�
void W25Q32_EraseSector(uint8_t block, uint8_t sector)
{
    // 1. Ҫ�ȴ�״̬��Ϊæ
    W25Q32_Wait4NotBusy();

    // 2. ����ǰ��Ҫ����дʹ��
    W25Q32_WriteEnable();

    // 3. ƴ�ӳ�������22λ��ַ��3���ֽڣ�
    uint32_t addr = (block << 16) + (sector << 12);

    // 4. ��Ƭѡ������SPIͨ��
    SPI_Start();

    // 5. ����ָ��
    SPI_SwapByte(0x20);

    // 6. ����ֽڷ��͵�ַ
    SPI_SwapByte(addr >> 16 & 0xff); // ���͵�ַ�ĵ�һ���ֽ�
    SPI_SwapByte(addr >> 8 & 0xff);  // ���͵�ַ�ĵڶ����ֽ�
    SPI_SwapByte(addr >> 0 & 0xff);  // ���͵�ַ�ĵ������ֽ�

    // 7. �ر�Ƭѡ
    SPI_Stop();

    // 8. �ر�дʹ��
    W25Q32_WriteDisable();
}

// ҳд����ַ�� ���-�κ�-ҳ�� ��ʽ��ʾ
void W25Q32_WritePage(uint8_t block, uint8_t sector, uint8_t page, uint8_t *data, uint16_t len)
{
    // 1. Ҫ�ȴ�״̬��Ϊæ
    W25Q32_Wait4NotBusy();

    // 2. д��ǰ��Ҫ����дʹ��
    W25Q32_WriteEnable();

    // 3. ƴ�ӳ�������22λ��ַ��3���ֽڣ�
    uint32_t addr = (block << 16) + (sector << 12) + (page << 8);

    // 4. ��Ƭѡ������SPIͨ��
    SPI_Start();

    // 5. ����ָ��
    SPI_SwapByte(0x02);

    // 6. ����ֽڷ��͵�ַ
    SPI_SwapByte(addr >> 16 & 0xff); // ���͵�ַ�ĵ�һ���ֽ�
    SPI_SwapByte(addr >> 8 & 0xff);  // ���͵�ַ�ĵڶ����ֽ�
    SPI_SwapByte(addr >> 0 & 0xff);  // ���͵�ַ�ĵ������ֽ�

    // 7. ��������
    for (uint16_t i = 0; i < len; i++)
    {
        SPI_SwapByte(data[i]);
    }

    // 8. �ر�Ƭѡ
    SPI_Stop();

    // 9. �ر�дʹ��
    W25Q32_WriteDisable();
}

// ��ȡ
void W25Q32_Read(uint32_t addr, uint8_t *buffer, uint16_t len)
{
    // 1. Ҫ�ȴ�״̬��Ϊæ
    W25Q32_Wait4NotBusy();

    // 2. ��Ƭѡ������SPIͨ��
    SPI_Start();

    // 3. ����ָ��
    SPI_SwapByte(0x03);

    // 4. ����ֽڷ��͵�ַ
    SPI_SwapByte(addr >> 16 & 0xff); // ���͵�ַ�ĵ�һ���ֽ�
    SPI_SwapByte(addr >> 8 & 0xff);  // ���͵�ַ�ĵڶ����ֽ�
    SPI_SwapByte(addr >> 0 & 0xff);  // ���͵�ַ�ĵ������ֽ�

    // 5. ��ȡ����
    for (uint16_t i = 0; i < len; i++)
    {
        buffer[i] = SPI_SwapByte(0xff);
    }
    
    // 6. �ر�Ƭѡ
    SPI_Stop();
}
