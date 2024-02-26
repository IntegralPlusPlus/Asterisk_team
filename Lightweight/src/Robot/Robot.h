#pragma once
#include "libraries.h"

#define IMU_CALIBRATE_TIME 10000
//20000
#define TIME_NOT_SEEN 750
#define USUAL_SPEED 0.55
#define MAX_VEC2B_LEN 0.91
#define TIME_LEAVE 2200
#define TIME_FINISH_LEAVE 2500

namespace Asterisk {
	Vec2b currentVector;
	volatile float ang, angRes;
	volatile bool imuCalibrated;
	volatile int16_t pow;
	volatile float angleIMU, angSoft, distSoft;
	volatile float dist, distOld, angOld;
	volatile uint32_t t, timeNotSeenBall, timeUpdateQueue, timeCheckLeave, timeInLeaving;
	volatile int16_t x, y;
	volatile int16_t angRaw, angRawOld;
	volatile int32_t distRaw, distSoftOld;
	volatile uint8_t myGoal, myRole;
	volatile int16_t dBl, dYe;
	volatile int16_t angBlue, angYellow;
	volatile int16_t target;
	volatile double speedForward;
	volatile int16_t angleBallGoal, angToGoal;
	volatile bool doesntSeeGoals = false;
	volatile int16_t ang0_360;
	volatile uint8_t myMode;
	volatile bool seeBall;
	volatile bool inLeave, inReturn;
	volatile double kLen, kAng;
	
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

	void init(uint8_t goal, uint8_t role, uint8_t mode) {
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
		angOld = -1;
		seeBall = true;
		
		inLeave = false;
		inReturn = false;
		
		currentVector.set(0, 90);
		timeNotSeenBall = time_service::millis();
		timeUpdateQueue = time_service::millis();
		timeCheckLeave = time_service::millis();
		processXY.setGoal(myGoal);
		processXY.setMaxLen(MAX_VEC2B_LEN);
	
		if (myRole == GOALKEEPER_ROLE) gyro.setTarget(0);
		myMode = mode;
	}
	
	bool calibrated() {
		return imuCalibrated;
	}	
	
	uint8_t getRole() {
		return myRole;
	}

	void update() {
		camera.read();
		angRawOld = angRaw;
		angRaw = locator.getAngle();
		distRaw = locator.getDist();
		
		if (distRaw && timeUpdateQueue != time_service::millis()) {
			ball.push(Vec2b(distRaw, angRaw), time_service::millis());
			timeUpdateQueue = time_service::millis();
		}
		
		if (!locator.distBad(distRaw)) {
			ang = ball.getCurrentVec2b().angle;
			dist = ball.getCurrentVec2b().length;
		}
		
		kAng = ball.getDerivativeAng();
		kLen = ball.getDerivativeDist();
		
		if (abs(dist - distOld) < 3 || distOld == -1) distSoft = 0.03f * dist + 0.97f * distSoft;
		distOld = dist;
		distSoftOld = distSoft;
			
		gyro.read();
		angleIMU = gyro.getCurrentAngle();
		
		camera.calculate(angleIMU, myGoal, myRole);
		dBl = camera.getDistBlue();
		dYe = camera.getDistYellow();
		if ((myRole == FORWARD_ROLE && !(dBl == 0 && dYe == 0)) ||
			 (myRole == GOALKEEPER_ROLE && ((myGoal == YELLOW_GOAL && dYe) || (myGoal == BLUE_GOAL && dBl)))) {
			x = camera.getX();
			y = camera.getY();
			doesntSeeGoals = false;
			processXY.setParams(x, y, angleIMU, dBl, dYe);
		} else doesntSeeGoals = true;
		
		angYellow = camera.getAngleYellow();
		angBlue = camera.getAngleBlue();
	
		if (!imuCalibrated && time_service::millis() - t > IMU_CALIBRATE_TIME) {
			gyro.setZeroAngle();
			imuCalibrated = true;
		}
		
		if (!locator.distBad(distRaw)) timeNotSeenBall = time_service::millis();
		seeBall = time_service::millis() - timeNotSeenBall < TIME_NOT_SEEN;
	}
	
