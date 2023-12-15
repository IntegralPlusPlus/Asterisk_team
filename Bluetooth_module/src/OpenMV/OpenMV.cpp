#include "OpenMV.h"

OpenMV::OpenMV(Pin &tx, Pin &rx, uint8_t usartNum) {
	initUSART(usartNum);
	_usartNumber = usartNum;
	
	_distBlue = 0;
	_angleBlue = 0;
	_distYellow = 0;
	_angleYellow = 0;
	_x = 0;
	_y = 0;
}

void OpenMV::read() {
	bool gotData = false;
	if (_usartNumber == 1 && uart1::available() >= 6) {
		if (uart1::read() == 255) { //                             START  BIT
			for (uint8_t i = 0; i < 4; ++i) data[i] = uart1::read(); 
			crc = uart1::read();
			gotData = true;
		}
	} else if (_usartNumber == 2 && uart2::available() >= 6) {
		if (uart2::read() == 255) { //                             START  BIT
			for (uint8_t i = 0; i < 4; ++i) data[i] = uart2::read(); 
			crc = uart2::read();
			gotData = true;
		}
	} else if (_usartNumber == 3 && uart3::available() >= 6) {
		if (uart3::read() == 255) { //                             START  BIT
			for (uint8_t i = 0; i < 4; ++i) data[i] = uart3::read(); 
			crc = uart3::read();
			gotData = true;
		}
	} else if (_usartNumber == 6 && uart6::available() >= 6) {
		if (uart6::read() == 255) { //                             START  BIT
			for (uint8_t i = 0; i < 4; ++i) data[i] = uart6::read(); 
			crc = uart6::read();
			gotData = true;
		}
	}
	
	if (gotData && crc == crc8(data, 4)) {
		_distBlue = data[0];
		_angleBlue = adduct(2 * data[1]);
		_distYellow = data[2];
		_angleYellow = adduct(2 * data[3]);
	}
}

void OpenMV::calculate(int16_t robotAngle, bool goal) {
	float xYellow, yYellow, xBlue, yBlue;
	if (goal == BLUE_GOAL) robotAngle = adduct(robotAngle + 180);
	
	_angleBlue += robotAngle;
	_angleYellow += robotAngle;
	
	xYellow = float(_distYellow) * -sin(float(_angleYellow) * DEG2RAD); 
	yYellow = float(_distYellow) * cos(float(_angleYellow) * DEG2RAD);
	xBlue = float(_distBlue) * -sin(float(180 - _angleBlue) * DEG2RAD); 
	yBlue = DIST_BETWEEN_GOALS - float(_distBlue) * cos(float(180 - _angleBlue) * DEG2RAD);
	
	if (yYellow * xYellow * yBlue * xBlue != 0) {
		_x = (1/yYellow * xYellow + 1/yBlue * xBlue) / (1/yYellow + 1/yBlue);
		_y = (1/xYellow * yYellow + 1/xBlue * yBlue) / (1/xYellow + 1/xBlue);
	} else {
		if (yYellow == 0) _x = xBlue;
		else if (yBlue == 0) _x = xYellow;
		else _x = 0;
		
		if (xYellow == 0) _y = yBlue;
		else if (xBlue == 0) _y = yYellow;
		else _y = 0;
	}
	
	if (goal == BLUE_GOAL) {
		_x *= -1;
		_y = DIST_BETWEEN_GOALS - _y;
	} 
}

int16_t OpenMV::getX() {
	return _x;
}

int16_t OpenMV::getY() {
	return _y;
}

uint8_t OpenMV::crc8(volatile uint8_t* data, uint8_t len) {
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

int16_t OpenMV::adduct(int16_t ang) {
	while (ang > 180) ang -= 360;
  while (ang < -180) ang += 360;
  return ang;
}

void OpenMV::initUSART(uint8_t num) {
	if (num == 1) uart1::usart1Init(115200, 1, 8);
	else if (num == 2) uart2::usart2Init(115200, 1, 8);
	else if (num == 3) uart3::usart3Init(115200, 1, 8);
	else if (num == 6) uart6::usart6Init(115200, 1, 8);
}