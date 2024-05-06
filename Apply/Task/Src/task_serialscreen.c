#include "task_serialscreen.h"

static uint8_t SerialScreen_RecvBuffer[50] = {0};   //���������ջ�����
static uint8_t Channel_Info_SendBuffer[99] = {0};   //ͨ����Ϣ���ͻ�����
static uint8_t System_Info_SendBuffer[24] = {0};   //ϵͳ��Ϣ���ͻ�����
static uint8_t Setting_Info_SendBuffer[22] = {0};   //������Ϣ���ͻ�����
//static uint8_t Log_Info_SendBuffer[100] = {0};   //��־��Ϣ���ͻ�����

static uint8_t Version[4] = "V1.0";        //�汾��

#define	GET_BIT(x, bit)	((x & (1 << bit)) >> bit)	/* ��ȡ��bitλ */

extern uint8_t LwIP_UDP_SendBuffer[306];
extern uint8_t NumberOfBoards;

/**
 * @brief ��ѯϵͳ��Ϣ������
 * @param Null
 */
static void S_System_Info_Handle(void)
{
    System_Info_SendBuffer[0] = 0x22;
    System_Info_SendBuffer[1] = 0xAA;

    //��Ӳ���忨����
    System_Info_SendBuffer[2] = NumberOfBoards/10 + '0';
    System_Info_SendBuffer[3] = NumberOfBoards%10 + '0';

    //���IP��ַ��Ϣ
    memcpy(&System_Info_SendBuffer[4],RemoteIPAddrString,15);

    //��Ӱ汾����Ϣ
    memcpy(&System_Info_SendBuffer[19],Version,4);

    System_Info_SendBuffer[23] = 0x0A;

    //�򴮿�����������
    Drv_Uart_Transmit(&Uart5,System_Info_SendBuffer,sizeof(System_Info_SendBuffer));
}

/**
 * @brief ��ѯ������Ϣ������
 * @param Null
 */
static void S_Setting_Info_Handle(void)
{
    Setting_Info_SendBuffer[0] = 0x33;
    Setting_Info_SendBuffer[1] = 0xAA;

    //��ӵ�ǰͨ����
    Setting_Info_SendBuffer[2] = NumberOfBoards*8/10 + '0';
    Setting_Info_SendBuffer[3] = NumberOfBoards*8%10 + '0';

    //��ӵ�ǰ��������
    Setting_Info_SendBuffer[4] = 250;        //��дһ��250hz�����ڸ�

    //��ӵ�ǰIP��ַ��Ϣ
    sprintf(LocalIPAddrString,"%d.%d.%d.%d",g_lwipdev.ip[0], g_lwipdev.ip[1], g_lwipdev.ip[2], g_lwipdev.ip[3]);
    memcpy(&Setting_Info_SendBuffer[5],LocalIPAddrString,15);
    
    //��ӵ�ǰģʽ��0x01ΪUDP��0x02ΪTCP
    if(LWIP_UDP == 1)
        Setting_Info_SendBuffer[20] = 0x01;
    else
        Setting_Info_SendBuffer[20] = 0x02;
    
    Setting_Info_SendBuffer[21] = 0x0A;

    //�򴮿�����������
    Drv_Uart_Transmit(&Uart5,Setting_Info_SendBuffer,sizeof(Setting_Info_SendBuffer));    
}

/**
 * @brief �ϴ���־��Ϣ������
 * @param Null
 */
static void S_Log_Info_Handle(void)
{

}

/**
 * @brief ��ѯͨ����Ϣ������
 * @param PageNum ҳ��ʶ��,0x0A-0x0D
 */
static void S_Channel_Info_Handle(uint8_t PageNum)
{
    uint16_t ValuePos = 0;       //������LwIP_UDP_SendBuffer[306]��Ѱ�Ҷ�Ӧ�Ĳ���ֵ
    uint16_t ValidPos = 0;      //������LwIP_UDP_SendBuffer[306]��Ѱ�Ҷ�Ӧ��ͨ��״̬
    uint8_t index = 0;

    if(PageNum == 0x0A)
    {
        ValuePos = 2;
        ValidPos = 290;
    }
    else if(PageNum == 0x0B)
    {
        ValuePos = 74;
        ValidPos = 293;
    }
    else if(PageNum == 0x0C)
    {
        ValuePos = 146;
        ValidPos = 296;
    }
    else if(PageNum == 0x0D)
    {
        ValuePos = 218;
        ValidPos = 299;
    }

    memset(Channel_Info_SendBuffer,0,sizeof(Channel_Info_SendBuffer));

    //�����һ��忨
    for(index = 2; index <= 30; index += 4)
    {
        uint8_t bit = 7;
        Channel_Info_SendBuffer[index] = GET_BIT(LwIP_UDP_SendBuffer[ValidPos], bit);
        memcpy(&Channel_Info_SendBuffer[index + 1],&LwIP_UDP_SendBuffer[ValuePos],3);
        bit--;
        ValuePos += 3;
    }

    ValidPos++;

    //����ڶ���忨
    for(index = 34; index <= 62; index += 4)
    {
        uint8_t bit = 7;
        Channel_Info_SendBuffer[index] = GET_BIT(LwIP_UDP_SendBuffer[ValidPos], bit);
        memcpy(&Channel_Info_SendBuffer[index + 1],&LwIP_UDP_SendBuffer[ValuePos],3);
        bit--;
        ValuePos += 3;
    }

    ValidPos++;

    //���������忨
    for(index = 66; index <= 94; index += 4)
    {
        uint8_t bit = 7;
        Channel_Info_SendBuffer[index] = GET_BIT(LwIP_UDP_SendBuffer[ValidPos], bit);
        memcpy(&Channel_Info_SendBuffer[index + 1],&LwIP_UDP_SendBuffer[ValuePos],3);
        bit--;
        ValuePos += 3;
    }

    Channel_Info_SendBuffer[0] = 0x55;
    Channel_Info_SendBuffer[1] = 0xAA;
    Channel_Info_SendBuffer[98] = 0x0A;

    //�򴮿�����������
    Drv_Uart_Transmit(&Uart5,Channel_Info_SendBuffer,sizeof(Channel_Info_SendBuffer));
}

