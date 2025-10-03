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
#define TASK2_STACK 256
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
 * @description: �����������չʾ����״̬��ѯ���API������ʹ��
 * @param {void} *pvParameters
 * @return {*}
 */
char task_info[500];
void task2(void *pvParameters)
{
    UBaseType_t task_priority = 0;
    UBaseType_t task_num = 0;
    TaskStatus_t task_status[10] = 0;
    TaskStatus_t task_state;
    TaskHandle_t task_handle = 0;
    UBaseType_t task_stack_remain_min = 0;
    eTaskState e_task_state = 0;

    /* ͨ���������ȡ��������ȼ� */
    task_priority = uxTaskPriorityGet(task1_handle);
    printf("task1�������ȼ�=%d\r\n", task_priority);
    /* ������������ȼ� */
    vTaskPrioritySet(task1_handle, 4);
    task_priority = uxTaskPriorityGet(task1_handle);
    printf("task1�������ȼ�=%d\r\n", task_priority);

    /* ��ȡϵͳ����������� */
    task_num = uxTaskGetNumberOfTasks();
    printf("ϵͳ����������=%d\r\n", task_num);

    /* ��ȡ��������״̬ */
    uxTaskGetSystemState(task_status, task_num, NULL);
    printf("������\t\t������\t\t����״̬\t\t��ǰ���ȼ�\t\t����ԭʼ���ȼ�\r\n");
    for (uint8_t i = 0; i < task_num; i++)
    {
        printf("%s\t\t%d\t\t%d\t\t%d\t\t%d\r\n",
               task_status[i].pcTaskName,
               task_status[i].xTaskNumber,
               task_status[i].eCurrentState,
               task_status[i].uxCurrentPriority,
               task_status[i].uxBasePriority);
    }

    /* ��ȡ�����������Ϣ */
    vTaskGetInfo(
        task1_handle, // Ҫ��ȡ��������Ϣ�ľ��
        &task_state,  // ���������ѯ��������״̬��Ϣ
        pdTRUE,       // Ҫ��ѯ ����ջ ��ʷʣ����Сֵ
        eInvalid);    // ��ΪeInvalid���Ż�����ȥ��ȡָ�������״̬
    printf("�������ƣ�%s\r\n", task_state.pcTaskName);
    printf("�����ţ�%d\r\n", task_state.xTaskNumber);
    printf("����״̬��%d\r\n", task_state.eCurrentState);
    printf("�������ȼ���%d\r\n", task_state.uxCurrentPriority);

    /* ��ȡ��ǰ����������� */
    task_handle = xTaskGetCurrentTaskHandle();
    printf("��ȡ���ĵ�ǰ������=%p,task2�ľ��=%p\r\n", task_handle, task2_handle);

    /* ͨ����������ȡָ������ľ�� */
    task_handle = xTaskGetHandle("task1");
    printf("��ȡ������Ϊtask1������=%p,task1�ľ��=%p\r\n", task_handle, task1_handle);

    /* ��������ȡ����ջ��ʷʣ����Сֵ�����������ж�����ջ����Ŀռ��Ƿ����(����ջ��С - ��ʷʹ�õ����ֵ) */
    task_stack_remain_min = uxTaskGetStackHighWaterMark(task2_handle);
    printf("task2����ջ��ʷʣ����Сֵ=%d\r\n",task_stack_remain_min);

    /* ��ȡ����״̬ */
    e_task_state = eTaskGetState(task1_handle);
    printf("task1����״̬=%d\r\n",e_task_state);


    /* �Ա����ʽ��ʾϵͳ�е�������Ϣ */
    vTaskList(task_info);
    printf("%s\r\n",task_info);

    while (1)
    {
        vTaskDelay(500);
    }
}
