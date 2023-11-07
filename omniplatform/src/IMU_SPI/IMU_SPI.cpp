#include "IMU_SPI.h"

void IMU::init() {
	_mpuSensor.initIMU();
	time_service::delay(100);
	
	setZeroAngle(); // IMU calibrated angle estimating
	
	imuFloatTime = time_service::millis();
	imuFloatValue = 0;
	angle = 0;
}

void IMU::setZeroAngle() {
	update();
	zeroAngle = angle;
}


unsigned int IMU::update() {
	updateAnglesFromFIFO();
	double neangle = _mpuSensor.yaw;
	angle = neangle;
	adduction(angle);
	return 0;
}


void IMU::updateAnglesFromFIFO() {
	_mpuSensor.updateAnglesFromFIFO();
}


void IMU::calibrate(uint32_t t) {
	_mpuSensor.calibrate(t);
}

float IMU::get_acceleration() {
  return _mpuSensor._acceleration;
}

double IMU::getAngle() {
	double a = angle-zeroAngle;
	adduction(a)
	return -a;
	//return _mpuSensor.yaw;
}

int IMU::gyro_sel() {
  return _mpuSensor.read_gyro_fs_sel();
}

int IMU::acc_sel() {
  return _mpuSensor.read_acc_fs_sel();
}

double IMU::getXa() {
	return _mpuSensor.getXa();
}

double IMU::getYa() {
	return _mpuSensor.getYa();
}

double IMU::getZa() {
	return _mpuSensor.getZa();
}

double IMU::getXg() {
	return _mpuSensor.getXg();
}

double IMU::getYg() {
	return _mpuSensor.getYg();
}

double IMU::getZg() {
	return _mpuSensor.getZg();
}