#include "task_eeprom.h"

/* 在EEPROM中的存储地址 */
#define IP_UPDATE           0       //需要更新IP信息标志位地址
#define IP_ADDR             1       //IP存储地址 1-4
#define MASK_ADDR           5       //Mask存储地址 5-7
#define GW_ADDR             9       //Gw存储地址 9-12

static uint8_t Update_YES = 'Y';        //需要更新IP信息标志
static uint8_t Update_NO = 'N';         //不需要更新IP信息标志

/**
 * @brief 往EEPROM中保存ip 掩码 网关信息
 * @param ip: 需要更新的ip
 * @param mask：更新的掩码
 * @param gw：更新的网关
 * @retval Null
 */
void Task_EEPROM_WriteAddrInfo(uint8_t *ip,uint8_t *mask,uint8_t *gw)
{
#ifdef PRINTF_DEBUG
    printf("IP:%d.%d.%d.%d\r\nMask:%d.%d.%d.%d\r\nGw:%d.%d.%d.%d\r\n",
                ip[0], ip[1], ip[2], ip[3], 
                mask[0], mask[1], mask[2], mask[3],
                gw[0], gw[1], gw[2], gw[3]);
#endif
    /* 将需要更改的IP信息写入对应地址上 */
    OCD_AT24CXX_Write(&EEPROM,IP_UPDATE,&Update_YES,1);
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
    uint8_t IP_Update_flag = 0;

    /* 读取IP信息标志位 */
    OCD_AT24CXX_Read(&EEPROM,IP_UPDATE,&IP_Update_flag,1);

#ifdef PRINTF_DEBUG
    printf("IP_Update_flag %c\r\n",IP_Update_flag);
#endif

    /* 如果需要更新则去对应地址读取 */
    if(IP_Update_flag == Update_YES)
    {
        OCD_AT24CXX_Read(&EEPROM,IP_ADDR,lwipx->ip,4);
        OCD_AT24CXX_Read(&EEPROM,MASK_ADDR,lwipx->netmask,4);
        OCD_AT24CXX_Read(&EEPROM,GW_ADDR,lwipx->gateway,4);
        OCD_AT24CXX_Write(&EEPROM,IP_UPDATE,&Update_NO,1);

#ifdef PRINTF_DEBUG
        printf("IP:%d.%d.%d.%d\r\nMask:%d.%d.%d.%d\r\nGw:%d.%d.%d.%d\r\n",
                lwipx->ip[0], lwipx->ip[1], lwipx->ip[2], lwipx->ip[3], 
                lwipx->netmask[0], lwipx->netmask[1], lwipx->netmask[2], lwipx->netmask[3],
                lwipx->gateway[0], lwipx->gateway[1], lwipx->gateway[2], lwipx->gateway[3]);
#endif
    }
}


