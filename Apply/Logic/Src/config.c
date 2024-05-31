#include "config.h"


/* 指示灯 */
tagGPIO_T LED[] =
{
    [0]=
	{ 
		.tGPIOInit.Pin 		= GPIO_PIN_10,				/* GPIO引脚 */
		.tGPIOInit.Mode 	= GPIO_MODE_OUTPUT_PP,		/* GPIO模式 */
		.tGPIOInit.Pull 	= GPIO_NOPULL,				/* GPIO上下拉设置，是否需要上下拉看硬件 */
		.tGPIOInit.Speed 	= GPIO_SPEED_FREQ_HIGH,		/* GPIO速度 */
		.tGPIOPort 			= GPIOF,					/* GPIO分组 */
	},
};

/* 调试串口 */
tagUART_T Uart1 = 
{
	//串口工作模式配置
	.tUARTHandle.Instance 						= USART1,					/* STM32 串口设备 */
	.tUARTHandle.Init.BaudRate   				= 9600,						/* 串口波特率 */
	.tUARTHandle.Init.WordLength 				= UART_WORDLENGTH_8B,		/* 数据位长度 */
	.tUARTHandle.Init.StopBits   				= UART_STOPBITS_1,			/* 停止位长度 */
	.tUARTHandle.Init.Parity     				= UART_PARITY_NONE,			/* 校验位设置 */
	.tUARTHandle.Init.HwFlowCtl  				= UART_HWCONTROL_NONE,		/* 硬件流控设置 */
	.tUARTHandle.Init.Mode       				= UART_MODE_TX_RX,			/* 串口模式 */
	.tUARTHandle.Init.OverSampling 				= UART_OVERSAMPLING_16,		/* 过采样 */
	
	.ucPriority									= 3,						/* 中断优先级 */
	.ucSubPriority								= 3,						/* 中断子优先级 */
	
	//串口DMA接收参数配置
	.tUartDMA.bRxEnable							= false,						/* DMA接收使能 */
	
	//串口DMA发送参数配置
	.tUartDMA.bTxEnable							= false,						/* DMA发送使能 */

	//串口GPIO配置
	.tGPIO[0].tGPIOInit.Pin 					= GPIO_PIN_9,				/* GPIO引脚 */
	.tGPIO[0].tGPIOInit.Mode 					= GPIO_MODE_AF_PP,			/* GPIO模式 */
	.tGPIO[0].tGPIOInit.Pull 					= GPIO_NOPULL,				/* GPIO上下拉设置，是否需要上下拉看硬件 */
	.tGPIO[0].tGPIOInit.Speed 					= GPIO_SPEED_FREQ_HIGH,		/* GPIO速度 */	
	.tGPIO[0].tGPIOPort 						= GPIOA,					/* GPIO分组 */
#ifdef STM32F1_SGA_ENABLE
	.tGPIO[0].ucAFMode							= NO_REMAP,					/* GPIO重映射 */
#endif
#ifdef STM32F4_SGA_ENABLE
	.tGPIO[0].tGPIOInit.Alternate				= GPIO_AF7_USART1,
#endif

	.tGPIO[1].tGPIOInit.Pin 					= GPIO_PIN_10,				/* GPIO引脚 */
	.tGPIO[1].tGPIOInit.Pull 					= GPIO_NOPULL,				/* GPIO上下拉设置，是否需要上下拉看硬件 */
	.tGPIO[1].tGPIOInit.Speed 					= GPIO_SPEED_FREQ_HIGH,		/* GPIO速度 */	
	.tGPIO[1].tGPIOPort 						= GPIOA,					/* GPIO分组 */
#ifdef STM32F1_SGA_ENABLE	
	.tGPIO[1].tGPIOInit.Mode 					= GPIO_MODE_INPUT,			/* F4系列需要设置为输入模式 */
	.tGPIO[1].ucAFMode							= NO_REMAP,					/* GPIO重映射 */
#endif
#ifdef STM32F4_SGA_ENABLE
	.tGPIO[1].tGPIOInit.Mode 					= GPIO_MODE_AF_PP,			/* F4系列需要设置为复用推挽 */
	.tGPIO[1].tGPIOInit.Alternate				= GPIO_AF7_USART1,
#endif
};

