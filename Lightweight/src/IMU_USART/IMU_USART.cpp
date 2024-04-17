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

float gyro_imu::adduct0_360(float angle) {
	while (angle < 0) angle += 360;
  while (angle > 360) angle -= 360;
  return angle;
}

void gyro_imu::read() {
	if (_usartNumber == 1 && uart1::available()) {
		_angleNow = adduct(float(uart1::read()) * RECEIVED2REAL - _zeroAngle);
	} else if (_usartNumber == 2 && uart2::available()) {
		_angleNow = adduct(float(uart2::read()) * RECEIVED2REAL - _zeroAngle);
	} else if (_usartNumber == 3 && uart3::available()) {
		_angleNow = adduct(float(uart3::read()) * RECEIVED2REAL - _zeroAngle);
	} else if (_usartNumber == 6 && uart6::available()) {
		_angleNow = adduct(float(uart6::read()) * RECEIVED2REAL - _zeroAngle);
	}
	
	//_angleNow *= -1;
}

void gyro_imu::setRotationForTarget() {
	int16_t err = int16_t(_target) - _angleNow;
	err = adduct(err);
	errOld = err;
	
	_rotation = float(err) * KOEFF_P + float(err - errOld) * KOEFF_D;
}

float gyro_imu::calculateSoft(float soft, float now, uint8_t softParam) {
	float kSoft;
	if (softParam == baseSoft) kSoft = K_SOFT;
	else if (softParam == leavingSoft) kSoft = 0.02f;
	
	if (abs(now - soft) < 180) {
		soft = kSoft * now + (1 - kSoft) * soft;
	} else {
		if (soft > now) soft = K_SOFT * (now + 360) + (1 - kSoft) * soft;
		else soft = kSoft * now + (1 - kSoft) * (soft + 360);
	}
			
	soft = adduct0_360(soft);
	
	return soft;
}

int16_t gyro_imu::getRotation() {
	return _rotation; 
}

void gyro_imu::setTarget(float targetRaw) {
	_target = (1 - TARGET_K_FOFT) * _target + TARGET_K_FOFT * targetRaw;	
}

void gyro_imu::setZeroAngle() {
	switch (_usartNumber) {
		case 1:
			while (!uart1::available());
			_zeroAngle = adduct(float(uart1::read()) * RECEIVED2REAL);
			break;
		case 2:
			while (!uart2::available());
			_zeroAngle = adduct(float(uart2::read()) * RECEIVED2REAL);
			break;
		case 3:
			while (!uart3::available());
			_zeroAngle = adduct(float(uart3::read()) * RECEIVED2REAL);
			break;
		default:
			while (!uart6::available());
			_zeroAngle = adduct(float(uart6::read()) * RECEIVED2REAL);
			break;
	} 
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