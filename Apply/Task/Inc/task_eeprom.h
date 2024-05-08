#ifndef __TASKEEPROM_H_
#define __TASKEEPROM_H_

#include "drv_hal_conf.h"

#include "usercode.h"
#include "config.h"

#include "lwip_comm.h"

void Task_EEPROM_WriteAddrInfo(uint8_t *ip,uint8_t *mask,uint8_t *gw);
void Task_EEPROM_ReadAddrInfo(__lwip_dev *lwipx);

#endif // !__TASKEEPROM_H_
