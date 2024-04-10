#include "task_serialscreen.h"

static uint8_t SerialScreenBuffer[50] = {0};
static uint8_t NoLinkSendBuffer[12] = {0x70,0x30,0x2E,0x70,0x69,0x63,0x3D,0x31,0x33,0xFF,0xFF,0xFF};   //δ����״̬�ر�����
static uint8_t LinkSendBuffer[12] = {0x70,0x30,0x2E,0x70,0x69,0x63,0x3D,0x31,0x34,0xFF,0xFF,0xFF};     //����״̬�ر�����

/**
 * @brief ������������
 * @param _tUART-����ʵ��ָ��
 */
void Task_SerialScreen_Handle(tagUART_T *_tUART)
{
    uint8_t SerialScreenRecvNum = 0;

    /* ���������ݽ��� */
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

    /* ���������ݽ��� */
    if(SerialScreenBuffer[SerialScreenRecvNum - 1] == 0xff
     &&SerialScreenBuffer[SerialScreenRecvNum - 2] == 0xff
     &&SerialScreenBuffer[SerialScreenRecvNum - 3] == 0xff)
     {
        switch(SerialScreenBuffer[0])
        {
            case 0x15:      //ѯ�ʵ�ѹ/����״̬     15 00 00 00 ff ff ff
                xSemaphoreGive(PowerDetect_Sema);               //�ͷŵ�ѹ����ź���
                if(g_lwipdev.link_status == LWIP_LINK_ON)       //��������״̬
                    Drv_Uart_Transmit(&Uart5, LinkSendBuffer, sizeof(LinkSendBuffer));
                else if(g_lwipdev.link_status == LWIP_LINK_OFF) //���ڶ���״̬
                    Drv_Uart_Transmit(&Uart5, NoLinkSendBuffer, sizeof(LinkSendBuffer));
                break;
            default:
                break;
        }
     }

    /* ��ջ����� */
    memset(SerialScreenBuffer,0,sizeof(SerialScreenBuffer));    //��ջ�����
}