/* RS232外接串口 */
tagUART_T Uart3 = 
{
	//串口工作模式配置
	.tUARTHandle.Instance 						= USART3,					/* STM32 串口设备 */
	.tUARTHandle.Init.BaudRate   				= 115200,					/* 串口波特率 */
	.tUARTHandle.Init.WordLength 				= UART_WORDLENGTH_8B,		/* 数据位长度 */
	.tUARTHandle.Init.StopBits   				= UART_STOPBITS_1,			/* 停止位长度 */
	.tUARTHandle.Init.Parity     				= UART_PARITY_NONE,			/* 校验位设置 */
	.tUARTHandle.Init.HwFlowCtl  				= UART_HWCONTROL_NONE,		/* 硬件流控设置 */
	.tUARTHandle.Init.Mode       				= UART_MODE_TX_RX,			/* 串口模式 */
	.tUARTHandle.Init.OverSampling 				= UART_OVERSAMPLING_16,		/* 过采样 */
	
	.ucPriority									= 3,						/* 中断优先级 */
	.ucSubPriority								= 3,						/* 中断子优先级 */
	
	//串口DMA接收参数配置
	.tUartDMA.bRxEnable							= false,						/* DMA接收使能 */
	
	//串口DMA发送参数配置
	.tUartDMA.bTxEnable							= false,						/* DMA发送使能 */

	//串口GPIO配置
	.tGPIO[0].tGPIOInit.Pin 					= GPIO_PIN_10,				/* GPIO引脚 */
	.tGPIO[0].tGPIOInit.Mode 					= GPIO_MODE_AF_PP,			/* GPIO模式 */
	.tGPIO[0].tGPIOInit.Pull 					= GPIO_NOPULL,				/* GPIO上下拉设置，是否需要上下拉看硬件 */
	.tGPIO[0].tGPIOInit.Speed 					= GPIO_SPEED_FREQ_HIGH,		/* GPIO速度 */	
	.tGPIO[0].tGPIOPort 						= GPIOB,					/* GPIO分组 */
#ifdef STM32F1_SGA_ENABLE
	.tGPIO[0].ucAFMode							= NO_REMAP,					/* GPIO重映射 */
#endif
#ifdef STM32F4_SGA_ENABLE
	.tGPIO[0].tGPIOInit.Alternate				= GPIO_AF7_USART3,
#endif

	.tGPIO[1].tGPIOInit.Pin 					= GPIO_PIN_11,				/* GPIO引脚 */
	.tGPIO[1].tGPIOInit.Pull 					= GPIO_NOPULL,				/* GPIO上下拉设置，是否需要上下拉看硬件 */
	.tGPIO[1].tGPIOInit.Speed 					= GPIO_SPEED_FREQ_HIGH,		/* GPIO速度 */	
	.tGPIO[1].tGPIOPort 						= GPIOB,					/* GPIO分组 */
#ifdef STM32F1_SGA_ENABLE	
	.tGPIO[1].tGPIOInit.Mode 					= GPIO_MODE_INPUT,			/* F4系列需要设置为输入模式 */
	.tGPIO[1].ucAFMode							= NO_REMAP,					/* GPIO重映射 */
#endif
#ifdef STM32F4_SGA_ENABLE
	.tGPIO[1].tGPIOInit.Mode 					= GPIO_MODE_AF_PP,			/* F4系列需要设置为复用推挽 */
	.tGPIO[1].tGPIOInit.Alternate				= GPIO_AF7_USART3,
#endif
};

