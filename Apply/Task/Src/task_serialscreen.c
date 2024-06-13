#include "task_serialscreen.h"

static uint8_t SerialScreen_RecvBuffer[50] = {0};   //���������ջ�����
static uint8_t Channel_Info_SendBuffer[99] = {0};   //ͨ����Ϣ���ͻ�����
static uint8_t System_Info_SendBuffer[27] = {0};   //ϵͳ��Ϣ���ͻ�����
static uint8_t Setting_Info_SendBuffer[26] = {0};   //������Ϣ���ͻ�����
static uint8_t Log_Info_SendBuffer[204] = {0};   //��־��Ϣ���ͻ�����

static uint8_t SettingCplt_SendBuffer[3] = {0x33,0xBB,0x0A};   //������ɷ��ظ����ڵ�����
static uint8_t All_Boards_StopBuffer[8] = {0xA3,0x00,0x00,0x00,0x00,0x00,0x00,0x00};   //���а忨ָֹͣ�ͨ��CAN�㲥
static uint8_t All_Boards_ResetBuffer[8] = {0xA5,0x00,0x00,0x00,0x00,0x00,0x00,0x00};   //���а忨��λָ�ͨ��CAN�㲥

static uint8_t Version[4] = "V1.0";        //�汾��

uint8_t SetRemoteip[4],SetPort[2];           //��Ҫ���õ�Զ��IP��ַ�Ͷ˿ں�
uint8_t SetIP[4],SetMask[4],SetGW[4];        //��Ҫ���õ�IP��ַ��Ϣ

#define	GET_BIT(x, bit)	((x & (1 << bit)) >> bit)	/* ��ȡ��bitλ */
#define RECENT_LOGNUM       5                       /* Ҫ��ȡ���log������ */

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
    Setting_Info_SendBuffer[2] = CurrentChannelNum / 10 + '0';
    Setting_Info_SendBuffer[3] = CurrentChannelNum % 10 + '0';

    //��ӵ�ǰ��������
    switch(CurrentSendRate)
    {
        case 250:
            Setting_Info_SendBuffer[4] = 0x11;
            Setting_Info_SendBuffer[5] = 0x00;
            break;
        case 200:
            Setting_Info_SendBuffer[4] = 0x10;
            Setting_Info_SendBuffer[5] = 0x00;
            break;
        case 160:
            Setting_Info_SendBuffer[4] = 0x01;
            Setting_Info_SendBuffer[5] = 0x00;
            break;
        case 100:
            Setting_Info_SendBuffer[4] = 0x00;
            Setting_Info_SendBuffer[5] = 0x00;
            break;
        default:
            break;
    }

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
    uint32_t ReadNum = 0;
    uint8_t ReceBuffer[LOG_SIZE] = {0};
    uint8_t TipsBuffer[] = "No TF card, please check!";   //������û�в���TF���µ���ʾ

    /* ֡ͷ֡β��ֵ */
    Log_Info_SendBuffer[0] = 0x44;
    Log_Info_SendBuffer[1] = 0xAA;
    Log_Info_SendBuffer[LOG_SIZE + 3] = 0x0A;

    /* TF�����ڼ�� */
    if(TFCardIsExist != true)
        goto TFCardError;

    /* TF���ڲ�������log�������� */
    if(logNum <= RECENT_LOGNUM)
    {
        for(uint32_t index = 0; index < logNum; index++)
        {
            OCD_FATFS_Read_SpecifyIndex(&TFCard, (char *)File_Name, ReceBuffer , LOG_SIZE, index * LOG_SIZE , &ReadNum);
            if(ReadNum)
            {
                printf("%s\r\n",ReceBuffer);
                Log_Info_SendBuffer[2] = ReadNum;
                memcpy(&Log_Info_SendBuffer[3],ReceBuffer,LOG_SIZE);
#ifdef PRINTF_DEBUG
                for(uint8_t i = 0; i < sizeof(Log_Info_SendBuffer);i++)
                {
                    printf("%x ",Log_Info_SendBuffer[i]);
                    if(i == sizeof(Log_Info_SendBuffer) - 1) printf("\r\n");
                }
#endif
                Drv_Uart_Transmit(&Uart5,Log_Info_SendBuffer,sizeof(Log_Info_SendBuffer));  //�򴮿�����������
            }
        }
    }

    /* TF���ڴ�������log�������� */
    else
    {
        for(uint32_t index = logNum - RECENT_LOGNUM; index < logNum; index++)
        {
            OCD_FATFS_Read_SpecifyIndex(&TFCard, (char *)File_Name, ReceBuffer , LOG_SIZE, index * LOG_SIZE , &ReadNum);
            if(ReadNum)
            {
                printf("%s\r\n",ReceBuffer);
                Log_Info_SendBuffer[2] = ReadNum;
                memcpy(&Log_Info_SendBuffer[3],ReceBuffer,LOG_SIZE);
#ifdef PRINTF_DEBUG
                for(uint8_t i = 0; i < sizeof(Log_Info_SendBuffer);i++)
                {
                    printf("%x ",Log_Info_SendBuffer[i]);
                    if(i == sizeof(Log_Info_SendBuffer) - 1) printf("\r\n");
                }
#endif
                Drv_Uart_Transmit(&Uart5,Log_Info_SendBuffer,sizeof(Log_Info_SendBuffer));  //�򴮿�����������
            }            
        }
    }

    return;

