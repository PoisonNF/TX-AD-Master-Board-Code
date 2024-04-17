#include "task_can.h"

static uint8_t HandleBuffer[9] = {0};		//等待处理的缓冲区,最后一个字节为ID号

/**
 * @brief CAN接受的AD数据处理函数
 * @param _ucCanID CAN编号，这边为0x41 - 0x4C
 * @param _ucCanMsg 由CAN接收到的数据缓存
 * @retval Null 
*/
static void S_Data_Process(uint8_t _ucCanID,uint8_t *_ucCanMsg)
{
	uint8_t NoNum = 0; 		//由CANID减去0x41得到
	uint8_t ChannelNum = 0;	//通道号
	uint8_t RoundNum = 0;	//采集轮数
	float ff = 0.0f;

	NoNum = _ucCanID - 0x41; 

	if(_ucCanMsg[2] == 0x2e)	//代表接收到了数据，该位置为小数点
	{
		ChannelNum = (_ucCanMsg[0]&0xf0)>>4;
		RoundNum = _ucCanMsg[0]&0x0f;

		if(_ucCanMsg[1]&0x80)	//收到为负值
		{
			_ucCanMsg[1] = _ucCanMsg[1]&0x7f;
			ff = (_ucCanMsg[1] - '0')*1.0+ (_ucCanMsg[3] - '0') * 0.1 + (_ucCanMsg[4] - '0') * 0.01 + 
							(_ucCanMsg[5] - '0') * 0.001 + (_ucCanMsg[6] - '0') * 0.0001 + (_ucCanMsg[7] - '0') * 0.00001;
			
			printf("ID = %c  num = %d  round = %d ff = -%.5f\r\n",NoNum+'A'-1, ChannelNum,RoundNum, ff);//ID 通道数 轮数 数据
			ff = 0; 
		}
		else	
		{
			ff = (_ucCanMsg[1] - '0')*1.0+ (_ucCanMsg[3] - '0') * 0.1 + (_ucCanMsg[4] - '0') * 0.01 + 
							(_ucCanMsg[5] - '0') * 0.001 + (_ucCanMsg[6] - '0') * 0.0001 + (_ucCanMsg[7] - '0') * 0.00001;
			printf("ID = %c  num = %d  round = %d ff = %.5f\r\n", NoNum+'A'-1, ChannelNum,RoundNum, ff);//ID 通道数 轮数 数据
			ff = 0; 
		}

	}
}


/**
 * @brief CAN处理函数
 * @param _tCAN-CAN结构体指针
 */
void Task_CAN_Handle(tagCAN_T *_tCAN)
{	
    // 从队列中接收CAN数据
    if(xQueueReceive(CANRecv_Queue, &HandleBuffer, 0) == pdPASS)
    {
#ifdef PRINTF_DEBUG
		for(int i = 0; i< 9;i++)
        	printf("%x", HandleBuffer[i]);
#endif
		S_Data_Process(HandleBuffer[8],HandleBuffer);
    }
}

