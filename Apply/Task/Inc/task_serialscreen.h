#ifndef __TASKSERIALSCREEN_H_
#define __TASKSERIALSCREEN_H_

#include "drv_hal_conf.h"

#include "usercode.h"
#include "config.h"

#include "lwipopts.h"
#include "task_lwipudp.h"
#include "lwip_comm.h"
#include "task_eeprom.h"
#include "task_tfcard.h"
#include "task_can.h"

void Task_SerialScreen_Handle(tagUART_T *_tUART);

#endif // !__TASKSERIALSCREEN_H_
