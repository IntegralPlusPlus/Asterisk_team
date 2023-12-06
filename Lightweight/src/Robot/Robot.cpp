/*#include "Robot.h"

Robot::init(): 
	locatorSCL('A', 8, i2c),
	locatorSDA('C', 9, i2c),
	locatorI2C(locatorSCL, locatorSDA),
	locator(locatorI2C, 0x0E),
	tx_imu('C', 6, usart6),
	rx_imu('C', 7, usart6),
	gyro(tx_imu, rx_imu, 6),
	motor1in1('E', 5, tim9), 
	motor1in2('E', 6, tim9),
	motor1(motor1in1, 1, motor1in2, 2),
	motor2in1('B', 3, tim2),
	motor2in2('B', 5, tim3),
	motor2(motor2in1, 2, motor2in2, 2),
	motor3in1('A', 0, tim5),
	motor3in2('A', 1, tim5),
	motor3(motor3in1, 1, motor3in2, 2),
	motor4in1('E', 11, tim1),
	motor4in2('E', 13, tim1),
	motor4(motor4in1, 2, motor4in2, 3),
	omni(motor3, motor4, motor2, motor1)
{
	time_service::init();
	time_service::startTime();
  time_service::delay(100);
	
	t = time_service::millis();
	imuCalibrated = false;
	
	angSoft = 0;
	distSoft = 0;
	distOld = -1;

	currentVector.set(double(55) / double(100), 90);
}


void Robot::updateSensors() {
	ang = locator.getAngle();
	dist = locator.getDist();
	gyro.read();
	
	angleIMU = gyro.getCurrentAngle();
	if (abs(dist - distOld) < 5 || distOld == -1) distSoft = 0.05f * dist + 0.95f * distSoft;
	distOld = dist;
	
	gyro.setRotationForTarget();
	pow = gyro.getRotation();
	
	if (!imuCalibrated && time_service::millis() - t > IMU_CALIBRATE_TIME) {
		gyro.setZeroAngle();
		imuCalibrated = true;
	}
}

void Robot::goToBall() {
	angRes = ang + locator.angleOffset(gyro.adduct(ang), distSoft) - 90;
	angRes *= -1;
	while (angRes < 0) angRes += 360;
		
	//angSoft = gyro.calculateSoft(angSoft, angRes, K_ANGLE);
	angSoft = angRes;
	
	if (time_service::millis() != t) {
		Vec2b goTo(double(55) / double(100), double(angSoft));
		currentVector.changeTo(goTo);
		t = time_service::millis();
	}

	omni.move(1, currentVector.length, currentVector.angle, pow, gyro.getMaxRotation());	
}

bool Robot::calibrated() {
	return imuCalibrated;
}*/