#include "usercode.h"		/* usercode头文件 */
#include "threadpool.h"		/* threadpool头文件 */
#include "drv_hal_conf.h"   /* SGA库头文件配置 */
#include "task_conf.h"      /* task层头文件配置 */
#include "ocd_conf.h"       /* OCD层头文件配置 */
#include "dev_conf.h"		/* Dev层头文件配置 */
#include "algo_conf.h"		/* Algo层头文件配置 */
#include "config.h"			/* I/O配置头文件配置 */

#include "lwip_comm.h"

/* 线程入口函数（使用裸机忽略此文件） */

/* LWIP_DEMO 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
#define LWIP_DMEO_TASK_PRIO     11          /* 任务优先级 */
#define LWIP_DMEO_STK_SIZE      1024        /* 任务堆栈大小 */
TaskHandle_t LWIP_Task_Handler;             /* 任务句柄 */
void lwip_demo_task(void *pvParameters);    /* 任务函数 */

/* LED_TASK 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
#define LED_TASK_PRIO           10          /* 任务优先级 */
#define LED_STK_SIZE            128         /* 任务堆栈大小 */
TaskHandle_t LEDTask_Handler;               /* 任务句柄 */
void led_task(void *pvParameters);          /* 任务函数 */


/**
 * @brief       lwIP运行例程
 * @param       pvParameters : 传入参数(未用到)
 * @retval      无
 */
void lwip_demo_task(void *pvParameters)
{
    pvParameters = pvParameters;
    
    uint8_t t = 0;
    
    while (1)
    {
        //printf("lwip_demo_task\r\n");
        t ++;
        
        if ((t % 10) == 0)
        {
            Drv_GPIO_Toggle(&demoGPIO[0]);    /* 翻转一次LED0 */
        }
        
        vTaskDelay(1000);
    }
}

/**
 * @brief       系统再运行
 * @param       pvParameters : 传入参数(未用到)
 * @retval      无
 */
void led_task(void *pvParameters)
{
    pvParameters = pvParameters;
    
    while (1)
    {
        //printf("led_task\r\n");
        Drv_GPIO_Toggle(&demoGPIO[1]);
        vTaskDelay(500);
    }
}

/**
 * @brief       start_task
 * @param       pvParameters : 传入参数(未用到)
 * @retval      无
 */
void start_task(void *pvParameters)
{
    pvParameters = pvParameters;
    
    while (lwip_comm_init() != 0)
    
    while (!ethernet_read_phy(PHY_SR))  /* 检查MCU与PHY芯片是否通信成功 */
    {
        printf("MCU与PHY芯片通信失败，请检查电路或者源码!!!\r\n");
    }
    
    printf("MCU与PHY芯片通信成功!!!\r\n");
    
    while ((g_lwipdev.dhcpstatus != 2)&&(g_lwipdev.dhcpstatus != 0XFF))  /* 等待DHCP获取成功/超时溢出 */
    {
        vTaskDelay(5);
    }
    
    
    taskENTER_CRITICAL();           /* 进入临界区 */

    /* 创建lwIP任务 */
    xTaskCreate((TaskFunction_t )lwip_demo_task,
                (const char*    )"lwip_demo_task",
                (uint16_t       )LWIP_DMEO_STK_SIZE, 
                (void*          )NULL,
                (UBaseType_t    )LWIP_DMEO_TASK_PRIO,
                (TaskHandle_t*  )&LWIP_Task_Handler);

    /* LED测试任务 */
    xTaskCreate((TaskFunction_t )led_task,
                (const char*    )"led_task",
                (uint16_t       )LED_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )LED_TASK_PRIO,
                (TaskHandle_t*  )&LEDTask_Handler);
       

    vTaskDelete(StartTask_Handler); /* 删除开始任务 */
    taskEXIT_CRITICAL();            /* 退出临界区 */  

    
}



