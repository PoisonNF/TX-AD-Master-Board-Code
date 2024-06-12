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
TaskHandle_t TFCard_Task_Handler;
TaskHandle_t Test_Task_Handler;

/* 信号量 */
SemaphoreHandle_t PowerDetect_Sema;     //电源检测信号量

/* 互斥量 */
SemaphoreHandle_t UDP_SendBuffer_Mutex; //UDP发送缓存读写锁

/* 消息队列 */
QueueHandle_t CANRecv_Queue;               //CAN接收消息队列
#define CAN_RX_QUEUE_LENGTH  200         //消息队列长度

/* 事件 */
EventGroupHandle_t Log_Event;            //记录事件

/* 插入板卡数量 */
uint8_t NumberOfBoards = 0;

/* 用户逻辑代码 */
void UserLogic_Code(void)
{
	/* 自检 */
    NumberOfBoards = Task_SelfCheck();    //板卡分配ID	
	
    PowerDetect_Sema = xSemaphoreCreateBinary();            //创建二值信号量

    UDP_SendBuffer_Mutex = xSemaphoreCreateMutex();         //创建互斥量

    CANRecv_Queue = xQueueCreate(CAN_RX_QUEUE_LENGTH, 9);      //创建单个9字节长，深度为200的消息队列

    Log_Event = xEventGroupCreate();    //创建事件

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

    /* 从EEPROM中读取保存通道数量和传输速率信息 */
    Task_EEPROM_ReadParameter(&CurrentChannelNum, &CurrentSendRate);

    /* 创建CAN任务 */
    xTaskCreate((TaskFunction_t )CAN_Task,
                (const char*    )"CAN_Task",
                (uint16_t       )CAN_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )CAN_TASK_PRIO,
                (TaskHandle_t*  )&CAN_Task_Handler);  
    taskEXIT_CRITICAL();                        /* 退出临界区 */  

    /* LwIP初始化函数 */
    LwIP_Init();

    taskENTER_CRITICAL();           /* 进入临界区 */

    /* 创建lwIP UDP任务 */
    xTaskCreate((TaskFunction_t )LwIP_UDP_Task,
                (const char*    )"LwIP_UDP_Task",
                (uint16_t       )LWIP_DEMO_STK_SIZE, 
                (void*          )NULL,
                (UBaseType_t    )LWIP_DEMO_TASK_PRIO,
                (TaskHandle_t*  )&LWIP_Task_Handler);

    /* 创建LED网络指示灯任务 */
    // xTaskCreate((TaskFunction_t )LED_Task,
    //             (const char*    )"LED_Task",
    //             (uint16_t       )LED_STK_SIZE,
    //             (void*          )NULL,
    //             (UBaseType_t    )LED_TASK_PRIO,
    //             (TaskHandle_t*  )&LED_Task_Handler);

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

    /* 创建TF卡任务，记录log */
    xTaskCreate((TaskFunction_t )TFCard_Task,
                (const char*    )"TFCard_Task",
                (uint16_t       )TFCARD_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )TFCARD_TASK_PRIO,
                (TaskHandle_t*  )&TFCard_Task_Handler);

    /* 创建测试任务，用于测试 */
    xTaskCreate((TaskFunction_t )Test_Task,
                (const char*    )"Test_Task",
                (uint16_t       )TEST_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )TEST_TASK_PRIO,
                (TaskHandle_t*  )&Test_Task_Handler);

    taskEXIT_CRITICAL();                        /* 退出临界区 */  
								
	if(NumberOfBoards != 0)
        Task_SYNC_Signal();     //发送同步信号，开始采集
								
    vTaskDelete(xTaskGetCurrentTaskHandle());   /* 删除开始任务 */
}



