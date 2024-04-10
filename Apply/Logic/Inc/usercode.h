#ifndef	__USERCODE_H_
#define	__USERCODE_H_

#include "drv_hal_conf.h"

#include "lwip_comm.h"
#include "semphr.h"

void UserLogic_Code(void);
void Start_Task(void *pvParameters);        /* ������ */

/* �ź������ */
extern SemaphoreHandle_t PowerDetect_Sema;  /* ��Դ����ź��� */

/* Start_Task ���� ����
 * ����: ������ �������ȼ� ��ջ��С ��������
 */
#define START_TASK_PRIO         5           /* �������ȼ� */
#define START_STK_SIZE          128         /* �����ջ��С */
extern TaskHandle_t Start_Task_Handler;      /* ������ */


/* LwIP_UDP_Task ���� ����
 * ����: ������ �������ȼ� ��ջ��С ��������
 */
#define LWIP_DMEO_TASK_PRIO     11          /* �������ȼ� */
#define LWIP_DMEO_STK_SIZE      1024        /* �����ջ��С */
extern TaskHandle_t LWIP_Task_Handler;      /* ������ */


/* LED_Task ���� ����
 * ����: ������ �������ȼ� ��ջ��С ��������
 */
#define LED_TASK_PRIO           5            /* �������ȼ� */
#define LED_STK_SIZE            128          /* �����ջ��С */
extern TaskHandle_t LED_Task_Handler;        /* ������ */

/* PowerDetect_Task ���� ����
 * ����: ������ �������ȼ� ��ջ��С ��������
 */
#define POWERDETECT_TASK_PRIO           6           /* �������ȼ� */
#define POWERDETECT_STK_SIZE            128         /* �����ջ��С */
extern TaskHandle_t PowerDetect_Task_Handler;       /* ������ */

/* SerialScreen_Task ���� ����
 * ����: ������ �������ȼ� ��ջ��С ��������
 */
#define SERIALSCREEN_TASK_PRIO           9           /* �������ȼ� */
#define SERIALSCREEN_STK_SIZE            512         /* �����ջ��С */
extern TaskHandle_t SerialScreen_Task_Handler;       /* ������ */

#endif
