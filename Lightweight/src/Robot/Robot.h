#pragma once
#include "project_config.h"
#include "Pin.h"
#include "usart1.h"
#include "usart2.h"
#include "usart3.h"
#include "usart6.h"
#include "adc.h"
#include "dma.h"
#include "time_service.h"
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

#define IMU_CALIBRATE_TIME 11000
#define TIME_NOT_SEEN 1500
#define K_ANGLE 0.01f

namespace Robot {
	Vec2b currentVector;
	volatile float ang, angRes;
	volatile bool imuCalibrated;
	volatile int16_t pow;
	volatile float angleIMU, angSoft, distSoft;
	volatile float dist, distOld;
	volatile int t, timeNotSeenBall;
	
	Pin locatorSCL('A', 8, i2c);
	Pin locatorSDA('C', 9, i2c);
	I2C locatorI2C(locatorSCL, locatorSDA);
	IRLocator locator(locatorI2C, 0x0E);
	
	Pin tx_imu('C', 6, usart6);
	Pin rx_imu('C', 7, usart6);
	gyro_imu gyro(tx_imu, rx_imu, 6);
	
	Pin motor1in1('E', 5, tim9);
	Pin motor1in2('E', 6, tim9);
	Motor motor1(motor1in1, 1, motor1in2, 2);
	
	Pin motor2in1('B', 3, tim2);
	Pin motor2in2('B', 5, tim3);
	Motor motor2(motor2in1, 2, motor2in2, 2);
	
	Pin motor3in1('A', 0, tim5);
	Pin motor3in2('A', 1, tim5);
	Motor motor3(motor3in1, 1, motor3in2, 2);
	
	Pin motor4in1('E', 11, tim1);
	Pin motor4in2('E', 13, tim1);
	Motor motor4(motor4in1, 2, motor4in2, 3);
	
	omniplatform omni(motor3, motor4, motor2, motor1);

	void init() {
		time_service::init();
		time_service::startTime();
		time_service::delay(100);
		
		t = time_service::millis();
		imuCalibrated = false;
		
		angSoft = 0;
		distSoft = 0;
		distOld = -1;

		currentVector.set(double(55) / double(100), 90);
		timeNotSeenBall = time_service::millis();
	}

	void updateSensors() {
		ang = locator.getAngle();
		dist = locator.getDist();
		gyro.read();
		
		angleIMU = gyro.getCurrentAngle();
		if (abs(dist - distOld) < 3 || distOld == -1) distSoft = 0.03f * dist + 0.97f * distSoft;
		distOld = dist;
		
		gyro.setRotationForTarget();
		pow = gyro.getRotation();
		
		if (!imuCalibrated && time_service::millis() - t > IMU_CALIBRATE_TIME) {
			gyro.setZeroAngle();
			imuCalibrated = true;
		}
		
		if (dist != 0) timeNotSeenBall = time_service::millis();
	}

	void goToBall() {
		double goToLen = 55 * 0.01; //55 * 0.01
		if (time_service::millis() - timeNotSeenBall < TIME_NOT_SEEN) {
			angRes = ang + locator.angleOffset(gyro.adduct(ang), distSoft) - 90;
			angRes *= -1;
			while (angRes < 0) angRes += 360;
				
			angSoft = gyro.calculateSoft(angSoft, angRes, K_ANGLE);	
		} else {
			goToLen = 0;
		}
		
		if (time_service::millis() != t) {
			Vec2b goTo(goToLen, double(angSoft));
			currentVector.changeTo(goTo);
			t = time_service::millis();
		}
		
		omni.move(1, currentVector.length, currentVector.angle, pow, gyro.getMaxRotation());
	}

	bool calibrated() {
		return imuCalibrated;
	}	
}