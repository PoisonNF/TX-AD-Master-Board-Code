#include "task_serialscreen.h"

static uint8_t SerialScreen_RecvBuffer[50] = {0};   //串口屏接收缓存区
static uint8_t Channel_Info_SendBuffer[99] = {0};   //通道信息发送缓存区
static uint8_t System_Info_SendBuffer[27] = {0};   //系统信息发送缓存区
static uint8_t Setting_Info_SendBuffer[26] = {0};   //设置信息发送缓存区
static uint8_t Log_Info_SendBuffer[204] = {0};   //日志信息发送缓存区

static uint8_t SettingCplt_SendBuffer[3] = {0x33,0xBB,0x0A};   //设置完成返回给串口的数组
static uint8_t All_Boards_StopBuffer[8] = {0xA3,0x00,0x00,0x00,0x00,0x00,0x00,0x00};   //所有板卡停止指令，通过CAN广播
static uint8_t All_Boards_ResetBuffer[8] = {0xA5,0x00,0x00,0x00,0x00,0x00,0x00,0x00};   //所有板卡复位指令，通过CAN广播

static uint8_t Version[4] = "V1.0";        //版本号

uint8_t SetRemoteip[4],SetPort[2];           //需要设置的远端IP地址和端口号
uint8_t SetIP[4],SetMask[4],SetGW[4];        //需要设置的IP地址信息

#define	GET_BIT(x, bit)	((x & (1 << bit)) >> bit)	/* 获取第bit位 */
#define RECENT_LOGNUM       5                       /* 要读取最近log的条数 */

/**
 * @brief 查询系统信息处理函数
 * @param Null
 */
static void S_System_Info_Handle(void)
{
    System_Info_SendBuffer[0] = 0x22;
    System_Info_SendBuffer[1] = 0xAA;

    //添加插入板卡数量
    System_Info_SendBuffer[2] = NumberOfBoards/10 + '0';
    System_Info_SendBuffer[3] = NumberOfBoards%10 + '0';

    //添加远程IP地址和端口号信息
    System_Info_SendBuffer[4] = RemotePort & 0xFF;
    System_Info_SendBuffer[5] = RemotePort >> 8;
    memcpy(&System_Info_SendBuffer[6],g_lwipdev.remoteip,4);

    //添加本地IP地址信息
    memcpy(&System_Info_SendBuffer[10],g_lwipdev.ip,4);
    memcpy(&System_Info_SendBuffer[14],g_lwipdev.netmask,4);
    memcpy(&System_Info_SendBuffer[18],g_lwipdev.gateway,4);    

    //添加版本号信息
    memcpy(&System_Info_SendBuffer[22],Version,4);

    System_Info_SendBuffer[26] = 0x0A;

    //向串口屏发送数据
    Drv_Uart_Transmit(&Uart5,System_Info_SendBuffer,sizeof(System_Info_SendBuffer));
}

/**
 * @brief 查询设置信息处理函数
 * @param Null
 */
static void S_Setting_Info_Handle(void)
{
    Setting_Info_SendBuffer[0] = 0x33;
    Setting_Info_SendBuffer[1] = 0xAA;

    //添加当前通道数
    Setting_Info_SendBuffer[2] = CurrentChannelNum / 10 + '0';
    Setting_Info_SendBuffer[3] = CurrentChannelNum % 10 + '0';

    //添加当前传输速率
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

    //添加远程IP地址和端口号信息
    Setting_Info_SendBuffer[6] = RemotePort & 0xFF;
    Setting_Info_SendBuffer[7] = RemotePort >> 8;

    memcpy(&Setting_Info_SendBuffer[8],g_lwipdev.remoteip,4);

    //添加当前IP地址信息
    memcpy(&Setting_Info_SendBuffer[12],g_lwipdev.ip,4);
    memcpy(&Setting_Info_SendBuffer[16],g_lwipdev.netmask,4);
    memcpy(&Setting_Info_SendBuffer[20],g_lwipdev.gateway,4);
    
    //添加当前模式，0x01为UDP，0x02为TCP
    if(LWIP_UDP == 1)
        Setting_Info_SendBuffer[24] = 0x01;
    else
        Setting_Info_SendBuffer[24] = 0x02;
    
    Setting_Info_SendBuffer[25] = 0x0A;

    //向串口屏发送数据
    Drv_Uart_Transmit(&Uart5,Setting_Info_SendBuffer,sizeof(Setting_Info_SendBuffer));
}

