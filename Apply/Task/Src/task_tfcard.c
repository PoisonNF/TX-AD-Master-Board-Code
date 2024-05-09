#include "task_tfcard.h"

static uint8_t File_Name[] = "/log/1.txt";  /* 文件名 */
static uint8_t Path_Name[] = "/log";        /* 文件夹名 */

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

}

/**
 * @brief TF卡读log到文件中函数
 * @param _tFATFS-FATFS结构体指针
 * @param Logdata-日志数据
 */
void Task_TFCard_ReadLog(tagFATFS_T *_tFATFS,uint8_t *Logdata)
{
    
}

/**
 * @brief TF卡处理函数
 * @param _tFATFS-FATFS结构体指针
 */
void Task_TFCard_Handle(tagFATFS_T *_tFATFS)
{
    uint32_t sendNum = 0;
    EventBits_t r_event;

    r_event = xEventGroupWaitBits(Log_Event,
                                  EVENT1|EVENT2|EVENT3,       //需要等待的事件合集
								  pdTRUE,                     //退出时消除标志位，自动释放
                                  pdFALSE,                    //或比较，满足其中一个就成立
                                  portMAX_DELAY);             //无限延时直到事件成立
    
    if((r_event & EVENT1) != 0) //如果是测试事件
    {
        if(OCD_FATFS_Write_End(_tFATFS, (char *)File_Name, "abcdefghijklmnopqrtsuvwxyz", sizeof("abcdefghijklmnopqrtsuvwxyz") , &sendNum) == FR_OK)
            printf("写入%d个字节 数据为%s\r\n",sendNum,"abcdefghijklmnopqrtsuvwxyz");
    }
}

