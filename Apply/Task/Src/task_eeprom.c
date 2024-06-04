#include "task_eeprom.h"

/* ��EEPROM�еĴ洢��ַ */
#define IP_UPDATE           0       //��Ҫ����IP��Ϣ��־λ��ַ

#define PORT_ADDR           1       //Զ��IP�洢��ַ 1-2
#define REMOTEIP_ADDR       3       //Զ�˶˿ںŴ洢��ַ 3-6

#define IP_ADDR             7        //IP�洢��ַ 7-10
#define MASK_ADDR           11       //Mask�洢��ַ 11-14
#define GW_ADDR             15       //Gw�洢��ַ 15-18

#define PARAM_UPDATE        19      //��Ҫ���²�����Ϣ��־λ��ַ
#define CHANNEL_ADDR        20      //ͨ�����洢��ַ 20
#define SENDRATE_ADDR       21      //�������ʴ洢��ַ 21-22

static uint8_t Update_YES = 'Y';        //��Ҫ������Ϣ��־
static uint8_t Update_NO = 'N';         //����Ҫ������Ϣ��־

/**
 * @brief ��EEPROM�б���Զ�˺ͱ��ص�IP��Ϣ
 * @param remoteip: ��Ҫ���µ�Զ��ip
 * @param port: ��Ҫ���µ�Զ�̶˿ں�
 * @param ip: ��Ҫ���µı���ip
 * @param mask�����µ�����
 * @param gw�����µ�����
 * @retval Null
 */
void Task_EEPROM_WriteAddrInfo(uint8_t *remoteip,uint8_t *port,uint8_t *ip,uint8_t *mask,uint8_t *gw)
{
#ifdef PRINTF_DEBUG
    printf("RemoteIP:%d.%d.%d.%d\r\nPort:%d\r\nIP:%d.%d.%d.%d\r\nMask:%d.%d.%d.%d\r\nGw:%d.%d.%d.%d\r\n",
                remoteip[0],remoteip[1],remoteip[2],remoteip[3],
                (port[0] << 8) + port[1],
                ip[0], ip[1], ip[2], ip[3], 
                mask[0], mask[1], mask[2], mask[3],
                gw[0], gw[1], gw[2], gw[3]);
#endif
    /* ����Ҫ���ĵ�IP��Ϣд���Ӧ��ַ�� */
    OCD_AT24CXX_Write(&EEPROM,IP_UPDATE,&Update_YES,1);
    OCD_AT24CXX_Write(&EEPROM,PORT_ADDR,port,2);
    OCD_AT24CXX_Write(&EEPROM,REMOTEIP_ADDR,remoteip,4);
    OCD_AT24CXX_Write(&EEPROM,IP_ADDR,ip,4);
    OCD_AT24CXX_Write(&EEPROM,MASK_ADDR,mask,4);
    OCD_AT24CXX_Write(&EEPROM,GW_ADDR,gw,4);
    printf("Write IPAddr Done!\r\n");
}

/**
 * @brief ��EEPROM�ж�ȡip ���� ������Ϣ
 * @param lwipx: lwip���ƽṹ��
 * @retval Null
 */
void Task_EEPROM_ReadAddrInfo(__lwip_dev *lwipx)
{
    uint8_t IP_Update_flag = 0; //��Ҫ����IP�ı�־
    uint8_t PortTemp[2] = {0};  //�ݴ�Port�Ļ�����

    /* ��ȡIP��Ϣ��־λ */
    OCD_AT24CXX_Read(&EEPROM,IP_UPDATE,&IP_Update_flag,1);

#ifdef PRINTF_DEBUG
    printf("IP_Update_flag %c\r\n",IP_Update_flag);
#endif

    /* �����Ҫ������ȥ��Ӧ��ַ��ȡ */
    if(IP_Update_flag == Update_YES)
    {
        OCD_AT24CXX_Read(&EEPROM,PORT_ADDR,PortTemp,2);
        RemotePort = (PortTemp[0] << 8) + PortTemp[1];

        OCD_AT24CXX_Read(&EEPROM,REMOTEIP_ADDR,lwipx->remoteip,4);
        OCD_AT24CXX_Read(&EEPROM,IP_ADDR,lwipx->ip,4);
        OCD_AT24CXX_Read(&EEPROM,MASK_ADDR,lwipx->netmask,4);
        OCD_AT24CXX_Read(&EEPROM,GW_ADDR,lwipx->gateway,4);
        OCD_AT24CXX_Write(&EEPROM,IP_UPDATE,&Update_NO,1);

        printf("Read IPAddr Done!\r\n");

#ifdef PRINTF_DEBUG
        printf("RemoteIP:%d.%d.%d.%d\r\nPort:%d\r\nIP:%d.%d.%d.%d\r\nMask:%d.%d.%d.%d\r\nGw:%d.%d.%d.%d\r\n",
                lwipx->remoteip[0], lwipx->remoteip[1], lwipx->remoteip[2], lwipx->remoteip[3],
                RemotePort,
                lwipx->ip[0], lwipx->ip[1], lwipx->ip[2], lwipx->ip[3], 
                lwipx->netmask[0], lwipx->netmask[1], lwipx->netmask[2], lwipx->netmask[3],
                lwipx->gateway[0], lwipx->gateway[1], lwipx->gateway[2], lwipx->gateway[3]);
#endif
    }
}

