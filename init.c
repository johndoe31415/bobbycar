#include <stm32f10x_rcc.h>
#include <stm32f10x_usart.h>
#include <stm32f10x_spi.h>
#include <stm32f10x_dma.h>
#include <stm32f10x.h>
#include <misc.h>
#include "system.h"
#include "init.h"

static void init_usart(void) {
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

	USART_InitTypeDef USART_InitStructure = {
		.USART_BaudRate = 921600,
		.USART_WordLength = USART_WordLength_8b,
		.USART_StopBits = USART_StopBits_1,
		.USART_Parity = USART_Parity_No,
		.USART_HardwareFlowControl = USART_HardwareFlowControl_None,
		.USART_Mode = USART_Mode_Rx | USART_Mode_Tx,
	};

	USART_Init(USART1, &USART_InitStructure);
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	USART_Cmd(USART1, ENABLE);
}

static void init_spi(void) {
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);

	SPI_Init(SPI1, &(SPI_InitTypeDef){
		.SPI_Direction = SPI_Direction_2Lines_FullDuplex,
		.SPI_Mode = SPI_Mode_Master,
		.SPI_DataSize = SPI_DataSize_8b,
		.SPI_CPOL = SPI_CPOL_Low,
		.SPI_CPHA = SPI_CPHA_1Edge,
		.SPI_NSS = SPI_NSS_Soft,
		.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8,		// 9 MHz; SPI Flash max 50 MHz
		.SPI_FirstBit = SPI_FirstBit_MSB,
		.SPI_CRCPolynomial = 0,
	});
	SPI_Cmd(SPI1, ENABLE);
	// SPI_SSOutputCmd
}

void init_spi_dma(void *vdata, unsigned int length) {
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

	DMA_Channel_TypeDef *dma_channel_rx = DMA1_Channel2;
	DMA_Channel_TypeDef *dma_channel_tx = DMA1_Channel3;

	DMA_Init(dma_channel_rx, &(DMA_InitTypeDef){
		.DMA_PeripheralBaseAddr = (uint32_t)(&SPI1->DR),
		.DMA_MemoryBaseAddr = (uint32_t)(vdata),
		.DMA_DIR = DMA_DIR_PeripheralSRC,
		.DMA_BufferSize = length,
		.DMA_PeripheralInc = DMA_PeripheralInc_Disable,
		.DMA_MemoryInc = DMA_MemoryInc_Enable,
		.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte,
		.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte,
		.DMA_Mode = DMA_Mode_Normal,
		.DMA_Priority = DMA_Priority_Medium,
		.DMA_M2M = DMA_M2M_Disable,
	});

	DMA_Init(dma_channel_tx, &(DMA_InitTypeDef){
		.DMA_PeripheralBaseAddr = (uint32_t)(&SPI1->DR),
		.DMA_MemoryBaseAddr = (uint32_t)(vdata),
		.DMA_DIR = DMA_DIR_PeripheralDST,
		.DMA_BufferSize = length,
		.DMA_PeripheralInc = DMA_PeripheralInc_Disable,
		.DMA_MemoryInc = DMA_MemoryInc_Enable,
		.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte,
		.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte,
		.DMA_Mode = DMA_Mode_Normal,
		.DMA_Priority = DMA_Priority_Medium,
		.DMA_M2M = DMA_M2M_Disable,
	});

	DMA_ITConfig(DMA1_Channel2, DMA_IT_TC, ENABLE);
	DMA_ITConfig(DMA1_Channel3, DMA_IT_TC, ENABLE);
}

static void init_nvic(void) {
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

	/* DMA1 SPI1 RX */
	NVIC_Init(&(NVIC_InitTypeDef){
		.NVIC_IRQChannel = DMA1_Channel2_IRQn,
		.NVIC_IRQChannelPreemptionPriority = 3,
		.NVIC_IRQChannelSubPriority = 3,
		.NVIC_IRQChannelCmd = ENABLE,
	});

	/* DMA1 SPI1 TX */
	NVIC_Init(&(NVIC_InitTypeDef){
		.NVIC_IRQChannel = DMA1_Channel3_IRQn,
		.NVIC_IRQChannelPreemptionPriority = 3,
		.NVIC_IRQChannelSubPriority = 3,
		.NVIC_IRQChannelCmd = ENABLE,
	});

	/* USART TXC */
	NVIC_Init(&(NVIC_InitTypeDef){
		.NVIC_IRQChannel = USART1_IRQn,
		.NVIC_IRQChannelPreemptionPriority = 3,
		.NVIC_IRQChannelSubPriority = 3,
		.NVIC_IRQChannelCmd = ENABLE,
	});
}

static void init_systick(void) {
	//SysTick_Config(72000000 / 100);	/* 10ms per systick */
	SysTick_Config(72000000 / 10);		/* 100ms per systick */
}

void system_init(void) {
	init_usart();
	init_spi();
	init_nvic();
	init_systick();
}