#include "freertos_demo.h"
/* freertos��ص�ͷ�ļ�������� */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h" //������ص�ͷ�ļ�
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

/* ����3������ */
#define TASK3_STACK 128
#define TASK3_PRIORITY 4
TaskHandle_t task3_handle;
void task3(void *pvParameters);

/* ���о�� */
QueueHandle_t queue1;
QueueHandle_t big_queue;

/**
 * @description: ����FreeRTOS
 * @return {*}
 */
void freertos_start(void)
{
    /* �ڴ�������֮ǰ���ȴ�������Ҫ�Ķ��� */
    /* ����һ�����KEYֵ��ŵ�С����queue1 */
    queue1 = xQueueCreate(2, sizeof(uint8_t));
    if (queue1 != NULL)
    {
        printf("queue1�����ɹ�\r\n");
    }
    else
    {
        printf("queue1����ʧ��\r\n");
    }

    /* ����һ����Ŵ����ݵĶ���big_queue ===> ��ŵ�ַ�ͺ� */
    big_queue = xQueueCreate(1, sizeof(char *));
    if (big_queue != NULL)
    {
        printf("big_queue�����ɹ�\r\n");
    }
    else
    {
        printf("big_queue����ʧ��\r\n");
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
    xTaskCreate((TaskFunction_t)task3,
                (char *)"task3",
                (configSTACK_DEPTH_TYPE)TASK3_STACK,
                (void *)NULL,
                (UBaseType_t)TASK3_PRIORITY,
                (TaskHandle_t *)&task3_handle);
    /* ��������ֻ��Ҫִ��һ�μ��ɣ������ɾ���Լ� */
    vTaskDelete(NULL);

    /* �˳��ٽ��� */
    taskEXIT_CRITICAL();
}

/**
 * @description: ����һ��������key1��key2���£�����ֵ����������queue1����ӣ���������key3���£�����������ݣ������������ݵĵ�ַ������big_queue��
 * @param {void} *pvParameters
 * @return {*}
 */
void task1(void *pvParameters)
{
    char *big_data = "������fdsahjkghasdjh123412412#!@@$!!@Z%$%&%&&#$dashjfhasdjkfhjsdafhjkdashfjkasdhjk";
    uint8_t key = 0;
    uint8_t res = 0;
    while (1)
    {
        key = Key_Detect();
        if (key == KEY1_PRESS || key == KEY2_PRESS)
        {
            /* ��key�ı��д��queue1 */
            res = xQueueSend(queue1, &key, portMAX_DELAY);
            if (res == pdPASS)
            {
                printf("��queue1��������[%d]�ɹ�\r\n", key);
            }
            else
            {
                printf("��queue1��������ʧ��\r\n");
            }
        }
        else if (key == KEY3_PRESS)
        {
            /* �������ݵĵ�ַ���͵�big_queue */
            res = xQueueSend(big_queue, &big_data, portMAX_DELAY);
            if (res == pdPASS)
            {
                printf("��big_queue�������ݳɹ�\r\n");
            }
            else
            {
                printf("��big_queue��������ʧ��\r\n");
            }
        }

        vTaskDelay(500);
    }
}

/**
 * @description: ���������ȡ����queue1�е���Ϣ�����ӣ�����ӡ�����յ��ļ�ֵ
 * @param {void} *pvParameters
 * @return {*}
 */
void task2(void *pvParameters)
{
    uint8_t receive = 0;
    uint8_t res = 0;
    while (1)
    {
        res = xQueueReceive(queue1, &receive, portMAX_DELAY);
        if (res == pdPASS)
        {
            printf("task2��queue1�ɹ���ȡ����[%d]\r\n", receive);
        }
        else
        {
            printf("task2��queue1��ȡ����ʧ��\r\n");
        }

    }
}

/**
 * @description: ���������Ӷ���big_queue��ȡ�����ݵ�ַ��ͨ����ַ���ʴ�����
 * @param {void} *pvParameters
 * @return {*}
 */
void task3(void *pvParameters)
{
    uint8_t key = 0;
    char * receive_buff;
    uint8_t res = 0;
    while (1)
    {
        res = xQueueReceive(big_queue, &receive_buff, portMAX_DELAY);
        if (res == pdPASS)
        {
            printf("task3��big_queue�ɹ���ȡ����[%s]\r\n", receive_buff);
        }
        else
        {
            printf("task3��big_queue��ȡ����ʧ��\r\n");
        }
    }
}
