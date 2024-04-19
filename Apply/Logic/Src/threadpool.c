#include "usercode.h"		/* usercodeͷ�ļ� */
#include "threadpool.h"		/* threadpoolͷ�ļ� */
#include "drv_hal_conf.h"   /* SGA��ͷ�ļ����� */
#include "task_conf.h"      /* task��ͷ�ļ����� */
#include "ocd_conf.h"       /* OCD��ͷ�ļ����� */
#include "dev_conf.h"		/* Dev��ͷ�ļ����� */
#include "algo_conf.h"		/* Algo��ͷ�ļ����� */
#include "config.h"			/* I/O����ͷ�ļ����� */

/* �߳���ں�����ʹ��������Դ��ļ��� */

/**
 * @brief lwIP UDP����
 * @param pvParameters : �������(δ�õ�)
 * @retval Null
 */
void LwIP_UDP_Task(void *pvParameters)
{
    UNUSED(pvParameters);

    Task_LwIP_UDP_Handle();
}

/**
 * @brief ������������ָʾ��
 * @param pvParameters : �������(δ�õ�)
 * @retval Null
 */
void LED_Task(void *pvParameters)
{
    UNUSED(pvParameters);
    
    while (1)
    {
        /* LwIP��������״̬ʱ����תLED */
        if(g_lwipdev.link_status == LWIP_LINK_ON)
            Drv_GPIO_Toggle(&LED[0]);
        vTaskDelay(500);
    }
}

/**
 * @brief ��Դ�������
 * @param pvParameters : �������(δ�õ�)
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
 * @brief ��������������
 * @param pvParameters : �������(δ�õ�)
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
 * @brief CAN��������
 * @param pvParameters : �������(δ�õ�)
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





