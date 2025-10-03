#include "freertos_demo.h"
/* freertos��ص�ͷ�ļ�������� */
#include "FreeRTOS.h"
#include "task.h"
/* ��Ҫ�õ�������ͷ�ļ� */
#include "LED.h"
#include "Key.h"

/* ������������� */
#define START_TASK_STACK 128
#define START_TASK_PRIORITY 1
TaskHandle_t start_task_handle;
void start_task(void *pvParameters);

/* ����1������ */
#define TASK1_STACK 128
#define TASK1_PRIORITY 2
TaskHandle_t task1_handle;
void task1(void *pvParameters);

/* ����2������ */
#define TASK2_STACK 128
#define TASK2_PRIORITY 3
TaskHandle_t task2_handle;
void task2(void *pvParameters);

/**
 * @description: ����FreeRTOS
 * @return {*}
 */
void freertos_start(void)
{
    /* 1.����һ���������� */
    xTaskCreate((TaskFunction_t)start_task,               // �������ĵ�ַ
                (char *)"start_task",                     // �������ַ���
                (configSTACK_DEPTH_TYPE)START_TASK_STACK, // ����ջ��С��Ĭ����С128����λ4�ֽ�
                (void *)NULL,                             // ���ݸ�����Ĳ���
                (UBaseType_t)START_TASK_PRIORITY,         // ��������ȼ�
                (TaskHandle_t *)&start_task_handle);      // �������ĵ�ַ

    /* 2.����������:���Զ������������� */
    vTaskStartScheduler();
}

/**
 * @description: ��������������������Task
 * @param {void} *pvParameters
 * @return {*}
 */
void start_task(void *pvParameters)
{
    /* �����ٽ���:�����ٽ�����Ĵ��벻�ᱻ��� */
    taskENTER_CRITICAL();

    /* ����3������ */
    xTaskCreate((TaskFunction_t)task1,
                (char *)"task1",
                (configSTACK_DEPTH_TYPE)TASK1_STACK,
                (void *)NULL,
                (UBaseType_t)TASK1_PRIORITY,
                (TaskHandle_t *)&task1_handle);
    xTaskCreate((TaskFunction_t)task2,
                (char *)"task2",
                (configSTACK_DEPTH_TYPE)TASK2_STACK,
                (void *)NULL,
                (UBaseType_t)TASK2_PRIORITY,
                (TaskHandle_t *)&task2_handle);

    /* ��������ֻ��Ҫִ��һ�μ��ɣ������ɾ���Լ� */
    vTaskDelete(NULL);

    /* �˳��ٽ��� */
    taskEXIT_CRITICAL();
}

/**
 * @description: ����һ�����ڰ���ɨ�裬����⵽����KEY1������ʱ������������֪ͨ��
 * @param {void} *pvParameters
 * @return {*}
 */
#define EVENTBIT_0 (1 << 0)
#define EVENTBIT_1 (1 << 1)
void task1(void *pvParameters)
{
    uint8_t key = 0;
    BaseType_t res = 0;
    while (1)
    {
        key = Key_Detect();
        if (key == KEY1_PRESS)
        {
            /* ��������֪ͨ */
            res = xTaskNotify(
                task2_handle, // ���շ���������
                EVENTBIT_0,   // Ҫ���͵�ֵ֪ͨ: ��Ҫ��λ��bit��1
                eSetBits      // д�����Ϊ������bitλ
            );

            if (res == pdPASS)
            {
                printf("KEY1���£�����bit0Ϊ1..\r\n");
            }
        }
        else if (key == KEY2_PRESS)
        {
            /* ��������֪ͨ */
            res = xTaskNotify(
                task2_handle, // ���շ���������
                EVENTBIT_1,   // Ҫ���͵�ֵ֪ͨ: ��Ҫ��λ��bit��1
                eSetBits      // д�����Ϊ������bitλ
            );

            if (res == pdPASS)
            {
                printf("KEY2���£�����bit1Ϊ1..\r\n");
            }
        }
        vTaskDelay(500);
    }
}

/**
 * @description: ����������ڽ�������֪ͨ������ӡ�����ʾ��Ϣ��
 * @param {void} *pvParameters
 * @return {*}
 */
void task2(void *pvParameters)
{
    uint32_t notify_value = 0;
    uint32_t expect_value = 0;
    BaseType_t res = 0;
    while (1)
    {
        printf("task2����...\r\n");

        res = xTaskNotifyWait(
            0x00000000,    // ����֪ͨǰ�Ƿ�����ֵ֪ͨ��ȫ0����ʾ32bit�Ķ���0����������
            0xffffffff,    // ���յ�ֵ֪ͨ���Ƿ�����ֵ֪ͨ�� ȫ1����ʾ32bit����1����Ҫ����
            &notify_value, // ���������ȡ����ֵ֪ͨ
            portMAX_DELAY);
        if (notify_value & EVENTBIT_0)
        {
            /* bit0 = 1 */
            printf("���յ���bit0=1\r\n");
            expect_value |= EVENTBIT_0;
        }
        if (notify_value & EVENTBIT_1)
        {
            /* bit1 = 1 */
            printf("���յ���bit1=1\r\n");
            expect_value |= EVENTBIT_1;
        }

        /* �жϣ��Ƿ�ϣ����bitλ����1������ǣ������***���� */
        if(expect_value == 3)
        {
            printf("�����������㣬ֵ=%d\r\n",expect_value);
        }

        vTaskDelay(500);
    }
}
