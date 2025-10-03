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

/* ����3������ */
#define TASK3_STACK 128
#define TASK3_PRIORITY 4
TaskHandle_t task3_handle;
void task3(void *pvParameters);

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
 * @description: ����һ��ʵ��LED1ÿ500ms��˸һ��
 * @param {void} *pvParameters
 * @return {*}
 */
void task1(void *pvParameters)
{

    while (1)
    {
        printf("task1����...\r\n");
        LED_Toggle(LED1_Pin);
        vTaskDelay(500);
    }
}

/**
 * @description: �������ʵ��LED2ÿ500ms��˸һ��
 * @param {void} *pvParameters
 * @return {*}
 */
void task2(void *pvParameters)
{
    while (1)
    {
        printf("task2����...\r\n");
        LED_Toggle(LED2_Pin);
        vTaskDelay(500);
    }
}

/**
 * @description: ���������жϰ��������߼���KEY1���£�����task1������KEY2�������лָ�task1��KEY3���£������������KEY4���£��ָ�������������ӡ�����״̬
 * @param {void} *pvParameters
 * @return {*}
 */
char task_info[500]; //����һ���ַ����飬���ڽ���������Ϣ�ַ���
void task3(void *pvParameters)
{
    uint8_t key = 0;
    while (1)
    {
        printf("task3����...\r\n");
        key = Key_Detect();
        if (key == KEY1_PRESS)
        {
            /* key1���£�����task1 */
            printf(">>>>����task1....\r\n");
            vTaskSuspend(task1_handle);
        }
        else if (key == KEY2_PRESS)
        {
            /* key2���£��ָ�task1 */
            printf(">>>>�ָ�task1....\r\n");
            vTaskResume(task1_handle);
        }
        else if (key == KEY3_PRESS)
        {
            /* key3���£���������� */
            printf("<<<<���������....\r\n");
            vTaskSuspendAll();
        }
        else if (key == KEY4_PRESS)
        {
            /* key4���£��ָ������� */
            printf("<<<<�ָ�������....\r\n");
            xTaskResumeAll();
        }

        /* ��ӡ�����״̬ */
        vTaskList(task_info);
        printf("%s\r\n",task_info);


        vTaskDelay(500);
    }
}
