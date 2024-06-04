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

extern uint8_t SetRemoteip[4],SetPort[2];
extern uint8_t SetIP[4],SetMask[4],SetGW[4];

void Task_SerialScreen_Handle(tagUART_T *_tUART);
void Task_TellSerialScreen_ResetCplt(void);

#endif // !__TASKSERIALSCREEN_H_
