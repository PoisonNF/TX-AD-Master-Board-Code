#ifndef __TASKSELFCHECK_H_
#define __TASKSELFCHECK_H_

#include "drv_hal_conf.h"

#include "usercode.h"
#include "config.h"

#include "task_can.h"
#include "task_eeprom.h"

uint8_t Task_SelfCheck(void);

void Task_SYNC_Signal(void);

#endif // !__TASKSELFCHECK_H_
