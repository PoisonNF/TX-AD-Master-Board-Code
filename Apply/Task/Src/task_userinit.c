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
    
    /* 初始化调试串口 */
    Drv_Uart_DMAInit(&Uart1);

    /* 初始化串口屏 */
    Drv_Uart_DMAInit(&Uart5);

    /* 初始化电源检测ADC */
    Drv_ADC_Init(PowerADC,1);

    /* 初始化CAN */
    Drv_CAN_ITInit(&CAN);
}
