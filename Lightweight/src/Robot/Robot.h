#pragma once
#include "libraries.h"

#define IMU_CALIBRATE_TIME 11000
#define TIME_NOT_SEEN 1500
#define USUAL_SPEED 0.55

namespace Robot {
	Vec2b currentVector;
	volatile float ang, angRes;
	volatile bool imuCalibrated;
	volatile int16_t pow;
	volatile float angleIMU, angSoft, distSoft;
	volatile float dist, distOld;
	volatile int t, timeNotSeenBall, timeUpdateQueue;
	volatile int16_t x, y;
	volatile int16_t angRaw;
	volatile int32_t distRaw;
	volatile uint8_t myGoal, myRole;
	volatile int16_t dBl, dYe;
	volatile int16_t angBlue, angYellow;
	volatile int16_t target;
	volatile double speedForward;
	volatile int16_t angleBallGoal;
	
	Pin locatorSCL('A', 8, i2c);
	Pin locatorSDA('C', 9, i2c);
	I2C locatorI2C(locatorSCL, locatorSDA);
	IRLocator locator(locatorI2C, 0x0E);
	
	Pin tx_openMV('B', 10, usart3);
	Pin rx_openMV('B', 11, usart3);
	OpenMV camera(tx_openMV, rx_openMV, 3);
	ProcessingCoord processXY;
	
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
	BallVec2b ball;

	void init(uint8_t goal, uint8_t role) {
		myGoal = goal;
		myRole = role;
		time_service::init();
		time_service::startTime();
		time_service::delay(100);
		
		t = time_service::millis();
		imuCalibrated = false;
		
		angSoft = 0;
		distSoft = 0;
		distOld = -1;

		currentVector.set(USUAL_SPEED, 90);
		timeNotSeenBall = time_service::millis();
		timeUpdateQueue = time_service::millis();
		processXY.setGoal(myGoal);
	
		if (myRole == GOALKEEPER_ROLE) gyro.setTarget(0);
	}
	
	bool calibrated() {
		return imuCalibrated;
	}	
	
	uint8_t getRole() {
		return myRole;
	}

	void update() {
		camera.read();
		angRaw = locator.getAngle();
		distRaw = locator.getDist();
		
		if (distRaw && timeUpdateQueue != time_service::millis()) {
			ball.push(Vec2b(distRaw, angRaw), time_service::millis());
			timeUpdateQueue = time_service::millis();
		}
		
		if (distRaw) {
			ang = ball.getCurrentVec2b().angle;
			dist = ball.getCurrentVec2b().length;
		}
		
		if (abs(dist - distOld) < 3 || distOld == -1) distSoft = 0.03f * dist + 0.97f * distSoft;
		distOld = dist;
			
		gyro.read();
		angleIMU = gyro.getCurrentAngle();
		
		camera.calculate(angleIMU, myGoal, myRole);
		dBl = camera.getDistBlue();
		dYe = camera.getDistYellow();
		x = camera.getX();
		y = camera.getY();
		angYellow = camera._angleYellow;
		angBlue = camera._angleBlue;
		
		processXY.setParams(x, y, angleIMU, camera.getDistBlue(), camera.getDistYellow());
	
		if (!imuCalibrated && time_service::millis() - t > IMU_CALIBRATE_TIME) {
			gyro.setZeroAngle();
			imuCalibrated = true;
		}
		
		if (distRaw) timeNotSeenBall = time_service::millis();
	}
	
	void goToBall() {	
		target = processXY.getTargetForward();
		gyro.setTarget(target);
		gyro.setRotationForTarget();
		pow = gyro.getRotation();
		
		speedForward = USUAL_SPEED;
		if (time_service::millis() - timeNotSeenBall < TIME_NOT_SEEN) {
			angRes = ang + locator.angleOffset(gyro.adduct(ang), distSoft) + 90;
			
			while (angRes > 360) angRes -= 360;
			while (angRes < 0) angRes += 360;
				
			angSoft = gyro.calculateSoft(angSoft, angRes);	
		} else {
			speedForward = 0;
		}
		
		if (time_service::millis() != t) {
			Vec2b goTo(speedForward, double(angSoft));
			currentVector.changeTo(goTo);
			t = time_service::millis();
		}
		
		currentVector = Vec2b(0, 0);//processXY.�heckOUTs(currentVector);
		
		//omni.move(1, currentVector.length, currentVector.angle, pow, gyro.getMaxRotation());
	}
	
	volatile int16_t angc;
	volatile float val;
	void protectGoal() {
		Vec2b vecToCenter = processXY.getVecToGoalCenter();
		Vec2b vecToBall = processXY.getVecToIntersection(ang);
		Vec2b goTo;
		goTo = vecToCenter;//.summ(vecToCenter, vecToCenter);
		
		gyro.setRotationForTarget();
		pow = gyro.getRotation();
		
		if (time_service::millis() != t) {
			currentVector.changeTo(goTo);
			t = time_service::millis();
		}
		
		angc = currentVector.angle;
		val = currentVector.length;
		omni.move(1, currentVector.length, currentVector.angle, pow, gyro.getMaxRotation());
	}
}