/**
 * @brief 上传日志信息处理函数
 * @param Null
 */
static void S_Log_Info_Handle(void)
{
    uint32_t ReadNum = 0;
    uint8_t ReceBuffer[LOG_SIZE] = {0};
    uint8_t TipsBuffer[] = "No TF card, please check!";   //用于在没有插入TF卡下的提示

    /* 帧头帧尾赋值 */
    Log_Info_SendBuffer[0] = 0x44;
    Log_Info_SendBuffer[1] = 0xAA;
    Log_Info_SendBuffer[LOG_SIZE + 3] = 0x0A;

    /* TF卡存在检测 */
    if(TFCardIsExist != true)
        goto TFCardError;

    /* TF卡内不足最少log条数处理 */
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
                Drv_Uart_Transmit(&Uart5,Log_Info_SendBuffer,sizeof(Log_Info_SendBuffer));  //向串口屏发送数据
            }
        }
    }

    /* TF卡内大于最少log条数处理 */
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
                Drv_Uart_Transmit(&Uart5,Log_Info_SendBuffer,sizeof(Log_Info_SendBuffer));  //向串口屏发送数据
            }            
        }
    }

    return;

/* 没有TF卡的处理，向串口屏打印提示信息 */
TFCardError:
    printf("No TF card, please check!\r\n");    //串口打印调试
    Log_Info_SendBuffer[2] = LOG_SIZE;
    memcpy(&Log_Info_SendBuffer[3],TipsBuffer,LOG_SIZE);
    Drv_Uart_Transmit(&Uart5,Log_Info_SendBuffer,sizeof(Log_Info_SendBuffer));  //向串口屏发送数据
}

/**
 * @brief 查询通道信息处理函数
 * @param PageNum 页标识符,0x0A-0x0D
 */
static void S_Channel_Info_Handle(uint8_t PageNum)
{
    uint16_t ValuePos = 0;       //用于在LwIP_UDP_SendBuffer[306]中寻找对应的采样值
    uint16_t ValidPos = 0;      //用于在LwIP_UDP_SendBuffer[306]中寻找对应的通道状态
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

    //处理第一块板卡
    for(index = 2; index <= 30; index += 4)
    {
        uint8_t bit = 7;
        Channel_Info_SendBuffer[index] = GET_BIT(LwIP_UDP_SendBuffer[0][ValidPos], bit);
        memcpy(&Channel_Info_SendBuffer[index + 1],&LwIP_UDP_SendBuffer[0][ValuePos],3);
        bit--;
        ValuePos += 3;
    }

    ValidPos++;

    //处理第二块板卡
    for(index = 34; index <= 62; index += 4)
    {
        uint8_t bit = 7;
        Channel_Info_SendBuffer[index] = GET_BIT(LwIP_UDP_SendBuffer[0][ValidPos], bit);
        memcpy(&Channel_Info_SendBuffer[index + 1],&LwIP_UDP_SendBuffer[0][ValuePos],3);
        bit--;
        ValuePos += 3;
    }

    ValidPos++;

    //处理第三块板卡
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

    //向串口屏发送数据
    Drv_Uart_Transmit(&Uart5,Channel_Info_SendBuffer,sizeof(Channel_Info_SendBuffer));
}

/**
 * @brief 设置信息应用处理函数
 * @param SettingData 串口屏的设置数据
 */
