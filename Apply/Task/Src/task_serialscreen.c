#include "task_serialscreen.h"

static uint8_t SerialScreen_RecvBuffer[50] = {0};   //���������ջ�����
static uint8_t Channel_Info_SendBuffer[99] = {0};   //ͨ����Ϣ���ͻ�����
static uint8_t System_Info_SendBuffer[27] = {0};   //ϵͳ��Ϣ���ͻ�����
static uint8_t Setting_Info_SendBuffer[26] = {0};   //������Ϣ���ͻ�����
//static uint8_t Log_Info_SendBuffer[100] = {0};   //��־��Ϣ���ͻ�����

static uint8_t SettingCplt_SendBuffer[3] = {0x33,0xBB,0x0A};   //������ɷ��ظ����ڵ�����
static uint8_t All_Boards_ResetBuffer[8] = {0xA5,0x00,0x00,0x00,0x00,0x00,0x00,0x00};   //���а忨��λָ�ͨ��CAN�㲥

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

    //���Զ��IP��ַ�Ͷ˿ں���Ϣ
    System_Info_SendBuffer[4] = RemotePort & 0xFF;
    System_Info_SendBuffer[5] = RemotePort >> 8;
    memcpy(&System_Info_SendBuffer[6],g_lwipdev.remoteip,4);

    //��ӱ���IP��ַ��Ϣ
    memcpy(&System_Info_SendBuffer[10],g_lwipdev.ip,4);
    memcpy(&System_Info_SendBuffer[14],g_lwipdev.netmask,4);
    memcpy(&System_Info_SendBuffer[18],g_lwipdev.gateway,4);    

    //��Ӱ汾����Ϣ
    memcpy(&System_Info_SendBuffer[22],Version,4);

    System_Info_SendBuffer[26] = 0x0A;

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

    //��ӵ�ǰ�������ʣ���8λ��ǰ
    Setting_Info_SendBuffer[4] = 250;         //��дһ��250hz�����ڸ�
    Setting_Info_SendBuffer[5] = 0;        //��дһ��250hz�����ڸ�

    //���Զ��IP��ַ�Ͷ˿ں���Ϣ
    Setting_Info_SendBuffer[6] = RemotePort & 0xFF;
    Setting_Info_SendBuffer[7] = RemotePort >> 8;

    memcpy(&Setting_Info_SendBuffer[8],g_lwipdev.remoteip,4);

    //��ӵ�ǰIP��ַ��Ϣ
    memcpy(&Setting_Info_SendBuffer[12],g_lwipdev.ip,4);
    memcpy(&Setting_Info_SendBuffer[16],g_lwipdev.netmask,4);
    memcpy(&Setting_Info_SendBuffer[20],g_lwipdev.gateway,4);
    
    //��ӵ�ǰģʽ��0x01ΪUDP��0x02ΪTCP
    if(LWIP_UDP == 1)
        Setting_Info_SendBuffer[24] = 0x01;
    else
        Setting_Info_SendBuffer[24] = 0x02;
    
    Setting_Info_SendBuffer[25] = 0x0A;

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
 * @param SettingData ����������������
 */
