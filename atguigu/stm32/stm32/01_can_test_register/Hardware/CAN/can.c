/*
 * @Author: wushengran
 * @Date: 2024-12-21 16:52:16
 * @Description: 
 * 
 * Copyright (c) 2024 by atguigu, All Rights Reserved. 
 */
#include "can.h"

static void CAN_FilterConfig(void);

// ��ʼ��
void CAN_Init(void)
{
    // 1. ����ʱ�ӣ�CAN��GPIO��AFIO
    RCC->APB1ENR |= RCC_APB1ENR_CAN1EN;
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
    RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;

    // 2. ��ӳ��PB8��PB9
    AFIO->MAPR |= AFIO_MAPR_CAN_REMAP_1;
    AFIO->MAPR &= ~AFIO_MAPR_CAN_REMAP_0;

    // 3. GPIO����ģʽ���壺
    // PB8 - �������� MODE 00��CNF 01
    GPIOB->CRH &= ~GPIO_CRH_MODE8;
    GPIOB->CRH &= ~GPIO_CRH_CNF8_1;
    GPIOB->CRH |= GPIO_CRH_CNF8_0;
    
    // PB9 - �����������, MODE 11��CNF 10
    GPIOB->CRH |= GPIO_CRH_MODE9;
    GPIOB->CRH |= GPIO_CRH_CNF9_1;
    GPIOB->CRH &= ~GPIO_CRH_CNF9_0;

    // 4. CAN ��ʼ����������
    // 4.1 �����ʼ��ģʽ
    CAN1->MCR |= CAN_MCR_INRQ;
    while ((CAN1->MSR & CAN_MSR_INAK) == 0)
    {}

    // 4.2 �˳�˯��ģʽ
    CAN1->MCR &= ~CAN_MCR_SLEEP;
    while ((CAN1->MSR & CAN_MSR_SLAK) != 0)
    {}

    // 4.3 �Զ����߹���
    CAN1->MCR |= CAN_MCR_ABOM;

    // 4.4 �Զ����ѹ���
    CAN1->MCR |= CAN_MCR_AWUM;

    // 4.5 ���û��ؾ�Ĭ����ģʽ
    CAN1->BTR |= CAN_BTR_SILM;
    CAN1->BTR |= CAN_BTR_LBKM;

    // 4.6 ����λʱ��
    // 4.6.1 ���ò����ʷ�Ƶ����36��Ƶ��Tq = 1us
    CAN1->BTR &= ~CAN_BTR_BRP;
    CAN1->BTR |= (35 << 0);

    // 4.6.2 ����BS1��BS2��ʱ�䳤��
    CAN1->BTR &= ~CAN_BTR_TS1;
    CAN1->BTR |= (2 << 16);

    CAN1->BTR &= ~CAN_BTR_TS2;
    CAN1->BTR |= (5 << 20);

    // 4.6.3 ��ͬ����Ծ���
    CAN1->BTR &= ~CAN_BTR_SJW;
    CAN1->BTR |= (1 << 24);

    // 4.7 �˳���ʼ��ģʽ
    CAN1->MCR &= ~CAN_MCR_INRQ;
    while ((CAN1->MSR & CAN_MSR_INAK) != 0)
    {}

    // 5. CAN����������
    CAN_FilterConfig();
}

// ���徲̬���������й���������
static void CAN_FilterConfig(void)
{
    // 1. �����ʼ��ģʽ
    CAN1->FMR |= CAN_FMR_FINIT;

    // 2. ���ù���������ģʽ��0 - ����λģʽ
    CAN1->FM1R &= ~CAN_FM1R_FBM0;

    // 3. ����λ��1 - 32λ
    CAN1->FS1R |= CAN_FS1R_FSC0;

    // 4. ���ù�����FIFO��FIFO0
    CAN1->FFA1R &= ~CAN_FFA1R_FFA0;

    // 5. ���ù�������0��ID�Ĵ�����FR1
    CAN1->sFilterRegister[0].FR1 = 0x06e << 21;

    // 6. ���ù�������0������Ĵ�����FR2 = 0���������ˣ�ȫ������
    CAN1->sFilterRegister[0].FR2 = 0x7f1 << 21;

    // 7. �����������0
    CAN1->FA1R |= CAN_FA1R_FACT0;

    // 8. �˳���ʼ��ģʽ
    CAN1->FMR &= ~CAN_FMR_FINIT;
}

