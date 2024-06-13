#include "task_tfcard.h"

uint8_t File_Name[] = "/log/1.txt";  /* 文件名 */
uint8_t Path_Name[] = "/log";        /* 文件夹名 */

static uint8_t CurrTime_Buffer[24] = {0};       //写入时间的缓冲区
static uint8_t WriteLog_Buffer[200] = {0};      //写入log的缓冲区

static uint8_t Test_log[15] = "Hello World!";       //测试log使用
static uint8_t Set_log[176] = "";                    //设置log使用
static uint8_t PowerON_log[15] = "Power ON!";       //开机log使用
static uint8_t PowerOFF_log[15] = "Power OFF!";     //关机log使用

uint32_t logNum = 0;                                //log条数记录
bool TFCardIsExist = true;                          //TF卡是否存在标志

/**
 * @brief TF卡创建log文件函数
 * @param _tFATFS-FATFS结构体指针
 */
void Task_TFCard_CreateFolder(tagFATFS_T *_tFATFS)
{
    uint8_t ret = 0;

    //创建文件夹
	ret = OCD_FATFS_CreateDir(_tFATFS,(char *)Path_Name);
    if(ret == FR_OK)
        printf("创建文件夹成功 %s\r\n",Path_Name);
    else if(ret == FR_EXIST)
        printf("文件夹已存在 %s\r\n",Path_Name);
}

/**
 * @brief TF卡根据event类型写log到文件中函数
 * @param _tFATFS-FATFS结构体指针
 * @param _cpFileName-文件路径指针
 * @param event-事件组
 */
void Task_TFCard_WriteLog(tagFATFS_T *_tFATFS,char *_cpFileName,EventBits_t event)
{
    uint32_t sendNum = 0;
    tagDS3231Time_T CurrSysTime = {0};

    /* 获取当前时间 */
    if(OCD_DS3231_TimeGetHex(&DS1337,&CurrSysTime))
    {
#ifdef PRINTF_DEBUG
        printf("Read Time:");
        printf("20%02x/%02x/%02x %02x:%02x:%02x 周%x\r\n",
                CurrSysTime.ucYear,CurrSysTime.ucMonth,CurrSysTime.ucDate,
                CurrSysTime.ucHour,CurrSysTime.ucMinute,CurrSysTime.ucSecond,
                CurrSysTime.ucWeek);
#endif
        sprintf((char *)CurrTime_Buffer,"[20%02x/%02x/%02x %02x:%02x:%02x %x]",
                CurrSysTime.ucYear,CurrSysTime.ucMonth,CurrSysTime.ucDate,
                CurrSysTime.ucHour,CurrSysTime.ucMinute,CurrSysTime.ucSecond,
                CurrSysTime.ucWeek);
    }

    /* 判断事件 */
    if((event & TEST_EVENT) != 0) //如果是测试事件
    {
        sprintf((char *)WriteLog_Buffer,"%s %s",CurrTime_Buffer,Test_log);
    }
    else if((event & SET_EVENT) != 0) //如果是设置信息事件
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
    else if((event & POWER_ON_EVENT) != 0) //如果是开机事件
    {
        sprintf((char *)WriteLog_Buffer,"%s %s",CurrTime_Buffer,PowerON_log);
    }
    else if((event & POWER_OFF_EVENT) != 0) //如果是关机事件
    {
        sprintf((char *)WriteLog_Buffer,"%s %s",CurrTime_Buffer,PowerOFF_log);
    }

    if(event)
    {
        WriteLog_Buffer[sizeof(WriteLog_Buffer) - 2] = '\r';
        WriteLog_Buffer[sizeof(WriteLog_Buffer) - 1] = '\n';     //补回车换行
        OCD_FATFS_Write_End(_tFATFS, (char *)_cpFileName, WriteLog_Buffer, LOG_SIZE , &sendNum);    //写入log日志
        logNum++;             //log条数计数器加一
        if(sendNum)
            printf("写入%d个字节 数据为%s\r\n",sendNum, WriteLog_Buffer);
    }
}

/**
 * @brief TF卡寻找内容的末尾，确定偏移量函数
 * @param _tFATFS-FATFS结构体指针
 * @return uint32_t log条数
 */
uint32_t Task_TFCard_FindEnd(tagFATFS_T *_tFATFS)
{
    uint32_t ulOffsetNum = 0;
    uint32_t ulReadNum = 0;
    uint8_t ucReceBuffer[LOG_SIZE] = {0};

    /* 寻找内容的末尾，确定偏移量 */
    do
    {
        OCD_FATFS_Read_SpecifyIndex(&TFCard, (char *)File_Name, ucReceBuffer , LOG_SIZE , ulOffsetNum * LOG_SIZE , &ulReadNum);
        ulOffsetNum++;
    }while(ulReadNum == LOG_SIZE);

    ulOffsetNum--;    //ulOffsetNum会比实际情况后面一个偏移数，所以要减一消除
    return ulOffsetNum;
}

/**
 * @brief TF卡处理函数
 * @param _tFATFS-FATFS结构体指针
 */
void Task_TFCard_Handle(tagFATFS_T *_tFATFS)
{
    EventBits_t r_event = 0;

    r_event = xEventGroupWaitBits(Log_Event,
                                  TEST_EVENT|SET_EVENT|POWER_ON_EVENT|POWER_OFF_EVENT,       //需要等待的事件合集
								  pdTRUE,                     //退出时消除标志位，自动释放
                                  pdFALSE,                    //或比较，满足其中一个就成立
                                  portMAX_DELAY);             //无限延时直到事件成立
    
    if(r_event != 0)
        Task_TFCard_WriteLog(_tFATFS,(char *)File_Name,r_event);
}

