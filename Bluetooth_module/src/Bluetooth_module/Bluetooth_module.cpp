#include "Bluetooth_module.h"

Bluetooth::Bluetooth(Pin& tx, Pin& rx, uint8_t usartNum): _tx(tx), _rx(rx)
{
	_usartNumber = usartNum;
	initUSART(usartNum);
}

void Bluetooth::read() {
	bool gotData = false;
	if (_usartNumber == 1 && uart1::available() >= PACKAGE_SIZE) {
		if (uart1::read() == 255) { //                             START  BIT
			for (uint8_t i = 0; i < DATA_SIZE; ++i) _data[i] = uart1::read(); 
			crc = uart1::read();
			gotData = true;
		}
	} else if (_usartNumber == 2 && uart2::available() >= PACKAGE_SIZE) {
		if (uart2::read() == 255) { //                             START  BIT
			for (uint8_t i = 0; i < DATA_SIZE; ++i) _data[i] = uart2::read(); 
			crc = uart2::read();
			gotData = true;
		}
	} else if (_usartNumber == 3 && uart3::available() >= PACKAGE_SIZE) {
		if (uart3::read() == 255) { //                             START  BIT
			for (uint8_t i = 0; i < DATA_SIZE; ++i) _data[i] = uart3::read(); 
			crc = uart3::read();
			gotData = true;
		}
	} else if (_usartNumber == 6 && uart6::available() >= PACKAGE_SIZE) {
		if (uart6::read() == 255) { //                             START  BIT
			for (uint8_t i = 0; i < DATA_SIZE; ++i) _data[i] = uart6::read(); 
			crc = uart6::read();
			gotData = true;
		}
	}
	
	if (gotData && crc == crc8(_data, DATA_SIZE)) {
		_otherX = convert(_data[0], UINT8T_TO_INT16T);
		_otherY = convert(_data[1], UINT8T_TO_INT16T);
		_otherAngle = convert(_data[2], ANGLE_255_TO_360);
	}
}

void Bluetooth::send(int16_t myX, int16_t myY, int16_t myAngle) {
	volatile uint8_t package[3] = {convert(myX, INT16T_TO_UINT8T), convert(myX, INT16T_TO_UINT8T), convert(myAngle, ANGLE_360_TO_255)};
	uint8_t crc = crc8(package, DATA_SIZE);
	if (_usartNumber == 1) {
		uart1::write(255);
		for (uint8_t i = 0; i < DATA_SIZE; ++i) uart1::write(package[i]);
		uart1::write(crc);
	} else if (_usartNumber == 2) {
		uart2::write(255);
		for (uint8_t i = 0; i < DATA_SIZE; ++i) uart2::write(package[i]);
		uart2::write(crc);
	} else if (_usartNumber == 3) {
		uart3::write(255);
		for (uint8_t i = 0; i < DATA_SIZE; ++i) uart3::write(package[i]);
		uart3::write(crc);
	} else if (_usartNumber == 6) {
		uart6::write(255);
		for (uint8_t i = 0; i < DATA_SIZE; ++i) uart6::write(package[i]);
		uart6::write(crc);
	}
}

int16_t Bluetooth::getXBall() {
	return _xBall;
}

int16_t Bluetooth::getYBall() {
	return _yBall;
}

int16_t Bluetooth::convert(int16_t data, uint8_t type) {
	if (type == ANGLE_360_TO_255) return float(data) / RECEIVED2REAL;
	else if (type == ANGLE_255_TO_360) return float(data) * RECEIVED2REAL;
	else if (type == UINT8T_TO_INT16T) return data / 10 + 25;
	else if (type == INT16T_TO_UINT8T) return (data - 25) * 10;
}

void Bluetooth::ballCalculate(int16_t myX, int16_t myY, int16_t myAngle) {
	float myTg = tan(float(myAngle) * DEG2RAD), otherTg = tan(float(myAngle) * DEG2RAD);
	_xBall = float(myTg * float(myX) - otherTg * float(_otherX) + (_otherY - myY)) / (myTg - otherTg);
	_yBall = myTg * float(_xBall - myX) + myY;
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