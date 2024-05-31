#include "config.h"


/* ָʾ�� */
tagGPIO_T LED[] =
{
    [0]=
	{ 
		.tGPIOInit.Pin 		= GPIO_PIN_10,				/* GPIO���� */
		.tGPIOInit.Mode 	= GPIO_MODE_OUTPUT_PP,		/* GPIOģʽ */
		.tGPIOInit.Pull 	= GPIO_NOPULL,				/* GPIO���������ã��Ƿ���Ҫ��������Ӳ�� */
		.tGPIOInit.Speed 	= GPIO_SPEED_FREQ_HIGH,		/* GPIO�ٶ� */
		.tGPIOPort 			= GPIOF,					/* GPIO���� */
	},
};

/* ���Դ��� */
tagUART_T Uart1 = 
{
	//���ڹ���ģʽ����
	.tUARTHandle.Instance 						= USART1,					/* STM32 �����豸 */
	.tUARTHandle.Init.BaudRate   				= 9600,						/* ���ڲ����� */
	.tUARTHandle.Init.WordLength 				= UART_WORDLENGTH_8B,		/* ����λ���� */
	.tUARTHandle.Init.StopBits   				= UART_STOPBITS_1,			/* ֹͣλ���� */
	.tUARTHandle.Init.Parity     				= UART_PARITY_NONE,			/* У��λ���� */
	.tUARTHandle.Init.HwFlowCtl  				= UART_HWCONTROL_NONE,		/* Ӳ���������� */
	.tUARTHandle.Init.Mode       				= UART_MODE_TX_RX,			/* ����ģʽ */
	.tUARTHandle.Init.OverSampling 				= UART_OVERSAMPLING_16,		/* ������ */
	
	.ucPriority									= 3,						/* �ж����ȼ� */
	.ucSubPriority								= 3,						/* �ж������ȼ� */
	
	//����DMA���ղ�������
	.tUartDMA.bRxEnable							= false,						/* DMA����ʹ�� */
	
	//����DMA���Ͳ�������
	.tUartDMA.bTxEnable							= false,						/* DMA����ʹ�� */

	//����GPIO����
	.tGPIO[0].tGPIOInit.Pin 					= GPIO_PIN_9,				/* GPIO���� */
	.tGPIO[0].tGPIOInit.Mode 					= GPIO_MODE_AF_PP,			/* GPIOģʽ */
	.tGPIO[0].tGPIOInit.Pull 					= GPIO_NOPULL,				/* GPIO���������ã��Ƿ���Ҫ��������Ӳ�� */
	.tGPIO[0].tGPIOInit.Speed 					= GPIO_SPEED_FREQ_HIGH,		/* GPIO�ٶ� */	
	.tGPIO[0].tGPIOPort 						= GPIOA,					/* GPIO���� */
#ifdef STM32F1_SGA_ENABLE
	.tGPIO[0].ucAFMode							= NO_REMAP,					/* GPIO��ӳ�� */
#endif
#ifdef STM32F4_SGA_ENABLE
	.tGPIO[0].tGPIOInit.Alternate				= GPIO_AF7_USART1,
#endif

	.tGPIO[1].tGPIOInit.Pin 					= GPIO_PIN_10,				/* GPIO���� */
	.tGPIO[1].tGPIOInit.Pull 					= GPIO_NOPULL,				/* GPIO���������ã��Ƿ���Ҫ��������Ӳ�� */
	.tGPIO[1].tGPIOInit.Speed 					= GPIO_SPEED_FREQ_HIGH,		/* GPIO�ٶ� */	
	.tGPIO[1].tGPIOPort 						= GPIOA,					/* GPIO���� */
#ifdef STM32F1_SGA_ENABLE	
	.tGPIO[1].tGPIOInit.Mode 					= GPIO_MODE_INPUT,			/* F4ϵ����Ҫ����Ϊ����ģʽ */
	.tGPIO[1].ucAFMode							= NO_REMAP,					/* GPIO��ӳ�� */
#endif
#ifdef STM32F4_SGA_ENABLE
	.tGPIO[1].tGPIOInit.Mode 					= GPIO_MODE_AF_PP,			/* F4ϵ����Ҫ����Ϊ�������� */
	.tGPIO[1].tGPIOInit.Alternate				= GPIO_AF7_USART1,
#endif
};

