#ifndef __TASKTFCARD_H_
#define __TASKTFCARD_H_

#include "drv_hal_conf.h"

#include "usercode.h"
#include "config.h"

// 声明事件
#define EVENT1 (0x01 << 0)      //测试事件
#define EVENT2 (0x01 << 1)      //设置IP信息事件
#define EVENT3 (0x01 << 2)      //关机或者重启时间事件

void Task_TFCard_CreateFolder(tagFATFS_T *_tFATFS);
void Task_TFCard_WriteLog(tagFATFS_T *_tFATFS,char *_cpFileName,EventBits_t event);
void Task_TFCard_ReadLog(tagFATFS_T *_tFATFS,uint8_t *Logdata);
void Task_TFCard_Handle(tagFATFS_T *_tFATFS);

#endif // !__TASKTFCARD_H_
