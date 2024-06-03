#ifndef __TASKLWIPUDP_H_
#define __TASKLWIPUDP_H_

#include "drv_hal_conf.h"
#include "usercode.h"

extern char RemoteIPAddrString[16]; 
extern char LocalIPAddrString[16];  

extern uint8_t LwIP_UDP_SendBuffer[4][306];
extern uint16_t RemotePort;

extern uint16_t CurrentSendRate;

#define SPLICE_NUM                      4                       /* 一帧UDP中拼接包数 */

void Task_LwIP_UDP_Handle(void);

#endif // !__TASKLWIPUDP_H_