/* RS232��Ӵ��� */
tagUART_T Uart3 = 
{
	//���ڹ���ģʽ����
	.tUARTHandle.Instance 						= USART3,					/* STM32 �����豸 */
	.tUARTHandle.Init.BaudRate   				= 115200,					/* ���ڲ����� */
	.tUARTHandle.Init.WordLength 				= UART_WORDLENGTH_8B,		/* ����λ���� */
	.tUARTHandle.Init.StopBits   				= UART_STOPBITS_1,			/* ֹͣλ���� */
	.tUARTHandle.Init.Parity     				= UART_PARITY_NONE,			/* У��λ���� */
	.tUARTHandle.Init.HwFlowCtl  				= UART_HWCONTROL_NONE,		/* Ӳ���������� */
	.tUARTHandle.Init.Mode       				= UART_MODE_TX_RX,			/* ����ģʽ */
	.tUARTHandle.Init.OverSampling 				= UART_OVERSAMPLING_16,		/* ������ */
	
	.ucPriority									= 3,						/* �ж����ȼ� */
	.ucSubPriority								= 3,						/* �ж������ȼ� */
	
	//����DMA���ղ�������
	.tUartDMA.bRxEnable							= false,						/* DMA����ʹ�� */
	
	//����DMA���Ͳ�������
	.tUartDMA.bTxEnable							= false,						/* DMA����ʹ�� */

	//����GPIO����
	.tGPIO[0].tGPIOInit.Pin 					= GPIO_PIN_10,				/* GPIO���� */
	.tGPIO[0].tGPIOInit.Mode 					= GPIO_MODE_AF_PP,			/* GPIOģʽ */
	.tGPIO[0].tGPIOInit.Pull 					= GPIO_NOPULL,				/* GPIO���������ã��Ƿ���Ҫ��������Ӳ�� */
	.tGPIO[0].tGPIOInit.Speed 					= GPIO_SPEED_FREQ_HIGH,		/* GPIO�ٶ� */	
	.tGPIO[0].tGPIOPort 						= GPIOB,					/* GPIO���� */
#ifdef STM32F1_SGA_ENABLE
	.tGPIO[0].ucAFMode							= NO_REMAP,					/* GPIO��ӳ�� */
#endif
#ifdef STM32F4_SGA_ENABLE
	.tGPIO[0].tGPIOInit.Alternate				= GPIO_AF7_USART3,
#endif

	.tGPIO[1].tGPIOInit.Pin 					= GPIO_PIN_11,				/* GPIO���� */
	.tGPIO[1].tGPIOInit.Pull 					= GPIO_NOPULL,				/* GPIO���������ã��Ƿ���Ҫ��������Ӳ�� */
	.tGPIO[1].tGPIOInit.Speed 					= GPIO_SPEED_FREQ_HIGH,		/* GPIO�ٶ� */	
	.tGPIO[1].tGPIOPort 						= GPIOB,					/* GPIO���� */
#ifdef STM32F1_SGA_ENABLE	
	.tGPIO[1].tGPIOInit.Mode 					= GPIO_MODE_INPUT,			/* F4ϵ����Ҫ����Ϊ����ģʽ */
	.tGPIO[1].ucAFMode							= NO_REMAP,					/* GPIO��ӳ�� */
#endif
#ifdef STM32F4_SGA_ENABLE
	.tGPIO[1].tGPIOInit.Mode 					= GPIO_MODE_AF_PP,			/* F4ϵ����Ҫ����Ϊ�������� */
	.tGPIO[1].tGPIOInit.Alternate				= GPIO_AF7_USART3,
#endif
};