static void S_Setting_Apply_Handle(uint8_t *SettingData)
{
    uint8_t SetChannelNum = 0;          //��Ҫ���õ�ͨ����
    uint8_t SetSamplingFreq = 0;        //��Ҫ���õĲ���Ƶ��
    uint8_t remoteip[4],port[2];        //��Ҫ���õ�Զ��IP��ַ�Ͷ˿ں�
    uint8_t ip[4],mask[4],gw[4];        //�洢����IP��ַ��Ϣ

    SetChannelNum = (SettingData[1] - '0')*10 + (SettingData[2] - '0');
    UNUSED(SetChannelNum);
    //������Ҫ���õ�ͨ�������д�������

    SetSamplingFreq = SettingData[3] << 8 + SettingData[4];
    UNUSED(SetSamplingFreq);
    //������Ҫ���õĲ���Ƶ�ʽ��д�������

    //��Ҫ���õ�Զ��IP��ַ�Ͷ˿ں�
    memcpy(port,&SettingData[5],2);
    memcpy(remoteip,&SettingData[7],4);

    //��Ҫ���õı���IP��ַ
    memcpy(ip,&SettingData[11],4);
    memcpy(mask,&SettingData[15],4);
    memcpy(gw,&SettingData[19],4);

    //��Ҫ���õ�ģʽ
    if(SettingData[23] == 0x01)
    {
        //���ó�UDPģʽ
    }
    else if(SettingData[23] == 0x02)
    {
        //���ó�TCPģʽ
    }

    //�����а忨�㲥��λָ��
    Drv_CAN_SendMsg(&CAN,All_Boards_ResetBuffer,8);
    //�ȴ�1s
    vTaskDelay(1000);

    //������EEPROM��
    Task_EEPROM_WriteAddrInfo(remoteip, port, ip, mask, gw);

    //�򴮿������������������
    Drv_Uart_Transmit(&Uart5,SettingCplt_SendBuffer,sizeof(SettingCplt_SendBuffer));

    vTaskDelay(3000);

    //���帴λ
    NVIC_SystemReset();
}

/**
 * @brief ʱ��ͬ��������
 * @param TimeData ����������������
 */
static void S_TimeSYNC_Handle(uint8_t *TimeData)
{
    tagDS3231Time_T SetSysTime = {0};

    SetSysTime.ucYear = Algo_DecToHex((TimeData[2] - '0')*10 + TimeData[3] - '0');
    SetSysTime.ucMonth = Algo_DecToHex((TimeData[4] - '0')*10 + TimeData[5] - '0');
    SetSysTime.ucDate = Algo_DecToHex((TimeData[6] - '0')*10 + TimeData[7] - '0');
    SetSysTime.ucHour = Algo_DecToHex((TimeData[8] - '0')*10 + TimeData[9] - '0');
    SetSysTime.ucMinute = Algo_DecToHex((TimeData[10] - '0')*10 + TimeData[11] - '0');
    SetSysTime.ucSecond = Algo_DecToHex((TimeData[12] - '0')*10 + TimeData[13] - '0');
    SetSysTime.ucWeek = Algo_DecToHex(TimeData[14] - '0');

    OCD_DS3231_TimeSetHex(&DS1337,&SetSysTime);

#ifdef PRINTF_DEBUG
    vTaskDelay(2000);
    if(OCD_DS3231_TimeGetHex(&DS1337,&SetSysTime))
    {
        printf("Read Time:");
        printf("20%02x/%02x/%02x %02x:%02x:%02x ��%x\r\n",
                SetSysTime.ucYear,SetSysTime.ucMonth,SetSysTime.ucDate,
                SetSysTime.ucHour,SetSysTime.ucMinute,SetSysTime.ucSecond,
                SetSysTime.ucWeek);
    }
	printf("\r\n");
#endif
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

//#ifdef PRINTF_DEBUG
    if(SerialScreenRecvNum != 0)
    {
        for(uint16_t i = 0; i < SerialScreenRecvNum;i++)
        {
            printf("%x ",SerialScreen_RecvBuffer[i]);
            if(i == SerialScreenRecvNum) printf("\r\n");
        }
    }
//#endif

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
            case 0x0E:      //�����´���Ϣ (����Ӧ�ð�ťʱ����)    0x0E ��Ϣ���� 0xFF 0xFF 0xFF
                S_Setting_Apply_Handle(SerialScreen_RecvBuffer);
                break;
            case 0x0F:      //���ѯͨ����Ϣ (�������ʱ���������ÿ���뷢��һ��) 0x0F (0x0A-0X0D) 0xFF 0xFF 0xFF
                S_Channel_Info_Handle(SerialScreen_RecvBuffer[1]);
                break;
            case 0x11:      //ʱ��ͬ��
                S_TimeSYNC_Handle(SerialScreen_RecvBuffer);
                break;
            default:
                break;
        }
    }

    /* ��ջ����� */
    memset(SerialScreen_RecvBuffer,0,sizeof(SerialScreen_RecvBuffer));    //��ջ�����
}

