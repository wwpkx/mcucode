#include "freertos_demo.h"
/* freertos相关的头文件，必须的 */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"  //队列相关的头文件
#include "semphr.h" //信号量相关的头文件
/* 需要用到的其他头文件 */
#include "LED.h"
#include "Key.h"

/* 启动任务的配置 */
#define START_TASK_STACK 128
#define START_TASK_PRIORITY 1
TaskHandle_t start_task_handle;
void start_task(void *pvParameters);

/* 任务1的配置 */
#define TASK1_STACK 128
#define TASK1_PRIORITY 2
TaskHandle_t task1_handle;
void task1(void *pvParameters);

/* 任务2的配置 */
#define TASK2_STACK 128
#define TASK2_PRIORITY 3
TaskHandle_t task2_handle;
void task2(void *pvParameters);

/* 队列句柄 */
QueueHandle_t queue1;
QueueHandle_t sem_handle;
QueueSetHandle_t queue_set_handle;

/**
 * @description: 启动FreeRTOS
 * @return {*}
 */
void freertos_start(void)
{

    BaseType_t res = 0;

    /* 在创建任务之前，先创建好需要的队列 */
    /* 创建一个存放KEY值编号的小队列queue1 */
    queue1 = xQueueCreate(2, sizeof(uint8_t));
    if (queue1 != NULL)
    {
        printf("queue1创建成功\r\n");
    }
    else
    {
        printf("queue1创建失败\r\n");
    }

    /* 创建二值信号量，创建完不主动释放 */
    sem_handle = xSemaphoreCreateBinary();
    if (sem_handle != NULL)
    {
        printf("二值信号量创建成功\r\n");
    }
    else
    {
        printf("二值信号量创建失败\r\n");
    }

    /* 创建队列集 */
    queue_set_handle = xQueueCreateSet(2);
    if (queue_set_handle != NULL)
    {
        printf("创建队列集成功\r\n");
    }

    /* 将队列、信号量添加到队列集中：添加时，队列/信号量 需要为空 */
    res = xQueueAddToSet(queue1, queue_set_handle);
    if (res == pdPASS)
    {
        printf("queue1添加到队列集成功\r\n");
    }
    res = xQueueAddToSet(sem_handle, queue_set_handle);
    if (res == pdPASS)
    {
        printf("二值信号量添加到队列集成功\r\n");
    }

    /* 1.创建一个启动任务 */
    xTaskCreate((TaskFunction_t)start_task,               // 任务函数的地址
                (char *)"start_task",                     // 任务名字符串
                (configSTACK_DEPTH_TYPE)START_TASK_STACK, // 任务栈大小，默认最小128，单位4字节
                (void *)NULL,                             // 传递给任务的参数
                (UBaseType_t)START_TASK_PRIORITY,         // 任务的优先级
                (TaskHandle_t *)&start_task_handle);      // 任务句柄的地址

    /* 2.启动调度器:会自动创建空闲任务 */
    vTaskStartScheduler();
}

/**
 * @description: 启动任务：用来创建其他Task
 * @param {void} *pvParameters
 * @return {*}
 */
void start_task(void *pvParameters)
{
    /* 进入临界区:保护临界区里的代码不会被打断 */
    taskENTER_CRITICAL();

    /* 创建3个任务 */
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

    /* 启动任务只需要执行一次即可，用完就删除自己 */
    vTaskDelete(NULL);

    /* 退出临界区 */
    taskEXIT_CRITICAL();
}

/**
 * @description: 任务一：用于扫描按键，当KEY1按下，往队列写入数据，当KEY2按下，释放二值信号量。
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
            /* 将key的编号写入queue1 */
            res = xQueueSend(queue1, &key, portMAX_DELAY);
            if (res == pdPASS)
            {
                printf("往queue1发送数据[%d]成功\r\n", key);
            }
            else
            {
                printf("往queue1发送数据失败\r\n");
            }
        }
        else if (key == KEY2_PRESS)
        {
            /* 释放信号量 */
            res = xSemaphoreGive(sem_handle);
            if (res == pdPASS)
            {
                printf("释放信号量成功\r\n");
            }
            else
            {
                printf("释放信号量失败\r\n");
            }
        }

        vTaskDelay(500);
    }
}

/**
 * @description: 任务二：读取队列集中的消息，并打印
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
        /* 查看哪个队列有数据来了 */
        member_handle = xQueueSelectFromSet(queue_set_handle, portMAX_DELAY);
        /* 根据对应的队列，去获取数据 */
        if (member_handle == queue1)
        {
            res = xQueueReceive(queue1, &receive, portMAX_DELAY);
            if (res == pdPASS)
            {
                printf("task2从queue1成功读取数据[%d]\r\n", receive);
            }
            else
            {
                printf("task2从queue1读取数据失败\r\n");
            }
        }
        else if (member_handle == sem_handle)
        {
            res = xSemaphoreTake(sem_handle,portMAX_DELAY);
            if(res == pdPASS)
            {
                printf("获取信号量成功\r\n");
            }
        }
        else 
        {
            printf("从队列集获取信息失败\r\n");
        }
    }
}


