#include "freertos_demo.h"
/* freertos相关的头文件，必须的 */
#include "FreeRTOS.h"
#include "task.h"
#include "event_groups.h" //事件标志组的头文件
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

/**
 * @description: 启动FreeRTOS
 * @return {*}
 */

EventGroupHandle_t event_group_handle;

void freertos_start(void)
{
    /* 创建事件标志组 */
    event_group_handle = xEventGroupCreate();
    if (event_group_handle != NULL)
    {
        printf("创建事件标志组成功...\r\n");
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
 * @description: 任务一：读取按键按下键值，根据不同键值将事件标志组相应事件位置一，模拟事件发生
 * @param {void} *pvParameters
 * @return {*}
 */
#define EVENTBIT_0 (1 << 0)
#define EVENTBIT_1 (1 << 1)
void task1(void *pvParameters)
{
    uint8_t key = 0;
    while (1)
    {
        key = Key_Detect();
        if (key == KEY1_PRESS)
        {
            /* key1按下，对bit0置1 */
            xEventGroupSetBits(event_group_handle, EVENTBIT_0);
            printf("按下KEY1，对BIT0置1...\r\n");
        }
        else if (key == KEY2_PRESS)
        {
            /* key2按下，对bit1置1 */
            xEventGroupSetBits(event_group_handle, EVENTBIT_1);
            printf("按下KEY2，对BIT1置1...\r\n");
        }

        vTaskDelay(500);
    }
}

/**
 * @description: 任务二：同时等待事件标志组中的多个事件位，当这些事件位都置 1 的话就执行相应的处理
 * @param {void} *pvParameters
 * @return {*}
 */
void task2(void *pvParameters)
{
    EventBits_t event_bits = 0;
    while (1)
    {
        event_bits = xEventGroupWaitBits(
            event_group_handle,      // 事件标志组的句柄
            EVENTBIT_0 | EVENTBIT_1, // 要等待的标志位，可以写多个，用|拼接
            pdTRUE,                  // 满足等待的条件后，是否将对应的bit位清零。pdTRUE要清零
            pdTRUE,                  // 所有bit都是1，还是部分是1；pdTRUE要求所有都是1，pdFALSE其中有1就行
            portMAX_DELAY);

        printf("task2接收到的事件标志组= %#x.....\r\n",event_bits);
    }
}
