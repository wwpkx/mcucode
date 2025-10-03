/*
 * @Author: wushengran
 * @Date: 2024-12-25 16:35:46
 * @Description: 
 * 
 * Copyright (c) 2024 by atguigu, All Rights Reserved. 
 */
#include "web_server.h"

// 全局变量，定义初始化服务器需要的参数
uint8_t txBuff[2048] = {0};
uint8_t rxBuff[2048] = {0};
uint8_t socketCount = 8;
uint8_t socketList[] = {0,1,2,3,4,5,6,7};

uint8_t *contentName = "index.html";

uint8_t content[] = "<!doctype html>\n"
                             "<html lang=\"en\">\n"
                             "<head>\n"
                             "    <meta charset=\"GBK\">\n"
                             "    <meta name=\"viewport\"\n"
                             "          content=\"width=device-width, user-scalable=no, initial-scale=1.0, maximum-scale=1.0, minimum-scale=1.0\">\n"
                             "    <meta http-equiv=\"X-UA-Compatible\" content=\"ie=edge\">\n"
                             "    <title>尚硅谷嵌入式课程</title>\n"
                             "\n"
                             "    <style type=\"text/css\">\n"
                             "        #open_red{\n"
                             "            color: red;\n"
                             "            width: 100px;\n"
                             "            height: 40px;\n"
                             "\n"
                             "\n"
                             "        }\n"
                             "        #close_red{\n"
                             "            color: black;\n"
                             "            width: 100px;\n"
                             "            height: 40px;\n"
                             "        }\n"
                             "    </style>\n"
                             "</head>\n"
                             "<body>\n"
                             "<a href=\"/index.html?action=1\"><button id=\"open_red\" >开灯</button></a>\n"
                             "<a href=\"/index.html?action=2\"><button id=\"close_red\" >关灯</button></a>\n"
                             "<a href=\"/index.html?action=3\"><button id=\"close_red\" >翻转</button></a>\n"
                             "</body>\n"
                             "</html>";


// 初始化Web服务器
void WebServer_Init(void)
{
    // 1. 初始化LED
    LED_Init();

    // 2. 初始化http服务器
    httpServer_init(txBuff, rxBuff, socketCount, socketList);

    // 3. 注册html页面，表示服务器要响应的内容（网页）
    reg_httpServer_webContent(contentName, content);
}

// 启动Web服务器
void WebServer_Start(void)
{
    for (uint8_t i = 0; i < sizeof(socketList); i++)
    {
        httpServer_run(i);
    }
}

// 定义内部函数
// 解析URL中的action
static uint8_t parse_url_action(uint8_t *url);

// 根据action执行LED操作
static void do_led_action(uint8_t action);

void handler_user_function(uint8_t *url)
{
    // 1. 从URL里提取出action的值
    uint8_t action = parse_url_action(url);

    // 2. 根据action的值，执行相应的LED操作
    do_led_action(action);
}

// 解析URL中的action
static uint8_t parse_url_action(uint8_t *url)
{
    // 查找action=子字符串的位置
    uint8_t *pAction = (uint8_t *)strstr((char *)url, "action=");

    if (pAction == NULL)
    {
        return '0';
    }
    else
    {
        return *(pAction + 7);
    }
    
}

// 根据action执行LED操作
static void do_led_action(uint8_t action)
{
    if (action == '1')
    {
        LED_On(LED_2);
    }
    else if (action == '2')
    {
        LED_Off(LED_2);
    }
    else if (action == '3')
    {
        LED_Toggle(LED_2);
    } 
}