	Vec2b getVec2bToBallFollow() {
		speedForward = USUAL_SPEED;
		if (seeBall) {
			angRes = ang + locator.angleOffset(gyro.adduct(ang), distSoft) + 90;
			
			while (angRes > 360) angRes -= 360;
			while (angRes < 0) angRes += 360;
				
			angSoft = gyro.calculateSoft(angSoft, angRes);	
		} else {
			speedForward = 0;
		}
		
		return Vec2b(speedForward, angSoft);
	}
	
	void goToBall() {	
		target = processXY.getTargetForward();
		gyro.setTarget(target);
		gyro.setRotationForTarget();
		pow = gyro.getRotation();
		
		if (time_service::millis() != t) {
			Vec2b goTo = getVec2bToBallFollow();
			currentVector.changeTo(goTo);
			t = time_service::millis();
		}
		
		//currentVector = Vec2b(0, 0);//processXY.�heckOUTs(currentVector);
		
		if (myMode == P_MODE) omni.move(1, currentVector.length, currentVector.angle, pow, gyro.getMaxRotation());
	}
	
	bool mustLeave() {
		if (seeBall && !(kAng < 0.0015 && kLen < 0.0015)) timeCheckLeave = time_service::millis();
		
		return time_service::millis() - timeCheckLeave > TIME_LEAVE;
	}

	void protectGoal() {
		Vec2b goTo;
		gyro.setRotationForTarget();
		pow = gyro.getRotation();

		if (!doesntSeeGoals) {
			bool robotMustLeave = mustLeave();
			if (!robotMustLeave && !inLeave && !inReturn) {
				Vec2b vecToBall, vecToCenter;
				angToGoal = int16_t(RAD2DEG * atan2(float(y), float(x)));
				ang0_360 = ang + 90;
				while (ang0_360 > 360) ang0_360 -= 360;
				while (ang0_360 < 0) ang0_360 += 360;
				
				if (!locator.distBad(dist) && seeBall) vecToBall = processXY.getVecToIntersection(ang0_360);
				else if (!seeBall) {
					vecToBall = processXY.getVecToPoint();
				} else vecToBall = Vec2b(0, 0);
					
				vecToCenter = processXY.getVecToGoalCenter();
				vecToCenter.length *= processXY.getCoeffToGoalCenter(vecToBall.length);
			
				goTo = vecToCenter + vecToBall;
			} else if (robotMustLeave && !inLeave && !inReturn) {
				inLeave = true;
				timeInLeaving = time_service::millis();
			} else if (inLeave) {
				goTo = getVec2bToBallFollow();
				if (time_service::millis() - timeInLeaving > TIME_FINISH_LEAVE 
					|| sqrt(float(x * x + y * y)) > 0.22 * DIST_BETWEEN_GOALS || doesntSeeGoals) {
					inLeave = false;
					inReturn = true;
				}
			} else if (inReturn) {
				if (!doesntSeeGoals) goTo = processXY.getVecToReturn();
					
				if (processXY.changeFromReturn()) {
					inReturn = false;
					timeCheckLeave = time_service::millis();
				}
			}
			
			if (goTo.length > MAX_VEC2B_LEN) goTo.length = MAX_VEC2B_LEN;
			
			if (time_service::millis() != t) {
				currentVector.changeTo(goTo);
				t = time_service::millis();
			}
		} else if (doesntSeeGoals) {
			if (!inReturn) currentVector = Vec2b(0.2, 90);
			else currentVector = Vec2b(0.5, 270);
		}
			
		if (myMode == P_MODE) omni.move(1, currentVector.length, currentVector.angle, pow, gyro.getMaxRotation());
	}
}