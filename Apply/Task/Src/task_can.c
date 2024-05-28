#include "task_can.h"

static uint8_t HandleBuffer[9] = {0};		//等待处理的缓冲区,最后一个字节为ID号
static uint8_t StoreLineNumber[96] = {0};	//用于存放96通道在LwIP_UDP_SendBuffer的存储行号

/**
 * @brief CAN接受的AD数据处理函数
 * @param _ucCanMsg 由CAN接收到的数据缓存
 * @retval Null 
*/
static void S_Data_Process(uint8_t *_ucCanMsg)
{
#if 0	//适用于板卡直接输出电压值
	uint8_t NoNum = 0; 		//由CANID减去0x41得到
	uint8_t ChannelNumF = 0;	//通道号
	uint8_t RoundNumF = 0;	//采集轮数
	float ff = 0.0f;

	NoNum = _ucCanMsg[8] - 0x41; 		//计算板卡位次，第一块板卡ID号为0x41

	if(_ucCanMsg[2] == 0x2e)	//代表接收到了数据，该位置为小数点
	{
		ChannelNumF = (_ucCanMsg[0]&0xf0)>>4;
		RoundNumF = _ucCanMsg[0]&0x0f;

		if(_ucCanMsg[1]&0x80)	//收到为负值
		{
			_ucCanMsg[1] = _ucCanMsg[1]&0x7f;
			ff = (_ucCanMsg[1] - '0')*1.0+ (_ucCanMsg[3] - '0') * 0.1 + (_ucCanMsg[4] - '0') * 0.01 + 
							(_ucCanMsg[5] - '0') * 0.001 + (_ucCanMsg[6] - '0') * 0.0001 + (_ucCanMsg[7] - '0') * 0.00001;
			
			printf("ID = %c  num = %d  round = %d ff = -%.5f\r\n",NoNum+'A'-1, ChannelNumF,RoundNumF, ff);//ID 通道数 轮数 数据
			ff = 0; 
		}
		else	
		{
			ff = (_ucCanMsg[1] - '0')*1.0+ (_ucCanMsg[3] - '0') * 0.1 + (_ucCanMsg[4] - '0') * 0.01 + 
							(_ucCanMsg[5] - '0') * 0.001 + (_ucCanMsg[6] - '0') * 0.0001 + (_ucCanMsg[7] - '0') * 0.00001;
			printf("ID = %c  num = %d  round = %d ff = %.5f\r\n", NoNum+'A'-1, ChannelNumF,RoundNumF, ff);//ID 通道数 轮数 数据
			ff = 0; 
		}
	}
#else	//适用于UDP传输
	uint8_t NoNum = 0; 					//由CANID减去0x41得到

	//前半部分的数据
	uint8_t ChannelNumF = 0;			//通道号
	//uint8_t RoundNumF = 0;			//采集轮数
	uint8_t ValidMarkF = 0;				//数据有效标志位
	uint16_t BoardAddrF = 0;			//板卡在数组中的起始地址
	uint16_t ChannelAddrF = 0;			//通道在数组中的起始地址
	uint8_t ChannelNumFInAll = 0;		//在所有通道中的通道号[0-95]

	//后半部分的数据
	uint8_t ChannelNumS = 0;			//通道号
	//uint8_t RoundNumS = 0;			//采集轮数
	uint8_t ValidMarkS = 0;				//数据有效标志位
	uint16_t BoardAddrS = 0;			//板卡在数组中的起始地址
	uint16_t ChannelAddrS = 0;			//通道在数组中的起始地址
	uint8_t ChannelNumSInAll = 0;		//在所有通道中的通道号[0-95]

	// UNUSED(RoundNumF);
	// UNUSED(RoundNumS);

	NoNum = _ucCanMsg[8] - 0x41; 		//计算板卡位次，第一块板卡ID号为0x41

	/* 先处理0-3字节 */
	ChannelNumF = _ucCanMsg[0]&0x0f;				//获取通道号 0-7
	//RoundNumF = _ucCanMsg[0]&0x07;				//获取轮数 0-7
	ValidMarkF = (_ucCanMsg[0] >> 7)&1;				//获取有效标志位
	BoardAddrF = NoNum * 24 + 2;					//计算板卡在数组中的地址，每个板卡占24字节，2为帧头长度
	ChannelAddrF = BoardAddrF + ChannelNumF * 3;	//通道地址在板卡地址的基础上偏移，每次偏移3个字节
	ChannelNumFInAll = NoNum * 8 + ChannelNumF;		//计算所有通道中的通道号

	/* 再处理4-7字节 */
	ChannelNumS = _ucCanMsg[4]&0x0f;				//获取通道号 0-7
	//RoundNumS = _ucCanMsg[4]&0x07;				//获取轮数 0-7
	ValidMarkS = (_ucCanMsg[4] >> 7)&1;				//获取有效标志位
	BoardAddrS = NoNum * 24 + 2;					//计算板卡在数组中的地址，每个板卡占24字节，2为帧头长度
	ChannelAddrS = BoardAddrS + ChannelNumS * 3;	//通道地址在板卡地址的基础上偏移，每次偏移3个字节
	ChannelNumSInAll = NoNum * 8 + ChannelNumS;		//计算所有通道中的通道号

	/* 数据拷贝 */
	if(xSemaphoreTake(UDP_SendBuffer_Mutex,portMAX_DELAY) == pdTRUE)	//获取互斥量，上锁
	{
		LwIP_UDP_SendBuffer[StoreLineNumber[ChannelNumFInAll]][ChannelAddrF] = _ucCanMsg[1];
		LwIP_UDP_SendBuffer[StoreLineNumber[ChannelNumFInAll]][ChannelAddrF + 1] = _ucCanMsg[2];
		LwIP_UDP_SendBuffer[StoreLineNumber[ChannelNumFInAll]][ChannelAddrF + 2] = _ucCanMsg[3];
		LwIP_UDP_SendBuffer[StoreLineNumber[ChannelNumFInAll]][290 + NoNum] |= (ValidMarkF << (7 - ChannelNumF)); 	//有效位拷贝

		LwIP_UDP_SendBuffer[StoreLineNumber[ChannelNumSInAll]][ChannelAddrS] = _ucCanMsg[5];
		LwIP_UDP_SendBuffer[StoreLineNumber[ChannelNumSInAll]][ChannelAddrS + 1] = _ucCanMsg[6];
		LwIP_UDP_SendBuffer[StoreLineNumber[ChannelNumSInAll]][ChannelAddrS + 2] = _ucCanMsg[7];
		LwIP_UDP_SendBuffer[StoreLineNumber[ChannelNumSInAll]][290 + NoNum] |= (ValidMarkS << (7 - ChannelNumS)); 	//有效位拷贝
		xSemaphoreGive(UDP_SendBuffer_Mutex);									//释放互斥量，解锁
	}

	/* 对应下一个存储行号偏移 */
	StoreLineNumber[ChannelNumFInAll]++;
	StoreLineNumber[ChannelNumSInAll]++;
	if(StoreLineNumber[ChannelNumFInAll] == SPLICE_NUM)
	{
		StoreLineNumber[ChannelNumFInAll] = 0;
	}
	if(StoreLineNumber[ChannelNumSInAll] == SPLICE_NUM)
	{
		StoreLineNumber[ChannelNumSInAll] = 0;
	}
#endif
}


/**
 * @brief CAN处理函数
 * @param _tCAN-CAN结构体指针
 */
void Task_CAN_Handle(tagCAN_T *_tCAN)
{	
    // 从队列中接收CAN数据
    if(xQueueReceive(CANRecv_Queue, &HandleBuffer, portMAX_DELAY) == pdPASS)
    {
#ifdef PRINTF_DEBUG
		for(int i = 0; i< 9;i++)
        	printf("%x", HandleBuffer[i]);
#endif
		S_Data_Process(HandleBuffer);
    }
}

