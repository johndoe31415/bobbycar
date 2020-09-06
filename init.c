#include <stm32f10x_rcc.h>
#include <stm32f10x_usart.h>
#include <stm32f10x_spi.h>
#include "system.h"
#include "init.h"

static void init_usart(void) {
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

	USART_InitTypeDef USART_InitStructure = {
		.USART_BaudRate = 115200,
		.USART_WordLength = USART_WordLength_8b,
		.USART_StopBits = USART_StopBits_1,
		.USART_Parity = USART_Parity_No,
		.USART_HardwareFlowControl = USART_HardwareFlowControl_None,
		.USART_Mode = USART_Mode_Rx | USART_Mode_Tx,
	};

	USART_Init(USART1, &USART_InitStructure);
	USART_Cmd(USART1, ENABLE);
}

static void init_spi(void) {
	SPI_Init(SPI1, &(SPI_InitTypeDef){
		.SPI_Direction = SPI_Direction_2Lines_FullDuplex,
		.SPI_Mode = SPI_Mode_Master,
		.SPI_DataSize = SPI_DataSize_8b,
		.SPI_CPOL = SPI_CPOL_High,
		.SPI_CPHA = SPI_CPHA_2Edge,
		.SPI_NSS = SPI_NSS_Hard,
		.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64,
		.SPI_FirstBit = SPI_FirstBit_MSB,
		.SPI_CRCPolynomial = 7,
	});
	SPI_Cmd(SPI1, ENABLE);
}

void system_init(void) {
	init_usart();
	init_spi();
}
