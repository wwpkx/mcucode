/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    can.c
  * @brief   This file provides code for the configuration
  *          of the CAN instances.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "can.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

CAN_HandleTypeDef hcan;

/* CAN init function */
void MX_CAN_Init(void)
{

  /* USER CODE BEGIN CAN_Init 0 */

  /* USER CODE END CAN_Init 0 */

  /* USER CODE BEGIN CAN_Init 1 */

  /* USER CODE END CAN_Init 1 */
  hcan.Instance = CAN1;
  hcan.Init.Prescaler = 36;
  hcan.Init.Mode = CAN_MODE_NORMAL;
  hcan.Init.SyncJumpWidth = CAN_SJW_2TQ;
  hcan.Init.TimeSeg1 = CAN_BS1_3TQ;
  hcan.Init.TimeSeg2 = CAN_BS2_6TQ;
  hcan.Init.TimeTriggeredMode = DISABLE;
  hcan.Init.AutoBusOff = ENABLE;
  hcan.Init.AutoWakeUp = ENABLE;
  hcan.Init.AutoRetransmission = DISABLE;
  hcan.Init.ReceiveFifoLocked = DISABLE;
  hcan.Init.TransmitFifoPriority = DISABLE;
  if (HAL_CAN_Init(&hcan) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN CAN_Init 2 */

  /* USER CODE END CAN_Init 2 */

}

void HAL_CAN_MspInit(CAN_HandleTypeDef* canHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(canHandle->Instance==CAN1)
  {
  /* USER CODE BEGIN CAN1_MspInit 0 */

  /* USER CODE END CAN1_MspInit 0 */
    /* CAN1 clock enable */
    __HAL_RCC_CAN1_CLK_ENABLE();

    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**CAN GPIO Configuration
    PB8     ------> CAN_RX
    PB9     ------> CAN_TX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_8;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    __HAL_AFIO_REMAP_CAN1_2();

  /* USER CODE BEGIN CAN1_MspInit 1 */

  /* USER CODE END CAN1_MspInit 1 */
  }
}

void HAL_CAN_MspDeInit(CAN_HandleTypeDef* canHandle)
{

  if(canHandle->Instance==CAN1)
  {
  /* USER CODE BEGIN CAN1_MspDeInit 0 */

  /* USER CODE END CAN1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_CAN1_CLK_DISABLE();

    /**CAN GPIO Configuration
    PB8     ------> CAN_RX
    PB9     ------> CAN_TX
    */
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_8|GPIO_PIN_9);

  /* USER CODE BEGIN CAN1_MspDeInit 1 */

  /* USER CODE END CAN1_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */

// ���ù�����
void CAN_FilterConfig(void)
{
  // ����ṹ�����
  CAN_FilterTypeDef filterConfig;

  // 1. ����FIFO0
  filterConfig.FilterFIFOAssignment = CAN_RX_FIFO0;

  // 2. ���������� 0
  filterConfig.FilterBank = 0;

  // 3. ����ģʽ������λģʽ
  filterConfig.FilterMode = CAN_FILTERMODE_IDMASK;

  // 4. λ��32λ
  filterConfig.FilterScale = CAN_FILTERSCALE_32BIT;

  // 5. ����ID�Ĵ���-FR1
  filterConfig.FilterIdHigh = 0x0000;
  filterConfig.FilterIdLow = 0x0000;

  // 6. ��������Ĵ���-FR2
  filterConfig.FilterMaskIdHigh = 0x0000;
  filterConfig.FilterMaskIdLow = 0x0000;

  // 7. �����������
  filterConfig.FilterActivation = ENABLE;

  HAL_CAN_ConfigFilter(&hcan, &filterConfig);
}

// ���ͱ���
void CAN_SendMsg(uint16_t stdID, uint8_t * data, uint8_t len)
{
  // 1. ����Ƿ��п������䣬�ȴ�ֱ�����������
  while ( HAL_CAN_GetTxMailboxesFreeLevel(&hcan) == 0 )
  {
  }
  
  // 2. ��������֡��Ϣ
  CAN_TxHeaderTypeDef txHeader;
  txHeader.StdId = stdID;
  txHeader.IDE = CAN_ID_STD;  // ��׼֡
  txHeader.RTR = CAN_RTR_DATA;  // ����֡
  txHeader.DLC = len;

  // 3. ���ͱ�����Ϣ
  uint32_t txMailBox;
  HAL_CAN_AddTxMessage(&hcan, &txHeader, data, &txMailBox);

  // 4. �ȴ����ͳɹ�
  while (__HAL_CAN_GET_FLAG(&hcan, CAN_FLAG_TXOK0) == 0)
  {
  }
}

// ���ձ���
void CAN_ReceiveMsg(RxMsg rxMsg[], uint8_t * msgCount)
{
  // 1. ���FIFO0���յ��ı��ĸ���
  * msgCount = HAL_CAN_GetRxFifoFillLevel(&hcan, CAN_RX_FIFO0);

  // 2. �����������б���
  CAN_RxHeaderTypeDef rxHeader;
  for (uint8_t i = 0; i < *msgCount; i++)
  {
    HAL_CAN_GetRxMessage(&hcan, CAN_RX_FIFO0, &rxHeader, rxMsg[i].data);
    rxMsg[i].stdID = rxHeader.StdId;
    rxMsg[i].len = rxHeader.DLC;
  }
}

/* USER CODE END 1 */
