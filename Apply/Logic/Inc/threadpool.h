#ifndef __THREADPOOL_H_
#define __THREADPOOL_H_

/* �߳���ں������� */
void LwIP_UDP_Task(void *pvParameters);
void LED_Task(void *pvParameters);
void PowerDetect_Task(void *pvParameters);
void SerialScreen_Task(void *pvParameters);

#endif