/* ������ */
tagUART_T Uart5 = 
{
	//���ڹ���ģʽ����
	.tUARTHandle.Instance 						= UART5,					/* STM32 �����豸 */
	.tUARTHandle.Init.BaudRate   				= 115200,					/* ���ڲ����� */
	.tUARTHandle.Init.WordLength 				= UART_WORDLENGTH_8B,		/* ����λ���� */
	.tUARTHandle.Init.StopBits   				= UART_STOPBITS_1,			/* ֹͣλ���� */
	.tUARTHandle.Init.Parity     				= UART_PARITY_NONE,			/* У��λ���� */
	.tUARTHandle.Init.HwFlowCtl  				= UART_HWCONTROL_NONE,		/* Ӳ���������� */
	.tUARTHandle.Init.Mode       				= UART_MODE_TX_RX,			/* ����ģʽ */
	.tUARTHandle.Init.OverSampling 				= UART_OVERSAMPLING_16,		/* ������ */

#if defined (STM32L4_SGA_ENABLE)
	.tUARTHandle.Init.OneBitSampling 			= UART_ONE_BIT_SAMPLE_DISABLE,
	.tUARTHandle.AdvancedInit.AdvFeatureInit 	= UART_ADVFEATURE_NO_INIT,
#endif
	
	.ucPriority									= 2,						/* �ж����ȼ� */
	.ucSubPriority								= 3,						/* �ж������ȼ� */
	
	.tUartDMA.bRxEnable							= true,						/* DMA����ʹ�� */
  	.tUartDMA.bTxEnable							= true,						/* DMA����ʹ�� */

	//����GPIO����
	.tGPIO[0].tGPIOInit.Pin 					= GPIO_PIN_12,				/* GPIO���� */
	.tGPIO[0].tGPIOInit.Mode 					= GPIO_MODE_AF_PP,			/* GPIOģʽ */
	.tGPIO[0].tGPIOInit.Pull 					= GPIO_NOPULL,				/* GPIO���������ã��Ƿ���Ҫ��������Ӳ�� */
	.tGPIO[0].tGPIOInit.Speed 					= GPIO_SPEED_FREQ_HIGH,		/* GPIO�ٶ� */	
	.tGPIO[0].tGPIOPort 						= GPIOC,					/* GPIO���� */
#ifdef STM32F1_SGA_ENABLE
	.tGPIO[0].ucAFMode							= NO_REMAP,					/* GPIO��ӳ�� */
#endif
#ifdef STM32F4_SGA_ENABLE
	.tGPIO[0].tGPIOInit.Alternate				= GPIO_AF8_UART5,
#endif

	.tGPIO[1].tGPIOInit.Pin 					= GPIO_PIN_2,				/* GPIO���� */
	.tGPIO[1].tGPIOInit.Pull 					= GPIO_NOPULL,				/* GPIO���������ã��Ƿ���Ҫ��������Ӳ�� */
	.tGPIO[1].tGPIOInit.Speed 					= GPIO_SPEED_FREQ_HIGH,		/* GPIO�ٶ� */	
	.tGPIO[1].tGPIOPort 						= GPIOD,					/* GPIO���� */
#ifdef STM32F1_SGA_ENABLE	
	.tGPIO[1].tGPIOInit.Mode 					= GPIO_MODE_INPUT,			/* F4ϵ����Ҫ����Ϊ����ģʽ */
	.tGPIO[1].ucAFMode							= NO_REMAP,					/* GPIO��ӳ�� */
#endif
#ifdef STM32F4_SGA_ENABLE
	.tGPIO[1].tGPIOInit.Mode 					= GPIO_MODE_AF_PP,			/* F4ϵ����Ҫ����Ϊ�������� */
	.tGPIO[1].tGPIOInit.Alternate				= GPIO_AF8_UART5,
#endif
};

/* ��Դ��� */
tagADC_T PowerADC[] = 
{
	[0]=
	{ 
		.tGPIO.tGPIOInit.Pin 					= GPIO_PIN_3,					/* GPIO���� */
		.tGPIO.tGPIOInit.Mode 					= GPIO_MODE_ANALOG,				/* GPIOģʽ */
		.tGPIO.tGPIOInit.Pull 					= GPIO_NOPULL,					/* GPIO���������ã��Ƿ���Ҫ��������Ӳ�� */
		.tGPIO.tGPIOInit.Speed 					= GPIO_SPEED_FREQ_HIGH,			/* GPIO�ٶ� */	
		.tGPIO.tGPIOPort 						= GPIOA,						/* GPIO���� */
		
		.tADCHandle.Instance					= ADC1,
#ifdef STM32F4_SGA_ENABLE
		.tADCHandle.Init.ClockPrescaler			= ADC_CLOCK_SYNC_PCLK_DIV4,		/* ʱ������ */
#endif
		.tADCHandle.Init.DataAlign				= ADC_DATAALIGN_RIGHT,			/* �Ҷ��� */
		.tADCHandle.Init.ScanConvMode			= DISABLE,						/* ��ɨ��ģʽ */
		.tADCHandle.Init.ContinuousConvMode		= DISABLE,						/* �ر�����ת�� */
		.tADCHandle.Init.NbrOfConversion		= 1,							/* 1��ת���ڹ��������� Ҳ����ֻת����������1 */ 
		.tADCHandle.Init.DiscontinuousConvMode	= DISABLE,						/* ��ֹ����������ģʽ */
		.tADCHandle.Init.NbrOfDiscConversion	= 0,							/* ����������ͨ����Ϊ0 */
		.tADCHandle.Init.ExternalTrigConv		= ADC_SOFTWARE_START,			/* ������� */
		
		.tADCChannel.Channel					= ADC_CHANNEL_3,				/* ͨ�� */
		.tADCChannel.Rank						= 1,							/* ��1�����У�����1 */
#ifdef STM32F1_SGA_ENABLE
		.tADCChannel.SamplingTime				= ADC_SAMPLETIME_239CYCLES_5,	/* ����ʱ�� */
#endif
#ifdef STM32F4_SGA_ENABLE
		.tADCChannel.SamplingTime				= ADC_SAMPLETIME_3CYCLES,		/* ����ʱ�� */
#endif
		.ucAverageNum							= 10,							/* ��ֵ�˲���ֵ�� */
	},
};

