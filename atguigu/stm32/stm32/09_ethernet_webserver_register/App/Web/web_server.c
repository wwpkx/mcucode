/*
 * @Author: wushengran
 * @Date: 2024-12-25 16:35:46
 * @Description: 
 * 
 * Copyright (c) 2024 by atguigu, All Rights Reserved. 
 */
#include "web_server.h"

// ȫ�ֱ����������ʼ����������Ҫ�Ĳ���
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
                             "    <title>�й��Ƕ��ʽ�γ�</title>\n"
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
                             "<a href=\"/index.html?action=1\"><button id=\"open_red\" >����</button></a>\n"
                             "<a href=\"/index.html?action=2\"><button id=\"close_red\" >�ص�</button></a>\n"
                             "<a href=\"/index.html?action=3\"><button id=\"close_red\" >��ת</button></a>\n"
                             "</body>\n"
                             "</html>";


// ��ʼ��Web������
void WebServer_Init(void)
{
    // 1. ��ʼ��LED
    LED_Init();

    // 2. ��ʼ��http������
    httpServer_init(txBuff, rxBuff, socketCount, socketList);

    // 3. ע��htmlҳ�棬��ʾ������Ҫ��Ӧ�����ݣ���ҳ��
    reg_httpServer_webContent(contentName, content);
}

// ����Web������
void WebServer_Start(void)
{
    for (uint8_t i = 0; i < sizeof(socketList); i++)
    {
        httpServer_run(i);
    }
}

// �����ڲ�����
// ����URL�е�action
static uint8_t parse_url_action(uint8_t *url);

// ����actionִ��LED����
static void do_led_action(uint8_t action);

void handler_user_function(uint8_t *url)
{
    // 1. ��URL����ȡ��action��ֵ
    uint8_t action = parse_url_action(url);

    // 2. ����action��ֵ��ִ����Ӧ��LED����
    do_led_action(action);
}

// ����URL�е�action
static uint8_t parse_url_action(uint8_t *url)
{
    // ����action=���ַ�����λ��
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

// ����actionִ��LED����
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
