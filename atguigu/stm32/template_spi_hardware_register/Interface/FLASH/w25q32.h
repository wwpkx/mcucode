/*
 * @Author: wushengran
 * @Date: 2024-10-09 16:18:27
 * @Description: 
 * 
 * Copyright (c) 2024 by atguigu, All Rights Reserved. 
 */
#ifndef __W25Q32_H
#define __W25Q32_H

#include "spi.h"

// ��ʼ��
void W25Q32_Init(void);

// ��ȡJEDEC ID�����в���
void W25Q32_ReadID(uint8_t * pMID, uint16_t * pDID);

// дʹ�ܺ͹ر�ʹ��
void W25Q32_WriteEnable(void);
void W25Q32_WriteDisable(void);

// �ȴ�ֱ��״̬��ΪBusy
void W25Q32_Wait4NotBusy(void);

// �β����������źͶκ�
void W25Q32_EraseSector(uint8_t block, uint8_t sector);

// ҳд����ַ�� ���-�κ�-ҳ�� ��ʽ��ʾ
void W25Q32_WritePage(uint8_t block, uint8_t sector, uint8_t page, uint8_t * data, uint16_t len);

// ��ȡ
void W25Q32_Read(uint32_t addr, uint8_t * buffer, uint16_t len);

#endif