// ���ͱ��ģ���׼ID�����ݡ����ȣ�ʹ�÷�������0��
void CAN_SendMsg(uint16_t stdID, uint8_t * data, uint8_t len)
{
    // 1. �ȴ���������0Ϊ��
    while ( (CAN1->TSR & CAN_TSR_TME0) == 0)
    {
    }

    // 2. ��װҪ���͵�����֡
    // 2.1 ���ñ�׼ID
    CAN1->sTxMailBox[0].TIR &= ~CAN_TI0R_STID;
    CAN1->sTxMailBox[0].TIR |= stdID << 21;

    // 2.2 ����Ϊ��׼֡
    CAN1->sTxMailBox[0].TIR &= ~CAN_TI0R_IDE;

    // 2.3 ����Ϊ����֡
    CAN1->sTxMailBox[0].TIR &= ~CAN_TI0R_RTR;

    // 2.4 �������ݳ���
    CAN1->sTxMailBox[0].TDTR &= ~CAN_TDT0R_DLC;
    CAN1->sTxMailBox[0].TDTR |= len << 0;

    // 2.5 ��������
    // ������Ĵ���
    CAN1->sTxMailBox[0].TDLR = 0;
    CAN1->sTxMailBox[0].TDHR = 0;

    // ѭ������ÿһ���ֽ�����
    for (uint8_t i = 0; i < len; i++)
    {
        // �ж�ǰ4���ֽڣ��ŵ�TDLR��
        if (i < 4)
        {
            CAN1->sTxMailBox[0].TDLR |= data[i] << (i * 8);
        } 
        // ��4���ֽڣ��ŵ�TDHR��
        else
        {
            CAN1->sTxMailBox[0].TDHR |= data[i] << ((i - 4) * 8);
        }
    }

    // 3. ����������֡
    CAN1->sTxMailBox[0].TIR |= CAN_TI0R_TXRQ;

    // 4. �ȴ��������
    while ((CAN1->TSR & CAN_TSR_TXOK0) == 0)
    {
    }
}

// ���ձ��ģ��ṹ�����飬���鳤�ȣ���FIFO0�ж�ȡ��
void CAN_ReceiveMsg(RxMsg rxMsg[], uint8_t * msgCount)
{
    // 1. ��ȡFIFO0�ı��ĸ�����ͨ��ָ�뷵��
    * msgCount = (CAN1->RF0R & CAN_RF0R_FMP0) >> 0;

    // 2. ѭ����ȡÿһ������
    for (uint8_t i = 0; i < *msgCount; i++)
    {
        // ����ָ�룬ָ��ǰ���汨�ĵ����ݶ���
        RxMsg * msg = &rxMsg[i];

        // 2.1 ��ȡID
        msg->stdID = (CAN1->sFIFOMailBox[0].RIR >> 21) & 0x7ff;

        // 2.2 ��ȡ���ݳ���
        msg->len = (CAN1->sFIFOMailBox[0].RDTR >> 0) & 0x0f;
        
        // 2.3 ��ȡ����
        uint32_t low = CAN1->sFIFOMailBox[0].RDLR;
        uint32_t high = CAN1->sFIFOMailBox[0].RDHR;

        for (uint8_t j = 0; j < msg->len; j++)
        {
            // �����ǰ4���ֽڣ��ʹ�RDLR����ȡ
            if (j < 4)
            {
                msg->data[j] = (low >> (8 * j)) & 0xff;
            }      
            // ����Ǻ�4���ֽڣ��ʹ�RDHR����ȡ
            else
            {
                msg->data[j] = (high >> (8 * (j - 4))) & 0xff;
            }
        }

        // 2.4 �ͷ�FIFO0�����ӣ��ٶ�ȡ��һ������
        CAN1->RF0R |= CAN_RF0R_RFOM0;
    }
}
