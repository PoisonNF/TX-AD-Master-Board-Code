#include "task_conf.h"
#include "usercode.h"
#include "config.h"


/**
 * @brief �ⲿ�ж�5-9������
 * @retval Null
*/
void EXTI9_5_IRQHandler(void)
{
    //Drv_GPIO_EXTI_IRQHandler(&DS3231_ALARM_IRQ);	/* ����ӣ��������޸� */
}

/**
 * @brief �ⲿ�ж�10-15������
 * @retval Null
*/
void EXTI15_10_IRQHandler(void)
{
    //Drv_GPIO_EXTI_IRQHandler(&tPCUart.tGPIO[1]);	/* ����ӣ��������޸� */
}

/**
 * @brief ����1�жϷ�����
 * @retval Null
*/
void USART1_IRQHandler(void)
{
	/* ʾ�� */
    //Drv_Uart_IRQHandler(&tPCUart);		/* ���貿�� */
    //Drv_Uart_IRQHandler(&demoUart);
//    Drv_Uart_DMA_RxHandler(&demoUart);
}

/*****************************���º�����DMA����ʱ����д***********************************/
/* �ҵ����ڶ�Ӧ��DMA�ź��������� */
//void DMA2_Stream7_IRQHandler(void)
//{
//    Drv_Uart_DMA_IRQHandler(&demoUart);
//}
///***************************************************************************************/

///*****************************���º�����DMA����ʱѡ����д***********************************/

//void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
//{
//    Drv_Uart_DMA_TxHandler(&demoUart);
//}

/***************************************************************************************/

/**
 * @brief ����2�жϷ�����
 * @retval Null
*/
void USART2_IRQHandler(void)
{
	/* ʾ�� */
    //Drv_Uart_IRQHandler(&tJY901B.tUART);		/* ���貿�� */
}

/**
 * @brief ����3�жϷ�����
 * @retval Null
*/
void USART3_IRQHandler(void)
{

}

/**
 * @brief ����4�жϷ�����
 * @retval Null
*/
void UART4_IRQHandler(void)
{

}

/**
 * @brief ����5�жϷ�����
 * @retval Null
*/
void UART5_IRQHandler(void)
{
    uint32_t Save_Status;
    Save_Status = taskENTER_CRITICAL_FROM_ISR();      //�жϼ������ٽ��
	Drv_Uart_IRQHandler(&Uart5);
	Drv_Uart_DMA_RxHandler(&Uart5);
    taskEXIT_CRITICAL_FROM_ISR(Save_Status);       //�жϼ��˳��ٽ��
}

/**
 * @brief ��ʱ��2�жϷ�����
 * @retval Null
*/
void TIM2_IRQHandler(void)
{
	/* ʾ�� */
    //Drv_Timer_IRQHandler(&demoTIM);
}

/**
 * @brief ��ʱ��3�жϷ�����
 * @retval Null
*/
void TIM3_IRQHandler(void)
{
	/* ʾ�� */
    //Drv_Timer_IRQHandler(&demoTIM);
}

/**
 * @brief ��ʱ��4�жϷ�����
 * @retval Null
*/
void TIM4_IRQHandler(void)
{
    /* ʾ�� */
    //Drv_Timer_IRQHandler(&demoTIM);
}

/**
 * @brief ��ʱ��5�жϷ�����
 * @retval Null
*/
void TIM5_IRQHandler(void)
{
    /* ʾ�� */
    //Drv_Timer_IRQHandler(&demoTIM);
}

/**
 * @brief ��ʱ��6�жϷ�����
 * @retval Null
*/
void TIM6_DAC_IRQHandler(void)
{

}

/**
 * @brief ��ʱ��7�жϷ�����
 * @retval Null
*/
void TIM7_IRQHandler(void)
{
	/* ʾ�� */
    //Drv_Timer_IRQHandler(&demoTIM);
}


void CAN1_RX0_IRQHandler(void)
{
    Drv_CAN_IRQHandler(&CAN);
}

void CAN1_RX1_IRQHandler(void)
{
    Drv_CAN_IRQHandler(&CAN);
}

void CAN1_TX_IRQHandler(void)
{
    Drv_CAN_IRQHandler(&CAN);
}

static uint8_t CANReceFifo0Buffer[9] = {0};      //���ڴ��CAN���յ����� 
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan1)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    /* CAN�жϽ��� */
    if(Drv_CAN_ReceMsg(&CAN,CANReceFifo0Buffer,CAN_RX_FIFO0))
    {
#ifdef PRINTF_DEBUG
        printf("Recv 0:");
        for(int i = 0; i< 8;i++)
    	    printf("%x ", CANReceFifo0Buffer[i]);
        printf("\r\n");
#endif
       
        if(xTaskGetSchedulerState() == taskSCHEDULER_RUNNING)
        {
            CANReceFifo0Buffer[8] = CAN.tCANRxHeader.StdId;  /* ���һ���ֽڴ洢�忨ID�� */
            
            /* �����յ������ݷ�����Ϣ���� */
            xQueueSendFromISR(CANRecv_Queue, &CANReceFifo0Buffer, &xHigherPriorityTaskWoken);
            /* ����и������ȼ���������Ҫ�������У�����������л� */
            portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
        }
    }  
}

static uint8_t CANReceFifo1Buffer[9] = {0};      //���ڴ��CAN���յ����� 
void HAL_CAN_RxFifo1MsgPendingCallback(CAN_HandleTypeDef *hcan1)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    /* CAN�жϽ��� */
    if(Drv_CAN_ReceMsg(&CAN,CANReceFifo1Buffer,CAN_RX_FIFO1))
    {
#ifdef PRINTF_DEBUG
        printf("Recv 1:");
        for(int i = 0; i< 8;i++)
    	    printf("%x ", CANReceFifo1Buffer[i]);
        printf("\r\n");
#endif

        if(xTaskGetSchedulerState() == taskSCHEDULER_RUNNING)
        {
            CANReceFifo1Buffer[8] = CAN.tCANRxHeader.StdId;  /* ���һ���ֽڴ洢�忨ID�� */
        
            /* �����յ������ݷ�����Ϣ���� */
            xQueueSendFromISR(CANRecv_Queue, &CANReceFifo1Buffer, &xHigherPriorityTaskWoken);
            /* ����и������ȼ���������Ҫ�������У�����������л� */
            portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
        }
    }  
}


