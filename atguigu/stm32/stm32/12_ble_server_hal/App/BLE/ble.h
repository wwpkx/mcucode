#ifndef __BLE_H
#define __BLE_H

#include "esp32.h"

// ��ʼ��
void BLE_Init(void);

// ��������
void BLE_SendData(uint8_t txBuff[], uint16_t txLen);

// ���գ���ȡ������
void BLE_ReadData(uint8_t rxBuff[], uint16_t *rxLen);

#endif
