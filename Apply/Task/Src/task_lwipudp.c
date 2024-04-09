#include "task_lwipudp.h"
#include "lwip/sockets.h"
#include "lwip/api.h"


/* Զ��IP��ַ */
#define REMOTE_IP_ADDR          "192.168.1.101"     //����ʵ���������

#define LWIP_UDP_RX_BUFSIZE         200                         /* ���������ݳ��� */
#define LWIP_UDP_PORT               8080                        /* ���ӵı��ض˿ں� */
#define LWIP_SEND_THREAD_PRIO       ( tskIDLE_PRIORITY + 3 )    /* ���������߳����ȼ� */

/* UDP���շ��ͻ����� */
uint8_t LwIP_UDP_RecvBuffer[LWIP_UDP_RX_BUFSIZE];
uint8_t LwIP_UDP_SendBuffer[] = "Test UDP\r\n";

/* Socket��� */
struct sockaddr_in Local_info;      /* ����Socket��ַ��Ϣ */
socklen_t Socket_fd;                /* ����һ��Socket�ļ������� */

/**
 * @brief UDP�����߳���ں���
 * @param pvParameters �������
 */
static void S_LwIP_UDP_Send_Entrance(void *pvParameters)
{
    UNUSED(pvParameters);
    uint16_t SendNum = 0;

    Local_info.sin_addr.s_addr = inet_addr(REMOTE_IP_ADDR);     //ѡ����Ҫ���͵�Զ��IP��ַ

    while(1)
    {
        //����sendto�������ͣ�UDP��Ҫʹ��sendto����
        SendNum = sendto(Socket_fd,                     //Socket�ļ�������
                        LwIP_UDP_SendBuffer,            //���͵�����
                        sizeof(LwIP_UDP_SendBuffer),    //���͵����ݳ���
                        0,                              //���͵ı�־
                        (struct sockaddr *)&Local_info,
                        sizeof(Local_info));                             
        if(SendNum) printf("Send Complete!\r\n");

        vTaskDelay(1000);
    }
}

/**
 * @brief ����UDP�����߳�
 * @param Null
 */
static inline void S_LwIP_UDP_Create_Send_Thread(void)
{
    sys_thread_new("lwip_send_thread",S_LwIP_UDP_Send_Entrance,NULL,512,LWIP_SEND_THREAD_PRIO);
}

/**
 * @brief UDP���������������պͷ���
 * @param Null
 */
void Task_LwIP_UDP_Handle(void)
{
    //BaseType_t LwIP_Err;    //����ֵ�ж�
    uint16_t RecvNum = 0;

    //����UDP�����߳�
    S_LwIP_UDP_Create_Send_Thread();        

    //����Socket��ַ��Ϣ����
    memset(&Local_info,0,sizeof(Local_info));
    Local_info.sin_len = sizeof(Local_info);
    Local_info.sin_family = AF_INET;                    //IPv4��ַ
    Local_info.sin_port = htons(LWIP_UDP_PORT);         //�˿ں�
    Local_info.sin_addr.s_addr = htons(INADDR_ANY);     //����IP��ַ

    //����Socket����
    Socket_fd = socket(AF_INET,SOCK_DGRAM,0);           //����UDP���͵�Socket

    //����Bind
    bind(Socket_fd,(struct sockaddr *)&Local_info,sizeof(struct sockaddr_in));

    while(1)
    {
        //UDP���մ���
        memset(&LwIP_UDP_RecvBuffer,0,sizeof(LwIP_UDP_RecvBuffer));

        RecvNum = recv(Socket_fd,LwIP_UDP_RecvBuffer,sizeof(LwIP_UDP_RecvBuffer),0);
        if(RecvNum) printf("%s\r\n",LwIP_UDP_RecvBuffer);

        vTaskDelay(1);  //�����л��߳�
    }
}

