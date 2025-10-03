/*
 * @Author: wushengran
 * @Date: 2024-10-09 16:18:21
 * @Description:
 *
 * Copyright (c) 2024 by atguigu, All Rights Reserved.
 */
#include "w25q32.h"

// 初始化
void W25Q32_Init(void)
{
    MX_SPI1_Init();
}

// 读取JEDEC ID，进行测试
void W25Q32_ReadID(uint8_t *pMID, uint16_t *pDID)
{
    // 1. 开启SPI通信，片选使能
    SPI_Start();

    // 2. 发送 9fh 指令
    SPI_SwapByte(0x9f);

    // 3. 依次读取三个字节的ID
    // 3.1 第一个字节，生产商ID
    *pMID = SPI_SwapByte(0xff);

    // 3.2 后两个字节，设备ID
    *pDID = 0;
    *pDID |= SPI_SwapByte(0xff) << 8;
    *pDID |= SPI_SwapByte(0xff) & 0xff;

    // 4. 关闭SPI通信
    SPI_Stop();
}

// 开启写使能和关闭使能
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

// 等待直到状态不为Busy
void W25Q32_Wait4NotBusy(void)
{
    // 1. 打开片选
    SPI_Start();

    // 2. 发送指令，只需要发送一次，就会不停地收到状态寄存器的值
    SPI_SwapByte(0x05);

    // 3. 等待接收到的数据末位（BUSY）为0
    while (SPI_SwapByte(0xff) & 0x01)
    {
    }

    // 4. 关闭片选
    SPI_Stop();
}

// 段擦除，传入块号和段号
void W25Q32_EraseSector(uint8_t block, uint8_t sector)
{
    // 1. 要等待状态不为忙
    W25Q32_Wait4NotBusy();

    // 2. 擦除前需要开启写使能
    W25Q32_WriteEnable();

    // 3. 拼接出完整的22位地址（3个字节）
    uint32_t addr = (block << 16) + (sector << 12);

    // 4. 打开片选，开启SPI通信
    SPI_Start();

    // 5. 发送指令
    SPI_SwapByte(0x20);

    // 6. 逐个字节发送地址
    SPI_SwapByte(addr >> 16 & 0xff); // 发送地址的第一个字节
    SPI_SwapByte(addr >> 8 & 0xff);  // 发送地址的第二个字节
    SPI_SwapByte(addr >> 0 & 0xff);  // 发送地址的第三个字节

    // 7. 关闭片选
    SPI_Stop();

    // 8. 关闭写使能
    W25Q32_WriteDisable();
}

// 页写，地址以 块号-段号-页号 形式表示
void W25Q32_WritePage(uint8_t block, uint8_t sector, uint8_t page, uint8_t *data, uint16_t len)
{
    // 1. 要等待状态不为忙
    W25Q32_Wait4NotBusy();

    // 2. 写入前需要开启写使能
    W25Q32_WriteEnable();

    // 3. 拼接出完整的22位地址（3个字节）
    uint32_t addr = (block << 16) + (sector << 12) + (page << 8);

    // 4. 打开片选，开启SPI通信
    SPI_Start();

    // 5. 发送指令
    SPI_SwapByte(0x02);

    // 6. 逐个字节发送地址
    SPI_SwapByte(addr >> 16 & 0xff); // 发送地址的第一个字节
    SPI_SwapByte(addr >> 8 & 0xff);  // 发送地址的第二个字节
    SPI_SwapByte(addr >> 0 & 0xff);  // 发送地址的第三个字节

    // 7. 发送数据
    for (uint16_t i = 0; i < len; i++)
    {
        SPI_SwapByte(data[i]);
    }

    // 8. 关闭片选
    SPI_Stop();

    // 9. 关闭写使能
    W25Q32_WriteDisable();
}

// 读取
void W25Q32_Read(uint32_t addr, uint8_t *buffer, uint16_t len)
{
    // 1. 要等待状态不为忙
    W25Q32_Wait4NotBusy();

    // 2. 打开片选，开启SPI通信
    SPI_Start();

    // 3. 发送指令
    SPI_SwapByte(0x03);

    // 4. 逐个字节发送地址
    SPI_SwapByte(addr >> 16 & 0xff); // 发送地址的第一个字节
    SPI_SwapByte(addr >> 8 & 0xff);  // 发送地址的第二个字节
    SPI_SwapByte(addr >> 0 & 0xff);  // 发送地址的第三个字节

    // 5. 读取数据
    for (uint16_t i = 0; i < len; i++)
    {
        buffer[i] = SPI_SwapByte(0xff);
    }
    
    // 6. 关闭片选
    SPI_Stop();
}
