/*
 * @Author: wushengran
 * @Date: 2024-12-28 11:07:55
 * @Description: 
 * 
 * Copyright (c) 2024 by atguigu, All Rights Reserved. 
 */
#include "wifi.h"

// 定义内部函数，设置Wi-Fi工作模式
static void WIFI_STA_Mode(void);
static void WIFI_AP_Mode(void);

// 初始化，传入WiFi工作模式
void WIFI_Init(WIFI_MODE mode)
{
    // 1. ESP32初始化
    ESP32_Init();

    // 2. 设置工作模式及IP设置
    if (mode == STA)
    {
        WIFI_STA_Mode();
    }
    else if (mode == AP)
    {
        WIFI_AP_Mode();
    }

    printf("Wi-Fi 初始化完成！\n");
}

static void WIFI_STA_Mode(void)
{
    // 1. 设置工作模式为STA
    printf("设置为STA模式...\n");
    uint8_t *cmd = "AT+CWMODE=1\r\n";
    ESP32_SendCmd(cmd, strlen((char *)cmd));

    // 2. 连接到指定的AP
    printf("连接AP...\n");
    cmd = "AT+CWJAP=\"wsr\",\"1234abcd\"\r\n";
    ESP32_SendCmd(cmd, strlen((char *)cmd));

    // 3. 查询本机IP
    printf("本机IP地址为：\n");
    cmd = "AT+CIPSTA?\r\n";
    ESP32_SendCmd(cmd, strlen((char *)cmd));
}

static void WIFI_AP_Mode(void)
{
    // 1. 设置工作模式为AP
    printf("设置为AP模式...\n");
    uint8_t *cmd = "AT+CWMODE=2\r\n";
    ESP32_SendCmd(cmd, strlen((char *)cmd));

    // 2. 设置AP参数
    printf("设置AP...\n");
    cmd = "AT+CWSAP=\"atguigu-esp32\",\"12345678\",5,3\r\n";
    ESP32_SendCmd(cmd, strlen((char *)cmd));

    // 3. 设置本机IP
    printf("设置本机IP地址...\n");
    cmd = "AT+CIPAP=\"192.168.8.1\"\r\n";
    ESP32_SendCmd(cmd, strlen((char *)cmd));
}

// 启动TCP服务器
void WIFI_TCP_ServerStart(void)
{
    // 1. 设置使能多连接
    printf("设置使能多连接...\n");
    uint8_t *cmd = "AT+CIPMUX=1\r\n";
    ESP32_SendCmd(cmd, strlen((char *)cmd));

    // 2. 开启TCP服务器
    printf("开启TCP服务器...\n");
    cmd = "AT+CIPSERVER=1,8080\r\n";
    ESP32_SendCmd(cmd, strlen((char *)cmd));

    // 3. 设置IPD数据格式（显示详情）
    printf("设置IPD数据格式...\n");
    cmd = "AT+CIPDINFO=1\r\n";
    ESP32_SendCmd(cmd, strlen((char *)cmd));
}

// 发送数据（包含连接ID）
void WIFI_TCP_SendData(uint8_t id, uint8_t *data, uint16_t len)
{
    // 1. 设置发送数据的连接ID和数据长度
    printf("给对方发送数据...\n");

    uint8_t sendCmd[50] = {0};
    sprintf((char *)sendCmd, "AT+CIPSEND=%d,%d\r\n", id, len);
    ESP32_SendCmd(sendCmd, strlen((char *)sendCmd));

    // 2. 进入数据模式，直接通过串口发送数据
    HAL_UART_Transmit(&huart2, data, len, 1000);
}

// 用临时缓冲区接收IPD数据
uint8_t tempBuff[1024];
uint16_t tempLen;

// 读取数据（包含连接ID、对端IP和端口号）
void WIFI_TCP_ReadData(uint8_t rxBuff[], uint16_t *rxLen, uint8_t *id, uint8_t *ip, uint16_t *port)
{
    // 1. 从串口接收数据
    HAL_UARTEx_ReceiveToIdle(&huart2, tempBuff, 1024, &tempLen, 1000);

    // 2. 从接收到的IPD数据中解析数据
    // 判断是否以+IPD开头，进行过滤
    if ( strstr((char *)tempBuff, "+IPD") != NULL )
    {
        // 利用sscanf和正则表达式提取所有的信息
        sscanf((char *)tempBuff, "%*[\r\n]+IPD,%hhu,%hu,\"%[^\"]\",%hu",
             id, rxLen, ip, port);
        
        // 将真正的数据复制到传入的缓冲区参数中
        char * pData = strstr((char *)tempBuff, ":") + 1;
        memcpy(rxBuff, pData, *rxLen);
    }  
}

