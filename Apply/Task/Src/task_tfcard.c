#include "task_tfcard.h"

static uint8_t File_Name[] = "/log/1.txt";  /* �ļ��� */
static uint8_t Path_Name[] = "/log";        /* �ļ����� */

static uint8_t CurrTime_Buffer[24] = {0};       //д��ʱ��Ļ�����
static uint8_t WriteLog_Buffer[100] = {0};      //д��log�Ļ�����

static uint8_t Test_log[15] = "Hello World!";   //����logʹ��

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
    if((event & EVENT1) != 0) //����ǲ����¼�
    {
        sprintf((char *)WriteLog_Buffer,"%s %s",CurrTime_Buffer,Test_log);
    }
    else if((event & EVENT2) != 0) //����ǲ����¼�
    {

    }
    else if((event & EVENT3) != 0) //����ǲ����¼�
    {

    }

    if(event)
    {
        WriteLog_Buffer[98] = '\r';
        WriteLog_Buffer[99] = '\n';     //���س�����
        OCD_FATFS_Write_End(_tFATFS, (char *)_cpFileName, WriteLog_Buffer, LOG_SIZE , &sendNum);
        if(sendNum)
            printf("д��%d���ֽ� ����Ϊ%s\r\n",sendNum, WriteLog_Buffer);
    }
}

/**
 * @brief TF��������
 * @param _tFATFS-FATFS�ṹ��ָ��
 */
void Task_TFCard_Handle(tagFATFS_T *_tFATFS)
{
    EventBits_t r_event = 0;

    r_event = xEventGroupWaitBits(Log_Event,
                                  EVENT1|EVENT2|EVENT3,       //��Ҫ�ȴ����¼��ϼ�
								  pdTRUE,                     //�˳�ʱ������־λ���Զ��ͷ�
                                  pdFALSE,                    //��Ƚϣ���������һ���ͳ���
                                  portMAX_DELAY);             //������ʱֱ���¼�����
    
    if(r_event != 0)
        Task_TFCard_WriteLog(_tFATFS,(char *)File_Name,r_event);
}

