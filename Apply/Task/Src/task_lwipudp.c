#include "task_lwipudp.h"
#include "lwip/sockets.h"
#include "lwip/api.h"


/* 远程IP地址 */
#define REMOTE_IP_ADDR          "192.168.1.101"     //根据实际情况更改

#define LWIP_UDP_RX_BUFSIZE         200                         /* 最大接收数据长度 */
#define LWIP_UDP_PORT               8080                        /* 连接的本地端口号 */
#define LWIP_SEND_THREAD_PRIO       ( tskIDLE_PRIORITY + 3 )    /* 发送数据线程优先级 */

/* UDP接收发送缓冲区 */
uint8_t LwIP_UDP_RecvBuffer[LWIP_UDP_RX_BUFSIZE];
uint8_t LwIP_UDP_SendBuffer[] = "Test UDP\r\n";

/* Socket相关 */
struct sockaddr_in Local_info;      /* 本地Socket地址信息 */
socklen_t Socket_fd;                /* 定义一个Socket文件描述符 */

/**
 * @brief UDP发送线程入口函数
 * @param pvParameters 传入参数
 */
static void S_LwIP_UDP_Send_Entrance(void *pvParameters)
{
    UNUSED(pvParameters);
    uint16_t SendNum = 0;

    Local_info.sin_addr.s_addr = inet_addr(REMOTE_IP_ADDR);     //选定需要发送的远程IP地址

    while(1)
    {
        //调用sendto函数发送，UDP需要使用sendto函数
        SendNum = sendto(Socket_fd,                     //Socket文件描述符
                        LwIP_UDP_SendBuffer,            //发送的数据
                        sizeof(LwIP_UDP_SendBuffer),    //发送的数据长度
                        0,                              //发送的标志
                        (struct sockaddr *)&Local_info,
                        sizeof(Local_info));                             
        if(SendNum) printf("Send Complete!\r\n");

        vTaskDelay(1000);
    }
}

/**
 * @brief 创建UDP发送线程
 * @param Null
 */
static inline void S_LwIP_UDP_Create_Send_Thread(void)
{
    sys_thread_new("lwip_send_thread",S_LwIP_UDP_Send_Entrance,NULL,512,LWIP_SEND_THREAD_PRIO);
}

/**
 * @brief UDP处理函数，包含接收和发送
 * @param Null
 */
void Task_LwIP_UDP_Handle(void)
{
    //BaseType_t LwIP_Err;    //返回值判断
    uint16_t RecvNum = 0;

    //创建UDP发送线程
    S_LwIP_UDP_Create_Send_Thread();        

    //本地Socket地址信息配置
    memset(&Local_info,0,sizeof(Local_info));
    Local_info.sin_len = sizeof(Local_info);
    Local_info.sin_family = AF_INET;                    //IPv4地址
    Local_info.sin_port = htons(LWIP_UDP_PORT);         //端口号
    Local_info.sin_addr.s_addr = htons(INADDR_ANY);     //本地IP地址

    //建立Socket连接
    Socket_fd = socket(AF_INET,SOCK_DGRAM,0);           //创建UDP类型的Socket

    //建立Bind
    bind(Socket_fd,(struct sockaddr *)&Local_info,sizeof(struct sockaddr_in));

    while(1)
    {
        //UDP接收处理
        memset(&LwIP_UDP_RecvBuffer,0,sizeof(LwIP_UDP_RecvBuffer));

        RecvNum = recv(Socket_fd,LwIP_UDP_RecvBuffer,sizeof(LwIP_UDP_RecvBuffer),0);
        if(RecvNum) printf("%s\r\n",LwIP_UDP_RecvBuffer);

        vTaskDelay(1);  //主动切换线程
    }
}

