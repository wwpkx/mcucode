#include "freertos_demo.h"
/* freertos��ص�ͷ�ļ�������� */
#include "FreeRTOS.h"
#include "task.h"
#include "event_groups.h" //�¼���־���ͷ�ļ�
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

EventGroupHandle_t event_group_handle;

void freertos_start(void)
{
    /* �����¼���־�� */
    event_group_handle = xEventGroupCreate();
    if (event_group_handle != NULL)
    {
        printf("�����¼���־��ɹ�...\r\n");
    }

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
 * @description: ����һ����ȡ�������¼�ֵ�����ݲ�ͬ��ֵ���¼���־����Ӧ�¼�λ��һ��ģ���¼�����
 * @param {void} *pvParameters
 * @return {*}
 */
#define EVENTBIT_0 (1 << 0)
#define EVENTBIT_1 (1 << 1)
void task1(void *pvParameters)
{
    uint8_t key = 0;
    while (1)
    {
        key = Key_Detect();
        if (key == KEY1_PRESS)
        {
            /* key1���£���bit0��1 */
            xEventGroupSetBits(event_group_handle, EVENTBIT_0);
            printf("����KEY1����BIT0��1...\r\n");
        }
        else if (key == KEY2_PRESS)
        {
            /* key2���£���bit1��1 */
            xEventGroupSetBits(event_group_handle, EVENTBIT_1);
            printf("����KEY2����BIT1��1...\r\n");
        }

        vTaskDelay(500);
    }
}

/**
 * @description: �������ͬʱ�ȴ��¼���־���еĶ���¼�λ������Щ�¼�λ���� 1 �Ļ���ִ����Ӧ�Ĵ���
 * @param {void} *pvParameters
 * @return {*}
 */
void task2(void *pvParameters)
{
    EventBits_t event_bits = 0;
    while (1)
    {
        event_bits = xEventGroupWaitBits(
            event_group_handle,      // �¼���־��ľ��
            EVENTBIT_0 | EVENTBIT_1, // Ҫ�ȴ��ı�־λ������д�������|ƴ��
            pdTRUE,                  // ����ȴ����������Ƿ񽫶�Ӧ��bitλ���㡣pdTRUEҪ����
            pdTRUE,                  // ����bit����1�����ǲ�����1��pdTRUEҪ�����ж���1��pdFALSE������1����
            portMAX_DELAY);

        printf("task2���յ����¼���־��= %#x.....\r\n",event_bits);
    }
}
