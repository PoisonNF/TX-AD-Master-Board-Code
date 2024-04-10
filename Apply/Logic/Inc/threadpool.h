#ifndef __THREADPOOL_H_
#define __THREADPOOL_H_

/* 线程入口函数声明 */
void LwIP_UDP_Task(void *pvParameters);
void LED_Task(void *pvParameters);
void PowerDetect_Task(void *pvParameters);
void SerialScreen_Task(void *pvParameters);

#endif
