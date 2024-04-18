#include "task_selfcheck.h"

#define ADBOARD_NUM_MAX	12			//���������޸���

static uint8_t SetIDBuffer[8] = {0xA1,0x00,0x00,0x00,0x00,0x00,0x00,0x00};		//����IDBuffer

static uint8_t IDCollection[13] = {0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48,0x49,0x4A,0x4B,0x4C,0x4D}; //�忨ID�ϼ�����'A' - 'L',0x4D��ֹ���һ���ò���ID

static uint8_t SYNCBuffer[8] = {0xA2,0x53,0x00,0x00,0x00,0x00,0x00,0x00};    //ͬ���ɼ��ź�

static uint8_t ID_Choose = 1;       //�忨�������ϵ�˳��ţ�NO1-12

/**
 * @brief �Լ캯����Ϊ�忨����ID
 * @param Null
 * @retval uint8_t ����忨����
 */
uint8_t Task_SelfCheck(void)
{
    uint8_t InsertNum = 0;

    for(ID_Choose = 0; ID_Choose <= ADBOARD_NUM_MAX; ID_Choose++)
    {
		Drv_GPIO_Toggle(&Control[ID_Choose]);    //���Ͷ�Ӧ�忨�������ţ�ѡ�а忨
		vTaskDelay(50);
        SetIDBuffer[1] = IDCollection[ID_Choose];
        Drv_CAN_SendMsg(&CAN,SetIDBuffer,8);        //���Ͱ���ID�ı���
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
			InsertNum++;						//����忨����һ	
		}
		else
		{
			printf("NO.%d No Card\r\n",ID_Choose);
		}

		Drv_GPIO_Toggle(&Control[ID_Choose]);
    }

	printf("Insert %d Card\r\n",InsertNum);

	vSemaphoreDelete(BoardDetect_Sema);		//������Ϻ�ɾ���ź���
    return InsertNum;
}

/**
 * @brief �����а忨����ͬ���ɼ��ź�
 * @param Null
 */
void Task_SYNC_Signal(void)
{
	printf("Start Acquisiting\r\n");
	Drv_GPIO_Set(&SYNC[0]);  
	vTaskDelay(100);

	Drv_CAN_SendMsg(&CAN,SYNCBuffer,8);	//���Ϳ�ʼͬ������	   
}

