#include "freertos_demo.h"
/* freertos相关的头文件，必须的 */
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h" // 软件定时器的头文件
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

/* 软件定时器的超时回调函数 */
void timer1_callback(TimerHandle_t xTimer); // 单次
void timer2_callback(TimerHandle_t xTimer); // 周期

/* 软件定时器的句柄 */
TimerHandle_t timer1_handle;
TimerHandle_t timer2_handle;

/**
 * @description: 启动FreeRTOS
 * @return {*}
 */
void freertos_start(void)
{

    /* 创建一次性软件定时器 */
    timer1_handle = xTimerCreate(
        "ergou",         // 软件定时器的名称
        (TickType_t)500, // 超时时间 = 500个 RTOS的时钟节拍
        pdFALSE,         // 是否自动重载：pdTRUE --- 周期型； pdFALSE --- 一次性
        (void *)1,       // 定时器的唯一ID
        timer1_callback);

    if (timer1_handle != NULL)
    {
        printf("timer1一次性定时器创建成功...\r\n");
    }

    /* 创建周期性软件定时器 */
    timer2_handle = xTimerCreate(
        "gousheng",       // 软件定时器的名称
        (TickType_t)1000, // 超时时间 = 1000个 RTOS的时钟节拍
        pdTRUE,           // 是否自动重载：pdTRUE --- 周期型； pdFALSE --- 一次性
        (void *)2,        // 定时器的唯一ID
        timer2_callback);
    if (timer2_handle != NULL)
    {
        printf("timer2周期性定时器创建成功...\r\n");
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

    /* 启动任务只需要执行一次即可，用完就删除自己 */
    vTaskDelete(NULL);

    /* 退出临界区 */
    taskEXIT_CRITICAL();
}

/**
 * @description: 任务一：用于按键扫描，并对软件定时器进行开启、停止操作
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
            /* 开启软件定时器 */
            res = xTimerStart(timer1_handle, portMAX_DELAY);
            if (res != pdFAIL)
            {
                printf("timer1单次定时器启动成功\r\n");
            }

            res = xTimerStart(timer2_handle, portMAX_DELAY);
            if (res != pdFAIL)
            {
                printf("timer2周期定时器启动成功\r\n");
            }
        }
        else if (key == KEY2_PRESS)
        {
            /* 停止软件定时器 */
            res = xTimerStop(timer1_handle, portMAX_DELAY);
            if (res != pdFAIL)
            {
                printf("timer1单次定时器停止成功\r\n");
            }
            res = xTimerStop(timer2_handle, portMAX_DELAY);
            if (res != pdFAIL)
            {
                printf("timer2周期定时器停止成功\r\n");
            }
        }

        vTaskDelay(500);
    }
}


void timer1_callback(TimerHandle_t xTimer)
{
    static uint16_t timer1_count = 0;
    printf("timer1超时回调=%d次..\r\n",++timer1_count);
}

void timer2_callback(TimerHandle_t xTimer)
{
    static uint16_t timer2_count = 0;
    printf("timer2超时回调=%d次..\r\n",++timer2_count);
}
