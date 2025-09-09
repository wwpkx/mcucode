/*
 * @Author: wushengran
 * @Date: 2024-12-25 11:27:08
 * @Description:
 *
 * Copyright (c) 2024 by atguigu, All Rights Reserved.
 */
#include "tcp.h"

// uint8_t isSaved;

// ����ȫ�ֱ���������ͻ��˵�IP�Ͷ˿ں�
uint8_t clientIP[4];
uint16_t clientPort;

// ����TCP������
void TCP_ServerStart(void)
{
    // 1. ��ȡsocket״̬
    uint8_t status = getSn_SR(SN);

    // 2. �жϵ�ǰ״̬��ִ����Ӧ������������һ��״̬
    if (status == SOCK_CLOSED)
    {
        // 2.1 ����ǹر�״̬���ʹ�socket
        int8_t n = socket(SN, Sn_MR_TCP, 8080, SF_TCP_NODELAY);
        if (n == SN)
        {
            printf("socket %d �򿪳ɹ���\n", SN);
        }
        else
        {
            printf("socket %d ��ʧ�ܣ������룺%d\n", SN, n);
        }
        // isSaved = 0;
    }
    else if (status == SOCK_INIT)
    {
        // 2.2 �����INIT״̬����ִ�м��������������
        int8_t res = listen(SN);
        if (res == SOCK_OK)
        {
            printf("socket %d �����ɹ���\n", SN);
        }
        else
        {
            printf("socket %d ����ʧ�ܣ������룺%d\n", SN, res);
        }
        // isSaved = 0;
    }
    // else if (status == SOCK_LISTEN)
    // {
    // }
    else if (status == SOCK_ESTABLISHED)
    {
        if ( getSn_IR(SN) & Sn_IR_CON )
        {
            getSn_DIPR(SN, clientIP);
            clientPort = getSn_DPORT(SN);
            
            printf("�ͻ������ӳɹ���IP��%d.%d.%d.%d, Port: %d\n", 
                clientIP[0], clientIP[1], clientIP[2], clientIP[3], clientPort);

            // �����־λ��д1��0��
            setSn_IR(SN, Sn_IR_CON);
        } 
    }
    else if (status == SOCK_CLOSE_WAIT)
    {
        // 2.3 ����ǰ�ر�״̬����ֱ�ӹر�socket
        printf("ʧȥ�ͻ��˵����ӣ�׼���ر�socket�����´�...\n");
        close(SN);

        // isSaved = 0;
    }
}

// ��������
void TCP_RecvData(uint8_t buff[], uint16_t *len)
{
    // 1. ��ȡsocket״̬
    uint8_t status = getSn_SR(SN);

    // 2. �ж�����ǽ�������״̬���ͽ�������
    if (status == SOCK_ESTABLISHED)
    {
        // // �ж��Ƿ񱣴�������û�оʹӼĴ�������ȡ���ӵĿͻ���IP�Ͷ˿ں�
        // if (isSaved == 0 && ROLE == SERVER)
        // {
        //     getSn_DIPR(SN, clientIP);
        //     clientPort = getSn_DPORT(SN);
            
        //     printf("�ͻ������ӳɹ���IP��%d.%d.%d.%d, Port: %d\n", 
        //         clientIP[0], clientIP[1], clientIP[2], clientIP[3], clientPort);

        //     isSaved = 1;
        // } 

        // �����¼���־λ���ж��Ƿ���յ����ݣ�������յ����Ͷ�ȡ��������
        if (getSn_IR(SN) & Sn_IR_RECV)
        {
            // �����־λ��д1��0��
            setSn_IR(SN, Sn_IR_RECV);

            // ��ȡ���ݳ���
            *len = getSn_RX_RSR(SN);
            // ��ȡ����
            recv(SN, buff, *len);
        }
    }
}

// ��������
void TCP_SendData(uint8_t data[], uint16_t len)
{
    // 1. ��ȡsocket״̬
    uint8_t status = getSn_SR(SN);

    // 2. �ж�����ǽ�������״̬���ͽ�������
    if (status == SOCK_ESTABLISHED)
    {
        send(SN, data, len);
    }
}
