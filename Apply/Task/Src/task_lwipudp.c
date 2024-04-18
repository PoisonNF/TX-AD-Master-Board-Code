#include "task_lwipudp.h"
#include "lwip/sockets.h"
#include "lwip/api.h"


/* Զ��IP��ַ */
#define REMOTE_IP_ADDR          "192.168.1.101"     //����ʵ���������

#define LWIP_UDP_RX_BUFSIZE         100                         /* ���������ݳ��� */
#define LWIP_UDP_PORT               8080                        /* ���ӵı��ض˿ں� */
#define LWIP_SEND_THREAD_PRIO       ( tskIDLE_PRIORITY + 9 )    /* ���������߳����ȼ� */

#define FRAMEHEADER1                0X02                        /* ֡ͷ1 */
#define FRAMEHEADER2                0X48                        /* ֡ͷ2 */
#define FRAMEEND                    0X03                        /* ֡β */

/* UDP���շ��ͻ����� */
uint8_t LwIP_UDP_RecvBuffer[LWIP_UDP_RX_BUFSIZE] = {0};
// uint8_t LwIP_UDP_SendBuffer[] = "Test UDP\r\n";
uint8_t LwIP_UDP_SendBuffer[306] = {0};

/* UDP����֡������ */
static uint16_t FrameNum = 0;

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
    int SendNum = 0;

    Local_info.sin_addr.s_addr = inet_addr(REMOTE_IP_ADDR);     //ѡ����Ҫ���͵�Զ��IP��ַ

    while(1)
    {
        if(xSemaphoreTake(UDP_SendBuffer_Mutex,portMAX_DELAY) == pdTRUE)	//��ȡ������������
	    {
            /* ֡������ͳ�� */
            FrameNum++;     //��������һ
            LwIP_UDP_SendBuffer[302] = (FrameNum & 0xff00) >> 8;
            LwIP_UDP_SendBuffer[303] = (FrameNum & 0x00ff);

            /* CRCУ�� */

            //����sendto�������ͣ�UDP��Ҫʹ��sendto����
            SendNum = sendto(Socket_fd,                     //Socket�ļ�������
                            LwIP_UDP_SendBuffer,            //���͵�����
                            sizeof(LwIP_UDP_SendBuffer),    //���͵����ݳ���
                            0,                              //���͵ı�־
                            (struct sockaddr *)&Local_info,
                            sizeof(Local_info));
            UNUSED(SendNum);
#ifdef PRINTF_DEBUG             
            if(SendNum != -1) printf("Send Complete!\r\n");
#endif
            /* ͨ��״̬��Ϣ������� */
            memset(LwIP_UDP_SendBuffer + 290,0,12);

            xSemaphoreGive(UDP_SendBuffer_Mutex);									//�ͷ��ź���������
        }
        vTaskDelay(10);
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
    int RecvNum = 0;

    //��֡ͷ֡β��Ϣ���뻺������
    LwIP_UDP_SendBuffer[0] = FRAMEHEADER1;
    LwIP_UDP_SendBuffer[1] = FRAMEHEADER2;
    LwIP_UDP_SendBuffer[305] = FRAMEEND;

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
        memset(LwIP_UDP_RecvBuffer,0,sizeof(LwIP_UDP_RecvBuffer));

        RecvNum = recv(Socket_fd,LwIP_UDP_RecvBuffer,sizeof(LwIP_UDP_RecvBuffer),0);
        if(RecvNum != -1) printf("%s\r\n",LwIP_UDP_RecvBuffer);

        vTaskDelay(1);  //�����л��߳�
    }
}