/**
 * @brief ������ϢӦ�ô�����
 * @param Null
 */
static void S_Setting_Apply_Handle(uint8_t *SettingData)
{
    uint8_t SetChannelNum = 0;          //��Ҫ���õ�ͨ����
    uint8_t SetSamplingFreq = 0;        //��Ҫ���õĲ���Ƶ��

    SetChannelNum = (SettingData[1] - '0')*10 + (SettingData[2] - '0');
    UNUSED(SetChannelNum);
    //������Ҫ���õ�ͨ�������д�������

    SetSamplingFreq = SettingData[3];
    UNUSED(SetSamplingFreq);
    //������Ҫ���õĲ���Ƶ�ʽ��д�������

    //��Ҫ���õ�Զ��IP��ַ
    memcpy(RemoteIPAddrString,&SettingData[4],15);

    //��Ҫ���õ�ģʽ
    if(SettingData[19] == 0x01)
    {
        //���ó�UDPģʽ
    }
    else if(SettingData[19] == 0x02)
    {
        //���ó�TCPģʽ
    }

    //��������lwip
}

/**
 * @brief ������������
 * @param _tUART-����ʵ��ָ��
 */
void Task_SerialScreen_Handle(tagUART_T *_tUART)
{
    uint8_t SerialScreenRecvNum = 0;

    /* ���������ݽ��� */
    SerialScreenRecvNum = Drv_Uart_Receive_DMA(_tUART,SerialScreen_RecvBuffer);
    UNUSED(SerialScreenRecvNum);

#ifdef PRINTF_DEBUG
    if(SerialScreenRecvNum != 0)
    {
        for(uint16_t i = 0; i < SerialScreenRecvNum;i++)
        {
            printf("%x ",SerialScreen_RecvBuffer[i]);
            if(i == SerialScreenRecvNum) printf("\r\n");
        }
        SerialScreenRecvNum = 0;
    }
#endif

    /* ���������ݽ��� */
    if(SerialScreen_RecvBuffer[SerialScreenRecvNum - 1] == 0xFF
     &&SerialScreen_RecvBuffer[SerialScreenRecvNum - 2] == 0xFF
     &&SerialScreen_RecvBuffer[SerialScreenRecvNum - 3] == 0xFF)
    {
        switch(SerialScreen_RecvBuffer[0])
        {
            case 0x0A:      //ѯ�ʵ�ѹ������״̬(ÿ��һ��)     0x0A 0xFF 0xFF 0xFF
                xSemaphoreGive(PowerDetect_Sema);   //�ͷŵ�ѹ����ź���
                break;        
            case 0x0B:      //��ѯϵͳ��Ϣ(������ʱ����һ��)    0x0B 0xFF 0xFF 0xFF
                S_System_Info_Handle();
                break;
            case 0x0C:      //��ѯ��������״̬(�������ʱ����һ��)   0x0C 0xFF 0xFF 0xFF
                S_Setting_Info_Handle();
                break;
            case 0x0D:      //�ϴ���־   0x0D 0xFF 0xFF 0xFF
                S_Log_Info_Handle();
                break;
            case 0x0E:      //�����´���Ϣ (����Ӧ�ð�ťʱ����)    0x0E 0xFF 0xFF 0xFF
                S_Setting_Apply_Handle(SerialScreen_RecvBuffer);
                break;
            case 0x0F:      //���ѯͨ����Ϣ (�������ʱ���������ÿ���뷢��һ��) 0x0F 0xFF 0xFF 0xFF
                S_Channel_Info_Handle(SerialScreen_RecvBuffer[1]);
                break;
            default:
                break;
        }
    }

    /* ��ջ����� */
    memset(SerialScreen_RecvBuffer,0,sizeof(SerialScreen_RecvBuffer));    //��ջ�����
}

