#ifndef __TASKTFCARD_H_
#define __TASKTFCARD_H_

#include "drv_hal_conf.h"

#include "usercode.h"
#include "config.h"

// �����¼�
#define EVENT1 (0x01 << 0)      //�����¼�
#define EVENT2 (0x01 << 1)      //����IP��Ϣ�¼�
#define EVENT3 (0x01 << 2)      //�ػ���������ʱ���¼�

#define LOG_SIZE        100 //ÿ��log�ĳ���

void Task_TFCard_CreateFolder(tagFATFS_T *_tFATFS);
void Task_TFCard_WriteLog(tagFATFS_T *_tFATFS,char *_cpFileName,EventBits_t event);
void Task_TFCard_Handle(tagFATFS_T *_tFATFS);

#endif // !__TASKTFCARD_H_