/* 串口屏 */
tagUART_T Uart5 = 
{
	//串口工作模式配置
	.tUARTHandle.Instance 						= UART5,					/* STM32 串口设备 */
	.tUARTHandle.Init.BaudRate   				= 115200,					/* 串口波特率 */
	.tUARTHandle.Init.WordLength 				= UART_WORDLENGTH_8B,		/* 数据位长度 */
	.tUARTHandle.Init.StopBits   				= UART_STOPBITS_1,			/* 停止位长度 */
	.tUARTHandle.Init.Parity     				= UART_PARITY_NONE,			/* 校验位设置 */
	.tUARTHandle.Init.HwFlowCtl  				= UART_HWCONTROL_NONE,		/* 硬件流控设置 */
	.tUARTHandle.Init.Mode       				= UART_MODE_TX_RX,			/* 串口模式 */
	.tUARTHandle.Init.OverSampling 				= UART_OVERSAMPLING_16,		/* 过采样 */

#if defined (STM32L4_SGA_ENABLE)
	.tUARTHandle.Init.OneBitSampling 			= UART_ONE_BIT_SAMPLE_DISABLE,
	.tUARTHandle.AdvancedInit.AdvFeatureInit 	= UART_ADVFEATURE_NO_INIT,
#endif
	
	.ucPriority									= 2,						/* 中断优先级 */
	.ucSubPriority								= 3,						/* 中断子优先级 */
	
	.tUartDMA.bRxEnable							= true,						/* DMA接收使能 */
  	.tUartDMA.bTxEnable							= true,						/* DMA发送使能 */

	//串口GPIO配置
	.tGPIO[0].tGPIOInit.Pin 					= GPIO_PIN_12,				/* GPIO引脚 */
	.tGPIO[0].tGPIOInit.Mode 					= GPIO_MODE_AF_PP,			/* GPIO模式 */
	.tGPIO[0].tGPIOInit.Pull 					= GPIO_NOPULL,				/* GPIO上下拉设置，是否需要上下拉看硬件 */
	.tGPIO[0].tGPIOInit.Speed 					= GPIO_SPEED_FREQ_HIGH,		/* GPIO速度 */	
	.tGPIO[0].tGPIOPort 						= GPIOC,					/* GPIO分组 */
#ifdef STM32F1_SGA_ENABLE
	.tGPIO[0].ucAFMode							= NO_REMAP,					/* GPIO重映射 */
#endif
#ifdef STM32F4_SGA_ENABLE
	.tGPIO[0].tGPIOInit.Alternate				= GPIO_AF8_UART5,
#endif

	.tGPIO[1].tGPIOInit.Pin 					= GPIO_PIN_2,				/* GPIO引脚 */
	.tGPIO[1].tGPIOInit.Pull 					= GPIO_NOPULL,				/* GPIO上下拉设置，是否需要上下拉看硬件 */
	.tGPIO[1].tGPIOInit.Speed 					= GPIO_SPEED_FREQ_HIGH,		/* GPIO速度 */	
	.tGPIO[1].tGPIOPort 						= GPIOD,					/* GPIO分组 */
#ifdef STM32F1_SGA_ENABLE	
	.tGPIO[1].tGPIOInit.Mode 					= GPIO_MODE_INPUT,			/* F4系列需要设置为输入模式 */
	.tGPIO[1].ucAFMode							= NO_REMAP,					/* GPIO重映射 */
#endif
#ifdef STM32F4_SGA_ENABLE
	.tGPIO[1].tGPIOInit.Mode 					= GPIO_MODE_AF_PP,			/* F4系列需要设置为复用推挽 */
	.tGPIO[1].tGPIOInit.Alternate				= GPIO_AF8_UART5,
#endif
};

/* 电源检测 */
tagADC_T PowerADC[] = 
{
	[0]=
	{ 
		.tGPIO.tGPIOInit.Pin 					= GPIO_PIN_3,					/* GPIO引脚 */
		.tGPIO.tGPIOInit.Mode 					= GPIO_MODE_ANALOG,				/* GPIO模式 */
		.tGPIO.tGPIOInit.Pull 					= GPIO_NOPULL,					/* GPIO上下拉设置，是否需要上下拉看硬件 */
		.tGPIO.tGPIOInit.Speed 					= GPIO_SPEED_FREQ_HIGH,			/* GPIO速度 */	
		.tGPIO.tGPIOPort 						= GPIOA,						/* GPIO分组 */
		
		.tADCHandle.Instance					= ADC1,
#ifdef STM32F4_SGA_ENABLE
		.tADCHandle.Init.ClockPrescaler			= ADC_CLOCK_SYNC_PCLK_DIV4,		/* 时钟配置 */
#endif
		.tADCHandle.Init.DataAlign				= ADC_DATAALIGN_RIGHT,			/* 右对齐 */
		.tADCHandle.Init.ScanConvMode			= DISABLE,						/* 非扫描模式 */
		.tADCHandle.Init.ContinuousConvMode		= DISABLE,						/* 关闭连续转换 */
		.tADCHandle.Init.NbrOfConversion		= 1,							/* 1个转换在规则序列中 也就是只转换规则序列1 */ 
		.tADCHandle.Init.DiscontinuousConvMode	= DISABLE,						/* 禁止不连续采样模式 */
		.tADCHandle.Init.NbrOfDiscConversion	= 0,							/* 不连续采样通道数为0 */
		.tADCHandle.Init.ExternalTrigConv		= ADC_SOFTWARE_START,			/* 软件触发 */
		
		.tADCChannel.Channel					= ADC_CHANNEL_3,				/* 通道 */
		.tADCChannel.Rank						= 1,							/* 第1个序列，序列1 */
#ifdef STM32F1_SGA_ENABLE
		.tADCChannel.SamplingTime				= ADC_SAMPLETIME_239CYCLES_5,	/* 采样时间 */
#endif
#ifdef STM32F4_SGA_ENABLE
		.tADCChannel.SamplingTime				= ADC_SAMPLETIME_3CYCLES,		/* 采样时间 */
#endif
		.ucAverageNum							= 10,							/* 均值滤波均值数 */
	},
};

