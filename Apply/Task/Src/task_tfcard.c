#include "task_tfcard.h"

static uint8_t File_Name[] = "/log/1.txt";  /* �ļ��� */
static uint8_t Path_Name[] = "/log";        /* �ļ����� */

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

}

/**
 * @brief TF����log���ļ��к���
 * @param _tFATFS-FATFS�ṹ��ָ��
 * @param Logdata-��־����
 */
void Task_TFCard_ReadLog(tagFATFS_T *_tFATFS,uint8_t *Logdata)
{
    
}

/**
 * @brief TF��������
 * @param _tFATFS-FATFS�ṹ��ָ��
 */
void Task_TFCard_Handle(tagFATFS_T *_tFATFS)
{
    uint32_t sendNum = 0;
    EventBits_t r_event;

    r_event = xEventGroupWaitBits(Log_Event,
                                  EVENT1|EVENT2|EVENT3,       //��Ҫ�ȴ����¼��ϼ�
								  pdTRUE,                     //�˳�ʱ������־λ���Զ��ͷ�
                                  pdFALSE,                    //��Ƚϣ���������һ���ͳ���
                                  portMAX_DELAY);             //������ʱֱ���¼�����
    
    if((r_event & EVENT1) != 0) //����ǲ����¼�
    {
        if(OCD_FATFS_Write_End(_tFATFS, (char *)File_Name, "abcdefghijklmnopqrtsuvwxyz", sizeof("abcdefghijklmnopqrtsuvwxyz") , &sendNum) == FR_OK)
            printf("д��%d���ֽ� ����Ϊ%s\r\n",sendNum,"abcdefghijklmnopqrtsuvwxyz");
    }
}

