#ifndef __TASKTFCARD_H_
#define __TASKTFCARD_H_

#include "drv_hal_conf.h"

#include "usercode.h"
#include "config.h"

// �����¼�
#define TEST_EVENT          (0x01 << 0)         //�����¼�
#define SET_EVENT           (0x01 << 1)         //������Ϣ�¼�
#define POWER_ON_EVENT      (0x01 << 2)         //�����¼�
#define POWER_OFF_EVENT     (0x01 << 3)         //�ػ���������ʱ���¼�

#define LOG_SIZE        100 //ÿ��log�ĳ���

void Task_TFCard_CreateFolder(tagFATFS_T *_tFATFS);
void Task_TFCard_WriteLog(tagFATFS_T *_tFATFS,char *_cpFileName,EventBits_t event);
void Task_TFCard_Handle(tagFATFS_T *_tFATFS);

#endif // !__TASKTFCARD_H_
