#pragma once
#include "project_config.h"

enum spi_direction {
	full,
	rx_only_2,
	rx_only_1,
	tx_only_1
};

enum firstBits {
	msb,
	lsb
};

#define master_mode 1 
#define	slave_mode 0

namespace SPI_1 {
  extern int _spi_mode;
	extern int _spi_direction;
  extern int _data_size;
  extern int _baud_rate;
  extern int _first_bit;
  //extern int _data;
  extern int _rx_buffer[8];
  extern uint16_t _cur_rx_read;
  extern uint16_t _cur_rx_write;
  extern uint16_t _buf_size;
	void init(int mode, int dir, int baudRate, int firstBit);
	uint16_t read();
	uint16_t available();
	void send(uint16_t data);
}