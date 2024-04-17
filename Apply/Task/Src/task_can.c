#include "task_can.h"

static uint8_t HandleBuffer[9] = {0};		//�ȴ�����Ļ�����,���һ���ֽ�ΪID��

/**
 * @brief CAN���ܵ�AD���ݴ�����
 * @param _ucCanID CAN��ţ����Ϊ0x41 - 0x4C
 * @param _ucCanMsg ��CAN���յ������ݻ���
 * @retval Null 
*/
static void S_Data_Process(uint8_t _ucCanID,uint8_t *_ucCanMsg)
{
	uint8_t NoNum = 0; 		//��CANID��ȥ0x41�õ�
	uint8_t ChannelNum = 0;	//ͨ����
	uint8_t RoundNum = 0;	//�ɼ�����
	float ff = 0.0f;

	NoNum = _ucCanID - 0x41; 

	if(_ucCanMsg[2] == 0x2e)	//������յ������ݣ���λ��ΪС����
	{
		ChannelNum = (_ucCanMsg[0]&0xf0)>>4;
		RoundNum = _ucCanMsg[0]&0x0f;

		if(_ucCanMsg[1]&0x80)	//�յ�Ϊ��ֵ
		{
			_ucCanMsg[1] = _ucCanMsg[1]&0x7f;
			ff = (_ucCanMsg[1] - '0')*1.0+ (_ucCanMsg[3] - '0') * 0.1 + (_ucCanMsg[4] - '0') * 0.01 + 
							(_ucCanMsg[5] - '0') * 0.001 + (_ucCanMsg[6] - '0') * 0.0001 + (_ucCanMsg[7] - '0') * 0.00001;
			
			printf("ID = %c  num = %d  round = %d ff = -%.5f\r\n",NoNum+'A'-1, ChannelNum,RoundNum, ff);//ID ͨ���� ���� ����
			ff = 0; 
		}
		else	
		{
			ff = (_ucCanMsg[1] - '0')*1.0+ (_ucCanMsg[3] - '0') * 0.1 + (_ucCanMsg[4] - '0') * 0.01 + 
							(_ucCanMsg[5] - '0') * 0.001 + (_ucCanMsg[6] - '0') * 0.0001 + (_ucCanMsg[7] - '0') * 0.00001;
			printf("ID = %c  num = %d  round = %d ff = %.5f\r\n", NoNum+'A'-1, ChannelNum,RoundNum, ff);//ID ͨ���� ���� ����
			ff = 0; 
		}

	}
}


/**
 * @brief CAN������
 * @param _tCAN-CAN�ṹ��ָ��
 */
void Task_CAN_Handle(tagCAN_T *_tCAN)
{	
    // �Ӷ����н���CAN����
    if(xQueueReceive(CANRecv_Queue, &HandleBuffer, 0) == pdPASS)
    {
#ifdef PRINTF_DEBUG
		for(int i = 0; i< 9;i++)
        	printf("%x", HandleBuffer[i]);
#endif
		S_Data_Process(HandleBuffer[8],HandleBuffer);
    }
}

