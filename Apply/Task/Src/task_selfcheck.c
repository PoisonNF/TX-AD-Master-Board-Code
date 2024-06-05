#include "task_selfcheck.h"

#define ADBOARD_NUM_MAX	4			//最大插入上限个数

static uint8_t SetIDBuffer[8] = {0xA1,0x00,0x00,0x00,0x00,0x00,0x00,0x00};		//设置IDBuffer

static uint8_t SYNCBuffer[8] = {0xA2,0x53,0x00,0x00,0x00,0x00,0x00,0x00};    //同步采集信号

uint8_t IDjugBuffer[12] = {0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48,0x49,0x4A,0x4B,0x4C}; //板卡ID判断数组

static uint8_t ID_Choose = 1;       //板卡在主板上的顺序号，NO1-12

__IO uint16_t TimeOut = 0;				//自检时间超时标志位
/**
 * @brief 自检函数，为板卡分配ID
 * @param Null
 * @retval uint8_t 插入板卡数量
 */
uint8_t Task_SelfCheck(void)
{
    uint8_t InsertNum = 0;
	uint8_t TryNum = 0;

	for(ID_Choose = 1; ID_Choose <= ADBOARD_NUM_MAX; ID_Choose++)
	{
next:	Drv_GPIO_Reset(&Control[ID_Choose - 1]);
		HAL_Delay(10);
		SetIDBuffer[1] = IDjugBuffer[ID_Choose - 1];
		Drv_CAN_SendMsg(&CAN,SetIDBuffer,8);	

		while((CAN.tCANRxHeader.StdId != IDjugBuffer[ID_Choose - 1]) && TimeOut < 0x100)
		{
            TimeOut++;
			HAL_Delay(10);
		}
		if(TimeOut >= 0x100)
		{
			TimeOut = 0;
			TryNum++;
			if(TryNum<3)
			{
				printf("NO.%d time out!!\r\n",ID_Choose);
				goto next;
			}
			else
			{
				TryNum = 0;
				printf("NO.%d No Card\r\n",ID_Choose);
			}
			
		}else
		{
			printf("NO.%d Find Card\r\n",ID_Choose);
			InsertNum++;						//插入板卡数加一
		}

		TimeOut = 0;
		Drv_GPIO_Set(&Control[ID_Choose - 1]);
	}
	printf("Insert %d Card\r\n",InsertNum);
	CurrentChannelNum = InsertNum * 8;		//记录当前通道数
	return InsertNum;
}

/**
 * @brief 向所有板卡发送同步采集信号
 * @param Null
 */
void Task_SYNC_Signal(void)
{
	printf("Start Acquisiting\r\n");
	Drv_GPIO_Set(&SYNC[0]);  
	vTaskDelay(100);

	Drv_CAN_SendMsg(&CAN,SYNCBuffer,8);	//发送开始同步命令	   
}

