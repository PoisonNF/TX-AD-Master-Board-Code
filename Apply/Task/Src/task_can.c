#include "task_can.h"

static uint8_t HandleBuffer[9] = {0};		//�ȴ�����Ļ�����,���һ���ֽ�ΪID��
extern uint8_t LwIP_UDP_SendBuffer[306];

/**
 * @brief CAN���ܵ�AD���ݴ�����
 * @param _ucCanMsg ��CAN���յ������ݻ���
 * @retval Null 
*/
static void S_Data_Process(uint8_t *_ucCanMsg)
{
#if 0	//�����ڰ忨ֱ�������ѹֵ
	uint8_t NoNum = 0; 		//��CANID��ȥ0x41�õ�
	uint8_t ChannelNumF = 0;	//ͨ����
	uint8_t RoundNumF = 0;	//�ɼ�����
	float ff = 0.0f;

	NoNum = _ucCanMsg[8] - 0x41; 		//����忨λ�Σ���һ��忨ID��Ϊ0x41

	if(_ucCanMsg[2] == 0x2e)	//������յ������ݣ���λ��ΪС����
	{
		ChannelNumF = (_ucCanMsg[0]&0xf0)>>4;
		RoundNumF = _ucCanMsg[0]&0x0f;

		if(_ucCanMsg[1]&0x80)	//�յ�Ϊ��ֵ
		{
			_ucCanMsg[1] = _ucCanMsg[1]&0x7f;
			ff = (_ucCanMsg[1] - '0')*1.0+ (_ucCanMsg[3] - '0') * 0.1 + (_ucCanMsg[4] - '0') * 0.01 + 
							(_ucCanMsg[5] - '0') * 0.001 + (_ucCanMsg[6] - '0') * 0.0001 + (_ucCanMsg[7] - '0') * 0.00001;
			
			printf("ID = %c  num = %d  round = %d ff = -%.5f\r\n",NoNum+'A'-1, ChannelNumF,RoundNumF, ff);//ID ͨ���� ���� ����
			ff = 0; 
		}
		else	
		{
			ff = (_ucCanMsg[1] - '0')*1.0+ (_ucCanMsg[3] - '0') * 0.1 + (_ucCanMsg[4] - '0') * 0.01 + 
							(_ucCanMsg[5] - '0') * 0.001 + (_ucCanMsg[6] - '0') * 0.0001 + (_ucCanMsg[7] - '0') * 0.00001;
			printf("ID = %c  num = %d  round = %d ff = %.5f\r\n", NoNum+'A'-1, ChannelNumF,RoundNumF, ff);//ID ͨ���� ���� ����
			ff = 0; 
		}
	}
#else	//������UDP����
	uint8_t NoNum = 0; 		//��CANID��ȥ0x41�õ�

	//ǰ�벿�ֵ�����
	uint8_t ChannelNumF = 0;	//ͨ����
	//uint8_t RoundNumF = 0;		//�ɼ�����
	uint8_t ValidMarkF = 0;		//������Ч��־λ
	uint16_t BoardAddrF = 0;	//�忨�������е���ʼ��ַ
	uint16_t ChannelAddrF = 0;	//ͨ���������е���ʼ��ַ

	//��벿�ֵ�����
	uint8_t ChannelNumS = 0;	//ͨ����
	//uint8_t RoundNumS = 0;		//�ɼ�����
	uint8_t ValidMarkS = 0;		//������Ч��־λ
	uint16_t BoardAddrS = 0;	//�忨�������е���ʼ��ַ
	uint16_t ChannelAddrS = 0;	//ͨ���������е���ʼ��ַ

	// UNUSED(RoundNumF);
	// UNUSED(RoundNumS);

	NoNum = _ucCanMsg[8] - 0x41; 		//����忨λ�Σ���һ��忨ID��Ϊ0x41

	/* �ȴ���0-3�ֽ� */
	ChannelNumF = _ucCanMsg[0]&0x0f;				//��ȡͨ���� 0-7
	//RoundNumF = _ucCanMsg[0]&0x07;					//��ȡ���� 0-7
	ValidMarkF = (_ucCanMsg[0] >> 7)&1;				//��ȡ��Ч��־λ
	BoardAddrF = NoNum * 24 + 2;					//����忨�������еĵ�ַ��ÿ���忨ռ24�ֽڣ�2Ϊ֡ͷ����
	ChannelAddrF = BoardAddrF + ChannelNumF * 3;	//ͨ����ַ�ڰ忨��ַ�Ļ�����ƫ�ƣ�ÿ��ƫ��3���ֽ�

	/* �ٴ���4-7�ֽ� */
	ChannelNumS = _ucCanMsg[4]&0x0f;				//��ȡͨ���� 0-7
	//RoundNumS = _ucCanMsg[4]&0x07;					//��ȡ���� 0-7
	ValidMarkS = (_ucCanMsg[4] >> 7)&1;				//��ȡ��Ч��־λ
	BoardAddrS = NoNum * 24 + 2;					//����忨�������еĵ�ַ��ÿ���忨ռ24�ֽڣ�2Ϊ֡ͷ����
	ChannelAddrS = BoardAddrS + ChannelNumS * 3;	//ͨ����ַ�ڰ忨��ַ�Ļ�����ƫ�ƣ�ÿ��ƫ��3���ֽ�

	/* ���ݿ��� */
	if(xSemaphoreTake(UDP_SendBuffer_Mutex,portMAX_DELAY) == pdTRUE)	//��ȡ������������
	{
		memcpy(LwIP_UDP_SendBuffer + ChannelAddrF,_ucCanMsg + 1,3);				//ԭ�뿽��
		LwIP_UDP_SendBuffer[290 + NoNum] |= (ValidMarkF << (7 - ChannelNumF)); 	//��Чλ����

		memcpy(LwIP_UDP_SendBuffer + ChannelAddrS,_ucCanMsg + 5,3);				//ԭ�뿽��
		LwIP_UDP_SendBuffer[290 + NoNum] |= (ValidMarkS << (7 - ChannelNumS)); 	//��Чλ����

		xSemaphoreGive(UDP_SendBuffer_Mutex);									//�ͷ��ź���������
	}
#endif
}


/**
 * @brief CAN������
 * @param _tCAN-CAN�ṹ��ָ��
 */
void Task_CAN_Handle(tagCAN_T *_tCAN)
{	
    // �Ӷ����н���CAN����
    if(xQueueReceive(CANRecv_Queue, &HandleBuffer, portMAX_DELAY) == pdPASS)
    {
#ifdef PRINTF_DEBUG
		for(int i = 0; i< 9;i++)
        	printf("%x", HandleBuffer[i]);
#endif
		S_Data_Process(HandleBuffer);
    }
}