/* û��TF���Ĵ����򴮿�����ӡ��ʾ��Ϣ */
TFCardError:
    printf("No TF card, please check!\r\n");    //���ڴ�ӡ����
    Log_Info_SendBuffer[2] = LOG_SIZE;
    memcpy(&Log_Info_SendBuffer[3],TipsBuffer,LOG_SIZE);
    Drv_Uart_Transmit(&Uart5,Log_Info_SendBuffer,sizeof(Log_Info_SendBuffer));  //�򴮿�����������
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
        Channel_Info_SendBuffer[index] = GET_BIT(LwIP_UDP_SendBuffer[0][ValidPos], bit);
        memcpy(&Channel_Info_SendBuffer[index + 1],&LwIP_UDP_SendBuffer[0][ValuePos],3);
        bit--;
        ValuePos += 3;
    }

    ValidPos++;

    //����ڶ���忨
    for(index = 34; index <= 62; index += 4)
    {
        uint8_t bit = 7;
        Channel_Info_SendBuffer[index] = GET_BIT(LwIP_UDP_SendBuffer[0][ValidPos], bit);
        memcpy(&Channel_Info_SendBuffer[index + 1],&LwIP_UDP_SendBuffer[0][ValuePos],3);
        bit--;
        ValuePos += 3;
    }

    ValidPos++;

    //���������忨
    for(index = 66; index <= 94; index += 4)
    {
        uint8_t bit = 7;
        Channel_Info_SendBuffer[index] = GET_BIT(LwIP_UDP_SendBuffer[0][ValidPos], bit);
        memcpy(&Channel_Info_SendBuffer[index + 1],&LwIP_UDP_SendBuffer[0][ValuePos],3);
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
    //����ͨ����[1,96]����A�忨��ʼ����
    CurrentChannelNum = (SettingData[1] - '0')*10 + (SettingData[2] - '0');

    //����udp��������
    if(SettingData[3] == 0x11)   //250hz���������16ͨ�������³���
        CurrentSendRate = 250;
    else if(SettingData[3] == 0x10)
        CurrentSendRate = 200;  //200hz���
    else if(SettingData[3] == 0x01)
        CurrentSendRate = 160;  //160hz���
    else if(SettingData[3] == 0x00)
        CurrentSendRate = 100;  //100hz���

    //��Ҫ���õ�Զ��IP��ַ�Ͷ˿ں�
    SetPort[0] = SettingData[6];
    SetPort[1] = SettingData[5];
    memcpy(SetRemoteip,&SettingData[7],4);

    //��Ҫ���õı���IP��ַ
    memcpy(SetIP,&SettingData[11],4);
    memcpy(SetMask,&SettingData[15],4);
    memcpy(SetGW,&SettingData[19],4);

    //��Ҫ���õ�ģʽ
    if(SettingData[23] == 0x01)
    {
        //���ó�UDPģʽ
    }
    else if(SettingData[23] == 0x02)
    {
        //���ó�TCPģʽ
    }

    //��������¼�����������Ϣ���浽TF����
    xEventGroupSetBits(Log_Event,SET_EVENT);

    //�����а忨�㲥ָֹͣ��
    Drv_CAN_SendMsg(&CAN,All_Boards_StopBuffer,8);
    vTaskDelay(1000);

    //�����а忨�㲥��λָ��
    Drv_CAN_SendMsg(&CAN,All_Boards_ResetBuffer,8);
    //�ȴ�1s
    vTaskDelay(1000);

    //��EEPROM�ﱣ��IP��ַ��Ϣ
    Task_EEPROM_WriteAddrInfo(SetRemoteip, SetPort, SetIP, SetMask, SetGW);

    //��EEPROM�ﱣ��ͨ�������ʹ���������Ϣ
    Task_EEPROM_WriteParameter(CurrentChannelNum, CurrentSendRate);

    vTaskDelay(2000);

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
 * @brief ��֪��������λ���
 * @param _tUART-����ʵ��ָ��
 */
void Task_TellSerialScreen_ResetCplt(void)
{
    //�򴮿������������������
    Drv_Uart_Transmit(&Uart5,SettingCplt_SendBuffer,sizeof(SettingCplt_SendBuffer));
}

/**
 * @brief ������������
 * @param _tUART-����ʵ��ָ��
 */
void Task_SerialScreen_Handle(tagUART_T *_tUART)
{
    uint8_t SerialScreenRecvNum = 0;

    /* ���������ݽ��� */
    SerialScreenRecvNum = Drv_Uart_Receive_IT(_tUART,SerialScreen_RecvBuffer);
    UNUSED(SerialScreenRecvNum);

#ifdef PRINTF_DEBUG
    if(SerialScreenRecvNum != 0)
    {
        for(uint8_t i = 0; i < SerialScreenRecvNum;i++)
        {
            printf("%x ",SerialScreen_RecvBuffer[i]);
            if(i == SerialScreenRecvNum - 1) printf("\r\n");
        }
    }
#endif

    /* ���������ݽ��� */
    if(SerialScreen_RecvBuffer[0] == 0x0A && SerialScreenRecvNum == 4)  //ѯ�ʵ�ѹ������״̬(ÿ��һ��)     0x0A 0xF0 0xF0 0xF0
    {
        xSemaphoreGive(PowerDetect_Sema);   //�ͷŵ�ѹ����ź���
    }
    else if(SerialScreen_RecvBuffer[0] == 0x0B && SerialScreenRecvNum == 4) //��ѯϵͳ��Ϣ(������ʱ����һ��)    0x0B 0xF0 0xF0 0xF0
    {
        S_System_Info_Handle();
    }
    else if(SerialScreen_RecvBuffer[0] == 0x0C && SerialScreenRecvNum == 4) //��ѯ��������״̬(�������ʱ����һ��)   0x0C 0xF0 0xF0 0xF0
    {
        S_Setting_Info_Handle();
    }
    else if(SerialScreen_RecvBuffer[0] == 0x0D && SerialScreenRecvNum == 4) //�ϴ���־   0x0D 0xF0 0xF0 0xF0
    {
        S_Log_Info_Handle();
    }
    else if(SerialScreen_RecvBuffer[0] == 0x0E && SerialScreenRecvNum == 27)    //�����´���Ϣ (����Ӧ�ð�ťʱ����)    0x0E ��Ϣ���� 0xF0 0xF0 0xF0
    {
        S_Setting_Apply_Handle(SerialScreen_RecvBuffer);
    }
    else if(SerialScreen_RecvBuffer[0] == 0x0F && SerialScreenRecvNum == 5)     //���ѯͨ����Ϣ (�������ʱ���������ÿ���뷢��һ��) 0x0F (0x0A-0X0D) 0xF0 0xF0 0xF0
    {
        S_Channel_Info_Handle(SerialScreen_RecvBuffer[1]);
    }
    else if(SerialScreen_RecvBuffer[0] == 0x11 && SerialScreenRecvNum == 18)    //ʱ��ͬ��
    {
        S_TimeSYNC_Handle(SerialScreen_RecvBuffer);
    }

    /* ��ջ����� */
    memset(SerialScreen_RecvBuffer,0,sizeof(SerialScreen_RecvBuffer));    //��ջ�����
}

