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



/**
 * @description: 启动FreeRTOS
 * @return {*}
 */
QueueHandle_t sem_handle;
void freertos_start(void)
{

    /* 创建二值信号量 */
    // xSemaphoreCreateBinary //x开头的，不会主动释放信号量
    vSemaphoreCreateBinary(sem_handle); //v开头的，创建完会主动释放一次信号量
    if(sem_handle == NULL)
    {
        printf("二值信号量创建失败\r\n");
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
 * @description: 任务一：用于按键扫描，当检测到按键KEY1被按下时，释放二值信号量。
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
            /* 释放二值信号量 */
            res = xSemaphoreGive(sem_handle);
            if(res == pdPASS)
            {
                printf("Task1释放二值信号量成功\r\n");
            }
            else 
            {
                printf("Task1释放二值信号量失败\r\n");
            }
        }
        vTaskDelay(500);
    }
}

/**
 * @description: 任务二：获取二值信号量，当成功获取后打印提示信息
 * @param {void} *pvParameters
 * @return {*}
 */
void task2(void *pvParameters)
{
    BaseType_t res = 0;
    while (1)
    {
        /* 获取二值信号量 */
        res = xSemaphoreTake(sem_handle,portMAX_DELAY);
        if(res == pdPASS)
        {
            printf("Task2获取信号量成功\r\n");
        }
        else 
        {
            printf("Task2获取信号量失败\r\n");   
        }

    }
}


