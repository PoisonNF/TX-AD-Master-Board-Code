#ifndef __CONFIG_H_
#define __CONFIG_H_

#include "drv_hal_conf.h"
#include "ocd_conf.h"
#include "algo_conf.h"
#include "dev_conf.h"

/* 用户句柄声明包含区 */

extern tagGPIO_T LED[];

extern tagUART_T Uart1;

extern tagUART_T Uart5;

extern tagADC_T PowerADC[];

extern tagCAN_T CAN;

extern tagGPIO_T Control[];

extern tagGPIO_T SYNC[];

extern tagDS3231_T DS1337;

extern tagAT24CXX_T EEPROM;

#endif