/* CAN */
tagCAN_T CAN =
{
	/* CAN配置 */
	.tCANHandle.Instance 								= CAN1,
	.tCANHandle.Init.Prescaler 							= 7,							/* 分频系数 */
	.tCANHandle.Init.Mode 								= CAN_MODE_NORMAL,				/* 模式设置 */
	.tCANHandle.Init.SyncJumpWidth 						= CAN_SJW_1TQ,					/* 重新同步跳跃宽度 */
	.tCANHandle.Init.TimeSeg1 							= CAN_BS1_4TQ,					/* Tbs1 */
	.tCANHandle.Init.TimeSeg2 							= CAN_BS2_1TQ,					/* Tbs2 */
	.tCANHandle.Init.TimeTriggeredMode 					= DISABLE,						/* 非时间触发通信模式 */
	.tCANHandle.Init.AutoBusOff 						= DISABLE,						/* 软件自动离线管理 */
	.tCANHandle.Init.AutoRetransmission 				= DISABLE,						/* 睡眠模式通过软件唤醒 */
	.tCANHandle.Init.AutoWakeUp 						= DISABLE,						/* 禁止报文自动传送 */
	.tCANHandle.Init.ReceiveFifoLocked 					= DISABLE,						/* 报文不锁定,新的覆盖旧的 */
	.tCANHandle.Init.TransmitFifoPriority 				= DISABLE,						/* 优先级由报文标识符决定 */

	/* CAN过滤器配置 */
	.ucCANFilterNum											= 3,							/* 需要配置过滤器个数 */

	.tCANFilter[0].FilterBank 								= 0,							/* 过滤器0 */
	.tCANFilter[0].FilterMode 								= CAN_FILTERMODE_IDLIST,		/* 过滤器模式 */
	.tCANFilter[0].FilterScale 							    = CAN_FILTERSCALE_16BIT,		/* 过滤器位数 */
	.tCANFilter[0].FilterIdHigh 							= 0x41 << 5,		//0x41通过
	.tCANFilter[0].FilterIdLow 							    = 0x42 << 5,		//0x42通过
	.tCANFilter[0].FilterMaskIdHigh 						= 0xffff,			//掩码全通过
	.tCANFilter[0].FilterMaskIdLow 						    = 0|0x02,			//只收数据帧
	.tCANFilter[0].FilterFIFOAssignment 					= CAN_RX_FIFO0,					/* 过滤器关联到FIFO0 */
	.tCANFilter[0].FilterActivation 						= ENABLE,						/* 激活过滤器0 */
	.tCANFilter[0].SlaveStartFilterBank 					= 14,							/* 从CAN过滤器起始 */

	.tCANFilter[1].FilterBank 								= 1,							/* 过滤器1 */
	.tCANFilter[1].FilterMode 								= CAN_FILTERMODE_IDLIST,		/* 过滤器模式 */
	.tCANFilter[1].FilterScale 							    = CAN_FILTERSCALE_16BIT,		/* 过滤器位数 */
	.tCANFilter[1].FilterIdHigh 							= 0x43 << 5,		//0x43通过
	.tCANFilter[1].FilterIdLow 							    = 0x0000,
	.tCANFilter[1].FilterMaskIdHigh 						= 0xffff,
	.tCANFilter[1].FilterMaskIdLow 						    = 0|0x02,			//只收数据帧
	.tCANFilter[1].FilterFIFOAssignment 					= CAN_RX_FIFO1,					/* 过滤器关联到FIFO0 */
	.tCANFilter[1].FilterActivation 						= ENABLE,						/* 激活过滤器0 */
	.tCANFilter[1].SlaveStartFilterBank 					= 14,							/* 从CAN过滤器起始 */

	.tCANFilter[2].FilterBank 								= 2,							/* 过滤器2 */
	.tCANFilter[2].FilterMode 								= CAN_FILTERMODE_IDMASK,		/* 过滤器模式 */
	.tCANFilter[2].FilterScale 							    = CAN_FILTERSCALE_32BIT,		/* 过滤器位数 */
	.tCANFilter[2].FilterIdHigh 							= 0x44 << 5,
	.tCANFilter[2].FilterIdLow 							    = 0x00,
	.tCANFilter[2].FilterMaskIdHigh 						= 0x44 << 5,		//0x44为掩码
	.tCANFilter[2].FilterMaskIdLow 						    = 0|0x02,			//只收数据帧
	.tCANFilter[2].FilterFIFOAssignment 					= CAN_RX_FIFO1,					/* 过滤器关联到FIFO1 */
	.tCANFilter[2].FilterActivation 						= ENABLE,						/* 激活过滤器0 */
	.tCANFilter[2].SlaveStartFilterBank 					= 14,							/* 从CAN过滤器起始 */

	/* CAN TX配置 */
	.tCANTxHeader.StdId 								= 0x55,							/* 标准标识符 */
	.tCANTxHeader.ExtId 								= 0x00,							/* 拓展标识符（29位） */
	.tCANTxHeader.IDE 									= CAN_ID_STD,    				/* 使用标准帧 */
	.tCANTxHeader.RTR 									= CAN_RTR_DATA,					/* 数据帧 */
  
	/* CAN 发送优先级 */
	.ucTxPriority										= 9,							/* 发送主优先级 */
	.ucTxSubPriority									= 1,							/* 发送次优先级 */

	/* CAN 接收优先级 */
	.ucRxPriority										= 0,							/* 接收主优先级 */
	.ucRxSubPriority									= 1,							/* 接收次优先级 */
	/* CAN GPIO配置 */
	.tGPIO[0].tGPIOInit.Pin 							= GPIO_PIN_12,					/* GPIO引脚 */
	.tGPIO[0].tGPIOInit.Mode 							= GPIO_MODE_AF_PP,				/* GPIO模式 */
	.tGPIO[0].tGPIOInit.Pull 							= GPIO_PULLUP,					/* GPIO上下拉设置，是否需要上下拉看硬件 */
	.tGPIO[0].tGPIOInit.Speed 							= GPIO_SPEED_FREQ_HIGH,			/* GPIO速度 */	
	.tGPIO[0].tGPIOPort 								= GPIOA,						/* GPIO分组 */			

	.tGPIO[1].tGPIOInit.Pin 							= GPIO_PIN_11,					/* GPIO引脚 */
#ifdef STM32F1_SGA_ENABLE
	.tGPIO[1].tGPIOInit.Mode 							= GPIO_MODE_AF_INPUT,			/* GPIO模式 */
#endif
#ifdef STM32F4_SGA_ENABLE
	.tGPIO[1].tGPIOInit.Mode 							= GPIO_MODE_AF_PP,				/* GPIO模式 */
#endif
	.tGPIO[1].tGPIOInit.Pull 							= GPIO_PULLUP,					/* GPIO上下拉设置，是否需要上下拉看硬件 */
	.tGPIO[1].tGPIOInit.Speed 							= GPIO_SPEED_FREQ_HIGH,			/* GPIO速度 */	
	.tGPIO[1].tGPIOPort 								= GPIOA,						/* GPIO分组 */			
};

