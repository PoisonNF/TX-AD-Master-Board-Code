#include "task_serialscreen.h"

static uint8_t SerialScreen_RecvBuffer[50] = {0};   //串口屏接收缓存区
static uint8_t Channel_Info_SendBuffer[99] = {0};   //通道信息发送缓存区
static uint8_t System_Info_SendBuffer[27] = {0};   //系统信息发送缓存区
static uint8_t Setting_Info_SendBuffer[26] = {0};   //设置信息发送缓存区
static uint8_t Log_Info_SendBuffer[104] = {0};   //日志信息发送缓存区

static uint8_t SettingCplt_SendBuffer[3] = {0x33,0xBB,0x0A};   //设置完成返回给串口的数组
static uint8_t All_Boards_StopBuffer[8] = {0xA3,0x00,0x00,0x00,0x00,0x00,0x00,0x00};   //所有板卡停止指令，通过CAN广播
static uint8_t All_Boards_ResetBuffer[8] = {0xA5,0x00,0x00,0x00,0x00,0x00,0x00,0x00};   //所有板卡复位指令，通过CAN广播

static uint8_t Version[4] = "V1.0";        //版本号

static uint8_t IsSettingData = 0;       //标记处理设置数据
static uint8_t SettingDataNum = 0;      //串口屏设置数据已接收长度计数
static uint8_t SettingData_Buffer[27] = {0};    //存储串口屏设置数据

#define	GET_BIT(x, bit)	((x & (1 << bit)) >> bit)	/* 获取第bit位 */

extern uint8_t NumberOfBoards;

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
    Setting_Info_SendBuffer[2] = NumberOfBoards*8/10 + '0';
    Setting_Info_SendBuffer[3] = NumberOfBoards*8%10 + '0';

    //添加当前采样速率，后8位在前
    Setting_Info_SendBuffer[4] = 250;         //先写一个250hz后面在改
    Setting_Info_SendBuffer[5] = 0;        //先写一个250hz后面在改

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
    uint16_t OffsetNum = 0;
    uint8_t ReceBuffer[100] = {0};
    char File_Name[] = "/log/1.txt";

    /* 帧头帧尾赋值 */
    Log_Info_SendBuffer[0] = 0x44;
    Log_Info_SendBuffer[1] = 0xAA;
    Log_Info_SendBuffer[103] = 0x0A;

    /* 寻找内容的末尾，确定偏移量 */
    do
    {
        OCD_FATFS_Read_SpecifyIndex(&TFCard, File_Name, ReceBuffer , LOG_SIZE , OffsetNum * LOG_SIZE , &ReadNum);
        OffsetNum++;
    }while(ReadNum == LOG_SIZE);

    OffsetNum--;    //OffsetNum会比实际情况后面一个偏移数，所以要减一消除

    if(OffsetNum <= 5)
    {
        for(uint8_t index = 0; index < OffsetNum; index++)
        {
            OCD_FATFS_Read_SpecifyIndex(&TFCard, File_Name, ReceBuffer , LOG_SIZE,index * LOG_SIZE , &ReadNum);
            if(ReadNum)
            {
                printf("%s\r\n",ReceBuffer);
                Log_Info_SendBuffer[2] = ReadNum;
                memcpy(&Log_Info_SendBuffer[3],ReceBuffer,LOG_SIZE);
                //向串口屏发送数据
                Drv_Uart_Transmit(&Uart5,Log_Info_SendBuffer,sizeof(Log_Info_SendBuffer));                
            }
        }        
    }
    else
    {
        for(uint8_t index = OffsetNum-5; index < OffsetNum; index++)
        {
            OCD_FATFS_Read_SpecifyIndex(&TFCard, File_Name, ReceBuffer , LOG_SIZE, index * LOG_SIZE , &ReadNum);
            if(ReadNum)
            {
                printf("%s\r\n",ReceBuffer);
                Log_Info_SendBuffer[2] = ReadNum;
                memcpy(&Log_Info_SendBuffer[3],ReceBuffer,LOG_SIZE);
                //向串口屏发送数据
                Drv_Uart_Transmit(&Uart5,Log_Info_SendBuffer,sizeof(Log_Info_SendBuffer));      
            }
        }
    }
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
    uint8_t SetChannelNum = 0;          //需要设置的通道数
    uint8_t SetSamplingFreq = 0;        //需要设置的采样频率
    uint8_t remoteip[4],port[2];        //需要设置的远端IP地址和端口号
    uint8_t ip[4],mask[4],gw[4];        //存储本地IP地址信息

    SetChannelNum = (SettingData[1] - '0')*10 + (SettingData[2] - '0');
    UNUSED(SetChannelNum);
    //根据需要设置的通道数进行处理，待定

    SetSamplingFreq = (SettingData[3] << 8) + SettingData[4];
    UNUSED(SetSamplingFreq);
    //根据需要设置的采样频率进行处理，待定

    //需要设置的远端IP地址和端口号
    port[0] = SettingData[6];
    port[1] = SettingData[5];
    memcpy(remoteip,&SettingData[7],4);

    //需要设置的本地IP地址
    memcpy(ip,&SettingData[11],4);
    memcpy(mask,&SettingData[15],4);
    memcpy(gw,&SettingData[19],4);

    //需要设置的模式
    if(SettingData[23] == 0x01)
    {
        //设置成UDP模式
    }
    else if(SettingData[23] == 0x02)
    {
        //设置成TCP模式
    }

    //向所有板卡广播停止指令
    Drv_CAN_SendMsg(&CAN,All_Boards_StopBuffer,8);
    vTaskDelay(1000);

    //向所有板卡广播复位指令
    Drv_CAN_SendMsg(&CAN,All_Boards_ResetBuffer,8);
    //等待1s
    vTaskDelay(1000);

    //保存在EEPROM里
    Task_EEPROM_WriteAddrInfo(remoteip, port, ip, mask, gw);

    //向串口屏发送主板设置完成
    Drv_Uart_Transmit(&Uart5,SettingCplt_SendBuffer,sizeof(SettingCplt_SendBuffer));

    vTaskDelay(2000);

    //主板复位
    NVIC_SystemReset();
}

