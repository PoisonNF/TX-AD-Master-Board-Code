#include "task_serialscreen.h"

static uint8_t SerialScreenBuffer[50] = {0};
static uint8_t NoLinkSendBuffer[12] = {0x70,0x30,0x2E,0x70,0x69,0x63,0x3D,0x31,0x33,0xFF,0xFF,0xFF};   //未连接状态回报数组
static uint8_t LinkSendBuffer[12] = {0x70,0x30,0x2E,0x70,0x69,0x63,0x3D,0x31,0x34,0xFF,0xFF,0xFF};     //连接状态回报数组

/**
 * @brief 串口屏处理函数
 * @param _tUART-串口实例指针
 */
void Task_SerialScreen_Handle(tagUART_T *_tUART)
{
    uint8_t SerialScreenRecvNum = 0;

    /* 串口屏数据接收 */
    SerialScreenRecvNum = Drv_Uart_Receive_DMA(_tUART,SerialScreenBuffer);
    UNUSED(SerialScreenRecvNum);

#ifdef PRINTF_DEBUG
    if(SerialScreenRecvNum != 0)
    {
        for(uint16_t i = 0; i < SerialScreenRecvNum;i++)
        {
            printf("%x ",SerialScreenBuffer[i]);
            if(i == SerialScreenRecvNum) printf("\r\n");
        }
        SerialScreenRecvNum = 0;
    }
#endif

    /* 串口屏数据解析 */
    if(SerialScreenBuffer[SerialScreenRecvNum - 1] == 0xff
     &&SerialScreenBuffer[SerialScreenRecvNum - 2] == 0xff
     &&SerialScreenBuffer[SerialScreenRecvNum - 3] == 0xff)
     {
        switch(SerialScreenBuffer[0])
        {
            case 0x15:      //询问电压/连接状态     15 00 00 00 ff ff ff
                xSemaphoreGive(PowerDetect_Sema);               //释放电压检测信号量
                if(g_lwipdev.link_status == LWIP_LINK_ON)       //处于连接状态
                    Drv_Uart_Transmit(&Uart5, LinkSendBuffer, sizeof(LinkSendBuffer));
                else if(g_lwipdev.link_status == LWIP_LINK_OFF) //处于断连状态
                    Drv_Uart_Transmit(&Uart5, NoLinkSendBuffer, sizeof(LinkSendBuffer));
                break;
            default:
                break;
        }
     }

    /* 清空缓存区 */
    memset(SerialScreenBuffer,0,sizeof(SerialScreenBuffer));    //清空缓存区
}