/* DS1337片外RTC */
tagDS3231_T DS1337 = 
{
	/* SCL线 */
	.tIICSoft.tIICSoft[0].tGPIOInit.Pin 	= GPIO_PIN_6,				/* GPIO引脚 */
	.tIICSoft.tIICSoft[0].tGPIOInit.Mode 	= GPIO_MODE_OUTPUT_PP,		/* GPIO模式 */
	.tIICSoft.tIICSoft[0].tGPIOInit.Pull 	= GPIO_NOPULL,				/* GPIO上下拉设置，是否需要上下拉看硬件 */
	.tIICSoft.tIICSoft[0].tGPIOInit.Speed 	= GPIO_SPEED_FREQ_HIGH,		/* GPIO速度 */	
	.tIICSoft.tIICSoft[0].tGPIOPort 		= GPIOF,					/* GPIO分组 */
	
	/* SDA线 */
	.tIICSoft.tIICSoft[1].tGPIOInit.Pin 	= GPIO_PIN_7,				/* GPIO引脚 */
	.tIICSoft.tIICSoft[1].tGPIOInit.Mode 	= GPIO_MODE_INPUT,			/* GPIO模式 */
	.tIICSoft.tIICSoft[1].tGPIOInit.Pull 	= GPIO_NOPULL,				/* GPIO上下拉设置，是否需要上下拉看硬件 */
	.tIICSoft.tIICSoft[1].tGPIOInit.Speed 	= GPIO_SPEED_FREQ_HIGH,		/* GPIO速度 */	
	.tIICSoft.tIICSoft[1].tGPIOPort 		= GPIOF,					/* GPIO分组 */
};

