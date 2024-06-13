#include "task_tfcard.h"

uint8_t File_Name[] = "/log/1.txt";  /* �ļ��� */
uint8_t Path_Name[] = "/log";        /* �ļ����� */

static uint8_t CurrTime_Buffer[24] = {0};       //д��ʱ��Ļ�����
static uint8_t WriteLog_Buffer[200] = {0};      //д��log�Ļ�����

static uint8_t Test_log[15] = "Hello World!";       //����logʹ��
static uint8_t Set_log[176] = "";                    //����logʹ��
static uint8_t PowerON_log[15] = "Power ON!";       //����logʹ��
static uint8_t PowerOFF_log[15] = "Power OFF!";     //�ػ�logʹ��

uint32_t logNum = 0;                                //log������¼
bool TFCardIsExist = true;                          //TF���Ƿ���ڱ�־

/**
 * @brief TF������log�ļ�����
 * @param _tFATFS-FATFS�ṹ��ָ��
 */
void Task_TFCard_CreateFolder(tagFATFS_T *_tFATFS)
{
    uint8_t ret = 0;

    //�����ļ���
	ret = OCD_FATFS_CreateDir(_tFATFS,(char *)Path_Name);
    if(ret == FR_OK)
        printf("�����ļ��гɹ� %s\r\n",Path_Name);
    else if(ret == FR_EXIST)
        printf("�ļ����Ѵ��� %s\r\n",Path_Name);
}

/**
 * @brief TF������event����дlog���ļ��к���
 * @param _tFATFS-FATFS�ṹ��ָ��
 * @param _cpFileName-�ļ�·��ָ��
 * @param event-�¼���
 */
void Task_TFCard_WriteLog(tagFATFS_T *_tFATFS,char *_cpFileName,EventBits_t event)
{
    uint32_t sendNum = 0;
    tagDS3231Time_T CurrSysTime = {0};

    /* ��ȡ��ǰʱ�� */
    if(OCD_DS3231_TimeGetHex(&DS1337,&CurrSysTime))
    {
#ifdef PRINTF_DEBUG
        printf("Read Time:");
        printf("20%02x/%02x/%02x %02x:%02x:%02x ��%x\r\n",
                CurrSysTime.ucYear,CurrSysTime.ucMonth,CurrSysTime.ucDate,
                CurrSysTime.ucHour,CurrSysTime.ucMinute,CurrSysTime.ucSecond,
                CurrSysTime.ucWeek);
#endif
        sprintf((char *)CurrTime_Buffer,"[20%02x/%02x/%02x %02x:%02x:%02x %x]",
                CurrSysTime.ucYear,CurrSysTime.ucMonth,CurrSysTime.ucDate,
                CurrSysTime.ucHour,CurrSysTime.ucMinute,CurrSysTime.ucSecond,
                CurrSysTime.ucWeek);
    }

    /* �ж��¼� */
    if((event & TEST_EVENT) != 0) //����ǲ����¼�
    {
        sprintf((char *)WriteLog_Buffer,"%s %s",CurrTime_Buffer,Test_log);
    }
    else if((event & SET_EVENT) != 0) //�����������Ϣ�¼�
    {
        sprintf((char *)Set_log,"\r\nChannelNum:%d SendRate:%d \r\nLocal IP:%d.%d.%d.%d Mask:%d.%d.%d.%d GW:%d.%d.%d.%d \r\nRemote IP:%d.%d.%d.%d Port:%d",
                                    CurrentChannelNum,CurrentSendRate,
                                    SetIP[0],SetIP[1],SetIP[2],SetIP[3],
                                    SetMask[0],SetMask[1],SetMask[2],SetMask[3],
                                    SetGW[0],SetGW[1],SetGW[2],SetGW[3],
                                    SetRemoteip[0],SetRemoteip[1],SetRemoteip[2],SetRemoteip[3],
                                    (SetPort[0] << 8) + SetPort[1]);
        sprintf((char *)WriteLog_Buffer,"%s %s",CurrTime_Buffer,Set_log);
    }
    else if((event & POWER_ON_EVENT) != 0) //����ǿ����¼�
    {
        sprintf((char *)WriteLog_Buffer,"%s %s",CurrTime_Buffer,PowerON_log);
    }
    else if((event & POWER_OFF_EVENT) != 0) //����ǹػ��¼�
    {
        sprintf((char *)WriteLog_Buffer,"%s %s",CurrTime_Buffer,PowerOFF_log);
    }

    if(event)
    {
        WriteLog_Buffer[sizeof(WriteLog_Buffer) - 2] = '\r';
        WriteLog_Buffer[sizeof(WriteLog_Buffer) - 1] = '\n';     //���س�����
        OCD_FATFS_Write_End(_tFATFS, (char *)_cpFileName, WriteLog_Buffer, LOG_SIZE , &sendNum);    //д��log��־
        logNum++;             //log������������һ
        if(sendNum)
            printf("д��%d���ֽ� ����Ϊ%s\r\n",sendNum, WriteLog_Buffer);
    }
}

/**
 * @brief TF��Ѱ�����ݵ�ĩβ��ȷ��ƫ��������
 * @param _tFATFS-FATFS�ṹ��ָ��
 * @return uint32_t log����
 */
uint32_t Task_TFCard_FindEnd(tagFATFS_T *_tFATFS)
{
    uint32_t ulOffsetNum = 0;
    uint32_t ulReadNum = 0;
    uint8_t ucReceBuffer[LOG_SIZE] = {0};

    /* Ѱ�����ݵ�ĩβ��ȷ��ƫ���� */
    do
    {
        OCD_FATFS_Read_SpecifyIndex(&TFCard, (char *)File_Name, ucReceBuffer , LOG_SIZE , ulOffsetNum * LOG_SIZE , &ulReadNum);
        ulOffsetNum++;
    }while(ulReadNum == LOG_SIZE);

    ulOffsetNum--;    //ulOffsetNum���ʵ���������һ��ƫ����������Ҫ��һ����
    return ulOffsetNum;
}

/**
 * @brief TF��������
 * @param _tFATFS-FATFS�ṹ��ָ��
 */
void Task_TFCard_Handle(tagFATFS_T *_tFATFS)
{
    EventBits_t r_event = 0;

    r_event = xEventGroupWaitBits(Log_Event,
                                  TEST_EVENT|SET_EVENT|POWER_ON_EVENT|POWER_OFF_EVENT,       //��Ҫ�ȴ����¼��ϼ�
								  pdTRUE,                     //�˳�ʱ������־λ���Զ��ͷ�
                                  pdFALSE,                    //��Ƚϣ���������һ���ͳ���
                                  portMAX_DELAY);             //������ʱֱ���¼�����
    
    if(r_event != 0)
        Task_TFCard_WriteLog(_tFATFS,(char *)File_Name,r_event);
}

