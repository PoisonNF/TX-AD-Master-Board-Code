#include "task_selfcheck.h"

#define ADBOARD_NUM_MAX	12			//最大插入上限个数

static uint8_t SetIDBuffer[8] = {0xA1,0x00,0x00,0x00,0x00,0x00,0x00,0x00};		//设置IDBuffer

static uint8_t IDCollection[13] = {0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48,0x49,0x4A,0x4B,0x4C,0x4D}; //板卡ID合集，从'A' - 'L',0x4D防止最后一块板得不到ID

static uint8_t SYNCBuffer[8] = {0xA2,0x53,0x00,0x00,0x00,0x00,0x00,0x00};    //同步采集信号

static uint8_t ID_Choose = 1;       //板卡在主板上的顺序号，NO1-12

/**
 * @brief 自检函数，为板卡分配ID
 * @param Null
 * @retval uint8_t 插入板卡数量
 */
uint8_t Task_SelfCheck(void)
{
    uint8_t InsertNum = 0;

    for(ID_Choose = 0; ID_Choose <= ADBOARD_NUM_MAX; ID_Choose++)
    {
		Drv_GPIO_Toggle(&Control[ID_Choose]);    //拉低对应板卡控制引脚，选中板卡
		vTaskDelay(50);
        SetIDBuffer[1] = IDCollection[ID_Choose];
        Drv_CAN_SendMsg(&CAN,SetIDBuffer,8);        //发送包含ID的报文
		vTaskDelay(150);
#ifdef PRINTF_DEBUG
		printf("Send:");
		for(int i = 0; i< 8;i++)
        	printf("%x ", SetIDBuffer[i]);
		printf("\r\n");
#endif

		if(xSemaphoreTake(BoardDetect_Sema,300) == pdTRUE)
		{
			printf("NO.%d Find Card\r\n",ID_Choose);
			InsertNum++;						//插入板卡数加一	
		}
		else
		{
			printf("NO.%d No Card\r\n",ID_Choose);
		}

		Drv_GPIO_Toggle(&Control[ID_Choose]);
    }

	printf("Insert %d Card\r\n",InsertNum);

	vSemaphoreDelete(BoardDetect_Sema);		//检验完毕后，删除信号量
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