/**
 * @brief ��EEPROM�б���ͨ�������ʹ���������Ϣ
 * @param _ucChannelsNum: ��Ҫ���µ�ͨ������
 * @param _usTransmissionRate: ��Ҫ���µĴ�������
 * @retval Null
 */
void Task_EEPROM_WriteParameter(uint8_t _ucChannelsNum,uint16_t _usTransmissionRate)
{
    uint8_t TxRateWriteTemp[2] = {0};    //���ڷָ�_usTransmissionRate

#ifdef PRINTF_DEBUG
    printf("Write --- ChannelsNum:%d  TransmissionRate:%d\r\n",
                _ucChannelsNum,_usTransmissionRate);
#endif

    TxRateWriteTemp[0] = _usTransmissionRate >> 8;
    TxRateWriteTemp[1] = _usTransmissionRate & 0xFF;

    /* ����Ҫ���ĵ�ͨ�������ʹ���������Ϣд���Ӧ��ַ�� */
    OCD_AT24CXX_Write(&EEPROM,PARAM_UPDATE,&Update_YES,1);
    OCD_AT24CXX_Write(&EEPROM,CHANNEL_ADDR,&_ucChannelsNum,1);
    OCD_AT24CXX_Write(&EEPROM,SENDRATE_ADDR,TxRateWriteTemp,2);
    printf("Write Parameter Done!\r\n");
}

/**
 * @brief ��EEPROM�ж�ȡ����ͨ�������ʹ���������Ϣ
 * @param _ucpChannelsNum: ��ȡ��ͨ������
 * @param _uspTransmissionRate: ��ȡ�Ĵ�������
 * @retval Null
 */
void Task_EEPROM_ReadParameter(uint8_t *_ucpChannelsNum,uint16_t *_uspTransmissionRate)
{
    uint8_t Param_Update_flag = 0;      //��Ҫ���²����ı�־
    uint8_t TxRateReadTemp[2] = {0};    //���ںϲ�_usTransmissionRate

    /* ��ȡIP��Ϣ��־λ */
    OCD_AT24CXX_Read(&EEPROM,PARAM_UPDATE,&Param_Update_flag,1);

#ifdef PRINTF_DEBUG
    printf("Param_Update_flag %c\r\n",Param_Update_flag);
#endif

    if(Param_Update_flag == Update_YES)
    {
        /* �Ӷ�Ӧ��ַ�϶�ȡ��Ҫ���ĵ�ͨ�������ʹ���������Ϣ */
        OCD_AT24CXX_Read(&EEPROM,CHANNEL_ADDR,_ucpChannelsNum,1);
        OCD_AT24CXX_Read(&EEPROM,SENDRATE_ADDR,TxRateReadTemp,2);
        OCD_AT24CXX_Write(&EEPROM,PARAM_UPDATE,&Update_NO,1);
        *_uspTransmissionRate = (TxRateReadTemp[0] << 8) + TxRateReadTemp[1];

#ifdef PRINTF_DEBUG
        printf("Read --- ChannelsNum:%d  TransmissionRate:%d\r\n",
                    *_ucpChannelsNum,*_uspTransmissionRate);
#endif
        printf("Read Parameter Done!\r\n");
    }
}