/* CAN */
tagCAN_T CAN =
{
	/* CAN���� */
	.tCANHandle.Instance 								= CAN1,
	.tCANHandle.Init.Prescaler 							= 7,							/* ��Ƶϵ�� */
	.tCANHandle.Init.Mode 								= CAN_MODE_NORMAL,				/* ģʽ���� */
	.tCANHandle.Init.SyncJumpWidth 						= CAN_SJW_1TQ,					/* ����ͬ����Ծ��� */
	.tCANHandle.Init.TimeSeg1 							= CAN_BS1_4TQ,					/* Tbs1 */
	.tCANHandle.Init.TimeSeg2 							= CAN_BS2_1TQ,					/* Tbs2 */
	.tCANHandle.Init.TimeTriggeredMode 					= DISABLE,						/* ��ʱ�䴥��ͨ��ģʽ */
	.tCANHandle.Init.AutoBusOff 						= DISABLE,						/* ����Զ����߹��� */
	.tCANHandle.Init.AutoRetransmission 				= DISABLE,						/* ˯��ģʽͨ��������� */
	.tCANHandle.Init.AutoWakeUp 						= DISABLE,						/* ��ֹ�����Զ����� */
	.tCANHandle.Init.ReceiveFifoLocked 					= DISABLE,						/* ���Ĳ�����,�µĸ��Ǿɵ� */
	.tCANHandle.Init.TransmitFifoPriority 				= DISABLE,						/* ���ȼ��ɱ��ı�ʶ������ */

	/* CAN���������� */
	.ucCANFilterNum											= 3,							/* ��Ҫ���ù��������� */

	.tCANFilter[0].FilterBank 								= 0,							/* ������0 */
	.tCANFilter[0].FilterMode 								= CAN_FILTERMODE_IDLIST,		/* ������ģʽ */
	.tCANFilter[0].FilterScale 							    = CAN_FILTERSCALE_16BIT,		/* ������λ�� */
	.tCANFilter[0].FilterIdHigh 							= 0x41 << 5,		//0x41ͨ��
	.tCANFilter[0].FilterIdLow 							    = 0x42 << 5,		//0x42ͨ��
	.tCANFilter[0].FilterMaskIdHigh 						= 0xffff,			//����ȫͨ��
	.tCANFilter[0].FilterMaskIdLow 						    = 0|0x02,			//ֻ������֡
	.tCANFilter[0].FilterFIFOAssignment 					= CAN_RX_FIFO0,					/* ������������FIFO0 */
	.tCANFilter[0].FilterActivation 						= ENABLE,						/* ���������0 */
	.tCANFilter[0].SlaveStartFilterBank 					= 14,							/* ��CAN��������ʼ */

	.tCANFilter[1].FilterBank 								= 1,							/* ������1 */
	.tCANFilter[1].FilterMode 								= CAN_FILTERMODE_IDLIST,		/* ������ģʽ */
	.tCANFilter[1].FilterScale 							    = CAN_FILTERSCALE_16BIT,		/* ������λ�� */
	.tCANFilter[1].FilterIdHigh 							= 0x43 << 5,		//0x43ͨ��
	.tCANFilter[1].FilterIdLow 							    = 0x0000,
	.tCANFilter[1].FilterMaskIdHigh 						= 0xffff,
	.tCANFilter[1].FilterMaskIdLow 						    = 0|0x02,			//ֻ������֡
	.tCANFilter[1].FilterFIFOAssignment 					= CAN_RX_FIFO1,					/* ������������FIFO0 */
	.tCANFilter[1].FilterActivation 						= ENABLE,						/* ���������0 */
	.tCANFilter[1].SlaveStartFilterBank 					= 14,							/* ��CAN��������ʼ */

	.tCANFilter[2].FilterBank 								= 2,							/* ������2 */
	.tCANFilter[2].FilterMode 								= CAN_FILTERMODE_IDMASK,		/* ������ģʽ */
	.tCANFilter[2].FilterScale 							    = CAN_FILTERSCALE_32BIT,		/* ������λ�� */
	.tCANFilter[2].FilterIdHigh 							= 0x44 << 5,
	.tCANFilter[2].FilterIdLow 							    = 0x00,
	.tCANFilter[2].FilterMaskIdHigh 						= 0x44 << 5,		//0x44Ϊ����
	.tCANFilter[2].FilterMaskIdLow 						    = 0|0x02,			//ֻ������֡
	.tCANFilter[2].FilterFIFOAssignment 					= CAN_RX_FIFO1,					/* ������������FIFO1 */
	.tCANFilter[2].FilterActivation 						= ENABLE,						/* ���������0 */
	.tCANFilter[2].SlaveStartFilterBank 					= 14,							/* ��CAN��������ʼ */

	/* CAN TX���� */
	.tCANTxHeader.StdId 								= 0x55,							/* ��׼��ʶ�� */
	.tCANTxHeader.ExtId 								= 0x00,							/* ��չ��ʶ����29λ�� */
	.tCANTxHeader.IDE 									= CAN_ID_STD,    				/* ʹ�ñ�׼֡ */
	.tCANTxHeader.RTR 									= CAN_RTR_DATA,					/* ����֡ */
  
	/* CAN �������ȼ� */
	.ucTxPriority										= 9,							/* ���������ȼ� */
	.ucTxSubPriority									= 1,							/* ���ʹ����ȼ� */

	/* CAN �������ȼ� */
	.ucRxPriority										= 0,							/* ���������ȼ� */
	.ucRxSubPriority									= 1,							/* ���մ����ȼ� */
	/* CAN GPIO���� */
	.tGPIO[0].tGPIOInit.Pin 							= GPIO_PIN_12,					/* GPIO���� */
	.tGPIO[0].tGPIOInit.Mode 							= GPIO_MODE_AF_PP,				/* GPIOģʽ */
	.tGPIO[0].tGPIOInit.Pull 							= GPIO_PULLUP,					/* GPIO���������ã��Ƿ���Ҫ��������Ӳ�� */
	.tGPIO[0].tGPIOInit.Speed 							= GPIO_SPEED_FREQ_HIGH,			/* GPIO�ٶ� */	
	.tGPIO[0].tGPIOPort 								= GPIOA,						/* GPIO���� */			

	.tGPIO[1].tGPIOInit.Pin 							= GPIO_PIN_11,					/* GPIO���� */
#ifdef STM32F1_SGA_ENABLE
	.tGPIO[1].tGPIOInit.Mode 							= GPIO_MODE_AF_INPUT,			/* GPIOģʽ */
#endif
#ifdef STM32F4_SGA_ENABLE
	.tGPIO[1].tGPIOInit.Mode 							= GPIO_MODE_AF_PP,				/* GPIOģʽ */
#endif
	.tGPIO[1].tGPIOInit.Pull 							= GPIO_PULLUP,					/* GPIO���������ã��Ƿ���Ҫ��������Ӳ�� */
	.tGPIO[1].tGPIOInit.Speed 							= GPIO_SPEED_FREQ_HIGH,			/* GPIO�ٶ� */	
	.tGPIO[1].tGPIOPort 								= GPIOA,						/* GPIO���� */			
};

