#include "usercode.h"		/* usercodeͷ�ļ� */
#include "drv_hal_conf.h"   /* SGA��ͷ�ļ����� */
#include "task_conf.h"      /* task��ͷ�ļ����� */
#include "ocd_conf.h"       /* OCD��ͷ�ļ����� */
#include "dev_conf.h"		/* Dev��ͷ�ļ����� */
#include "algo_conf.h"		/* Algo��ͷ�ļ����� */
#include "config.h"			/* I/O����ͷ�ļ����� */

/* ������ */
TaskHandle_t Start_Task_Handler;             
TaskHandle_t LWIP_Task_Handler;        
TaskHandle_t LED_Task_Handler;         
TaskHandle_t PowerDetect_Task_Handler; 
TaskHandle_t SerialScreen_Task_Handler;
TaskHandle_t CAN_Task_Handler;

/* �ź��� */
SemaphoreHandle_t PowerDetect_Sema;     //��Դ����ź���
SemaphoreHandle_t BoardDetect_Sema;     //�忨����ź���

/* ������ */
SemaphoreHandle_t UDP_SendBuffer_Mutex; //UDP���ͻ����д��

/* ��Ϣ���� */
QueueHandle_t CANRecv_Queue;               //CAN������Ϣ����
#define CAN_RX_QUEUE_LENGTH  200         //��Ϣ���г���

/* �û��߼����� */
void UserLogic_Code(void)
{
	    /* �Լ� */
    Task_SelfCheck();    //�忨����ID	
	
    PowerDetect_Sema = xSemaphoreCreateBinary();            //������ֵ�ź���
    BoardDetect_Sema = xSemaphoreCreateBinary();            //������ֵ�ź���
    UDP_SendBuffer_Mutex = xSemaphoreCreateMutex();         //����������

    CANRecv_Queue = xQueueCreate(CAN_RX_QUEUE_LENGTH, 9);      //��������9�ֽڳ������Ϊ200����Ϣ����

    /* Start_Task */
    xTaskCreate((TaskFunction_t )Start_Task,
                (const char *   )"Start_Task",
                (uint16_t       )START_STK_SIZE,
                (void *         )NULL,
                (UBaseType_t    )START_TASK_PRIO,
                (TaskHandle_t * )&Start_Task_Handler);

    vTaskStartScheduler(); /* ����������� */
}

/**
 * @brief Start_Task
 * @param pvParameters : �������(δ�õ�)
 * @retval Null
 */
void Start_Task(void *pvParameters)
{
    UNUSED(pvParameters);

    taskENTER_CRITICAL();           /* �����ٽ��� */   
    /* ����CAN���� */
    xTaskCreate((TaskFunction_t )CAN_Task,
                (const char*    )"CAN_Task",
                (uint16_t       )CAN_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )CAN_TASK_PRIO,
                (TaskHandle_t*  )&CAN_Task_Handler);  
    taskEXIT_CRITICAL();                        /* �˳��ٽ��� */  

    while(lwip_comm_init() != 0)
    
    while(!ethernet_read_phy(PHY_SR))  /* ���MCU��PHYоƬ�Ƿ�ͨ�ųɹ� */
    {
        printf("MCU and PHY chip communication failed, please check the circuit or source code!!\r\n");
    }
    
    while((g_lwipdev.dhcpstatus != 2)&&(g_lwipdev.dhcpstatus != 0XFF))  /* �ȴ�DHCP��ȡ�ɹ�/��ʱ��� */
    {
        vTaskDelay(5);
    }


    
    taskENTER_CRITICAL();           /* �����ٽ��� */

    /* ����lwIP UDP���� */
    xTaskCreate((TaskFunction_t )LwIP_UDP_Task,
                (const char*    )"LwIP_UDP_Task",
                (uint16_t       )LWIP_DMEO_STK_SIZE, 
                (void*          )NULL,
                (UBaseType_t    )LWIP_DMEO_TASK_PRIO,
                (TaskHandle_t*  )&LWIP_Task_Handler);

    /* ����LED����ָʾ������ */
    xTaskCreate((TaskFunction_t )LED_Task,
                (const char*    )"LED_Task",
                (uint16_t       )LED_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )LED_TASK_PRIO,
                (TaskHandle_t*  )&LED_Task_Handler);

    /* ������Դ������� */
    xTaskCreate((TaskFunction_t )PowerDetect_Task,
                (const char*    )"PowerDetect_Task",
                (uint16_t       )POWERDETECT_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )POWERDETECT_TASK_PRIO,
                (TaskHandle_t*  )&PowerDetect_Task_Handler);

    /* �������������� */
    xTaskCreate((TaskFunction_t )SerialScreen_Task,
                (const char*    )"SerialScreen_Task",
                (uint16_t       )SERIALSCREEN_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )SERIALSCREEN_TASK_PRIO,
                (TaskHandle_t*  )&SerialScreen_Task_Handler);

    taskEXIT_CRITICAL();                        /* �˳��ٽ��� */  
								
		Task_SYNC_Signal();     //����ͬ���źţ���ʼ�ɼ�
								
    vTaskDelete(xTaskGetCurrentTaskHandle());   /* ɾ����ʼ���� */
}



