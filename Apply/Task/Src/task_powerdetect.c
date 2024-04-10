#include "task_powerdetect.h"

#define CORRECTIONFACTOR    4.5f            //�����ѹ����ϵ��

static float CurrentVol = 0.0f;             //��¼��ǰ��ѹֵ����λV
static uint8_t CurrentVolString[10] = {0};  //��ѹֵת���ַ���
static uint8_t VoltageSendBuffer[14] = {0x78,0x30,0x2E,0x76,0x61,0x6C,0x3D,0x30,0x30,0x30,0x30,0xFF,0xFF,0xFF}; //�����ѹ��������ģ��

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
            VoltageSendBuffer[7] = 0x30;        //�ַ�0
        else
            VoltageSendBuffer[7] = CurrentVolString[PointPos - 2];
        
        VoltageSendBuffer[8]  = CurrentVolString[PointPos - 1];
        VoltageSendBuffer[9]  = CurrentVolString[PointPos + 1];
        VoltageSendBuffer[10] = CurrentVolString[PointPos + 2];

        //�򴮿�����������
        Drv_Uart_Transmit(&Uart5,VoltageSendBuffer,sizeof(VoltageSendBuffer));
    }
}