/* DS1337Ƭ��RTC */
tagDS3231_T DS1337 = 
{
	/* SCL�� */
	.tIICSoft.tIICSoft[0].tGPIOInit.Pin 	= GPIO_PIN_6,				/* GPIO���� */
	.tIICSoft.tIICSoft[0].tGPIOInit.Mode 	= GPIO_MODE_OUTPUT_PP,		/* GPIOģʽ */
	.tIICSoft.tIICSoft[0].tGPIOInit.Pull 	= GPIO_NOPULL,				/* GPIO���������ã��Ƿ���Ҫ��������Ӳ�� */
	.tIICSoft.tIICSoft[0].tGPIOInit.Speed 	= GPIO_SPEED_FREQ_HIGH,		/* GPIO�ٶ� */	
	.tIICSoft.tIICSoft[0].tGPIOPort 		= GPIOF,					/* GPIO���� */
	
	/* SDA�� */
	.tIICSoft.tIICSoft[1].tGPIOInit.Pin 	= GPIO_PIN_7,				/* GPIO���� */
	.tIICSoft.tIICSoft[1].tGPIOInit.Mode 	= GPIO_MODE_INPUT,			/* GPIOģʽ */
	.tIICSoft.tIICSoft[1].tGPIOInit.Pull 	= GPIO_NOPULL,				/* GPIO���������ã��Ƿ���Ҫ��������Ӳ�� */
	.tIICSoft.tIICSoft[1].tGPIOInit.Speed 	= GPIO_SPEED_FREQ_HIGH,		/* GPIO�ٶ� */	
	.tIICSoft.tIICSoft[1].tGPIOPort 		= GPIOF,					/* GPIO���� */
};

