#include "SPI_1.h"

namespace SPI_1 {
	int _spi_mode;
  int _spi_direction;
  int _baud_rate;
  int _first_bit;
  //int _data;
  int _rx_buffer[8];
  uint16_t _cur_rx_read;
  uint16_t _cur_rx_write;
  uint16_t _buf_size;
    
	void init(int mode, int dir, int baudRate, int firstBit)
  {
    //_SPIx = SPI1;
    _spi_mode = mode;
    _spi_direction = dir;
    _baud_rate = baudRate;
    _first_bit = firstBit;
    _cur_rx_read = 0;
    _cur_rx_write = 0;
    _buf_size = 0;
    for(int i = 0; i < 8; i++) {
      _rx_buffer[i] = 0;
    }
    
    //sets clock for spi
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE); 
    
    SPI_InitTypeDef SPIConf;
    
    //sets direction of transmition of data
    if(_spi_direction == full) SPIConf.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    else if(_spi_direction == rx_only_2) SPIConf.SPI_Direction = SPI_Direction_2Lines_RxOnly;
    else if(_spi_direction == rx_only_1) SPIConf.SPI_Direction = SPI_Direction_1Line_Rx;
    else SPIConf.SPI_Direction = SPI_Direction_1Line_Tx;
    
    if(_spi_mode == master_mode) SPIConf.SPI_Mode = SPI_Mode_Master; // sets to master mode
    else SPIConf.SPI_Mode = SPI_Mode_Slave; // sets to slave mode
    
    
    SPIConf.SPI_CPOL = SPI_CPOL_Low; //sets polarity(0 - low, 1 - high)
    SPIConf.SPI_CPHA = SPI_CPHA_1Edge; //reading during the first front line
    SPIConf.SPI_NSS = SPI_NSS_Soft; //sets to soft slave select
    
    //sets the baudrate(speed of transmition)
    if(baudRate == 2) SPIConf.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2; 
    else if(baudRate == 4) SPIConf.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4; 
    else if(baudRate == 8) SPIConf.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8; 
    else if(baudRate == 16) SPIConf.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16; 
    else if(baudRate == 32) SPIConf.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_32;
    else if(baudRate == 64) SPIConf.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64; 
    else if(baudRate == 128) SPIConf.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_128; 
    else if(baudRate == 256) SPIConf.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256; 
    
    //sets the start bit(msb or lsb)
    if(firstBit == msb) SPIConf.SPI_FirstBit = SPI_FirstBit_MSB;
    else if(firstBit == lsb) SPIConf.SPI_FirstBit = SPI_FirstBit_LSB;
    
    if(_spi_mode == master_mode) {
      SPI1->CR1 |= SPI_CR1_SSM;
      SPI1->CR1 |= SPI_CR1_SSI;
    }
    
    SPI_Init(SPI1, &SPIConf); //init spi
    SPI_ITConfig(SPI1, SPI_I2S_IT_RXNE, ENABLE);//configure interrupt to the empty rx buffer
    NVIC_EnableIRQ(SPI1_IRQn);
    
    SPI_Cmd(SPI1, ENABLE); 
  }

	
	uint16_t read() {	
		int data;
		
		if(_buf_size > 0) {
      data = _rx_buffer[_cur_rx_read];
      if(_cur_rx_read < 7) _cur_rx_read++;
      else _cur_rx_read = 0;
      
      _buf_size--;
    } else data = 0;
			
		return data;
	}
	
	void send(uint16_t data) {
    ENTER_CRITICAL_SECTION();
    while(!(SPI1->SR & SPI_SR_TXE));

    SPI1->DR = data;
    EXIT_CRITICAL_SECTION();
  } 
	
	uint16_t available() {
		return _buf_size;
	}
}  

extern "C" {
  void SPI1_IRQHandler(void) {
    uint16_t it_data;
    if (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE)) {
      SPI1->DR = 0; 

      while(!(SPI1->SR & SPI_SR_RXNE));
      
      it_data = SPI1->DR;
      
      SPI_1::_rx_buffer[SPI_1::_cur_rx_write] = it_data;
      
      if(SPI_1::_cur_rx_write == 7) SPI_1::_cur_rx_write = 0;
      else SPI_1::_cur_rx_write++;
      
      if(SPI_1::_buf_size < 7) SPI_1::_buf_size++;
    }
  } 
}  