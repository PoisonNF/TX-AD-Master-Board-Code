#include "task_powerdetect.h"

#define CORRECTIONFACTOR    4.5f            //主板电压修正系数

static float CurrentVol = 0.0f;             //记录当前电压值，单位V
static uint8_t CurrentVolString[10] = {0};  //电压值转成字符串
static uint8_t CurrentVolString_4Bit[4] = {0};   //4位的电压值存储数组，例如"12.10" 保存位 "1210"
static uint8_t Vol_Link_SendBuffer[6] = {0x11,0xAA,0x00,0x00,0x00,0x0A}; //主板电压和连接状态发送数组模板

/**
 * @brief 电源检测处理函数
 * @param _tADC-ADC结构体指针
 */
void Task_PowerDetect_Handle(tagADC_T *_tADC)
{
    uint8_t PointPos = 0;

    if(xSemaphoreTake(PowerDetect_Sema,0) == pdTRUE)   //一直等待获取信号量
    {
        memset(CurrentVolString,0,sizeof(CurrentVolString));
        CurrentVol = Drv_ADC_PollGetValue(_tADC);
        sprintf((char *)CurrentVolString,"%f",CurrentVol * CORRECTIONFACTOR);
#ifdef PRINTF_DEBUG
        printf("CurrentVol = %f\r\n",CurrentVol);
        printf("CurrentVolString = %s\r\n",CurrentVolString);
#endif
        //寻找小数点位置
        for(uint8_t i = 0; i < sizeof(CurrentVolString); i++)
        {
            if(CurrentVolString[i] != '.') continue;
            else
            {
                PointPos = i;   //记录小数点的位置
                break;
            }
        }

        //赋值操作
        if(PointPos == 1)   //防止访问越界
            CurrentVolString_4Bit[0] = 0x30;        //字符'0'
        else
            CurrentVolString_4Bit[0] = CurrentVolString[PointPos - 2];
        
        CurrentVolString_4Bit[1] = CurrentVolString[PointPos - 1];
        CurrentVolString_4Bit[2] = CurrentVolString[PointPos + 1];
        CurrentVolString_4Bit[3] = CurrentVolString[PointPos + 2];

        if(g_lwipdev.link_status == LWIP_LINK_ON)       //处于连接状态
            Vol_Link_SendBuffer[2] = 0X0C;
        else if(g_lwipdev.link_status == LWIP_LINK_OFF) //处于断连状态
            Vol_Link_SendBuffer[2] = 0X0D;
        
        //电压值赋值，大端输入
        Vol_Link_SendBuffer[3] = atoi((const char *)CurrentVolString_4Bit) % 256;
        Vol_Link_SendBuffer[4] = atoi((const char *)CurrentVolString_4Bit) / 256;

        //向串口屏发送数据
        Drv_Uart_Transmit(&Uart5,Vol_Link_SendBuffer,sizeof(Vol_Link_SendBuffer));
    }
}