/* AT24C02 */
tagAT24CXX_T EEPROM = 
{
	/* SCL�� */
	.tIIC.tIICSoft[0].tGPIOInit.Pin 		= GPIO_PIN_4,				/* GPIO���� */
	.tIIC.tIICSoft[0].tGPIOInit.Mode		= GPIO_MODE_OUTPUT_PP,		/* GPIOģʽ */
	.tIIC.tIICSoft[0].tGPIOInit.Pull		= GPIO_NOPULL,				/* GPIO���������ã��Ƿ���Ҫ��������Ӳ�� */
	.tIIC.tIICSoft[0].tGPIOInit.Speed		= GPIO_SPEED_FREQ_HIGH,		/* GPIO�ٶ� */	
	.tIIC.tIICSoft[0].tGPIOPort 			= GPIOE,					/* GPIO���� */
	
	/* SDA�� */
	.tIIC.tIICSoft[1].tGPIOInit.Pin 		= GPIO_PIN_3,				/* GPIO���� */
	.tIIC.tIICSoft[1].tGPIOInit.Mode		= GPIO_MODE_INPUT,			/* GPIOģʽ */
	.tIIC.tIICSoft[1].tGPIOInit.Pull		= GPIO_NOPULL,				/* GPIO���������ã��Ƿ���Ҫ��������Ӳ�� */
	.tIIC.tIICSoft[1].tGPIOInit.Speed		= GPIO_SPEED_FREQ_HIGH,		/* GPIO�ٶ� */	
	.tIIC.tIICSoft[1].tGPIOPort 			= GPIOE,					/* GPIO���� */
};

/* TF��ʹ��FatFs������־ */
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
	
	.tSPI.tGPIO[0].tGPIOInit.Pin 			= GPIO_PIN_3,				/* GPIO���� */
	.tSPI.tGPIO[0].tGPIOInit.Mode 			= GPIO_MODE_AF_PP,			/* GPIOģʽ */
	.tSPI.tGPIO[0].tGPIOInit.Pull 			= GPIO_NOPULL,				/* GPIO���������ã��Ƿ���Ҫ��������Ӳ�� */
	.tSPI.tGPIO[0].tGPIOInit.Speed 			= GPIO_SPEED_FREQ_HIGH,		/* GPIO�ٶ� */	
	.tSPI.tGPIO[0].tGPIOPort 				= GPIOB,					/* GPIO���� */
	
	.tSPI.tGPIO[1].tGPIOInit.Pin 			= GPIO_PIN_4,				/* GPIO���� */
	.tSPI.tGPIO[1].tGPIOInit.Mode 			= GPIO_MODE_AF_PP,			/* GPIOģʽ */
	.tSPI.tGPIO[1].tGPIOInit.Pull 			= GPIO_NOPULL,				/* GPIO���������ã��Ƿ���Ҫ��������Ӳ�� */
	.tSPI.tGPIO[1].tGPIOInit.Speed 			= GPIO_SPEED_FREQ_HIGH,		/* GPIO�ٶ� */	
	.tSPI.tGPIO[1].tGPIOPort 				= GPIOB,					/* GPIO���� */
	
	.tSPI.tGPIO[2].tGPIOInit.Pin 			= GPIO_PIN_5,				/* GPIO���� */
	.tSPI.tGPIO[2].tGPIOInit.Mode 			= GPIO_MODE_AF_PP,			/* GPIOģʽ */
	.tSPI.tGPIO[2].tGPIOInit.Pull 			= GPIO_NOPULL,				/* GPIO���������ã��Ƿ���Ҫ��������Ӳ�� */
	.tSPI.tGPIO[2].tGPIOInit.Speed 			= GPIO_SPEED_FREQ_HIGH,		/* GPIO�ٶ� */	
	.tSPI.tGPIO[2].tGPIOPort 				= GPIOB,					/* GPIO���� */
};

