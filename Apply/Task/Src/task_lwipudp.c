#include "task_lwipudp.h"
#include "lwip/sockets.h"
#include "lwip/api.h"


/* 远程IP地址 */
char RemoteIPAddrString[16] = "";                                     /* 存放远程IP地址字符串 */
char LocalIPAddrString[16] = "";                                      /* 存放本地IP地址字符串 */
uint16_t RemotePort = 4001;                                           /* 连接的远端端口号，默认4001 */

#define LWIP_UDP_RX_BUFSIZE         100                         /* 最大接收数据长度 */
#define LWIP_SEND_THREAD_PRIO       ( tskIDLE_PRIORITY + 9 )    /* 发送数据线程优先级 */

#define FRAMEHEADER1                0X02                        /* 帧头1 */
#define FRAMEHEADER2                0X48                        /* 帧头2 */
#define FRAMEEND                    0X03                        /* 帧尾 */

/* CRC-8 x^8 + x^2 + x^1 + 1 查找表 */
uint8_t CRC8_Table[256] = {
    0x00, 0x07, 0x0E, 0x09, 0x1C, 0x1B, 0x12, 0x15, 0x38, 0x3F, 0x36, 0x31, 0x24, 0x23, 0x2A, 0x2D,
    0x70, 0x77, 0x7E, 0x79, 0x6C, 0x6B, 0x62, 0x65, 0x48, 0x4F, 0x46, 0x41, 0x54, 0x53, 0x5A, 0x5D,
    0xE0, 0xE7, 0xEE, 0xE9, 0xFC, 0xFB, 0xF2, 0xF5, 0xD8, 0xDF, 0xD6, 0xD1, 0xC4, 0xC3, 0xCA, 0xCD,
    0x90, 0x97, 0x9E, 0x99, 0x8C, 0x8B, 0x82, 0x85, 0xA8, 0xAF, 0xA6, 0xA1, 0xB4, 0xB3, 0xBA, 0xBD,
    0xC7, 0xC0, 0xC9, 0xCE, 0xDB, 0xDC, 0xD5, 0xD2, 0xFF, 0xF8, 0xF1, 0xF6, 0xE3, 0xE4, 0xED, 0xEA,
    0xB7, 0xB0, 0xB9, 0xBE, 0xAB, 0xAC, 0xA5, 0xA2, 0x8F, 0x88, 0x81, 0x86, 0x93, 0x94, 0x9D, 0x9A,
    0x27, 0x20, 0x29, 0x2E, 0x3B, 0x3C, 0x35, 0x32, 0x1F, 0x18, 0x11, 0x16, 0x03, 0x04, 0x0D, 0x0A,
    0x57, 0x50, 0x59, 0x5E, 0x4B, 0x4C, 0x45, 0x42, 0x6F, 0x68, 0x61, 0x66, 0x73, 0x74, 0x7D, 0x7A,
    0x89, 0x8E, 0x87, 0x80, 0x95, 0x92, 0x9B, 0x9C, 0xB1, 0xB6, 0xBF, 0xB8, 0xAD, 0xAA, 0xA3, 0xA4,
    0xF9, 0xFE, 0xF7, 0xF0, 0xE5, 0xE2, 0xEB, 0xEC, 0xC1, 0xC6, 0xCF, 0xC8, 0xDD, 0xDA, 0xD3, 0xD4,
    0x69, 0x6E, 0x67, 0x60, 0x75, 0x72, 0x7B, 0x7C, 0x51, 0x56, 0x5F, 0x58, 0x4D, 0x4A, 0x43, 0x44,
    0x19, 0x1E, 0x17, 0x10, 0x05, 0x02, 0x0B, 0x0C, 0x21, 0x26, 0x2F, 0x28, 0x3D, 0x3A, 0x33, 0x34,
    0x4E, 0x49, 0x40, 0x47, 0x52, 0x55, 0x5C, 0x5B, 0x76, 0x71, 0x78, 0x7F, 0x6A, 0x6D, 0x64, 0x63,
    0x3E, 0x39, 0x30, 0x37, 0x22, 0x25, 0x2C, 0x2B, 0x06, 0x01, 0x08, 0x0F, 0x1A, 0x1D, 0x14, 0x13,
    0xAE, 0xA9, 0xA0, 0xA7, 0xB2, 0xB5, 0xBC, 0xBB, 0x96, 0x91, 0x98, 0x9F, 0x8A, 0x8D, 0x84, 0x83,
    0xDE, 0xD9, 0xD0, 0xD7, 0xC2, 0xC5, 0xCC, 0xCB, 0xE6, 0xE1, 0xE8, 0xEF, 0xFA, 0xFD, 0xF4, 0xF3
};

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
 * @brief CRC8校验函数-查表
 * @param p 数组地址
 * @param counter 计算长度
 */
static uint8_t S_CRC8_Table(uint8_t *p, uint16_t counter)  
{  
    uint8_t crc8 = 0;  
  
    for( ; counter > 0; counter--){  
        crc8 = CRC8_Table[crc8^*p];  
        p++;  
    }  
    return crc8;  
} 

/**
 * @brief UDP发送线程入口函数
 * @param pvParameters 传入参数
 */
static void S_LwIP_UDP_Send_Entrance(void *pvParameters)
{
    UNUSED(pvParameters);
    int SendNum = 0;

    sprintf(RemoteIPAddrString,"%d.%d.%d.%d",g_lwipdev.remoteip[0],g_lwipdev.remoteip[1],g_lwipdev.remoteip[2],g_lwipdev.remoteip[3]);
    Local_info.sin_addr.s_addr = inet_addr(RemoteIPAddrString);     //选定需要发送的远程IP地址

    while(1)
    {
        if(xSemaphoreTake(UDP_SendBuffer_Mutex,0) == pdTRUE)	//获取互斥量，上锁
	    {
            /* 帧计数器统计 */
            FrameNum++;     //计数器加一
            LwIP_UDP_SendBuffer[302] = (FrameNum & 0xff00) >> 8;
            LwIP_UDP_SendBuffer[303] = (FrameNum & 0x00ff);

            /* CRC校验 */
            LwIP_UDP_SendBuffer[304] = S_CRC8_Table(LwIP_UDP_SendBuffer,304);

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
    Local_info.sin_port = htons(RemotePort);            //端口号
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

