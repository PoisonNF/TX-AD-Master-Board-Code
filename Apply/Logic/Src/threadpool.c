#include "usercode.h"		/* usercode头文件 */
#include "threadpool.h"		/* threadpool头文件 */
#include "drv_hal_conf.h"   /* SGA库头文件配置 */
#include "task_conf.h"      /* task层头文件配置 */
#include "ocd_conf.h"       /* OCD层头文件配置 */
#include "dev_conf.h"		/* Dev层头文件配置 */
#include "algo_conf.h"		/* Algo层头文件配置 */
#include "config.h"			/* I/O配置头文件配置 */

/* 线程入口函数（使用裸机忽略此文件） */

/**
 * @brief lwIP运行例程
 * @param pvParameters : 传入参数(未用到)
 * @retval Null
 */
void LwIP_UDP_Task(void *pvParameters)
{
    pvParameters = pvParameters;

    Task_LwIP_UDP_Handle();
}

/**
 * @brief 系统再运行指示灯
 * @param pvParameters : 传入参数(未用到)
 * @retval Null
 */
void LED_Task(void *pvParameters)
{
    pvParameters = pvParameters;
    
    while (1)
    {
        Drv_GPIO_Toggle(&demoGPIO[1]);
        vTaskDelay(500);
    }
}




