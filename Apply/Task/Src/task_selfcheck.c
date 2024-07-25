#include "task_selfcheck.h"

#define ADBOARD_NUM_MAX			4			//���������޸���

static uint8_t SetIDBuffer[8] = {0xA1,0x00,0x00,0x00,0x00,0x00,0x00,0x00};		//����IDBuffer

static uint8_t SYNCBuffer[8] = {0xA2,0x00,0x00,0x00,0x00,0x00,0x00,0x00};    	//ͬ���ɼ��ź�

uint8_t IDjugBuffer[12] = {0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48,0x49,0x4A,0x4B,0x4C}; //�忨ID�ж�����

static uint8_t All_Boards_SpeedChangeBuffer[8] = {0xA4,0x00,0x00,0x00,0x00,0x00,0x00,0x00};   //���а忨�������ʣ�ͨ��CAN�㲥

static uint8_t All_Boards_ResetBuffer[8] = {0xA5,0x00,0x00,0x00,0x00,0x00,0x00,0x00};   //���а忨��λָ�ͨ��CAN�㲥

static uint8_t ID_Choose = 1;       //�忨�������ϵ�˳��ţ�NO1-12

static uint16_t BoardSpeedSet;		//��֪�ɼ��忨�趨��������

__IO uint16_t TimeOut = 0;				//�Լ�ʱ�䳬ʱ��־λ
/**
 * @brief �Լ캯����Ϊ�忨����ID
 * @param Null
 * @retval uint8_t ����忨����
 */
uint8_t Task_SelfCheck(void)
{
    uint8_t InsertNum = 0;
    
    //�����а忨�㲥��λָ��
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
			InsertNum++;						//����忨����һ
		}

		TimeOut = 0;
		Drv_GPIO_Set(&Control[ID_Choose - 1]);
	}
	printf("Insert %d Card\r\n",InsertNum);
	CurrentChannelNum = InsertNum * 8;		//��¼��ǰͨ����

	Drv_Delay_Ms(1000);		//��ֹ���һ��忨�������Ĳ������ʣ�һ��Ҫ��

    /* ��EEPROM�ж�ȡ����ͨ�������ʹ���������Ϣ */
    Task_EEPROM_ReadParameter(&CurrentChannelNum, &CurrentSendRate);

	/* ����Ҫ���ͨ���������Ʋɼ��忨�ķ������� */
	if(CurrentChannelNum/8 <= 2){
		BoardSpeedSet = 400;					//�忨�����ٶ�
	}
	else if(CurrentChannelNum/8 <= 4){
		BoardSpeedSet = 400;					//�忨�����ٶ�
	}
	else if(CurrentChannelNum/8 <= 6){
		BoardSpeedSet = 400;					//�忨�����ٶ�
	}
	else if(CurrentChannelNum/8 <= 12){
		BoardSpeedSet = 200;					//�忨�����ٶ�
	}

	All_Boards_SpeedChangeBuffer[1] = BoardSpeedSet >> 24;
	All_Boards_SpeedChangeBuffer[2] = BoardSpeedSet >> 16;
	All_Boards_SpeedChangeBuffer[3] = BoardSpeedSet >> 8;
	All_Boards_SpeedChangeBuffer[4] = BoardSpeedSet;
    Drv_CAN_SendMsg(&CAN,All_Boards_SpeedChangeBuffer,8);

	return InsertNum;
}

/**
 * @brief ��忨����ͬ���ɼ��ź�
 * @param Null
 */
void Task_SYNC_Signal(void)
{
	printf("Start Acquisiting\r\n");
	Drv_GPIO_Set(&SYNC[0]);  			//���а忨�ϵ�AD4111ͬ�����
	vTaskDelay(1000);

	/* ������Ҫ��ͨ��������SYNCBuffer�еĶ�Ӧλ��1��
		���ｫ��־λ������SYNCBuffer[1]��4λ��SYNCBuffer[2]��ֻ����ǰX�ţ�X��Χ[0,12] */
	for(uint8_t index = 0; index < CurrentChannelNum/8; index++)
	{
		if(index <= 7)
			SYNCBuffer[2] |= (1 << index);
		else
			SYNCBuffer[1] |= (1 << (index - 8));
	}

	Drv_CAN_SendMsg(&CAN,SYNCBuffer,8);	//���Ϳ�ʼͬ������	   
}

