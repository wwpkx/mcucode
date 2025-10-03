#include "freertos_demo.h"
/* freertos��ص�ͷ�ļ�������� */
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h" // �����ʱ����ͷ�ļ�
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

/* �����ʱ���ĳ�ʱ�ص����� */
void timer1_callback(TimerHandle_t xTimer); // ����
void timer2_callback(TimerHandle_t xTimer); // ����

/* �����ʱ���ľ�� */
TimerHandle_t timer1_handle;
TimerHandle_t timer2_handle;

/**
 * @description: ����FreeRTOS
 * @return {*}
 */
void freertos_start(void)
{

    /* ����һ���������ʱ�� */
    timer1_handle = xTimerCreate(
        "ergou",         // �����ʱ��������
        (TickType_t)500, // ��ʱʱ�� = 500�� RTOS��ʱ�ӽ���
        pdFALSE,         // �Ƿ��Զ����أ�pdTRUE --- �����ͣ� pdFALSE --- һ����
        (void *)1,       // ��ʱ����ΨһID
        timer1_callback);

    if (timer1_handle != NULL)
    {
        printf("timer1һ���Զ�ʱ�������ɹ�...\r\n");
    }

    /* ���������������ʱ�� */
    timer2_handle = xTimerCreate(
        "gousheng",       // �����ʱ��������
        (TickType_t)1000, // ��ʱʱ�� = 1000�� RTOS��ʱ�ӽ���
        pdTRUE,           // �Ƿ��Զ����أ�pdTRUE --- �����ͣ� pdFALSE --- һ����
        (void *)2,        // ��ʱ����ΨһID
        timer2_callback);
    if (timer2_handle != NULL)
    {
        printf("timer2�����Զ�ʱ�������ɹ�...\r\n");
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

    /* ��������ֻ��Ҫִ��һ�μ��ɣ������ɾ���Լ� */
    vTaskDelete(NULL);

    /* �˳��ٽ��� */
    taskEXIT_CRITICAL();
}

/**
 * @description: ����һ�����ڰ���ɨ�裬���������ʱ�����п�����ֹͣ����
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
            /* ���������ʱ�� */
            res = xTimerStart(timer1_handle, portMAX_DELAY);
            if (res != pdFAIL)
            {
                printf("timer1���ζ�ʱ�������ɹ�\r\n");
            }

            res = xTimerStart(timer2_handle, portMAX_DELAY);
            if (res != pdFAIL)
            {
                printf("timer2���ڶ�ʱ�������ɹ�\r\n");
            }
        }
        else if (key == KEY2_PRESS)
        {
            /* ֹͣ�����ʱ�� */
            res = xTimerStop(timer1_handle, portMAX_DELAY);
            if (res != pdFAIL)
            {
                printf("timer1���ζ�ʱ��ֹͣ�ɹ�\r\n");
            }
            res = xTimerStop(timer2_handle, portMAX_DELAY);
            if (res != pdFAIL)
            {
                printf("timer2���ڶ�ʱ��ֹͣ�ɹ�\r\n");
            }
        }

        vTaskDelay(500);
    }
}


void timer1_callback(TimerHandle_t xTimer)
{
    static uint16_t timer1_count = 0;
    printf("timer1��ʱ�ص�=%d��..\r\n",++timer1_count);
}

void timer2_callback(TimerHandle_t xTimer)
{
    static uint16_t timer2_count = 0;
    printf("timer2��ʱ�ص�=%d��..\r\n",++timer2_count);
}
