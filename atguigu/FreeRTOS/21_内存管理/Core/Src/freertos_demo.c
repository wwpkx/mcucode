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
    /* ��������ֻ��Ҫִ��һ�μ��ɣ������ɾ���Լ� */
    vTaskDelete(NULL);

    /* �˳��ٽ��� */
    taskEXIT_CRITICAL();
}

/**
 * @description: ����һ����KEY1�����������ڴ棬��KEY2�������ͷ��ڴ棬����ӡʣ���ڴ���Ϣ
 * @param {void} *pvParameters
 * @return {*}
 */
void task1(void *pvParameters)
{
    uint8_t key = 0;
    void *buff = 0;
    while (1)
    {
        key = Key_Detect();
        if (key == KEY1_PRESS)
        {
            /* �����ڴ棺ʵ�������ȴ���Ĳ������󣬶�������֣��� �ѽṹ��Ĵ�С���� �ֽڶ�����˷�һ���� */
            buff = pvPortMalloc(20);
            printf("����20�ڴ棬��ǰʣ������ڴ�=%d\r\n", xPortGetFreeHeapSize());
        }
        else if (key == KEY2_PRESS)
        {
            /* �ͷ��ڴ� */
            vPortFree(buff);
            printf("�ͷ��ڴ棬��ǰʣ������ڴ�=%d\r\n", xPortGetFreeHeapSize());
        }

        vTaskDelay(500);
    }
}
