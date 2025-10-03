/*
 * @Author: wushengran
 * @Date: 2024-12-27 15:06:13
 * @Description: 
 * 
 * Copyright (c) 2024 by atguigu, All Rights Reserved. 
 */
#include "esp32.h"

// 定义全局变量，接收响应缓冲区和长度
uint8_t respBuff[1024];
uint16_t respLen;

// 初始化
void ESP32_Init(void)
{
    // 1. 初始化串口2
    MX_USART2_UART_Init();

    // 2. 发送AT指令，复位ESP32
    uint8_t *cmd = "AT+RST=0\r\n";
    ESP32_SendCmd(cmd, strlen((char *)cmd));

    // 3. 引入延时，确保重启成功
    HAL_Delay(2000);
}

// 发送命令
void ESP32_SendCmd(uint8_t *cmd, uint16_t cmdLen)
{
    // 清空响应缓冲区
    memset(respBuff, 0, 1024);

    // 直接通过串口2发送命令
    HAL_UART_Transmit(&huart2, cmd, cmdLen, 1000);

    // 读取响应
    do
    {
        ESP32_ReadResp(respBuff, &respLen);
    } while (strstr((char *)respBuff, "OK") == NULL);

    // 将响应打印输出
    printf("%.*s\n", respLen, respBuff);
}

// 读取响应
void ESP32_ReadResp(uint8_t buff[], uint16_t *len)
{
    // 串口2接收变长数据
    HAL_UARTEx_ReceiveToIdle(&huart2, buff, 1024, len, 1000);
}
