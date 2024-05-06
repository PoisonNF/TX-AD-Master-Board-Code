#include "task_powerdetect.h"

#define CORRECTIONFACTOR    4.5f            //�����ѹ����ϵ��

static float CurrentVol = 0.0f;             //��¼��ǰ��ѹֵ����λV
static uint8_t CurrentVolString[10] = {0};  //��ѹֵת���ַ���
static uint8_t CurrentVolString_4Bit[4] = {0};   //4λ�ĵ�ѹֵ�洢���飬����"12.10" ����λ "1210"
static uint8_t Vol_Link_SendBuffer[6] = {0x11,0xAA,0x00,0x00,0x00,0x0A}; //�����ѹ������״̬��������ģ��

/**
 * @brief ��Դ��⴦����
 * @param _tADC-ADC�ṹ��ָ��
 */
void Task_PowerDetect_Handle(tagADC_T *_tADC)
{
    uint8_t PointPos = 0;

    if(xSemaphoreTake(PowerDetect_Sema,0) == pdTRUE)   //һֱ�ȴ���ȡ�ź���
    {
        memset(CurrentVolString,0,sizeof(CurrentVolString));
        CurrentVol = Drv_ADC_PollGetValue(_tADC);
        sprintf((char *)CurrentVolString,"%f",CurrentVol * CORRECTIONFACTOR);
#ifdef PRINTF_DEBUG
        printf("CurrentVol = %f\r\n",CurrentVol);
        printf("CurrentVolString = %s\r\n",CurrentVolString);
#endif
        //Ѱ��С����λ��
        for(uint8_t i = 0; i < sizeof(CurrentVolString); i++)
        {
            if(CurrentVolString[i] != '.') continue;
            else
            {
                PointPos = i;   //��¼С�����λ��
                break;
            }
        }

        //��ֵ����
        if(PointPos == 1)   //��ֹ����Խ��
            CurrentVolString_4Bit[0] = 0x30;        //�ַ�'0'
        else
            CurrentVolString_4Bit[0] = CurrentVolString[PointPos - 2];
        
        CurrentVolString_4Bit[1] = CurrentVolString[PointPos - 1];
        CurrentVolString_4Bit[2] = CurrentVolString[PointPos + 1];
        CurrentVolString_4Bit[3] = CurrentVolString[PointPos + 2];

        if(g_lwipdev.link_status == LWIP_LINK_ON)       //��������״̬
            Vol_Link_SendBuffer[2] = 0X0C;
        else if(g_lwipdev.link_status == LWIP_LINK_OFF) //���ڶ���״̬
            Vol_Link_SendBuffer[2] = 0X0D;
        
        //��ѹֵ��ֵ���������
        Vol_Link_SendBuffer[3] = atoi((const char *)CurrentVolString_4Bit) % 256;
        Vol_Link_SendBuffer[4] = atoi((const char *)CurrentVolString_4Bit) / 256;

        //�򴮿�����������
        Drv_Uart_Transmit(&Uart5,Vol_Link_SendBuffer,sizeof(Vol_Link_SendBuffer));
    }
}

