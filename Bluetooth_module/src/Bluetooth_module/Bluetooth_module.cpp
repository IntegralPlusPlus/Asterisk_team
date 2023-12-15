#include "Bluetooth_module.h"

Bluetooth::Bluetooth(Pin& tx, Pin& rx, uint8_t usartNum): _tx(tx), _rx(rx)
{
	_usartNumber = usartNum;
	initUSART(usartNum);
}

void Bluetooth::read() {
	bool gotData = false;
	if (_usartNumber == 1 && uart1::available() >= 6) {
		if (uart1::read() == 255) { //                             START  BIT
			for (uint8_t i = 0; i < 3; ++i) _data[i] = uart1::read(); 
			crc = uart1::read();
			gotData = true;
		}
	} else if (_usartNumber == 2 && uart2::available() >= 6) {
		if (uart2::read() == 255) { //                             START  BIT
			for (uint8_t i = 0; i < 3; ++i) _data[i] = uart2::read(); 
			crc = uart2::read();
			gotData = true;
		}
	} else if (_usartNumber == 3 && uart3::available() >= 6) {
		if (uart3::read() == 255) { //                             START  BIT
			for (uint8_t i = 0; i < 3; ++i) _data[i] = uart3::read(); 
			crc = uart3::read();
			gotData = true;
		}
	} else if (_usartNumber == 6 && uart6::available() >= 6) {
		if (uart6::read() == 255) { //                             START  BIT
			for (uint8_t i = 0; i < 3; ++i) _data[i] = uart6::read(); 
			crc = uart6::read();
			gotData = true;
		}
	}
	
	if (gotData && crc == crc8(_data, 3)) {
		_otherX = _data[0];
		_otherY = _data[1];
		_otherAngle = _data[2];
	}
}

void Bluetooth::ballCalculate(int16_t myX, int16_t myY, int16_t myAngle) {
	CoordsXY myPoint1(myX, myY);
	CoordsXY myPoint2(myX + 100.f * cos(float(myAngle) * DEG2RAD), myY + 100.f * sin(float(myAngle) * DEG2RAD));
	CoordsXY otherRobotPoint1(_otherX, _otherY);
	CoordsXY otherRobotPoint2(_otherX + 100.f * cos(float(_otherAngle) * DEG2RAD), _otherY + 100.f * sin(float(_otherAngle) * DEG2RAD));
}

void Bluetooth::initUSART(uint8_t num) {
	if (num == 1) uart1::usart1Init(115200, 1, 8);
	else if (num == 2) uart2::usart2Init(115200, 1, 8);
	else if (num == 3) uart3::usart3Init(115200, 1, 8);
	else if (num == 6) uart6::usart6Init(115200, 1, 8);
}

uint8_t Bluetooth::crc8(volatile uint8_t* data, uint8_t len) {
	uint8_t crc = 0xFF, i, j;
  for (i = 0; i < len; i++) {
		crc ^= data[i];
    for (j = 0; j < 8; j++) {
			if (crc & 0x80) crc = ((crc << 1) ^ 0x31);
      else crc <<= 1;
    }
	}
		
  return crc;
}