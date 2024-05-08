#include "task_eeprom.h"

/* ��EEPROM�еĴ洢��ַ */
#define IP_UPDATE           0       //��Ҫ����IP��Ϣ��־λ��ַ
#define IP_ADDR             1       //IP�洢��ַ 1-4
#define MASK_ADDR           5       //Mask�洢��ַ 5-7
#define GW_ADDR             9       //Gw�洢��ַ 9-12

static uint8_t Update_YES = 'Y';        //��Ҫ����IP��Ϣ��־
static uint8_t Update_NO = 'N';         //����Ҫ����IP��Ϣ��־

/**
 * @brief ��EEPROM�б���ip ���� ������Ϣ
 * @param ip: ��Ҫ���µ�ip
 * @param mask�����µ�����
 * @param gw�����µ�����
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
    /* ����Ҫ���ĵ�IP��Ϣд���Ӧ��ַ�� */
    OCD_AT24CXX_Write(&EEPROM,IP_UPDATE,&Update_YES,1);
    OCD_AT24CXX_Write(&EEPROM,IP_ADDR,ip,4);
    OCD_AT24CXX_Write(&EEPROM,MASK_ADDR,mask,4);
    OCD_AT24CXX_Write(&EEPROM,GW_ADDR,gw,4);
}

/**
 * @brief ��EEPROM�ж�ȡip ���� ������Ϣ
 * @param lwipx: lwip���ƽṹ��
 * @retval Null
 */
void Task_EEPROM_ReadAddrInfo(__lwip_dev *lwipx)
{
    uint8_t IP_Update_flag = 0;

    /* ��ȡIP��Ϣ��־λ */
    OCD_AT24CXX_Read(&EEPROM,IP_UPDATE,&IP_Update_flag,1);

#ifdef PRINTF_DEBUG
    printf("IP_Update_flag %c\r\n",IP_Update_flag);
#endif

    /* �����Ҫ������ȥ��Ӧ��ַ��ȡ */
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