/* AT24C02 */
tagAT24CXX_T EEPROM = 
{
	/* SCL线 */
	.tIIC.tIICSoft[0].tGPIOInit.Pin 		= GPIO_PIN_4,				/* GPIO引脚 */
	.tIIC.tIICSoft[0].tGPIOInit.Mode		= GPIO_MODE_OUTPUT_PP,		/* GPIO模式 */
	.tIIC.tIICSoft[0].tGPIOInit.Pull		= GPIO_NOPULL,				/* GPIO上下拉设置，是否需要上下拉看硬件 */
	.tIIC.tIICSoft[0].tGPIOInit.Speed		= GPIO_SPEED_FREQ_HIGH,		/* GPIO速度 */	
	.tIIC.tIICSoft[0].tGPIOPort 			= GPIOE,					/* GPIO分组 */
	
	/* SDA线 */
	.tIIC.tIICSoft[1].tGPIOInit.Pin 		= GPIO_PIN_3,				/* GPIO引脚 */
	.tIIC.tIICSoft[1].tGPIOInit.Mode		= GPIO_MODE_INPUT,			/* GPIO模式 */
	.tIIC.tIICSoft[1].tGPIOInit.Pull		= GPIO_NOPULL,				/* GPIO上下拉设置，是否需要上下拉看硬件 */
	.tIIC.tIICSoft[1].tGPIOInit.Speed		= GPIO_SPEED_FREQ_HIGH,		/* GPIO速度 */	
	.tIIC.tIICSoft[1].tGPIOPort 			= GPIOE,					/* GPIO分组 */
};

/* TF卡使用FatFs保存日志 */
tagFATFS_T TFCard = 
{
	.tSPI.tSPIHandle.Instance 				= SPI1,
	.tSPI.tSPIHandle.Init.Mode 				= SPI_MODE_MASTER,
	.tSPI.tSPIHandle.Init.Direction 		= SPI_DIRECTION_2LINES,
	.tSPI.tSPIHandle.Init.DataSize 			= SPI_DATASIZE_8BIT,
	.tSPI.tSPIHandle.Init.CLKPolarity 		= SPI_POLARITY_HIGH,
	.tSPI.tSPIHandle.Init.CLKPhase 			= SPI_PHASE_2EDGE,
	.tSPI.tSPIHandle.Init.NSS 				= SPI_NSS_SOFT,
	.tSPI.tSPIHandle.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256,
	.tSPI.tSPIHandle.Init.FirstBit 			= SPI_FIRSTBIT_MSB,
	.tSPI.tSPIHandle.Init.TIMode 			= SPI_TIMODE_DISABLE,
	.tSPI.tSPIHandle.Init.CRCCalculation 	= SPI_CRCCALCULATION_DISABLE,
	.tSPI.tSPIHandle.Init.CRCPolynomial 	= 7,	
#if defined (STM32L4_SGA_ENABLE)
	.tSPI.tSPIHandle.Init.CRCLength 		= SPI_CRC_LENGTH_DATASIZE,
	.tSPI.tSPIHandle.Init.NSSPMode 			= SPI_NSS_PULSE_ENABLE,	
#endif
	
	.tSPI.tGPIO[0].tGPIOInit.Pin 			= GPIO_PIN_3,				/* GPIO引脚 */
	.tSPI.tGPIO[0].tGPIOInit.Mode 			= GPIO_MODE_AF_PP,			/* GPIO模式 */
	.tSPI.tGPIO[0].tGPIOInit.Pull 			= GPIO_NOPULL,				/* GPIO上下拉设置，是否需要上下拉看硬件 */
	.tSPI.tGPIO[0].tGPIOInit.Speed 			= GPIO_SPEED_FREQ_HIGH,		/* GPIO速度 */	
	.tSPI.tGPIO[0].tGPIOPort 				= GPIOB,					/* GPIO分组 */
	
	.tSPI.tGPIO[1].tGPIOInit.Pin 			= GPIO_PIN_4,				/* GPIO引脚 */
	.tSPI.tGPIO[1].tGPIOInit.Mode 			= GPIO_MODE_AF_PP,			/* GPIO模式 */
	.tSPI.tGPIO[1].tGPIOInit.Pull 			= GPIO_NOPULL,				/* GPIO上下拉设置，是否需要上下拉看硬件 */
	.tSPI.tGPIO[1].tGPIOInit.Speed 			= GPIO_SPEED_FREQ_HIGH,		/* GPIO速度 */	
	.tSPI.tGPIO[1].tGPIOPort 				= GPIOB,					/* GPIO分组 */
	
	.tSPI.tGPIO[2].tGPIOInit.Pin 			= GPIO_PIN_5,				/* GPIO引脚 */
	.tSPI.tGPIO[2].tGPIOInit.Mode 			= GPIO_MODE_AF_PP,			/* GPIO模式 */
	.tSPI.tGPIO[2].tGPIOInit.Pull 			= GPIO_NOPULL,				/* GPIO上下拉设置，是否需要上下拉看硬件 */
	.tSPI.tGPIO[2].tGPIOInit.Speed 			= GPIO_SPEED_FREQ_HIGH,		/* GPIO速度 */	
	.tSPI.tGPIO[2].tGPIOPort 				= GPIOB,					/* GPIO分组 */
};

