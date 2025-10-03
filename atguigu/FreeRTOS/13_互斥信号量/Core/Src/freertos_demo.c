#include "freertos_demo.h"
/* freertos相关的头文件，必须的 */
#include "FreeRTOS.h"
#include "task.h"
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

/* 任务3的配置 */
#define TASK3_STACK 128
#define TASK3_PRIORITY 4
TaskHandle_t task3_handle;
void task3(void *pvParameters);

/**
 * @description: 启动FreeRTOS
 * @return {*}
 */
QueueHandle_t sem_handle;
void freertos_start(void)
{
    /* 创建互斥信号量，创建成功会自动释放一次信号量 */
    sem_handle = xSemaphoreCreateMutex();

    if (sem_handle == NULL)
    {
        printf("互斥信号量创建失败\r\n");
    }
    else 
    {
        printf("互斥信号量创建成功\r\n");
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
 * @description: 任务一：用于按键扫描，当检测到按键KEY1被按下时，释放二值信号量。
 * @param {void} *pvParameters
 * @return {*}
 */
void task1(void *pvParameters)
{

    BaseType_t res = 0;
    while (1)
    {

        /* 获取信号量 */
        printf("低优先级的Task1获取信号量\r\n");
        res = xSemaphoreTake(sem_handle, portMAX_DELAY);
        if (res != pdPASS)
        {
            printf("Task1获取信号量失败\r\n");
        }

        /* 执行其他逻辑 */
        printf("低优先级的Task1正在执行\r\n");
        HAL_Delay(3000);

        /* 释放信号量 */
        printf("低优先级的Task1释放信号量\r\n");
        res = xSemaphoreGive(sem_handle);
        if (res != pdPASS)
        {
            printf("Task1释放二值信号量失败\r\n");
        }

        vTaskDelay(1000);
    }
}

/**
 * @description: 任务二：
 * @param {void} *pvParameters
 * @return {*}
 */
void task2(void *pvParameters)
{
    BaseType_t res = 0;
    while (1)
    {
        printf("中优先级的Task2正在执行\r\n");
        HAL_Delay(1500);
        printf("Task2 执行完成一次\r\n");
        vTaskDelay(1000);
    }
}

/**
 * @description: 任务三：获取二值信号量，当成功获取后打印提示信息
 * @param {void} *pvParameters
 * @return {*}
 */
void task3(void *pvParameters)
{
    BaseType_t res = 0;
    while (1)
    {

        /* 获取信号量 */
        printf("高优先级的Task3获取信号量\r\n");
        res = xSemaphoreTake(sem_handle, portMAX_DELAY);
        if (res != pdPASS)
        {
            printf("Task3获取信号量失败\r\n");
        }

        /* 执行其他逻辑 */
        printf("高优先级的Task3正在执行...\r\n");
        HAL_Delay(1000);

        /* 释放信号量 */
        printf("高优先级的Task3释放信号量\r\n");
        res = xSemaphoreGive(sem_handle);
        if (res != pdPASS)
        {
            printf("Task3释放二值信号量失败\r\n");
        }

        vTaskDelay(1000);
    }
}

