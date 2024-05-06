#ifndef __TASKCAN_H_
#define __TASKCAN_H_

#include "drv_hal_conf.h"

#include "usercode.h"
#include "config.h"

#include "task_lwipudp.h"

void Task_CAN_Handle(tagCAN_T *_tCAN);

#endif // !__TASKCAN_H_
