#include "UART.h"

uint8_t USART_Read_NonBlock (USART_TypeDef * USARTx) {
	// SR_RXNE (Read data register not empty) bit is set by hardware
	if ((USARTx->ISR & USART_ISR_RXNE))
	{
		return ((uint8_t)(USARTx->RDR & 0xFF));
	}
	return ((uint8_t)0);
}