/**
 * @brief 拼接串口屏发送的设置数据
 * @param RecvData 串口接收的数据
 * @param RecvNum 接收到的长度
 */
static void S_Setting_Apply_DataRecv(uint8_t *RecvData,uint8_t RecvNum)
{
    memcpy(&SettingData_Buffer[SettingDataNum],RecvData,RecvNum);
    SettingDataNum += RecvNum;

    if(SettingDataNum == sizeof(SettingData_Buffer))    //说明完全接收
    {
        IsSettingData = 0;  //标志位恢复进入数据处理
        SettingDataNum = 0; //计数器归零
        S_Setting_Apply_Handle(SettingData_Buffer);
    }
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
 * @brief 串口屏处理函数
 * @param _tUART-串口实例指针
 */
void Task_SerialScreen_Handle(tagUART_T *_tUART)
{
    uint8_t SerialScreenRecvNum = 0;

    /* 串口屏数据接收 */
    SerialScreenRecvNum = Drv_Uart_Receive_DMA(_tUART,SerialScreen_RecvBuffer);
    UNUSED(SerialScreenRecvNum);

#ifdef PRINTF_DEBUG
    if(SerialScreenRecvNum != 0)
    {
        for(uint16_t i = 0; i < SerialScreenRecvNum;i++)
        {
            printf("%x ",SerialScreen_RecvBuffer[i]);
            if(i == SerialScreenRecvNum - 1) printf("\r\n");
        }
    }
#endif

    /* 串口屏数据解析 */
    if(SerialScreen_RecvBuffer[SerialScreenRecvNum - 1] == 0xFF
     &&SerialScreen_RecvBuffer[SerialScreenRecvNum - 2] == 0xFF
     &&SerialScreen_RecvBuffer[SerialScreenRecvNum - 3] == 0xFF)
    {
        switch(SerialScreen_RecvBuffer[0])
        {
            case 0x0A:      //询问电压和连接状态(每秒一次)     0x0A 0xFF 0xFF 0xFF
                xSemaphoreGive(PowerDetect_Sema);   //释放电压检测信号量
                break;        
            case 0x0B:      //查询系统信息(仅进入时发送一次)    0x0B 0xFF 0xFF 0xFF
                S_System_Info_Handle();
                break;
            case 0x0C:      //查询设置数据状态(进入界面时发送一次)   0x0C 0xFF 0xFF 0xFF
                S_Setting_Info_Handle();
                break;
            case 0x0D:      //上传日志   0x0D 0xFF 0xFF 0xFF
                S_Log_Info_Handle();
                break;
            case 0x0E:      //设置下传信息 (按下应用按钮时发送)    0x0E 信息数据 0xFF 0xFF 0xFF
                /* 
                    由于串口屏的发送特性，只能通过STM32进行拼接，代码在下面 
                */
                break;
            case 0x0F:      //设查询通道信息 (进入界面时与进入界面后每五秒发送一次) 0x0F (0x0A-0X0D) 0xFF 0xFF 0xFF
                S_Channel_Info_Handle(SerialScreen_RecvBuffer[1]);
                break;
            case 0x11:      //时间同步
                S_TimeSYNC_Handle(SerialScreen_RecvBuffer);
                break;
            default:
                break;
        }
    }

    /* 串口屏设置数据拼接 */
    if(SerialScreenRecvNum)
    {
        if(SerialScreen_RecvBuffer[0] == 0x0E)  //说明开始发送设置数据
            IsSettingData = 1;  //标志位置1
        if(IsSettingData)
            S_Setting_Apply_DataRecv(SerialScreen_RecvBuffer,SerialScreenRecvNum);
    }

    /* 清空缓存区 */
    memset(SerialScreen_RecvBuffer,0,sizeof(SerialScreen_RecvBuffer));    //清空缓存区
}

