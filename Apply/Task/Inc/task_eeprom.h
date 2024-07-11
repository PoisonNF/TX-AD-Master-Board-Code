#ifndef __TASKEEPROM_H_
#define __TASKEEPROM_H_

#include "drv_hal_conf.h"

#include "usercode.h"
#include "config.h"

#include "lwip_comm.h"
#include "task_lwipudp.h"

void Task_EEPROM_WriteDefaultAddrInfo(__lwip_dev *lwipx);
void Task_EEPROM_WriteAddrInfo(uint8_t *remoteip,uint8_t *port,uint8_t *ip,uint8_t *mask,uint8_t *gw);
void Task_EEPROM_ReadAddrInfo(__lwip_dev *lwipx);
void Task_EEPROM_WriteParameter(uint8_t _ucChannelsNum,uint16_t _usTransmissionRate);
void Task_EEPROM_ReadParameter(uint8_t *_ucpChannelsNum,uint16_t *_uspTransmissionRate);

#endif // !__TASKEEPROM_H_
