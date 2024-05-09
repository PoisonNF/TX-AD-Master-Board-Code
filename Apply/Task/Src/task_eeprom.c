#include "task_eeprom.h"

/* 在EEPROM中的存储地址 */
#define IP_UPDATE           0       //需要更新IP信息标志位地址

#define PORT_ADDR           1       //远端IP存储地址 1-2
#define REMOTEIP_ADDR       3       //远端端口号存储地址 3-6

#define IP_ADDR             7        //IP存储地址 7-10
#define MASK_ADDR           11       //Mask存储地址 11-14
#define GW_ADDR             15       //Gw存储地址 15-18

static uint8_t Update_YES = 'Y';        //需要更新IP信息标志
static uint8_t Update_NO = 'N';         //不需要更新IP信息标志

/**
 * @brief 往EEPROM中保存远端和本地的IP信息
 * @param remoteip: 需要更新的远程ip
 * @param port: 需要更新的远程端口号
 * @param ip: 需要更新的本地ip
 * @param mask：更新的掩码
 * @param gw：更新的网关
 * @retval Null
 */
void Task_EEPROM_WriteAddrInfo(uint8_t *remoteip,uint8_t *port,uint8_t *ip,uint8_t *mask,uint8_t *gw)
{
//#ifdef PRINTF_DEBUG
    printf("RemoteIP:%d.%d.%d.%d\r\nPort:%d\r\nIP:%d.%d.%d.%d\r\nMask:%d.%d.%d.%d\r\nGw:%d.%d.%d.%d\r\n",
                remoteip[0],remoteip[1],remoteip[2],remoteip[3],
                (port[0] << 8) + port[1],
                ip[0], ip[1], ip[2], ip[3], 
                mask[0], mask[1], mask[2], mask[3],
                gw[0], gw[1], gw[2], gw[3]);
//#endif
    /* 将需要更改的IP信息写入对应地址上 */
    OCD_AT24CXX_Write(&EEPROM,IP_UPDATE,&Update_YES,1);
    OCD_AT24CXX_Write(&EEPROM,PORT_ADDR,port,2);
    OCD_AT24CXX_Write(&EEPROM,REMOTEIP_ADDR,remoteip,4);
    OCD_AT24CXX_Write(&EEPROM,IP_ADDR,ip,4);
    OCD_AT24CXX_Write(&EEPROM,MASK_ADDR,mask,4);
    OCD_AT24CXX_Write(&EEPROM,GW_ADDR,gw,4);
}

/**
 * @brief 从EEPROM中读取ip 掩码 网关信息
 * @param lwipx: lwip控制结构体
 * @retval Null
 */
void Task_EEPROM_ReadAddrInfo(__lwip_dev *lwipx)
{
    uint8_t IP_Update_flag = 0; //需要更新IP的标志
    uint8_t PortTemp[2] = {0};  //暂存Port的缓冲区

    /* 读取IP信息标志位 */
    OCD_AT24CXX_Read(&EEPROM,IP_UPDATE,&IP_Update_flag,1);

#ifdef PRINTF_DEBUG
    printf("IP_Update_flag %c\r\n",IP_Update_flag);
#endif

    /* 如果需要更新则去对应地址读取 */
    if(IP_Update_flag == Update_YES)
    {
        OCD_AT24CXX_Read(&EEPROM,PORT_ADDR,PortTemp,2);
        RemotePort = (PortTemp[0] << 8) + PortTemp[1];

        OCD_AT24CXX_Read(&EEPROM,REMOTEIP_ADDR,lwipx->remoteip,4);
        OCD_AT24CXX_Read(&EEPROM,IP_ADDR,lwipx->ip,4);
        OCD_AT24CXX_Read(&EEPROM,MASK_ADDR,lwipx->netmask,4);
        OCD_AT24CXX_Read(&EEPROM,GW_ADDR,lwipx->gateway,4);
        OCD_AT24CXX_Write(&EEPROM,IP_UPDATE,&Update_NO,1);

//#ifdef PRINTF_DEBUG
        printf("RemoteIP:%d.%d.%d.%d\r\nPort:%d\r\nIP:%d.%d.%d.%d\r\nMask:%d.%d.%d.%d\r\nGw:%d.%d.%d.%d\r\n",
                lwipx->remoteip[0], lwipx->remoteip[1], lwipx->remoteip[2], lwipx->remoteip[3],
                RemotePort,
                lwipx->ip[0], lwipx->ip[1], lwipx->ip[2], lwipx->ip[3], 
                lwipx->netmask[0], lwipx->netmask[1], lwipx->netmask[2], lwipx->netmask[3],
                lwipx->gateway[0], lwipx->gateway[1], lwipx->gateway[2], lwipx->gateway[3]);
//#endif
    }
}