/* 板卡控制引脚 */
tagGPIO_T Control[] =
{
	
	[0]=
	{ 
		.tGPIOInit.Pin 		= GPIO_PIN_15,				/* GPIO引脚 */
		.tGPIOInit.Mode 	= GPIO_MODE_OUTPUT_PP,		/* GPIO模式 */
		.tGPIOInit.Pull 	= GPIO_NOPULL,				/* GPIO上下拉设置，是否需要上下拉看硬件 */
		.tGPIOInit.Speed 	= GPIO_SPEED_FREQ_HIGH,		/* GPIO速度 */
		.tGPIOPort 			= GPIOA,					/* GPIO分组 */
	},
    [1]=
	{ 
		.tGPIOInit.Pin 		= GPIO_PIN_10,				/* GPIO引脚 */
		.tGPIOInit.Mode 	= GPIO_MODE_OUTPUT_PP,		/* GPIO模式 */
		.tGPIOInit.Pull 	= GPIO_NOPULL,				/* GPIO上下拉设置，是否需要上下拉看硬件 */
		.tGPIOInit.Speed 	= GPIO_SPEED_FREQ_HIGH,		/* GPIO速度 */
		.tGPIOPort 			= GPIOC,					/* GPIO分组 */
	},
	[2]=
	{ 
		.tGPIOInit.Pin 		= GPIO_PIN_11,				/* GPIO引脚 */
		.tGPIOInit.Mode 	= GPIO_MODE_OUTPUT_PP,		/* GPIO模式 */
		.tGPIOInit.Pull 	= GPIO_NOPULL,				/* GPIO上下拉设置，是否需要上下拉看硬件 */
		.tGPIOInit.Speed 	= GPIO_SPEED_FREQ_HIGH,		/* GPIO速度 */
		.tGPIOPort 			= GPIOC,					/* GPIO分组 */
	},
	[3]=
	{ 
		.tGPIOInit.Pin 		= GPIO_PIN_0,				/* GPIO引脚 */
		.tGPIOInit.Mode 	= GPIO_MODE_OUTPUT_PP,		/* GPIO模式 */
		.tGPIOInit.Pull 	= GPIO_NOPULL,				/* GPIO上下拉设置，是否需要上下拉看硬件 */
		.tGPIOInit.Speed 	= GPIO_SPEED_FREQ_HIGH,		/* GPIO速度 */
		.tGPIOPort 			= GPIOD,					/* GPIO分组 */
	},
	[4]=
	{ 
		.tGPIOInit.Pin 		= GPIO_PIN_1,				/* GPIO引脚 */
		.tGPIOInit.Mode 	= GPIO_MODE_OUTPUT_PP,		/* GPIO模式 */
		.tGPIOInit.Pull 	= GPIO_NOPULL,				/* GPIO上下拉设置，是否需要上下拉看硬件 */
		.tGPIOInit.Speed 	= GPIO_SPEED_FREQ_HIGH,		/* GPIO速度 */
		.tGPIOPort 			= GPIOD,					/* GPIO分组 */
	},
	[5]=
	{ 
		.tGPIOInit.Pin 		= GPIO_PIN_4,				/* GPIO引脚 */
		.tGPIOInit.Mode 	= GPIO_MODE_OUTPUT_PP,		/* GPIO模式 */
		.tGPIOInit.Pull 	= GPIO_NOPULL,				/* GPIO上下拉设置，是否需要上下拉看硬件 */
		.tGPIOInit.Speed 	= GPIO_SPEED_FREQ_HIGH,		/* GPIO速度 */
		.tGPIOPort 			= GPIOD,					/* GPIO分组 */
	},
	[6]=
	{ 
		.tGPIOInit.Pin 		= GPIO_PIN_5,				/* GPIO引脚 */
		.tGPIOInit.Mode 	= GPIO_MODE_OUTPUT_PP,		/* GPIO模式 */
		.tGPIOInit.Pull 	= GPIO_NOPULL,				/* GPIO上下拉设置，是否需要上下拉看硬件 */
		.tGPIOInit.Speed 	= GPIO_SPEED_FREQ_HIGH,		/* GPIO速度 */
		.tGPIOPort 			= GPIOD,					/* GPIO分组 */
	},
	[7]=
	{ 
		.tGPIOInit.Pin 		= GPIO_PIN_6,				/* GPIO引脚 */
		.tGPIOInit.Mode 	= GPIO_MODE_OUTPUT_PP,		/* GPIO模式 */
		.tGPIOInit.Pull 	= GPIO_NOPULL,				/* GPIO上下拉设置，是否需要上下拉看硬件 */
		.tGPIOInit.Speed 	= GPIO_SPEED_FREQ_HIGH,		/* GPIO速度 */
		.tGPIOPort 			= GPIOD,					/* GPIO分组 */
	},
	[8]=
	{ 
		.tGPIOInit.Pin 		= GPIO_PIN_7,				/* GPIO引脚 */
		.tGPIOInit.Mode 	= GPIO_MODE_OUTPUT_PP,		/* GPIO模式 */
		.tGPIOInit.Pull 	= GPIO_NOPULL,				/* GPIO上下拉设置，是否需要上下拉看硬件 */
		.tGPIOInit.Speed 	= GPIO_SPEED_FREQ_HIGH,		/* GPIO速度 */
		.tGPIOPort 			= GPIOD,					/* GPIO分组 */
	},
	[9]=
	{ 
		.tGPIOInit.Pin 		= GPIO_PIN_9,				/* GPIO引脚 */
		.tGPIOInit.Mode 	= GPIO_MODE_OUTPUT_PP,		/* GPIO模式 */
		.tGPIOInit.Pull 	= GPIO_NOPULL,				/* GPIO上下拉设置，是否需要上下拉看硬件 */
		.tGPIOInit.Speed 	= GPIO_SPEED_FREQ_HIGH,		/* GPIO速度 */
		.tGPIOPort 			= GPIOG,					/* GPIO分组 */
	},
	[10]=
	{ 
		.tGPIOInit.Pin 		= GPIO_PIN_10,				/* GPIO引脚 */
		.tGPIOInit.Mode 	= GPIO_MODE_OUTPUT_PP,		/* GPIO模式 */
		.tGPIOInit.Pull 	= GPIO_NOPULL,				/* GPIO上下拉设置，是否需要上下拉看硬件 */
		.tGPIOInit.Speed 	= GPIO_SPEED_FREQ_HIGH,		/* GPIO速度 */
		.tGPIOPort 			= GPIOG,					/* GPIO分组 */
	},
	[11]=
	{ 
		.tGPIOInit.Pin 		= GPIO_PIN_12,				/* GPIO引脚 */
		.tGPIOInit.Mode 	= GPIO_MODE_OUTPUT_PP,		/* GPIO模式 */
		.tGPIOInit.Pull 	= GPIO_NOPULL,				/* GPIO上下拉设置，是否需要上下拉看硬件 */
		.tGPIOInit.Speed 	= GPIO_SPEED_FREQ_HIGH,		/* GPIO速度 */
		.tGPIOPort 			= GPIOG,					/* GPIO分组 */
	},

};

/* SYNC控制引脚 */
tagGPIO_T SYNC[] =
{
	[0]=
	{ 
		.tGPIOInit.Pin 		= GPIO_PIN_0,				/* GPIO引脚 */
		.tGPIOInit.Mode 	= GPIO_MODE_OUTPUT_PP,		/* GPIO模式 */
		.tGPIOInit.Pull 	= GPIO_NOPULL,				/* GPIO上下拉设置，是否需要上下拉看硬件 */
		.tGPIOInit.Speed 	= GPIO_SPEED_FREQ_HIGH,		/* GPIO速度 */
		.tGPIOPort 			= GPIOE,					/* GPIO分组 */
	},
};

