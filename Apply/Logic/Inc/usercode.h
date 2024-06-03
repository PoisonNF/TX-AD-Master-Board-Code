#ifndef	__USERCODE_H_
#define	__USERCODE_H_

#include "drv_hal_conf.h"

#include "lwip_comm.h"
#include "semphr.h"

void UserLogic_Code(void);
void Start_Task(void *pvParameters);        /* ������ */

extern uint8_t NumberOfBoards;
extern uint8_t File_Name[];
extern uint32_t logNum;

/* �ź������ */
extern SemaphoreHandle_t PowerDetect_Sema;  /* ��Դ����ź��� */

/* ��������� */
extern SemaphoreHandle_t UDP_SendBuffer_Mutex;     /* UDP���ͻ����д�� */

/* ��Ϣ���� */
extern QueueHandle_t CANRecv_Queue;

/* �¼� */
extern EventGroupHandle_t Log_Event;

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
#define LED_STK_SIZE            8          /* �����ջ��С */
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
#define SERIALSCREEN_STK_SIZE            256         /* �����ջ��С */
extern TaskHandle_t SerialScreen_Task_Handler;       /* ������ */

/* CAN_Task ���� ����
 * ����: ������ �������ȼ� ��ջ��С ��������
 */
#define CAN_TASK_PRIO           10           /* �������ȼ� */
#define CAN_STK_SIZE            512         /* �����ջ��С */
extern TaskHandle_t CAN_Task_Handler;

/* TFCard_Task ���� ����
 * ����: ������ �������ȼ� ��ջ��С ��������
 */
#define TFCARD_TASK_PRIO           4           /* �������ȼ� */
#define TFCARD_STK_SIZE            512         /* �����ջ��С */
extern TaskHandle_t TFCard_Handler;
#endif
