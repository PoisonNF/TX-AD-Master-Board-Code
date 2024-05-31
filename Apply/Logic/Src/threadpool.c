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
 * @brief lwIP UDP任务
 * @param pvParameters : 传入参数(未用到)
 * @retval Null
 */
void LwIP_UDP_Task(void *pvParameters)
{
    UNUSED(pvParameters);

    Task_LwIP_UDP_Handle();
}

/**
 * @brief 网络连接正常指示灯
 * @param pvParameters : 传入参数(未用到)
 * @retval Null
 */
void LED_Task(void *pvParameters)
{
    UNUSED(pvParameters);
    
    while (1)
    {
        /* LwIP处于连接状态时，翻转LED */
        if(g_lwipdev.link_status == LWIP_LINK_ON)
            Drv_GPIO_Toggle(&LED[0]);
        vTaskDelay(500);
    }
}

/**
 * @brief 电源检测任务
 * @param pvParameters : 传入参数(未用到)
 * @retval Null
 */
void PowerDetect_Task(void *pvParameters)
{
    UNUSED(pvParameters);
    
    while (1)
    {
        Task_PowerDetect_Handle(&PowerADC[0]);
        vTaskDelay(1);
    }
}

/**
 * @brief 串口屏处理任务
 * @param pvParameters : 传入参数(未用到)
 * @retval Null
 */
void SerialScreen_Task(void *pvParameters)
{
    UNUSED(pvParameters);
    
    while (1)
    {
        Task_SerialScreen_Handle(&Uart5);
        vTaskDelay(1);
    }
}

/**
 * @brief CAN处理任务
 * @param pvParameters : 传入参数(未用到)
 * @retval Null
 */
void CAN_Task(void *pvParameters)
{
    UNUSED(pvParameters);
    
    while (1)
    {
        Task_CAN_Handle(&CAN);
		taskYIELD();
    }
}

extern uint32_t logNum;
/**
 * @brief TF卡存储log任务
 * @param pvParameters : 传入参数(未用到)
 * @retval Null
 */
void TFCard_Task(void *pvParameters)
{
    UNUSED(pvParameters);

    Task_TFCard_CreateFolder(&TFCard);  //创建保存log文件夹

    logNum = Task_TFCard_FindEnd(&TFCard);          //遍历TF卡计算总条数
    printf("TF卡现有%d条log\r\n",logNum);

    //xEventGroupSetBits(Log_Event,EVENT1);         //标记测试事件成立
    xEventGroupSetBits(Log_Event,POWER_ON_EVENT);   //标记开机事件成立 

    while (1)
    {
        Task_TFCard_Handle(&TFCard);
        vTaskDelay(1);
    }
}




