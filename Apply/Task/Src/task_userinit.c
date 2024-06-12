#include "task_conf.h"
#include "ocd_conf.h"
#include "config.h"

/* 外设初始化函数 */
void Task_UserInit(void)
{
    /* 初始化LED指示灯 */
	Drv_GPIO_Init(LED, 1);

	/* 初始化板卡控制引脚 */
	Drv_GPIO_Init(Control, 12);
	for(int x = 0;x<12;x++)
	{
		Drv_GPIO_Set(&Control[x]);
	}

	/* 初始化AD芯片同步引脚 */
	Drv_GPIO_Init(SYNC, 1);
	Drv_GPIO_Reset(SYNC);//SYNC初始化置低电平

    /* 初始化TF卡和FATFS文件系统 */
   	if(OCD_FATFS_Init(&TFCard) != 0)
		TKCardIsExist = false;		//标记TF卡并不存在，后续将不对TF卡操作

	/* 初始化片外RTC */
	OCD_DS3231_Init(&DS1337);

    /* 初始化AT24C02 */
    OCD_AT24CXX_Init(&EEPROM);
    
    /* 初始化调试串口 */
    Drv_Uart_DMAInit(&Uart1);

    /* 初始化RS232外接串口 */
    Drv_Uart_DMAInit(&Uart3);

    /* 初始化串口屏 */
    Drv_Uart_ITInit(&Uart5);

    /* 初始化电源检测ADC */
    Drv_ADC_Init(PowerADC,1);

    /* 初始化CAN */
    Drv_CAN_ITInit(&CAN);

	/* 初始化PVD电源监测 */
	Drv_PWR_PVD_Init(PWR_PVDLEVEL_4);		//监测最低电压为2.7V
}

