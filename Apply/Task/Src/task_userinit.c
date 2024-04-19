#include "task_conf.h"
#include "ocd_conf.h"
#include "config.h"

/* �����ʼ������ */
void Task_UserInit(void)
{
    /* ��ʼ��LEDָʾ�� */
	Drv_GPIO_Init(LED, 1);

	/* ��ʼ���忨�������� */
	Drv_GPIO_Init(Control, 12);
	for(int x = 0;x<12;x++)
	{
		Drv_GPIO_Set(&Control[x]);
	}

	/* ��ʼ��ADоƬͬ������ */
	Drv_GPIO_Init(SYNC, 1);
	Drv_GPIO_Reset(SYNC);//SYNC��ʼ���õ͵�ƽ
    
    /* ��ʼ�����Դ��� */
    Drv_Uart_DMAInit(&Uart1);

    /* ��ʼ�������� */
    Drv_Uart_DMAInit(&Uart5);

    /* ��ʼ����Դ���ADC */
    Drv_ADC_Init(PowerADC,1);

    /* ��ʼ��CAN */
    Drv_CAN_ITInit(&CAN);
}
