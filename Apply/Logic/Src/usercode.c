#include "usercode.h"		/* usercode头文件 */
#include "drv_hal_conf.h"   /* SGA库头文件配置 */
#include "task_conf.h"      /* task层头文件配置 */
#include "ocd_conf.h"       /* OCD层头文件配置 */
#include "dev_conf.h"		/* Dev层头文件配置 */
#include "algo_conf.h"		/* Algo层头文件配置 */
#include "config.h"			/* I/O配置头文件配置 */

/* 任务句柄 */
TaskHandle_t Start_Task_Handler;             
TaskHandle_t LWIP_Task_Handler;        
TaskHandle_t LED_Task_Handler;         
TaskHandle_t PowerDetect_Task_Handler; 
TaskHandle_t SerialScreen_Task_Handler;
TaskHandle_t CAN_Task_Handler;

/* 信号量 */
SemaphoreHandle_t PowerDetect_Sema;     //电源检测信号量
SemaphoreHandle_t BoardDetect_Sema;     //板卡检测信号量

/* 互斥量 */
SemaphoreHandle_t UDP_SendBuffer_Mutex; //UDP发送缓存读写锁

/* 消息队列 */
QueueHandle_t CANRecv_Queue;               //CAN接收消息队列
#define CAN_RX_QUEUE_LENGTH  200         //消息队列长度

/* 用户逻辑代码 */
void UserLogic_Code(void)
{
	    /* 自检 */
    Task_SelfCheck();    //板卡分配ID	
	
    PowerDetect_Sema = xSemaphoreCreateBinary();            //创建二值信号量
    BoardDetect_Sema = xSemaphoreCreateBinary();            //创建二值信号量
    UDP_SendBuffer_Mutex = xSemaphoreCreateMutex();         //创建互斥量

    CANRecv_Queue = xQueueCreate(CAN_RX_QUEUE_LENGTH, 9);      //创建单个9字节长，深度为200的消息队列

    /* Start_Task */
    xTaskCreate((TaskFunction_t )Start_Task,
                (const char *   )"Start_Task",
                (uint16_t       )START_STK_SIZE,
                (void *         )NULL,
                (UBaseType_t    )START_TASK_PRIO,
                (TaskHandle_t * )&Start_Task_Handler);

    vTaskStartScheduler(); /* 开启任务调度 */
}

/**
 * @brief Start_Task
 * @param pvParameters : 传入参数(未用到)
 * @retval Null
 */
void Start_Task(void *pvParameters)
{
    UNUSED(pvParameters);

    taskENTER_CRITICAL();           /* 进入临界区 */   
    /* 创建CAN任务 */
    xTaskCreate((TaskFunction_t )CAN_Task,
                (const char*    )"CAN_Task",
                (uint16_t       )CAN_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )CAN_TASK_PRIO,
                (TaskHandle_t*  )&CAN_Task_Handler);  
    taskEXIT_CRITICAL();                        /* 退出临界区 */  

    while(lwip_comm_init() != 0)
    
    while(!ethernet_read_phy(PHY_SR))  /* 检查MCU与PHY芯片是否通信成功 */
    {
        printf("MCU and PHY chip communication failed, please check the circuit or source code!!\r\n");
    }
    
    while((g_lwipdev.dhcpstatus != 2)&&(g_lwipdev.dhcpstatus != 0XFF))  /* 等待DHCP获取成功/超时溢出 */
    {
        vTaskDelay(5);
    }


    
    taskENTER_CRITICAL();           /* 进入临界区 */

    /* 创建lwIP UDP任务 */
    xTaskCreate((TaskFunction_t )LwIP_UDP_Task,
                (const char*    )"LwIP_UDP_Task",
                (uint16_t       )LWIP_DMEO_STK_SIZE, 
                (void*          )NULL,
                (UBaseType_t    )LWIP_DMEO_TASK_PRIO,
                (TaskHandle_t*  )&LWIP_Task_Handler);

    /* 创建LED网络指示灯任务 */
    xTaskCreate((TaskFunction_t )LED_Task,
                (const char*    )"LED_Task",
                (uint16_t       )LED_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )LED_TASK_PRIO,
                (TaskHandle_t*  )&LED_Task_Handler);

    /* 创建电源检测任务 */
    xTaskCreate((TaskFunction_t )PowerDetect_Task,
                (const char*    )"PowerDetect_Task",
                (uint16_t       )POWERDETECT_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )POWERDETECT_TASK_PRIO,
                (TaskHandle_t*  )&PowerDetect_Task_Handler);

    /* 创建串口屏任务 */
    xTaskCreate((TaskFunction_t )SerialScreen_Task,
                (const char*    )"SerialScreen_Task",
                (uint16_t       )SERIALSCREEN_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )SERIALSCREEN_TASK_PRIO,
                (TaskHandle_t*  )&SerialScreen_Task_Handler);

    taskEXIT_CRITICAL();                        /* 退出临界区 */  
								
		Task_SYNC_Signal();     //发送同步信号，开始采集
								
    vTaskDelete(xTaskGetCurrentTaskHandle());   /* 删除开始任务 */
}



