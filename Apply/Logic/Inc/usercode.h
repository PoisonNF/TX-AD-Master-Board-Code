#ifndef	__USERCODE_H_
#define	__USERCODE_H_

#include "drv_hal_conf.h"

#include "lwip_comm.h"
#include "semphr.h"

void UserLogic_Code(void);
void Start_Task(void *pvParameters);        /* 任务函数 */

/* 信号量句柄 */
extern SemaphoreHandle_t PowerDetect_Sema;  /* 电源检测信号量 */

/* Start_Task 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
#define START_TASK_PRIO         5           /* 任务优先级 */
#define START_STK_SIZE          128         /* 任务堆栈大小 */
extern TaskHandle_t Start_Task_Handler;      /* 任务句柄 */


/* LwIP_UDP_Task 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
#define LWIP_DMEO_TASK_PRIO     11          /* 任务优先级 */
#define LWIP_DMEO_STK_SIZE      1024        /* 任务堆栈大小 */
extern TaskHandle_t LWIP_Task_Handler;      /* 任务句柄 */


/* LED_Task 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
#define LED_TASK_PRIO           5            /* 任务优先级 */
#define LED_STK_SIZE            128          /* 任务堆栈大小 */
extern TaskHandle_t LED_Task_Handler;        /* 任务句柄 */

/* PowerDetect_Task 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
#define POWERDETECT_TASK_PRIO           6           /* 任务优先级 */
#define POWERDETECT_STK_SIZE            128         /* 任务堆栈大小 */
extern TaskHandle_t PowerDetect_Task_Handler;       /* 任务句柄 */

/* SerialScreen_Task 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
#define SERIALSCREEN_TASK_PRIO           9           /* 任务优先级 */
#define SERIALSCREEN_STK_SIZE            512         /* 任务堆栈大小 */
extern TaskHandle_t SerialScreen_Task_Handler;       /* 任务句柄 */

#endif
