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


