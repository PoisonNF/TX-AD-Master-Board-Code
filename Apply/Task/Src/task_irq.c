#include "task_conf.h"
#include "usercode.h"
#include "config.h"


/**
 * @brief 外部中断5-9服务函数
 * @retval Null
*/
void EXTI9_5_IRQHandler(void)
{
    //Drv_GPIO_EXTI_IRQHandler(&DS3231_ALARM_IRQ);	/* 必须加，参数需修改 */
}

/**
 * @brief 外部中断10-15服务函数
 * @retval Null
*/
void EXTI15_10_IRQHandler(void)
{
    //Drv_GPIO_EXTI_IRQHandler(&tPCUart.tGPIO[1]);	/* 必须加，参数需修改 */
}

/**
 * @brief 串口1中断服务函数
 * @retval Null
*/
void USART1_IRQHandler(void)
{
	/* 示例 */
    //Drv_Uart_IRQHandler(&tPCUart);		/* 必需部分 */
    //Drv_Uart_IRQHandler(&demoUart);
//    Drv_Uart_DMA_RxHandler(&demoUart);
}

/*****************************以下函数在DMA发送时必须写***********************************/
/* 找到串口对应的DMA号和数据流号 */
//void DMA2_Stream7_IRQHandler(void)
//{
//    Drv_Uart_DMA_IRQHandler(&demoUart);
//}
///***************************************************************************************/

///*****************************以下函数在DMA发送时选择性写***********************************/

//void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
//{
//    Drv_Uart_DMA_TxHandler(&demoUart);
//}

/***************************************************************************************/

/**
 * @brief 串口2中断服务函数
 * @retval Null
*/
void USART2_IRQHandler(void)
{
	/* 示例 */
    //Drv_Uart_IRQHandler(&tJY901B.tUART);		/* 必需部分 */
}

/**
 * @brief 串口3中断服务函数
 * @retval Null
*/
void USART3_IRQHandler(void)
{

}

/**
 * @brief 串口4中断服务函数
 * @retval Null
*/
void UART4_IRQHandler(void)
{

}

/**
 * @brief 串口5中断服务函数
 * @retval Null
*/
void UART5_IRQHandler(void)
{
    uint32_t Save_Status;
    Save_Status = taskENTER_CRITICAL_FROM_ISR();      //中断级进入临界段
	Drv_Uart_IRQHandler(&Uart5);
	Drv_Uart_DMA_RxHandler(&Uart5);
    taskEXIT_CRITICAL_FROM_ISR(Save_Status);       //中断级退出临界段
}

/**
 * @brief 定时器2中断服务函数
 * @retval Null
*/
void TIM2_IRQHandler(void)
{
	/* 示例 */
    //Drv_Timer_IRQHandler(&demoTIM);
}

/**
 * @brief 定时器3中断服务函数
 * @retval Null
*/
void TIM3_IRQHandler(void)
{
	/* 示例 */
    //Drv_Timer_IRQHandler(&demoTIM);
}

/**
 * @brief 定时器4中断服务函数
 * @retval Null
*/
void TIM4_IRQHandler(void)
{
    /* 示例 */
    //Drv_Timer_IRQHandler(&demoTIM);
}

/**
 * @brief 定时器5中断服务函数
 * @retval Null
*/
void TIM5_IRQHandler(void)
{
    /* 示例 */
    //Drv_Timer_IRQHandler(&demoTIM);
}

/**
 * @brief 定时器6中断服务函数
 * @retval Null
*/
void TIM6_DAC_IRQHandler(void)
{

}

/**
 * @brief 定时器7中断服务函数
 * @retval Null
*/
void TIM7_IRQHandler(void)
{
	/* 示例 */
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

static uint8_t CANReceFifo0Buffer[9] = {0};      //用于存放CAN接收的数据 
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan1)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    /* CAN中断接收 */
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
            CANReceFifo0Buffer[8] = CAN.tCANRxHeader.StdId;  /* 最后一个字节存储板卡ID号 */
            
            /* 将接收到的数据放入消息队列 */
            xQueueSendFromISR(CANRecv_Queue, &CANReceFifo0Buffer, &xHigherPriorityTaskWoken);
            /* 如果有更高优先级的任务需要立即运行，则进行任务切换 */
            portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
        }
    }  
}

static uint8_t CANReceFifo1Buffer[9] = {0};      //用于存放CAN接收的数据 
void HAL_CAN_RxFifo1MsgPendingCallback(CAN_HandleTypeDef *hcan1)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    /* CAN中断接收 */
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
            CANReceFifo1Buffer[8] = CAN.tCANRxHeader.StdId;  /* 最后一个字节存储板卡ID号 */
        
            /* 将接收到的数据放入消息队列 */
            xQueueSendFromISR(CANRecv_Queue, &CANReceFifo1Buffer, &xHigherPriorityTaskWoken);
            /* 如果有更高优先级的任务需要立即运行，则进行任务切换 */
            portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
        }
    }  
}


