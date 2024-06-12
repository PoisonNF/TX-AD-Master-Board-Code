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

    /* ��ʼ��TF����FATFS�ļ�ϵͳ */
   	if(OCD_FATFS_Init(&TFCard) != 0)
		TKCardIsExist = false;		//���TF���������ڣ�����������TF������

	/* ��ʼ��Ƭ��RTC */
	OCD_DS3231_Init(&DS1337);

    /* ��ʼ��AT24C02 */
    OCD_AT24CXX_Init(&EEPROM);
    
    /* ��ʼ�����Դ��� */
    Drv_Uart_DMAInit(&Uart1);

    /* ��ʼ��RS232��Ӵ��� */
    Drv_Uart_DMAInit(&Uart3);

    /* ��ʼ�������� */
    Drv_Uart_ITInit(&Uart5);

    /* ��ʼ����Դ���ADC */
    Drv_ADC_Init(PowerADC,1);

    /* ��ʼ��CAN */
    Drv_CAN_ITInit(&CAN);

	/* ��ʼ��PVD��Դ��� */
	Drv_PWR_PVD_Init(PWR_PVDLEVEL_4);		//�����͵�ѹΪ2.7V
}

