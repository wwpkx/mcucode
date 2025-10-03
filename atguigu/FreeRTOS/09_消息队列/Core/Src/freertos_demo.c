#include "freertos_demo.h"
/* freertos相关的头文件，必须的 */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h" //队列相关的头文件
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

/* 任务3的配置 */
#define TASK3_STACK 128
#define TASK3_PRIORITY 4
TaskHandle_t task3_handle;
void task3(void *pvParameters);

/* 队列句柄 */
QueueHandle_t queue1;
QueueHandle_t big_queue;

/**
 * @description: 启动FreeRTOS
 * @return {*}
 */
void freertos_start(void)
{
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

    /* 创建一个存放大数据的队列big_queue ===> 存放地址就好 */
    big_queue = xQueueCreate(1, sizeof(char *));
    if (big_queue != NULL)
    {
        printf("big_queue创建成功\r\n");
    }
    else
    {
        printf("big_queue创建失败\r\n");
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
    xTaskCreate((TaskFunction_t)task3,
                (char *)"task3",
                (configSTACK_DEPTH_TYPE)TASK3_STACK,
                (void *)NULL,
                (UBaseType_t)TASK3_PRIORITY,
                (TaskHandle_t *)&task3_handle);
    /* 启动任务只需要执行一次即可，用完就删除自己 */
    vTaskDelete(NULL);

    /* 退出临界区 */
    taskEXIT_CRITICAL();
}

/**
 * @description: 任务一：当按键key1或key2按下，将键值拷贝到队列queue1（入队）；当按键key3按下，将传输大数据，这里拷贝大数据的地址到队列big_queue中
 * @param {void} *pvParameters
 * @return {*}
 */
void task1(void *pvParameters)
{
    char *big_data = "大大大大大fdsahjkghasdjh123412412#!@@$!!@Z%$%&%&&#$dashjfhasdjkfhjsdafhjkdashfjkasdhjk";
    uint8_t key = 0;
    uint8_t res = 0;
    while (1)
    {
        key = Key_Detect();
        if (key == KEY1_PRESS || key == KEY2_PRESS)
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
        else if (key == KEY3_PRESS)
        {
            /* 将大数据的地址发送到big_queue */
            res = xQueueSend(big_queue, &big_data, portMAX_DELAY);
            if (res == pdPASS)
            {
                printf("往big_queue发送数据成功\r\n");
            }
            else
            {
                printf("往big_queue发送数据失败\r\n");
            }
        }

        vTaskDelay(500);
    }
}

/**
 * @description: 任务二：读取队列queue1中的消息（出队），打印出接收到的键值
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
            printf("task2从queue1成功读取数据[%d]\r\n", receive);
        }
        else
        {
            printf("task2从queue1读取数据失败\r\n");
        }

    }
}

/**
 * @description: 任务三：从队列big_queue读取大数据地址，通过地址访问大数据
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
            printf("task3从big_queue成功读取数据[%s]\r\n", receive_buff);
        }
        else
        {
            printf("task3从big_queue读取数据失败\r\n");
        }
    }
}