/* �忨�������� */
tagGPIO_T Control[] =
{
	
	[0]=
	{ 
		.tGPIOInit.Pin 		= GPIO_PIN_15,				/* GPIO���� */
		.tGPIOInit.Mode 	= GPIO_MODE_OUTPUT_PP,		/* GPIOģʽ */
		.tGPIOInit.Pull 	= GPIO_NOPULL,				/* GPIO���������ã��Ƿ���Ҫ��������Ӳ�� */
		.tGPIOInit.Speed 	= GPIO_SPEED_FREQ_HIGH,		/* GPIO�ٶ� */
		.tGPIOPort 			= GPIOA,					/* GPIO���� */
	},
    [1]=
	{ 
		.tGPIOInit.Pin 		= GPIO_PIN_10,				/* GPIO���� */
		.tGPIOInit.Mode 	= GPIO_MODE_OUTPUT_PP,		/* GPIOģʽ */
		.tGPIOInit.Pull 	= GPIO_NOPULL,				/* GPIO���������ã��Ƿ���Ҫ��������Ӳ�� */
		.tGPIOInit.Speed 	= GPIO_SPEED_FREQ_HIGH,		/* GPIO�ٶ� */
		.tGPIOPort 			= GPIOC,					/* GPIO���� */
	},
	[2]=
	{ 
		.tGPIOInit.Pin 		= GPIO_PIN_11,				/* GPIO���� */
		.tGPIOInit.Mode 	= GPIO_MODE_OUTPUT_PP,		/* GPIOģʽ */
		.tGPIOInit.Pull 	= GPIO_NOPULL,				/* GPIO���������ã��Ƿ���Ҫ��������Ӳ�� */
		.tGPIOInit.Speed 	= GPIO_SPEED_FREQ_HIGH,		/* GPIO�ٶ� */
		.tGPIOPort 			= GPIOC,					/* GPIO���� */
	},
	[3]=
	{ 
		.tGPIOInit.Pin 		= GPIO_PIN_0,				/* GPIO���� */
		.tGPIOInit.Mode 	= GPIO_MODE_OUTPUT_PP,		/* GPIOģʽ */
		.tGPIOInit.Pull 	= GPIO_NOPULL,				/* GPIO���������ã��Ƿ���Ҫ��������Ӳ�� */
		.tGPIOInit.Speed 	= GPIO_SPEED_FREQ_HIGH,		/* GPIO�ٶ� */
		.tGPIOPort 			= GPIOD,					/* GPIO���� */
	},
	[4]=
	{ 
		.tGPIOInit.Pin 		= GPIO_PIN_1,				/* GPIO���� */
		.tGPIOInit.Mode 	= GPIO_MODE_OUTPUT_PP,		/* GPIOģʽ */
		.tGPIOInit.Pull 	= GPIO_NOPULL,				/* GPIO���������ã��Ƿ���Ҫ��������Ӳ�� */
		.tGPIOInit.Speed 	= GPIO_SPEED_FREQ_HIGH,		/* GPIO�ٶ� */
		.tGPIOPort 			= GPIOD,					/* GPIO���� */
	},
	[5]=
	{ 
		.tGPIOInit.Pin 		= GPIO_PIN_4,				/* GPIO���� */
		.tGPIOInit.Mode 	= GPIO_MODE_OUTPUT_PP,		/* GPIOģʽ */
		.tGPIOInit.Pull 	= GPIO_NOPULL,				/* GPIO���������ã��Ƿ���Ҫ��������Ӳ�� */
		.tGPIOInit.Speed 	= GPIO_SPEED_FREQ_HIGH,		/* GPIO�ٶ� */
		.tGPIOPort 			= GPIOD,					/* GPIO���� */
	},
	[6]=
	{ 
		.tGPIOInit.Pin 		= GPIO_PIN_5,				/* GPIO���� */
		.tGPIOInit.Mode 	= GPIO_MODE_OUTPUT_PP,		/* GPIOģʽ */
		.tGPIOInit.Pull 	= GPIO_NOPULL,				/* GPIO���������ã��Ƿ���Ҫ��������Ӳ�� */
		.tGPIOInit.Speed 	= GPIO_SPEED_FREQ_HIGH,		/* GPIO�ٶ� */
		.tGPIOPort 			= GPIOD,					/* GPIO���� */
	},
	[7]=
	{ 
		.tGPIOInit.Pin 		= GPIO_PIN_6,				/* GPIO���� */
		.tGPIOInit.Mode 	= GPIO_MODE_OUTPUT_PP,		/* GPIOģʽ */
		.tGPIOInit.Pull 	= GPIO_NOPULL,				/* GPIO���������ã��Ƿ���Ҫ��������Ӳ�� */
		.tGPIOInit.Speed 	= GPIO_SPEED_FREQ_HIGH,		/* GPIO�ٶ� */
		.tGPIOPort 			= GPIOD,					/* GPIO���� */
	},
	[8]=
	{ 
		.tGPIOInit.Pin 		= GPIO_PIN_7,				/* GPIO���� */
		.tGPIOInit.Mode 	= GPIO_MODE_OUTPUT_PP,		/* GPIOģʽ */
		.tGPIOInit.Pull 	= GPIO_NOPULL,				/* GPIO���������ã��Ƿ���Ҫ��������Ӳ�� */
		.tGPIOInit.Speed 	= GPIO_SPEED_FREQ_HIGH,		/* GPIO�ٶ� */
		.tGPIOPort 			= GPIOD,					/* GPIO���� */
	},
	[9]=
	{ 
		.tGPIOInit.Pin 		= GPIO_PIN_9,				/* GPIO���� */
		.tGPIOInit.Mode 	= GPIO_MODE_OUTPUT_PP,		/* GPIOģʽ */
		.tGPIOInit.Pull 	= GPIO_NOPULL,				/* GPIO���������ã��Ƿ���Ҫ��������Ӳ�� */
		.tGPIOInit.Speed 	= GPIO_SPEED_FREQ_HIGH,		/* GPIO�ٶ� */
		.tGPIOPort 			= GPIOG,					/* GPIO���� */
	},
	[10]=
	{ 
		.tGPIOInit.Pin 		= GPIO_PIN_10,				/* GPIO���� */
		.tGPIOInit.Mode 	= GPIO_MODE_OUTPUT_PP,		/* GPIOģʽ */
		.tGPIOInit.Pull 	= GPIO_NOPULL,				/* GPIO���������ã��Ƿ���Ҫ��������Ӳ�� */
		.tGPIOInit.Speed 	= GPIO_SPEED_FREQ_HIGH,		/* GPIO�ٶ� */
		.tGPIOPort 			= GPIOG,					/* GPIO���� */
	},
	[11]=
	{ 
		.tGPIOInit.Pin 		= GPIO_PIN_12,				/* GPIO���� */
		.tGPIOInit.Mode 	= GPIO_MODE_OUTPUT_PP,		/* GPIOģʽ */
		.tGPIOInit.Pull 	= GPIO_NOPULL,				/* GPIO���������ã��Ƿ���Ҫ��������Ӳ�� */
		.tGPIOInit.Speed 	= GPIO_SPEED_FREQ_HIGH,		/* GPIO�ٶ� */
		.tGPIOPort 			= GPIOG,					/* GPIO���� */
	},

};

/* SYNC�������� */
tagGPIO_T SYNC[] =
{
	[0]=
	{ 
		.tGPIOInit.Pin 		= GPIO_PIN_0,				/* GPIO���� */
		.tGPIOInit.Mode 	= GPIO_MODE_OUTPUT_PP,		/* GPIOģʽ */
		.tGPIOInit.Pull 	= GPIO_NOPULL,				/* GPIO���������ã��Ƿ���Ҫ��������Ӳ�� */
		.tGPIOInit.Speed 	= GPIO_SPEED_FREQ_HIGH,		/* GPIO�ٶ� */
		.tGPIOPort 			= GPIOE,					/* GPIO���� */
	},
};

