#include "freertos_demo.h"
/* freertos相关的头文件，必须的 */
#include "FreeRTOS.h"
#include "task.h"
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
#define TASK2_STACK 256
#define TASK2_PRIORITY 3
TaskHandle_t task2_handle;
void task2(void *pvParameters);

/**
 * @description: 启动FreeRTOS
 * @return {*}
 */
void freertos_start(void)
{
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
 * @description: 任务一：实现LED1每500ms闪烁一次
 * @param {void} *pvParameters
 * @return {*}
 */
void task1(void *pvParameters)
{

    while (1)
    {
        printf("task1运行...\r\n");
        LED_Toggle(LED1_Pin);
        vTaskDelay(500);
    }
}

/**
 * @description: 任务二：用于展示任务状态查询相关API函数的使用
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

    /* 通过句柄，获取任务的优先级 */
    task_priority = uxTaskPriorityGet(task1_handle);
    printf("task1任务优先级=%d\r\n", task_priority);
    /* 设置任务的优先级 */
    vTaskPrioritySet(task1_handle, 4);
    task_priority = uxTaskPriorityGet(task1_handle);
    printf("task1任务优先级=%d\r\n", task_priority);

    /* 获取系统中任务的数量 */
    task_num = uxTaskGetNumberOfTasks();
    printf("系统中任务数量=%d\r\n", task_num);

    /* 获取所有任务状态 */
    uxTaskGetSystemState(task_status, task_num, NULL);
    printf("任务名\t\t任务编号\t\t任务状态\t\t当前优先级\t\t任务原始优先级\r\n");
    for (uint8_t i = 0; i < task_num; i++)
    {
        printf("%s\t\t%d\t\t%d\t\t%d\t\t%d\r\n",
               task_status[i].pcTaskName,
               task_status[i].xTaskNumber,
               task_status[i].eCurrentState,
               task_status[i].uxCurrentPriority,
               task_status[i].uxBasePriority);
    }

    /* 获取单个任务的信息 */
    vTaskGetInfo(
        task1_handle, // 要获取的任务信息的句柄
        &task_state,  // 用来保存查询到的任务状态信息
        pdTRUE,       // 要查询 任务栈 历史剩余最小值
        eInvalid);    // 设为eInvalid，才会真正去获取指定任务的状态
    printf("任务名称：%s\r\n", task_state.pcTaskName);
    printf("任务编号：%d\r\n", task_state.xTaskNumber);
    printf("任务状态：%d\r\n", task_state.eCurrentState);
    printf("任务优先级：%d\r\n", task_state.uxCurrentPriority);

    /* 获取当前任务的任务句柄 */
    task_handle = xTaskGetCurrentTaskHandle();
    printf("获取到的当前任务句柄=%p,task2的句柄=%p\r\n", task_handle, task2_handle);

    /* 通过任务名获取指定任务的句柄 */
    task_handle = xTaskGetHandle("task1");
    printf("获取到的名为task1任务句柄=%p,task1的句柄=%p\r\n", task_handle, task1_handle);

    /* ！！！获取任务栈历史剩余最小值：帮助我们判断任务栈分配的空间是否合适(任务栈大小 - 历史使用的最大值) */
    task_stack_remain_min = uxTaskGetStackHighWaterMark(task2_handle);
    printf("task2任务栈历史剩余最小值=%d\r\n",task_stack_remain_min);

    /* 获取任务状态 */
    e_task_state = eTaskGetState(task1_handle);
    printf("task1任务状态=%d\r\n",e_task_state);


    /* 以表格形式显示系统中的任务信息 */
    vTaskList(task_info);
    printf("%s\r\n",task_info);

    while (1)
    {
        vTaskDelay(500);
    }
}
