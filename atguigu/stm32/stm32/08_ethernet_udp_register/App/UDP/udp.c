/*
 * @Author: wushengran
 * @Date: 2024-12-25 11:27:08
 * @Description:
 *
 * Copyright (c) 2024 by atguigu, All Rights Reserved.
 */
#include "udp.h"

// ����UDPģʽsocket
void UDP_Start(void)
{
    // 1. ��ȡsocket״̬
    uint8_t status = getSn_SR(SN);

    // 2. �жϵ�ǰ״̬��ִ����Ӧ������������һ��״̬
    if (status == SOCK_CLOSED)
    {
        // 2.1 ����ǹر�״̬���ʹ�socket
        int8_t n = socket(SN, Sn_MR_UDP, 9999, 0);
        if (n == SN)
        {
            printf("socket %d �򿪳ɹ���\n", SN);
        }
        else
        {
            printf("socket %d ��ʧ�ܣ������룺%d\n", SN, n);
        }
    }
}

// ��������
void UDP_RecvData(uint8_t buff[], uint16_t *len, uint8_t *srcIP, uint16_t *srcPort)
{
    // 1. ��ȡsocket״̬
    uint8_t status = getSn_SR(SN);

    // 2. �ж������UDP״̬���ͽ�������
    if (status == SOCK_UDP)
    {
        // �����¼���־λ���ж��Ƿ���յ����ݣ�������յ����Ͷ�ȡ��������
        if (getSn_IR(SN) & Sn_IR_RECV)
        {
            // �����־λ��д1��0��
            setSn_IR(SN, Sn_IR_RECV);

            // ��ȡ���ݳ��ȣ�������8�ֽڵ��ײ�
            uint16_t tmp = getSn_RX_RSR(SN);

            // �ж����tmp > 8����ʾ���յ�����
            if (tmp > 8)
            {
                *len = tmp - 8;
                // ��ȡ����
                recvfrom(SN, buff, *len, srcIP, srcPort);
            }
        }
    }
}

// ��������
void UDP_SendData(uint8_t data[], uint16_t len, uint8_t *dstIP, uint16_t dstPort)
{
    // 1. ��ȡsocket״̬
    uint8_t status = getSn_SR(SN);

    // 2. �ж������UDP״̬���ͷ�������
    if (status == SOCK_UDP)
    {
        sendto(SN, data, len, dstIP, dstPort);
        printf("������ϣ�����: %.*s\n", len, data);
    }
}
