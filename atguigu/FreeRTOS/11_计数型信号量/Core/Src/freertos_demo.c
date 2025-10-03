#include "freertos_demo.h"
/* freertos��ص�ͷ�ļ�������� */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h" //�ź�����ص�ͷ�ļ�
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
QueueHandle_t sem_count_handle;
void freertos_start(void)
{
    UBaseType_t count = 0;

    /* �����������ź��� */
    sem_count_handle = xSemaphoreCreateCounting(100, 0);

    if (sem_count_handle == NULL)
    {
        printf("�������ź�������ʧ��\r\n");
    }
    else
    {
        /* �����ɹ�����ȡһ�μ���ֵ����ӡ���� */
        count = uxSemaphoreGetCount(sem_count_handle);
        printf("�����������ź����ɹ�����ʼ����ֵ=%d\r\n", count);
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
 * @description: ����һ�����ڰ���ɨ�裬����⵽����KEY1������ʱ���ͷż������ź�����
 * @param {void} *pvParameters
 * @return {*}
 */
void task1(void *pvParameters)
{

    uint8_t key = 0;
    BaseType_t res = 0;
    while (1)
    {
        key = Key_Detect();
        if (key == KEY1_PRESS)
        {
            /* �ͷż������ź���������ֵ��+1 */
            res = xSemaphoreGive(sem_count_handle);
            if (res == pdPASS)
            {
                printf("Task1�ͷż������ź����ɹ�\r\n");
            }
            else
            {
                printf("Task1�ͷż������ź���ʧ��\r\n");
            }
        }
        vTaskDelay(500);
    }
}

/**
 * @description: �������ÿ��һ���ȡһ�μ������ź��������ɹ���ȡ���ӡ�ź�������ֵ��
 * @param {void} *pvParameters
 * @return {*}
 */
void task2(void *pvParameters)
{
    BaseType_t res = 0;
    UBaseType_t currnet_count = 0;
    while (1)
    {
        /* ��ȡ��ֵ�ź��� */
        res = xSemaphoreTake(sem_count_handle, portMAX_DELAY);
        if (res == pdPASS)
        {
            printf("Task2��ȡ�ź����ɹ�\r\n");
        }
        else
        {
            printf("Task2��ȡ�ź���ʧ��\r\n");
        }

        currnet_count = uxSemaphoreGetCount(sem_count_handle);
        printf("��ǰ����ֵ=%d\r\n", currnet_count);
        
        vTaskDelay(1000);
    }
}
