#include "task_lwipudp.h"
#include "lwip/sockets.h"
#include "lwip/api.h"


/* 远程IP地址 */
#define REMOTE_IP_ADDR          "192.168.1.101"     //根据实际情况更改

#define LWIP_UDP_RX_BUFSIZE         100                         /* 最大接收数据长度 */
#define LWIP_UDP_PORT               8080                        /* 连接的本地端口号 */
#define LWIP_SEND_THREAD_PRIO       ( tskIDLE_PRIORITY + 9 )    /* 发送数据线程优先级 */

#define FRAMEHEADER1                0X02                        /* 帧头1 */
#define FRAMEHEADER2                0X48                        /* 帧头2 */
#define FRAMEEND                    0X03                        /* 帧尾 */

/* UDP接收发送缓冲区 */
uint8_t LwIP_UDP_RecvBuffer[LWIP_UDP_RX_BUFSIZE] = {0};
// uint8_t LwIP_UDP_SendBuffer[] = "Test UDP\r\n";
uint8_t LwIP_UDP_SendBuffer[306] = {0};

/* UDP数据帧计数器 */
static uint16_t FrameNum = 0;

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
    int SendNum = 0;

    Local_info.sin_addr.s_addr = inet_addr(REMOTE_IP_ADDR);     //选定需要发送的远程IP地址

    while(1)
    {
        if(xSemaphoreTake(UDP_SendBuffer_Mutex,portMAX_DELAY) == pdTRUE)	//获取互斥量，上锁
	    {
            /* 帧计数器统计 */
            FrameNum++;     //计数器加一
            LwIP_UDP_SendBuffer[302] = (FrameNum & 0xff00) >> 8;
            LwIP_UDP_SendBuffer[303] = (FrameNum & 0x00ff);

            /* CRC校验 */

            //调用sendto函数发送，UDP需要使用sendto函数
            SendNum = sendto(Socket_fd,                     //Socket文件描述符
                            LwIP_UDP_SendBuffer,            //发送的数据
                            sizeof(LwIP_UDP_SendBuffer),    //发送的数据长度
                            0,                              //发送的标志
                            (struct sockaddr *)&Local_info,
                            sizeof(Local_info));
            UNUSED(SendNum);
#ifdef PRINTF_DEBUG             
            if(SendNum != -1) printf("Send Complete!\r\n");
#endif
            /* 通道状态信息部分清除 */
            memset(LwIP_UDP_SendBuffer + 290,0,12);

            xSemaphoreGive(UDP_SendBuffer_Mutex);									//释放信号量，解锁
        }
        vTaskDelay(10);
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
    int RecvNum = 0;

    //将帧头帧尾信息放入缓冲区中
    LwIP_UDP_SendBuffer[0] = FRAMEHEADER1;
    LwIP_UDP_SendBuffer[1] = FRAMEHEADER2;
    LwIP_UDP_SendBuffer[305] = FRAMEEND;

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
        memset(LwIP_UDP_RecvBuffer,0,sizeof(LwIP_UDP_RecvBuffer));

        RecvNum = recv(Socket_fd,LwIP_UDP_RecvBuffer,sizeof(LwIP_UDP_RecvBuffer),0);
        if(RecvNum != -1) printf("%s\r\n",LwIP_UDP_RecvBuffer);

        vTaskDelay(1);  //主动切换线程
    }
}

