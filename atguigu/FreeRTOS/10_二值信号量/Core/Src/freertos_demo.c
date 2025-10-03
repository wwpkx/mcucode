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
QueueHandle_t sem_handle;
void freertos_start(void)
{

    /* ������ֵ�ź��� */
    // xSemaphoreCreateBinary //x��ͷ�ģ����������ͷ��ź���
    vSemaphoreCreateBinary(sem_handle); //v��ͷ�ģ�������������ͷ�һ���ź���
    if(sem_handle == NULL)
    {
        printf("��ֵ�ź�������ʧ��\r\n");
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
 * @description: ����һ�����ڰ���ɨ�裬����⵽����KEY1������ʱ���ͷŶ�ֵ�ź�����
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
        if(key==KEY1_PRESS)
        {
            /* �ͷŶ�ֵ�ź��� */
            res = xSemaphoreGive(sem_handle);
            if(res == pdPASS)
            {
                printf("Task1�ͷŶ�ֵ�ź����ɹ�\r\n");
            }
            else 
            {
                printf("Task1�ͷŶ�ֵ�ź���ʧ��\r\n");
            }
        }
        vTaskDelay(500);
    }
}

/**
 * @description: ���������ȡ��ֵ�ź��������ɹ���ȡ���ӡ��ʾ��Ϣ
 * @param {void} *pvParameters
 * @return {*}
 */
void task2(void *pvParameters)
{
    BaseType_t res = 0;
    while (1)
    {
        /* ��ȡ��ֵ�ź��� */
        res = xSemaphoreTake(sem_handle,portMAX_DELAY);
        if(res == pdPASS)
        {
            printf("Task2��ȡ�ź����ɹ�\r\n");
        }
        else 
        {
            printf("Task2��ȡ�ź���ʧ��\r\n");   
        }

    }
}


