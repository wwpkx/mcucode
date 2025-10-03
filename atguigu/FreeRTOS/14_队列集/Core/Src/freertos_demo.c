#include "freertos_demo.h"
/* freertos��ص�ͷ�ļ�������� */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"  //������ص�ͷ�ļ�
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

/* ���о�� */
QueueHandle_t queue1;
QueueHandle_t sem_handle;
QueueSetHandle_t queue_set_handle;

/**
 * @description: ����FreeRTOS
 * @return {*}
 */
void freertos_start(void)
{

    BaseType_t res = 0;

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

    /* ������ֵ�ź����������겻�����ͷ� */
    sem_handle = xSemaphoreCreateBinary();
    if (sem_handle != NULL)
    {
        printf("��ֵ�ź��������ɹ�\r\n");
    }
    else
    {
        printf("��ֵ�ź�������ʧ��\r\n");
    }

    /* �������м� */
    queue_set_handle = xQueueCreateSet(2);
    if (queue_set_handle != NULL)
    {
        printf("�������м��ɹ�\r\n");
    }

    /* �����С��ź�����ӵ����м��У����ʱ������/�ź��� ��ҪΪ�� */
    res = xQueueAddToSet(queue1, queue_set_handle);
    if (res == pdPASS)
    {
        printf("queue1��ӵ����м��ɹ�\r\n");
    }
    res = xQueueAddToSet(sem_handle, queue_set_handle);
    if (res == pdPASS)
    {
        printf("��ֵ�ź�����ӵ����м��ɹ�\r\n");
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
 * @description: ����һ������ɨ�谴������KEY1���£�������д�����ݣ���KEY2���£��ͷŶ�ֵ�ź�����
 * @param {void} *pvParameters
 * @return {*}
 */
void task1(void *pvParameters)
{
    uint8_t key = 0;
    uint8_t res = 0;
    while (1)
    {
        key = Key_Detect();
        if (key == KEY1_PRESS)
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
        else if (key == KEY2_PRESS)
        {
            /* �ͷ��ź��� */
            res = xSemaphoreGive(sem_handle);
            if (res == pdPASS)
            {
                printf("�ͷ��ź����ɹ�\r\n");
            }
            else
            {
                printf("�ͷ��ź���ʧ��\r\n");
            }
        }

        vTaskDelay(500);
    }
}

/**
 * @description: ���������ȡ���м��е���Ϣ������ӡ
 * @param {void} *pvParameters
 * @return {*}
 */
void task2(void *pvParameters)
{

    QueueSetMemberHandle_t member_handle;
    uint8_t receive = 0;
    uint8_t res = 0;
    while (1)
    {
        /* �鿴�ĸ��������������� */
        member_handle = xQueueSelectFromSet(queue_set_handle, portMAX_DELAY);
        /* ���ݶ�Ӧ�Ķ��У�ȥ��ȡ���� */
        if (member_handle == queue1)
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
        else if (member_handle == sem_handle)
        {
            res = xSemaphoreTake(sem_handle,portMAX_DELAY);
            if(res == pdPASS)
            {
                printf("��ȡ�ź����ɹ�\r\n");
            }
        }
        else 
        {
            printf("�Ӷ��м���ȡ��Ϣʧ��\r\n");
        }
    }
}


