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
StackType_t start_task_stack[START_TASK_STACK]; // ��̬���������ջ����������ʽ�洢
StaticTask_t start_task_tcb;                    // ��̬�����TCB�ṹ������
void start_task(void *pvParameters);

/* ����1������ */
#define TASK1_STACK 128
#define TASK1_PRIORITY 2
TaskHandle_t task1_handle;
StackType_t task1_stack[TASK1_STACK]; // ��̬���������ջ����������ʽ�洢
StaticTask_t task1_tcb;               // ��̬�����TCB�ṹ������
void task1(void *pvParameters);

/* ����2������ */
#define TASK2_STACK 128
#define TASK2_PRIORITY 3
TaskHandle_t task2_handle;
StackType_t task2_stack[TASK2_STACK]; // ��̬���������ջ����������ʽ�洢
StaticTask_t task2_tcb;               // ��̬�����TCB�ṹ������
void task2(void *pvParameters);

/* ����3������ */
#define TASK3_STACK 128
#define TASK3_PRIORITY 4
TaskHandle_t task3_handle;
StackType_t task3_stack[TASK3_STACK]; // ��̬���������ջ����������ʽ�洢
StaticTask_t task3_tcb;               // ��̬�����TCB�ṹ������
void task3(void *pvParameters);

/* ======================= ��̬������ʽ����Ҫ�ֶ�ָ��2�������������Դ=============================== */
/* ������������� */
StackType_t idle_task_stack[configMINIMAL_STACK_SIZE]; // ��̬���������ջ����������ʽ�洢
StaticTask_t idle_task_tcb;                            // ��̬�����TCB�ṹ������

/* �����ʱ����������� */
StackType_t timer_task_stack[configTIMER_TASK_STACK_DEPTH]; // ��̬���������ջ����������ʽ�洢
StaticTask_t timer_task_tcb;                            // ��̬�����TCB�ṹ������

/* ��������������Դ */
void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer,
                                   StackType_t **ppxIdleTaskStackBuffer,
                                   uint32_t *pulIdleTaskStackSize)
{
    *ppxIdleTaskTCBBuffer = &idle_task_tcb;
    *ppxIdleTaskStackBuffer = idle_task_stack;
    *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
}

/* ���������ʱ���������Դ */
void vApplicationGetTimerTaskMemory(StaticTask_t **ppxTimerTaskTCBBuffer,
                                    StackType_t **ppxTimerTaskStackBuffer,
                                    uint32_t *pulTimerTaskStackSize)
{
    *ppxTimerTaskTCBBuffer = &timer_task_tcb;
    *ppxTimerTaskStackBuffer = timer_task_stack;
    *pulTimerTaskStackSize = configTIMER_TASK_STACK_DEPTH;
}

/**
 * @description: ����FreeRTOS
 * @return {*}
 */
void freertos_start(void)
{
    /* 1.����һ���������� */
    start_task_handle = xTaskCreateStatic(
        (TaskFunction_t)start_task,       // �������ĵ�ַ
        (char *)"start_task",             // �������ַ���
        (uint32_t)START_TASK_STACK,       // ����ջ��С��Ĭ����С128����λ4�ֽ�
        (void *)NULL,                     // ���ݸ�����Ĳ���
        (UBaseType_t)START_TASK_PRIORITY, // ��������ȼ�
        (StackType_t *)start_task_stack,  // ���Ǵ���������ջ������洢���ĵ�ַ
        (StaticTask_t *)&start_task_tcb   // ���Ǵ�����tcb�ĵ�ַ
    );

    /* 2.����������:���Զ�������������������ʱ���������������
       ��̬�����ķ�ʽ��Ҫȥʵ��2��������Դ�Ľӿں��� */
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

    /* ʹ�þ�̬����3������ */
    task1_handle = xTaskCreateStatic(
        (TaskFunction_t)task1,
        (char *)"task1",
        (configSTACK_DEPTH_TYPE)TASK1_STACK,
        (void *)NULL,
        (UBaseType_t)TASK1_PRIORITY,
        (StackType_t *)task1_stack, // ���Ǵ���������ջ������洢���ĵ�ַ
        (StaticTask_t *)&task1_tcb  // ���Ǵ�����tcb�ĵ�ַ
    );

    task2_handle = xTaskCreateStatic(
        (TaskFunction_t)task2,
        (char *)"task2",
        (configSTACK_DEPTH_TYPE)TASK2_STACK,
        (void *)NULL,
        (UBaseType_t)TASK2_PRIORITY,
        (StackType_t *)task2_stack, // ���Ǵ���������ջ������洢���ĵ�ַ
        (StaticTask_t *)&task2_tcb  // ���Ǵ�����tcb�ĵ�ַ
    );

    task3_handle = xTaskCreateStatic(
        (TaskFunction_t)task3,
        (char *)"task3",
        (configSTACK_DEPTH_TYPE)TASK3_STACK,
        (void *)NULL,
        (UBaseType_t)TASK3_PRIORITY,
        (StackType_t *)task3_stack, // ���Ǵ���������ջ������洢���ĵ�ַ
        (StaticTask_t *)&task3_tcb  // ���Ǵ�����tcb�ĵ�ַ
    );

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
 * @description: ���������жϰ���KEY1�Ƿ��£�������ɾ��task1
 * @param {void} *pvParameters
 * @return {*}
 */
void task3(void *pvParameters)
{
    uint8_t key = 0;
    while (1)
    {
        printf("task3����...\r\n");
        key = Key_Detect();
        if (key == KEY1_PRESS)
        {
            /* �ж��Ƿ��Ѿ�ɾ���������ظ�ִ��ɾ�� */
            if (task1_handle != NULL)
            {
                /* key1�����ˣ�ɾ��task1 */
                printf("ִ��ɾ��Task1....\r\n");
                vTaskDelete(task1_handle);
                task1_handle = NULL;
            }
        }
        vTaskDelay(500);
        // HAL_Delay(500); //����delay�����������������̬�������ó�cpuʹ��Ȩ
    }
}
