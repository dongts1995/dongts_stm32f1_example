#include "spi.h"
#include <main.h>

#include "stm32f10x.h"

extern SPI_InitTypeDef  SPI_InitStructure;
extern uint32_t TxIdx, RxIdx;
extern uint16_t SPI1_Buffer_Rx[BufferSize], SPI2_Buffer_Rx[BufferSize];
extern uint16_t SPI1_Buffer_Tx[BufferSize];
extern uint16_t SPI2_Buffer_Tx[BufferSize];
extern __IO uint16_t CRC1Value, CRC2Value;



typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;
volatile TestStatus TransferStatus1 = FAILED, TransferStatus2 = FAILED;
TestStatus Buffercmp(uint16_t* pBuffer1, uint16_t* pBuffer2, uint16_t BufferLength);

void spi_config(void)
{
	/* SPI1 configuration ------------------------------------------------------*/
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_16b;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_InitStructure.SPI_CRCPolynomial = 7;
  SPI_Init(SPI1, &SPI_InitStructure);

  /* SPI2 configuration ------------------------------------------------------*/
  SPI_InitStructure.SPI_Mode = SPI_Mode_Slave;
  SPI_Init(SPI2, &SPI_InitStructure);
	
	/* Enable SPI1 CRC calculation */
  SPI_CalculateCRC(SPI1, ENABLE);
  /* Enable SPI2 CRC calculation */
  SPI_CalculateCRC(SPI2, ENABLE);

  /* Enable SPI1 */
  SPI_Cmd(SPI1, ENABLE);
  /* Enable SPI2 */
  SPI_Cmd(SPI2, ENABLE);
}


void spi_bidirectional_CRC(void)
{
	TxIdx = 0;
  while (TxIdx < BufferSize - 1)
  {
    /* Wait for SPI1 Tx buffer empty */
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
    /* Send SPI2 data */
    SPI_I2S_SendData(SPI2, SPI2_Buffer_Tx[TxIdx]);	//SPI2->DR = SPI2_Buffer_Tx[TxIdx]
    /* Send SPI1 data */
    SPI_I2S_SendData(SPI1, SPI1_Buffer_Tx[TxIdx++]);
    /* Wait for SPI2 data reception */
    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET); 	
    /* Read SPI2 received data */
    SPI2_Buffer_Rx[RxIdx] = SPI_I2S_ReceiveData(SPI2);
    /* Wait for SPI1 data reception */
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
    /* Read SPI1 received data */
    SPI1_Buffer_Rx[RxIdx++] = SPI_I2S_ReceiveData(SPI1);
  }

  /* Wait for SPI1 Tx buffer empty */
  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
  /* Wait for SPI2 Tx buffer empty */
  while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);
	
  /* Send last SPI2_Buffer_Tx data */
  SPI_I2S_SendData(SPI2, SPI2_Buffer_Tx[TxIdx]);
  /* Enable SPI2 CRC transmission */
  SPI_TransmitCRC(SPI2);
  /* Send last SPI1_Buffer_Tx data */
  SPI_I2S_SendData(SPI1, SPI1_Buffer_Tx[TxIdx]);
  /* Enable SPI1 CRC transmission */
  SPI_TransmitCRC(SPI1);

  /* Wait for SPI1 last data reception */
  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
  /* Read SPI1 last received data */
  SPI1_Buffer_Rx[RxIdx] = SPI_I2S_ReceiveData(SPI1);

  /* Wait for SPI2 last data reception */
  while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET);
  /* Read SPI2 last received data */
  SPI2_Buffer_Rx[RxIdx] = SPI_I2S_ReceiveData(SPI2);

  /* Wait for SPI1 data reception: CRC transmitted by SPI2 */
  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
  /* Wait for SPI2 data reception: CRC transmitted by SPI1 */
  while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET);

  /* Check the received data with the send ones */
  TransferStatus1 = Buffercmp(SPI2_Buffer_Rx, SPI1_Buffer_Tx, BufferSize);
  TransferStatus2 = Buffercmp(SPI1_Buffer_Rx, SPI2_Buffer_Tx, BufferSize);
  /* TransferStatus1, TransferStatus2 = PASSED, if the data transmitted and received
     are correct */
  /* TransferStatus1, TransferStatus2 = FAILED, if the data transmitted and received
     are different */

  /* Test on the SPI1 CRC Error flag */
  if ((SPI_I2S_GetFlagStatus(SPI1, SPI_FLAG_CRCERR)) == SET)
  {
    TransferStatus2 = FAILED;
  }

  /* Test on the SPI2 CRC Error flag */
  if ((SPI_I2S_GetFlagStatus(SPI2, SPI_FLAG_CRCERR)) == SET)
  {
    TransferStatus1 = FAILED;
  }

  /* Read SPI1 received CRC value */
  CRC1Value = SPI_I2S_ReceiveData(SPI1);
  /* Read SPI2 received CRC value */
  CRC2Value = SPI_I2S_ReceiveData(SPI2);
}

TestStatus Buffercmp(uint16_t* pBuffer1, uint16_t* pBuffer2, uint16_t BufferLength)
{
  while (BufferLength--)
  {
    if (*pBuffer1 != *pBuffer2)
    {
      return FAILED;
    }

    pBuffer1++;
    pBuffer2++;
  }

  return PASSED;
}

void spi_simplex(void)
	{
		TxIdx = 0;
		while (TxIdx < BufferSize - 1)
		{
			/* Wait for SPI1 Tx buffer empty */
			while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
			/* Send SPI1 data */
			SPI_I2S_SendData(SPI1, SPI1_Buffer_Tx[TxIdx++]);
			/* Wait for SPI2 data reception */
			while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET); 	
			/* Read SPI2 received data */
			SPI2_Buffer_Rx[RxIdx++] = SPI_I2S_ReceiveData(SPI2);
		}

		/* Wait for SPI1 Tx buffer empty */
		while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
	
		/* Send last SPI1_Buffer_Tx data */
		SPI_I2S_SendData(SPI1, SPI1_Buffer_Tx[TxIdx]);
		/* Enable SPI1 CRC transmission */
		SPI_TransmitCRC(SPI1);

		/* Wait for SPI2 last data reception */
		while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET);
		/* Read SPI2 last received data */
		SPI2_Buffer_Rx[RxIdx] = SPI_I2S_ReceiveData(SPI2);

	}
