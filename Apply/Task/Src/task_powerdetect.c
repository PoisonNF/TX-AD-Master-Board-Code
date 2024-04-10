#include "task_powerdetect.h"

#define CORRECTIONFACTOR    4.5f            //主板电压修正系数

static float CurrentVol = 0.0f;             //记录当前电压值，单位V
static uint8_t CurrentVolString[10] = {0};  //电压值转成字符串
static uint8_t VoltageSendBuffer[14] = {0x78,0x30,0x2E,0x76,0x61,0x6C,0x3D,0x30,0x30,0x30,0x30,0xFF,0xFF,0xFF}; //主板电压发送数组模板

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
            VoltageSendBuffer[7] = 0x30;        //字符0
        else
            VoltageSendBuffer[7] = CurrentVolString[PointPos - 2];
        
        VoltageSendBuffer[8]  = CurrentVolString[PointPos - 1];
        VoltageSendBuffer[9]  = CurrentVolString[PointPos + 1];
        VoltageSendBuffer[10] = CurrentVolString[PointPos + 2];

        //向串口屏发送数据
        Drv_Uart_Transmit(&Uart5,VoltageSendBuffer,sizeof(VoltageSendBuffer));
    }
}

