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
 * @brief lwIP��������
 * @param pvParameters : �������(δ�õ�)
 * @retval Null
 */
void LwIP_UDP_Task(void *pvParameters)
{
    pvParameters = pvParameters;

    Task_LwIP_UDP_Handle();
}

/**
 * @brief ϵͳ������ָʾ��
 * @param pvParameters : �������(δ�õ�)
 * @retval Null
 */
void LED_Task(void *pvParameters)
{
    pvParameters = pvParameters;
    
    while (1)
    {
        Drv_GPIO_Toggle(&demoGPIO[1]);
        vTaskDelay(500);
    }
}




