#ifndef __TASKTFCARD_H_
#define __TASKTFCARD_H_

#include "drv_hal_conf.h"

#include "usercode.h"
#include "config.h"

// 声明事件
#define TEST_EVENT          (0x01 << 0)         //测试事件
#define SET_EVENT           (0x01 << 1)         //设置信息事件
#define POWER_ON_EVENT      (0x01 << 2)         //开机事件
#define POWER_OFF_EVENT     (0x01 << 3)         //关机或者重启时间事件

#define LOG_SIZE        100 //每组log的长度

void Task_TFCard_CreateFolder(tagFATFS_T *_tFATFS);
void Task_TFCard_WriteLog(tagFATFS_T *_tFATFS,char *_cpFileName,EventBits_t event);
void Task_TFCard_Handle(tagFATFS_T *_tFATFS);

#endif // !__TASKTFCARD_H_
