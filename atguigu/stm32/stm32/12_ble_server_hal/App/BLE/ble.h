#ifndef __BLE_H
#define __BLE_H

#include "esp32.h"

// 初始化
void BLE_Init(void);

// 发送数据
void BLE_SendData(uint8_t txBuff[], uint16_t txLen);

// 接收（读取）数据
void BLE_ReadData(uint8_t rxBuff[], uint16_t *rxLen);

#endif
