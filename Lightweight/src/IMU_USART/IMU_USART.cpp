#include "IMU_USART.h"

gyro_imu::gyro_imu(Pin &tx, Pin &rx, uint8_t usartNum) {
	initUSART(usartNum);
	_usartNumber = usartNum;
	
	_zeroAngle = 0;
	_target = 0;
	errOld = 0;
}

int16_t gyro_imu::adduct(int16_t ang) {
	while (ang > 180) ang -= 360;
  while (ang < -180) ang += 360;
  return ang;
}

void gyro_imu::read() {
	if (_usartNumber == 1 && uart1::available()) _angleNow = adduct(float(uart1::read()) * RECEIVED2REAL - _zeroAngle);
	else if (_usartNumber == 2 && uart2::available()) _angleNow = adduct(float(uart2::read()) * RECEIVED2REAL - _zeroAngle);
	else if (_usartNumber == 3 && uart3::available()) _angleNow = adduct(float(uart3::read()) * RECEIVED2REAL - _zeroAngle);
	else if (_usartNumber == 6 && uart6::available()) _angleNow = adduct(float(uart6::read()) * RECEIVED2REAL - _zeroAngle);
}

void gyro_imu::setRotationForTarget() {
	int err = -(_target - _angleNow);
	if (err > 180) err -= 360;
	else if (err < -180) err += 360;
	
	_rotation = float(err) * KOEFF_P;// + float(err - errOld) * KOEFF_D;
	
	//if (_rotation > MAXROTATION) _rotation = MAXROTATION;
	//else if (_rotation < -MAXROTATION) _rotation = -MAXROTATION;
	
	errOld = err;
}

float gyro_imu::calculateSoft(float soft, float now, float koeff) {
	if (abs(now - soft) < 180) {
		soft = koeff * now + (1 - koeff) * soft;
	} else {
		if (soft > now) soft = koeff * (now + 360) + (1 - koeff) * soft;
		else soft = koeff * now + (1 - koeff) * (soft + 360);
	}
			
	if (soft < 0) soft += 360;
	else if (soft > 360) soft -= 360;
	
	return soft;
}

int16_t gyro_imu::getRotation() {
	return _rotation; 
}

void gyro_imu::setTarget(int16_t target) {
	_target = target;
}

void gyro_imu::setZeroAngle() {
	_zeroAngle = _angleNow;
}

int16_t gyro_imu::getCurrentAngle() {
	return _angleNow;
}

int16_t gyro_imu::getTarget() {
	return _target;
}


int16_t gyro_imu::getMaxRotation() {
	return MAXROTATION;
}

void gyro_imu::initUSART(uint8_t num) {
	if (num == 1) uart1::usart1Init(115200, 1, 8);
	else if (num == 2) uart2::usart2Init(115200, 1, 8);
	else if (num == 3) uart3::usart3Init(115200, 1, 8);
	else if (num == 6) uart6::usart6Init(115200, 1, 8);
}