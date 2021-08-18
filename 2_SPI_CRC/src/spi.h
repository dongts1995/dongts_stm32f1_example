#ifndef __SPI_H
#define __SPI_H

#include "stm32f10x.h"

void spi_config(void);
void spi_bidirectional_CRC(void);
void spi_simplex(void);
void Sync_Send_Byte(uint8_t byte);
void Sync_Send_Array (uint8_t text_style, const uint8_t * ptrString, uint8_t len);



#endif