static void S_Setting_Apply_Handle(uint8_t *SettingData)
{
    //设置通道数[1,96]，从A板卡开始计算
    CurrentChannelNum = (SettingData[1] - '0')*10 + (SettingData[2] - '0');

    //设置udp发送速率
    if(SettingData[3] == 0x11)   //250hz情况，仅在16通道及以下出现
        CurrentSendRate = 250;
    else if(SettingData[3] == 0x10)
        CurrentSendRate = 200;  //200hz情况
    else if(SettingData[3] == 0x01)
        CurrentSendRate = 160;  //160hz情况
    else if(SettingData[3] == 0x00)
        CurrentSendRate = 100;  //100hz情况

    //需要设置的远端IP地址和端口号
    SetPort[0] = SettingData[6];
    SetPort[1] = SettingData[5];
    memcpy(SetRemoteip,&SettingData[7],4);

    //需要设置的本地IP地址
    memcpy(SetIP,&SettingData[11],4);
    memcpy(SetMask,&SettingData[15],4);
    memcpy(SetGW,&SettingData[19],4);

    //需要设置的模式
    if(SettingData[23] == 0x01)
    {
        //设置成UDP模式
    }
    else if(SettingData[23] == 0x02)
    {
        //设置成TCP模式
    }

    //标记设置事件成立，将信息保存到TF卡内
    xEventGroupSetBits(Log_Event,SET_EVENT);

    //向所有板卡广播停止指令
    Drv_CAN_SendMsg(&CAN,All_Boards_StopBuffer,8);
    vTaskDelay(1000);

    //向所有板卡广播复位指令
    Drv_CAN_SendMsg(&CAN,All_Boards_ResetBuffer,8);
    //等待1s
    vTaskDelay(1000);

    //往EEPROM里保存IP地址信息
    Task_EEPROM_WriteAddrInfo(SetRemoteip, SetPort, SetIP, SetMask, SetGW);

    //往EEPROM里保存通道数量和传输速率信息
    Task_EEPROM_WriteParameter(CurrentChannelNum, CurrentSendRate);

    vTaskDelay(2000);

    //主板复位
    NVIC_SystemReset();
}

/**
 * @brief 时间同步处理函数
 * @param TimeData 串口屏的设置数据
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
        printf("20%02x/%02x/%02x %02x:%02x:%02x 周%x\r\n",
                SetSysTime.ucYear,SetSysTime.ucMonth,SetSysTime.ucDate,
                SetSysTime.ucHour,SetSysTime.ucMinute,SetSysTime.ucSecond,
                SetSysTime.ucWeek);
    }
	printf("\r\n");
#endif
}

/**
 * @brief 告知串口屏复位完毕
 * @param _tUART-串口实例指针
 */
void Task_TellSerialScreen_ResetCplt(void)
{
    //向串口屏发送主板设置完成
    Drv_Uart_Transmit(&Uart5,SettingCplt_SendBuffer,sizeof(SettingCplt_SendBuffer));
}

/**
 * @brief 串口屏处理函数
 * @param _tUART-串口实例指针
 */
void Task_SerialScreen_Handle(tagUART_T *_tUART)
{
    uint8_t SerialScreenRecvNum = 0;

    /* 串口屏数据接收 */
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

    /* 串口屏数据解析 */
    if(SerialScreen_RecvBuffer[0] == 0x0A && SerialScreenRecvNum == 4)  //询问电压和连接状态(每秒一次)     0x0A 0xF0 0xF0 0xF0
    {
        xSemaphoreGive(PowerDetect_Sema);   //释放电压检测信号量
    }
    else if(SerialScreen_RecvBuffer[0] == 0x0B && SerialScreenRecvNum == 4) //查询系统信息(仅进入时发送一次)    0x0B 0xF0 0xF0 0xF0
    {
        S_System_Info_Handle();
    }
    else if(SerialScreen_RecvBuffer[0] == 0x0C && SerialScreenRecvNum == 4) //查询设置数据状态(进入界面时发送一次)   0x0C 0xF0 0xF0 0xF0
    {
        S_Setting_Info_Handle();
    }
    else if(SerialScreen_RecvBuffer[0] == 0x0D && SerialScreenRecvNum == 4) //上传日志   0x0D 0xF0 0xF0 0xF0
    {
        S_Log_Info_Handle();
    }
    else if(SerialScreen_RecvBuffer[0] == 0x0E && SerialScreenRecvNum == 27)    //设置下传信息 (按下应用按钮时发送)    0x0E 信息数据 0xF0 0xF0 0xF0
    {
        S_Setting_Apply_Handle(SerialScreen_RecvBuffer);
    }
    else if(SerialScreen_RecvBuffer[0] == 0x0F && SerialScreenRecvNum == 5)     //设查询通道信息 (进入界面时与进入界面后每五秒发送一次) 0x0F (0x0A-0X0D) 0xF0 0xF0 0xF0
    {
        S_Channel_Info_Handle(SerialScreen_RecvBuffer[1]);
    }
    else if(SerialScreen_RecvBuffer[0] == 0x11 && SerialScreenRecvNum == 18)    //时间同步
    {
        S_TimeSYNC_Handle(SerialScreen_RecvBuffer);
    }

    /* 清空缓存区 */
    memset(SerialScreen_RecvBuffer,0,sizeof(SerialScreen_RecvBuffer));    //清空缓存区
}

