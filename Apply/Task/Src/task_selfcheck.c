#include "task_selfcheck.h"

#define ADBOARD_NUM_MAX			4			//最大插入上限个数

static uint8_t SetIDBuffer[8] = {0xA1,0x00,0x00,0x00,0x00,0x00,0x00,0x00};		//设置IDBuffer

static uint8_t SYNCBuffer[8] = {0xA2,0x00,0x00,0x00,0x00,0x00,0x00,0x00};    	//同步采集信号

uint8_t IDjugBuffer[12] = {0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48,0x49,0x4A,0x4B,0x4C}; //板卡ID判断数组

static uint8_t All_Boards_SpeedChangeBuffer[8] = {0xA4,0x00,0x00,0x00,0x00,0x00,0x00,0x00};   //所有板卡调整速率，通过CAN广播

static uint8_t All_Boards_ResetBuffer[8] = {0xA5,0x00,0x00,0x00,0x00,0x00,0x00,0x00};   //所有板卡复位指令，通过CAN广播

static uint8_t ID_Choose = 1;       //板卡在主板上的顺序号，NO1-12

static uint16_t BoardSpeedSet;		//告知采集板卡设定发送速率

__IO uint16_t TimeOut = 0;				//自检时间超时标志位
/**
 * @brief 自检函数，为板卡分配ID
 * @param Null
 * @retval uint8_t 插入板卡数量
 */
uint8_t Task_SelfCheck(void)
{
    uint8_t InsertNum = 0;
    
    //向所有板卡广播复位指令
    Drv_CAN_SendMsg(&CAN,All_Boards_ResetBuffer,8);
    Drv_Delay_Ms(1000);
    
	for(ID_Choose = 1; ID_Choose <= ADBOARD_NUM_MAX; ID_Choose++)
	{
		Drv_GPIO_Reset(&Control[ID_Choose - 1]);
		Drv_Delay_Ms(10);
		SetIDBuffer[1] = IDjugBuffer[ID_Choose - 1];
		Drv_CAN_SendMsg(&CAN,SetIDBuffer,8);	

		while((CAN.tCANRxHeader.StdId != IDjugBuffer[ID_Choose - 1]) && TimeOut < 0x100)
		{
            TimeOut++;
			Drv_Delay_Ms(10);
		}
		if(TimeOut >= 0x100)
		{
			TimeOut = 0;
			printf("NO.%d No Card\r\n",ID_Choose);
		}
		else
		{
			printf("NO.%d Find Card\r\n",ID_Choose);
			InsertNum++;						//插入板卡数加一
		}

		TimeOut = 0;
		Drv_GPIO_Set(&Control[ID_Choose - 1]);
	}
	printf("Insert %d Card\r\n",InsertNum);
	CurrentChannelNum = InsertNum * 8;		//记录当前通道数

	Drv_Delay_Ms(1000);		//防止最后一块板卡来不更改采样速率，一定要加

    /* 从EEPROM中读取保存通道数量和传输速率信息 */
    Task_EEPROM_ReadParameter(&CurrentChannelNum, &CurrentSendRate);

	/* 根据要求的通道数量限制采集板卡的发送速率 */
	if(CurrentChannelNum/8 <= 2){
		BoardSpeedSet = 400;					//板卡发送速度
	}
	else if(CurrentChannelNum/8 <= 4){
		BoardSpeedSet = 400;					//板卡发送速度
	}
	else if(CurrentChannelNum/8 <= 6){
		BoardSpeedSet = 400;					//板卡发送速度
	}
	else if(CurrentChannelNum/8 <= 12){
		BoardSpeedSet = 200;					//板卡发送速度
	}

	All_Boards_SpeedChangeBuffer[1] = BoardSpeedSet >> 24;
	All_Boards_SpeedChangeBuffer[2] = BoardSpeedSet >> 16;
	All_Boards_SpeedChangeBuffer[3] = BoardSpeedSet >> 8;
	All_Boards_SpeedChangeBuffer[4] = BoardSpeedSet;
    Drv_CAN_SendMsg(&CAN,All_Boards_SpeedChangeBuffer,8);

	return InsertNum;
}

/**
 * @brief 向板卡发送同步采集信号
 * @param Null
 */
void Task_SYNC_Signal(void)
{
	printf("Start Acquisiting\r\n");
	Drv_GPIO_Set(&SYNC[0]);  			//所有板卡上的AD4111同步完成
	vTaskDelay(1000);

	/* 根据需要的通道数，将SYNCBuffer中的对应位置1，
		这里将标志位放置在SYNCBuffer[1]后4位、SYNCBuffer[2]，只启动前X张，X范围[0,12] */
	for(uint8_t index = 0; index < CurrentChannelNum/8; index++)
	{
		if(index <= 7)
			SYNCBuffer[2] |= (1 << index);
		else
			SYNCBuffer[1] |= (1 << (index - 8));
	}

	Drv_CAN_SendMsg(&CAN,SYNCBuffer,8);	//发送开始同步命令	   
}

