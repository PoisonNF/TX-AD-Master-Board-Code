#include "task_serialscreen.h"

static uint8_t SerialScreen_RecvBuffer[50] = {0};   //串口屏接收缓存区
static uint8_t Channel_Info_SendBuffer[99] = {0};   //通道信息发送缓存区
static uint8_t System_Info_SendBuffer[24] = {0};   //系统信息发送缓存区
static uint8_t Setting_Info_SendBuffer[22] = {0};   //设置信息发送缓存区
//static uint8_t Log_Info_SendBuffer[100] = {0};   //日志信息发送缓存区

static uint8_t Version[4] = "V1.0";        //版本号

#define	GET_BIT(x, bit)	((x & (1 << bit)) >> bit)	/* 获取第bit位 */

extern uint8_t LwIP_UDP_SendBuffer[306];
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

    //添加IP地址信息
    memcpy(&System_Info_SendBuffer[4],RemoteIPAddrString,15);

    //添加版本号信息
    memcpy(&System_Info_SendBuffer[19],Version,4);

    System_Info_SendBuffer[23] = 0x0A;

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

    //添加当前采样速率
    Setting_Info_SendBuffer[4] = 250;        //先写一个250hz后面在改

    //添加当前IP地址信息
    sprintf(LocalIPAddrString,"%d.%d.%d.%d",g_lwipdev.ip[0], g_lwipdev.ip[1], g_lwipdev.ip[2], g_lwipdev.ip[3]);
    memcpy(&Setting_Info_SendBuffer[5],LocalIPAddrString,15);
    
    //添加当前模式，0x01为UDP，0x02为TCP
    if(LWIP_UDP == 1)
        Setting_Info_SendBuffer[20] = 0x01;
    else
        Setting_Info_SendBuffer[20] = 0x02;
    
    Setting_Info_SendBuffer[21] = 0x0A;

    //向串口屏发送数据
    Drv_Uart_Transmit(&Uart5,Setting_Info_SendBuffer,sizeof(Setting_Info_SendBuffer));    
}

/**
 * @brief 上传日志信息处理函数
 * @param Null
 */
static void S_Log_Info_Handle(void)
{

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
        Channel_Info_SendBuffer[index] = GET_BIT(LwIP_UDP_SendBuffer[ValidPos], bit);
        memcpy(&Channel_Info_SendBuffer[index + 1],&LwIP_UDP_SendBuffer[ValuePos],3);
        bit--;
        ValuePos += 3;
    }

    ValidPos++;

    //处理第二块板卡
    for(index = 34; index <= 62; index += 4)
    {
        uint8_t bit = 7;
        Channel_Info_SendBuffer[index] = GET_BIT(LwIP_UDP_SendBuffer[ValidPos], bit);
        memcpy(&Channel_Info_SendBuffer[index + 1],&LwIP_UDP_SendBuffer[ValuePos],3);
        bit--;
        ValuePos += 3;
    }

    ValidPos++;

    //处理第三块板卡
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

    //向串口屏发送数据
    Drv_Uart_Transmit(&Uart5,Channel_Info_SendBuffer,sizeof(Channel_Info_SendBuffer));
}

/**
 * @brief 设置信息应用处理函数
 * @param Null
 */
static void S_Setting_Apply_Handle(uint8_t *SettingData)
{
    uint8_t SetChannelNum = 0;          //需要设置的通道数
    uint8_t SetSamplingFreq = 0;        //需要设置的采样频率

    SetChannelNum = (SettingData[1] - '0')*10 + (SettingData[2] - '0');
    UNUSED(SetChannelNum);
    //根据需要设置的通道数进行处理，待定

    SetSamplingFreq = SettingData[3];
    UNUSED(SetSamplingFreq);
    //根据需要设置的采样频率进行处理，待定

    //需要设置的远程IP地址
    memcpy(RemoteIPAddrString,&SettingData[4],15);

    //需要设置的模式
    if(SettingData[19] == 0x01)
    {
        //设置成UDP模式
    }
    else if(SettingData[19] == 0x02)
    {
        //设置成TCP模式
    }

    //重新配置lwip
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
            if(i == SerialScreenRecvNum) printf("\r\n");
        }
        SerialScreenRecvNum = 0;
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
            case 0x0E:      //设置下传信息 (按下应用按钮时发送)    0x0E 0xFF 0xFF 0xFF
                S_Setting_Apply_Handle(SerialScreen_RecvBuffer);
                break;
            case 0x0F:      //设查询通道信息 (进入界面时与进入界面后每五秒发送一次) 0x0F 0xFF 0xFF 0xFF
                S_Channel_Info_Handle(SerialScreen_RecvBuffer[1]);
                break;
            default:
                break;
        }
    }

    /* 清空缓存区 */
    memset(SerialScreen_RecvBuffer,0,sizeof(SerialScreen_RecvBuffer));    //清空缓存区
}

