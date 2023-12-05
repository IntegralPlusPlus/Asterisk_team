#include "Pin.h"
#include "usart1.h"
#include "usart2.h"
#include "usart3.h"
#include "usart6.h"
#include "adc.h"
#include "dma.h"
#include "time_service.h"
#include "project_config.h"
#include "Motor.h"
#include "SPI_1.h"
#include "SPI_DOT.h"
#include "mpu9250_spi.h"
#include "IMU_SPI.h"
#include "omniplatform.h"
#include "IMU_USART.h"
#include "OpenMV.h"
#include "TSOP.h"
#include "I2C.h"
#include "IR_Locator.h"
#include "Vec2b.h"
#include "usable.h"

#define IMU_CALIBRATE_TIME 10000
//18000
#define K_ANGLE 0.004f

int main() {
	time_service::init();
	time_service::startTime();
  time_service::delay(100);
	
	Pin VLXscl('A', 8, i2c);
	Pin VLXsda('C', 9, i2c);
	I2C locatorI2C(VLXscl, VLXsda);
	IRLocator locator(locatorI2C, 0x0E);
	
	Pin tx_imu('C', 6, usart6);
	Pin rx_imu('C', 7, usart6);
	gyro_imu gyro(tx_imu, rx_imu, 6);
	
	Pin motor1in1('E', 5, tim9);
	Pin motor1in2('E', 6, tim9);
	motor1in1.pwmInit(2048, 1, 1);
	motor1in2.pwmInit(2048, 1, 2);
	Motor motor1(motor1in1, motor1in2);
	
	Pin motor2in1('B', 3, tim2);
	Pin motor2in2('B', 5, tim3);
	motor2in1.pwmInit(2048, 1, 2);
	motor2in2.pwmInit(2048, 1, 2);
	Motor motor2(motor2in1, motor2in2);
	
	Pin motor3in1('A', 0, tim5);
	Pin motor3in2('A', 1, tim5);
	motor3in1.pwmInit(2048, 1, 1);
	motor3in2.pwmInit(2048, 1, 2);
	Motor motor3(motor3in1, motor3in2);
	
	Pin motor4in1('E', 11, tim1);
	Pin motor4in2('E', 13, tim1);
	motor4in1.pwmInit(2048, 1, 2);
	motor4in2.pwmInit(2048, 1, 3);
	Motor motor4(motor4in1, motor4in2);
	
	omniplatform omni(motor3, motor4, motor2, motor1);
	
	volatile float ang, angRes;
	int t = time_service::millis();
	volatile bool imuCalibrated = false;
	volatile int16_t pow;
	volatile float angleIMU, angSoft = 0, distSoft = 0;
	volatile float dist, distOld = -1;
	volatile double angle, speed;
	volatile double x1, y1, x2, y2, len, lyambda;
	
	Vec2b currentVector, goTo;
	currentVector.length = double(55) / double(100);
	currentVector.angle = 90;
	
	while (true) {
		ang = locator.getAngle();
		dist = locator.getDist();
		if (abs(dist - distOld) < 5 || distOld == -1) distSoft = 0.02f * dist + 0.98f * distSoft;
		distOld = dist;
		
		gyro.read();
		angleIMU = gyro.getCurrentAngle();
		
		if (!imuCalibrated && time_service::millis() - t > IMU_CALIBRATE_TIME) { //20000
			gyro.setTarget(0);
			gyro.setZeroAngle();
			imuCalibrated = true;
			t = time_service::millis();
		} else if (imuCalibrated) {
			gyro.setRotationForTarget();
			pow = gyro.getRotation();
			
			angRes = ang + locator.angleOffset(gyro.adduct(ang), distSoft) - 90;
			angRes *= -1;
			while (angRes < 0) angRes += 360;
			
			if (abs(angRes - angSoft) < 180) {
				angSoft = K_ANGLE * angRes + (1 - K_ANGLE) * angSoft;
			} else {
				if (angSoft > angRes) angSoft = K_ANGLE * (angRes + 360) + (1 - K_ANGLE) * angSoft;
				else angSoft = K_ANGLE * angRes + (1 - K_ANGLE) * (angSoft + 360);
			}
			
			if (angSoft < 0) angSoft += 360;
			else if (angSoft > 360) angSoft -= 360;
			
			if (time_service::millis() != t) {
				goTo.length = double(55) / double(100);
				goTo.angle = double(angSoft);
				currentVector.update(goTo);
				//currentVector.length = goTo.length;
				//currentVector.angle = goTo.angle;
				t = time_service::millis();
			}
			
			angle = currentVector.angle;
			speed = currentVector.length;
			omni.move(1, currentVector.length, currentVector.angle, pow, gyro.getMaxRotation());	
		}
	}
	
	return 0;
}