/*
 * @Author: wushengran
 * @Date: 2024-12-30 15:15:53
 * @Description: 
 * 
 * Copyright (c) 2024 by atguigu, All Rights Reserved. 
 */
#include "ble.h"

// 初始化
void BLE_Init(void)
{
    // 1. 初始化ESP32
    ESP32_Init();

    // 2. 设置蓝牙模块角色：2-server
    printf("设置模块角色为：服务器...\n");
    uint8_t *cmd = "AT+BLEINIT=2\r\n";
    ESP32_SendCmd(cmd, strlen((char *)cmd));

    // 3. 服务端创建服务
    printf("服务端创建服务...\n");
    cmd = "AT+BLEGATTSSRVCRE\r\n";
    ESP32_SendCmd(cmd, strlen((char *)cmd));

    // 4. 服务端开启服务
    printf("服务端开启服务...\n");
    cmd = "AT+BLEGATTSSRVSTART\r\n";
    ESP32_SendCmd(cmd, strlen((char *)cmd));

    // 5. 获取服务端MAC地址
    printf("获取服务端MAC地址...\n");
    cmd = "AT+BLEADDR?\r\n";
    ESP32_SendCmd(cmd, strlen((char *)cmd));

    // 6. 设置蓝牙广播参数
    printf("设置蓝牙广播参数...\n");
    cmd = "AT+BLEADVPARAM=50,50,0,0,7,0\r\n";
    ESP32_SendCmd(cmd, strlen((char *)cmd));

    // 7. 设置广播数据
    printf("设置广播数据...\n");
    cmd = "AT+BLEADVDATAEX=\"atguigu-ble\",\"A123\",\"0102030405\",1\r\n";
    ESP32_SendCmd(cmd, strlen((char *)cmd));

    // 8. 开始广播
    printf("开始广播...\n");
    cmd = "AT+BLEADVSTART\r\n";
    ESP32_SendCmd(cmd, strlen((char *)cmd));

    // 9. 配置 BLE SPP参数
    printf("配置 BLE SPP 参数...\n");
    cmd = "AT+BLESPPCFG=1,1,7,1,5\r\n";
    ESP32_SendCmd(cmd, strlen((char *)cmd));

    // 10. 配置在透传模式下，连接发生变化时会传输一个提示信息
    printf("设置系统提示信息...\n");
    cmd = "AT+SYSMSG=4\r\n";
    ESP32_SendCmd(cmd, strlen((char *)cmd));
}

// 发送数据
void BLE_SendData(uint8_t txBuff[], uint16_t txLen)
{
    // 直接通过串口发送数据
    HAL_UART_Transmit(&huart2, txBuff, txLen, 1000);
}

// 自定义函数，判断是否有连接变化信息；如果有则返回1，没有返回0
static uint8_t BLE_IsConnChanged(uint8_t msg[]);

// 接收（读取）数据
void BLE_ReadData(uint8_t rxBuff[], uint16_t *rxLen)
{
    // 从串口接收数据或状态改变信息
    HAL_UARTEx_ReceiveToIdle(&huart2, rxBuff, 1024, rxLen, 1000);

    // 如果是空数据，rxLen = 0，就直接返回
    if (*rxLen == 0)
    {
        return;
    }

    // 如果是连接变化信息，执行相应的SPP操作，并清零len，表示不是正常数据
    if ( BLE_IsConnChanged(rxBuff) )
    {
        *rxLen = 0;
    }
}

static uint8_t BLE_IsConnChanged(uint8_t msg[])
{
    // 1. 如果是BLE建立连接，就进入SPP透传模式
    if (strstr((char *)msg, "+BLECONN:") != NULL)
    {
        printf("有BLE客户端连接，马上进入SPP模式...\n");
        uint8_t * cmd = "AT+BLESPP\r\n";
        ESP32_SendCmd(cmd, strlen((char *)cmd));

        return 1;
    }

    // 2. 如果是BLE断开连接，就退出透传模式
    else if (strstr((char *)msg, "+BLEDISCONN:") != NULL)
    {
        printf("BLE客户端断开连接，即将退出SPP模式...\n");
        
        // 直接串口发送 +++ 退出透传模式
        HAL_UART_Transmit(&huart2, "+++", 3, 1000);

        // 延时等待
        HAL_Delay(2000);

        return 1;
    }

    // 3. 如果是其它类型的连接变化（Wi-Fi），不做任何处理，返回1
    else if(strstr((char *)msg, "WIFI CONNECTED") != NULL
        || strstr((char *)msg, "WIFI GOT IP") != NULL
        || strstr((char *)msg, "+DIST_STA_IP:") != NULL)
    {
        return 1;
    }
    
    // 4. 其它情况默认是正常数据
    return 0;
}
