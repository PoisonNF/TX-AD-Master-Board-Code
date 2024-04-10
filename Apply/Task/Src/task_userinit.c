#include "task_conf.h"
#include "ocd_conf.h"
#include "config.h"

/* �����ʼ������ */
void Task_UserInit(void)
{
    /* ��ʼ��LEDָʾ�� */
	Drv_GPIO_Init(LED, 1);
    
    /* ��ʼ�����Դ��� */
    Drv_Uart_DMAInit(&Uart1);

    /* ��ʼ�������� */
    Drv_Uart_DMAInit(&Uart5);

    /* ��ʼ����Դ���ADC */
    Drv_ADC_Init(PowerADC,1);
}
