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
TaskHandle_t TFCard_Task_Handler;
TaskHandle_t Test_Task_Handler;

/* �ź��� */
SemaphoreHandle_t PowerDetect_Sema;     //��Դ����ź���

/* ������ */
SemaphoreHandle_t UDP_SendBuffer_Mutex; //UDP���ͻ����д��

/* ��Ϣ���� */
QueueHandle_t CANRecv_Queue;               //CAN������Ϣ����
#define CAN_RX_QUEUE_LENGTH  200         //��Ϣ���г���

/* �¼� */
EventGroupHandle_t Log_Event;            //��¼�¼�

/* ����忨���� */
uint8_t NumberOfBoards = 0;

/* �û��߼����� */
void UserLogic_Code(void)
{
	/* �Լ� */
    NumberOfBoards = Task_SelfCheck();    //�忨����ID	
	
    PowerDetect_Sema = xSemaphoreCreateBinary();            //������ֵ�ź���

    UDP_SendBuffer_Mutex = xSemaphoreCreateMutex();         //����������

    CANRecv_Queue = xQueueCreate(CAN_RX_QUEUE_LENGTH, 9);      //��������9�ֽڳ������Ϊ200����Ϣ����

    Log_Event = xEventGroupCreate();    //�����¼�

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

    /* ��EEPROM�ж�ȡ����ͨ�������ʹ���������Ϣ */
    Task_EEPROM_ReadParameter(&CurrentChannelNum, &CurrentSendRate);

    /* ����CAN���� */
    xTaskCreate((TaskFunction_t )CAN_Task,
                (const char*    )"CAN_Task",
                (uint16_t       )CAN_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )CAN_TASK_PRIO,
                (TaskHandle_t*  )&CAN_Task_Handler);  
    taskEXIT_CRITICAL();                        /* �˳��ٽ��� */  

    /* LwIP��ʼ������ */
    LwIP_Init();

    taskENTER_CRITICAL();           /* �����ٽ��� */

    /* ����lwIP UDP���� */
    xTaskCreate((TaskFunction_t )LwIP_UDP_Task,
                (const char*    )"LwIP_UDP_Task",
                (uint16_t       )LWIP_DEMO_STK_SIZE, 
                (void*          )NULL,
                (UBaseType_t    )LWIP_DEMO_TASK_PRIO,
                (TaskHandle_t*  )&LWIP_Task_Handler);

    /* ����LED����ָʾ������ */
    // xTaskCreate((TaskFunction_t )LED_Task,
    //             (const char*    )"LED_Task",
    //             (uint16_t       )LED_STK_SIZE,
    //             (void*          )NULL,
    //             (UBaseType_t    )LED_TASK_PRIO,
    //             (TaskHandle_t*  )&LED_Task_Handler);

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

    /* ����TF�����񣬼�¼log */
    xTaskCreate((TaskFunction_t )TFCard_Task,
                (const char*    )"TFCard_Task",
                (uint16_t       )TFCARD_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )TFCARD_TASK_PRIO,
                (TaskHandle_t*  )&TFCard_Task_Handler);

    /* ���������������ڲ��� */
    xTaskCreate((TaskFunction_t )Test_Task,
                (const char*    )"Test_Task",
                (uint16_t       )TEST_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )TEST_TASK_PRIO,
                (TaskHandle_t*  )&Test_Task_Handler);

    taskEXIT_CRITICAL();                        /* �˳��ٽ��� */  
								
	if(NumberOfBoards != 0)
        Task_SYNC_Signal();     //����ͬ���źţ���ʼ�ɼ�
								
    vTaskDelete(xTaskGetCurrentTaskHandle());   /* ɾ����ʼ���� */
}



