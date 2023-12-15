#pragma once
#include "project_config.h"

namespace uart3
{
	void usart3Init(uint32_t bods, float stopBits, uint8_t len);
	uint16_t read();
	void write(uint8_t _w);
	uint16_t available();
	extern volatile uint8_t tx[8];
	extern volatile uint8_t rx[8];
	extern volatile uint16_t _rxCnt;
	extern volatile uint16_t _txCnt;
	extern volatile uint16_t _readCnt;
	extern volatile uint16_t _sendCnt;
	extern volatile bool flag;
}