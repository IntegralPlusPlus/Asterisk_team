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
		_angleBlue = 2 * data[1];//adductionMV(2 * data[1]);
		_distYellow = data[2];
		_angleYellow = 2 * data[3];//adductionMV(2 * data[3]);
	}
}

void OpenMV::calculate(int16_t robotAngle, bool goal, bool role) {
	float xRaw = -1, yRaw = -1;
	float angBlueWithIMU, angYellowWithIMU; 
	if (goal == BLUE_GOAL) robotAngle = adductionMV(robotAngle + 180);
	
	angBlueWithIMU = adductionMV(_angleBlue + robotAngle);
	angYellowWithIMU = adductionMV(_angleYellow + robotAngle);
	
	xYellow = float(_distYellow) * -sin(angYellowWithIMU * DEG2RAD); 
	yYellow = float(_distYellow) * abs(cos(angYellowWithIMU * DEG2RAD));
	xBlue = -float(_distBlue) * sin((180 - angBlueWithIMU) * DEG2RAD);
		
	yBlue = DIST_BETWEEN_GOALS - float(_distBlue) * abs(cos((180 - angBlueWithIMU) * DEG2RAD));
	if (_distBlue && _distYellow && _distBlue + _distYellow < DIST_BETWEEN_GOALS - 40) {
		if (_distBlue < _distYellow) _distYellow = 0;
		else _distBlue = 0;
	}
	
	if (_distYellow * _distBlue) {
		xRaw = (xYellow / float(_distYellow) + xBlue / float(_distBlue)) / (1 / float(_distYellow) + 1 / float(_distBlue));
		yRaw = (yYellow / float(_distYellow) + yBlue / float(_distBlue)) / (1 / float(_distYellow) + 1 / float(_distBlue));
	} else if (!_distYellow) {
		xRaw = xBlue;
		yRaw = yBlue;
	} else if (!_distBlue) {
		xRaw = xYellow;
		yRaw = yYellow;
	}
	
	if (goal == BLUE_GOAL) {
		xRaw *= -1;
		yRaw = DIST_BETWEEN_GOALS - yRaw;
	}
		
	if (!(xRaw == -1 && yRaw == -1)) {
		_x = float(_x) * float(1 - XY_KOEFF) + float(XY_KOEFF) * xRaw;
		_y = float(_y) * float(1 - XY_KOEFF) + float(XY_KOEFF) * yRaw;
	}
}

int16_t OpenMV::getAngleYellow() {
	return _angleYellow;
}

int16_t OpenMV::getAngleBlue() {
	return _angleBlue;
}

int16_t OpenMV::getDistYellow() {
	return _distYellow;
}

int16_t OpenMV::getDistBlue() {
	return _distBlue;
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

int16_t OpenMV::adductionMV(int16_t angleIMU) {
	while (angleIMU > 360) angleIMU -= 360;
  while (angleIMU < 0) angleIMU += 360;
  return angleIMU;
}

void OpenMV::initUSART(uint8_t num) {
	if (num == 1) uart1::usart1Init(460800, 1, 8);
	else if (num == 2) uart2::usart2Init(460800, 1, 8);
	else if (num == 3) uart3::usart3Init(460800, 1, 8);
	else if (num == 6) uart6::usart6Init(460800, 1, 8);
}