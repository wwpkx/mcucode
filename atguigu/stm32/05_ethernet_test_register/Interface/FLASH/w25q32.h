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

// 初始化
void W25Q32_Init(void);

// 读取JEDEC ID，进行测试
void W25Q32_ReadID(uint8_t * pMID, uint16_t * pDID);

// 写使能和关闭使能
void W25Q32_WriteEnable(void);
void W25Q32_WriteDisable(void);

// 等待直到状态不为Busy
void W25Q32_Wait4NotBusy(void);

// 段擦除，传入块号和段号
void W25Q32_EraseSector(uint8_t block, uint8_t sector);

// 页写，地址以 块号-段号-页号 形式表示
void W25Q32_WritePage(uint8_t block, uint8_t sector, uint8_t page, uint8_t * data, uint16_t len);

// 读取
void W25Q32_Read(uint32_t addr, uint8_t * buffer, uint16_t len);

#endif
