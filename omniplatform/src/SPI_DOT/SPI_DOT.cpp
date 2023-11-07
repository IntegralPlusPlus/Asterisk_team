#include "SPI_DOT.h"

uint16_t _RxData[2];
bool _available[2];
uint8_t num = 1;

void initSPI2(bool mode, uint8_t dataFrameFormat, uint16_t clkPrescaler)
{
  _RxData[num] = 0;														
	_available[num] = 0;
	
	SPI_TypeDef *spi;
	uint8_t afNumber = 5;
  
  spi = SPI2;
	RCC->APB1ENR |= RCC_APB1ENR_SPI2EN;

  if(mode == 1)
  {
    spi->CR1 &= ~SPI_CR1_BR;
    
    switch(clkPrescaler){
			case 4: spi->CR1 |= SPI_CR1_BR_0; break;
			case 8: spi->CR1 |= SPI_CR1_BR_1; break;
			case 16: spi->CR1 |= SPI_CR1_BR_1 | SPI_CR1_BR_0; break;
			case 32: spi->CR1 |= SPI_CR1_BR_2; break;
			case 64: spi->CR1 |= SPI_CR1_BR_2 | SPI_CR1_BR_0; break;
			case 128: spi->CR1 |= SPI_CR1_BR_2 | SPI_CR1_BR_1; break;
			case 256: spi->CR1 |= SPI_CR1_BR; break;
		}	
    spi->CR1 |= SPI_CR1_SSM;
		spi->CR1 |= SPI_CR1_SSI;
  }
  
  if(dataFrameFormat == 16) spi->CR1 |= SPI_CR1_DFF;		
	else spi->CR1 &= ~SPI_CR1_DFF;
  
  spi->CR1 &= ~SPI_CR1_CPOL;						
	spi->CR1 &= ~SPI_CR1_CPHA;
  
  spi->CR1 &= ~SPI_CR1_LSBFIRST;		
  
  if(mode == 0)spi->CR2 |= SPI_CR2_RXNEIE;
  
  if(mode == 1)spi->CR1 |= SPI_CR1_MSTR;		
	else spi->CR1 &= ~SPI_CR1_MSTR;
  
  spi->CR1 |= SPI_CR1_SPE;	
}


uint16_t writeSPI(uint16_t TxData){						//for master
	
	//select SPI register name
	SPI_TypeDef * spi;								
	spi = SPI2;
	
	//write data
	while(!(spi->SR & SPI_SR_TXE));		
	spi->DR = TxData;
	
	//read data
	while(!(spi->SR & SPI_SR_RXNE));
	return spi->DR;		
	
}