#pragma once
#include "libraries.h"

#define IMU_CALIBRATE_TIME 7000
//20000
#define TIME_NOT_SEEN 550
#define TIME_LEAVE 2850
#define TIME_FINISH_LEAVE 2650
#define TIME_GO_FROM_OUT 0

#define USUAL_SPEED 0.7
#define MAX_VEC2B_LEN 0.4

namespace Asterisk {
	Vec2b currentVector, goTo;
	volatile float ang, angRes;
	volatile bool imuCalibrated;
	volatile int16_t pow;
	volatile float angleIMU, angSoft, distSoft;
	volatile float dist, distOld, angOld;
	volatile uint64_t t, timeNotSeenBall, timeUpdateQueue, timeCheckLeave, timeInLeaving;
	volatile uint64_t timeCalibrEnd, timeOUT;
	volatile int16_t x, y;
	volatile int16_t angRaw, angRawOld;
	volatile int16_t distRaw, distRaw2, distSoftOld;
	volatile uint8_t myGoal, myRole;
	volatile int16_t dBl, dYe;
	volatile int16_t angBlue, angYellow;
	volatile float targetRaw, target;
	volatile double speedForward;
	volatile int16_t angleBallGoal, angToGoal;
	volatile bool doesntSeeGoals;
	volatile int16_t ang0_360;
	volatile uint8_t myMode;
	volatile bool seeBall;
	volatile bool robotMustLeave, robotInOutOld;
	volatile bool inLeave, inReturn, goFromOUT, goFromOutTime;
	volatile double kLen, kAng;
	volatile int16_t currLeaveTime;
	volatile uint8_t outStatus, outStatusNow;
	
	Pin locatorSCL('A', 8, i2c);
	Pin locatorSDA('C', 9, i2c);
	I2C locatorI2C(locatorSCL, locatorSDA);
	IRLocator locator(locatorI2C, 0x0E);
	
	Pin tx_openMV('B', 10, usart3);
	Pin rx_openMV('B', 11, usart3);
	OpenMV camera(tx_openMV, rx_openMV, 3);
	
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

	Forward myForward;
	Goalkeeper myGoalkeeper;

	void init(uint8_t goal, uint8_t role, uint8_t mode) {
		myGoal = goal;
		myRole = role;
		time_service::init();
		time_service::startTime();
		time_service::delay(100);
		
		t = time_service::millis();
		imuCalibrated = false;
		target = 0;
		
		angSoft = 0;
		distSoft = 0;
		distOld = -1;
		angOld = -1;
		seeBall = true;
		outStatus = unknow;
		
		doesntSeeGoals = false;
		inLeave = false;
		inReturn = false;
		currLeaveTime = TIME_FINISH_LEAVE;
		robotInOutOld = false;
		goFromOUT = false;
		goFromOutTime = false;
		
		currentVector.set(0, 90);
		timeNotSeenBall = time_service::millis();
		timeUpdateQueue = time_service::millis();
		timeCheckLeave = time_service::millis();
		timeCalibrEnd = 0;
		timeOUT = 0;
		
		if (myRole == GOALKEEPER_ROLE) {
			myGoalkeeper.setGoal(myGoal);
			myGoalkeeper.setMaxLen(MAX_VEC2B_LEN);
			myGoalkeeper.setLeaveTime(TIME_FINISH_LEAVE);
		} else {
			myForward.setGoal(myGoal);
			myForward.setMaxLen(MAX_VEC2B_LEN);
		}
	
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
		
		if (abs(dist - distOld) < 3 || distOld == -1) distSoft = 0.02f * dist + 0.98f * distSoft;
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
			
			if (myRole == GOALKEEPER_ROLE) myGoalkeeper.setParams(x, y, angleIMU, dBl, dYe);
			else myForward.setParams(x, y, angleIMU, dBl, dYe);
		} else doesntSeeGoals = true;
		
		angYellow = camera.getAngleYellow();
		angBlue = camera.getAngleBlue();
	
		if (!imuCalibrated && time_service::millis() - t > IMU_CALIBRATE_TIME) {
			gyro.setZeroAngle();
			imuCalibrated = true;
			timeCalibrEnd = time_service::millis();
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
		//targetRaw = float(processXY.getTargetForward());
		//gyro.setTarget(targetRaw);
		//target = gyro.getTarget();
		gyro.setRotationForTarget();
		pow = gyro.getRotation();
		
		if (time_service::millis() != t) {
			Vec2b goTo = getVec2bToBallFollow();
			
			if (goTo.length > MAX_VEC2B_LEN) goTo.length = MAX_VEC2B_LEN;
			goTo.set(0.25, 270);
			
			currentVector.changeTo(goTo);
			t = time_service::millis();
		}
		
		if (!doesntSeeGoals) {
			outStatus = myForward.checkOUTs();
			
			if (!goFromOUT && !goFromOutTime && myForward.robotInOUT()) {
				goFromOUT = true;
				outStatusNow = outStatus;
			} else if (goFromOUT || goFromOutTime) {
				currentVector = myForward.setOUTVector(outStatusNow, currentVector);
				
				if (goFromOUT && !goFromOutTime && myForward.robotInFreeField()) {
					goFromOUT = false;
					goFromOutTime = true;
					timeOUT = time_service::millis();
				} else if (!goFromOUT && goFromOutTime && time_service::millis() - timeOUT >= TIME_GO_FROM_OUT) {
					goFromOutTime = false;
				}
				
				if (outStatus != unknow && outStatus != outStatusNow) {
					goFromOUT = false;
					goFromOutTime = false;
					timeOUT = 0;
				}
			} 
		} else {
			goFromOUT = false;
			goFromOutTime = false;
			myForward.resetCounts();
			timeOUT = 0;
		}
		
		if (myMode == P_MODE) omni.move(1, currentVector.length, currentVector.angle, pow, gyro.getMaxRotation());
	}
	
	bool mustLeave() {
		//0.073 0.015
		if (time_service::millis() - timeCalibrEnd < 3000 || !seeBall || 
				(seeBall && !(abs(kAng) < 0.75 && abs(kLen) < 0.023))) timeCheckLeave = time_service::millis();
		
		return time_service::millis() - timeCheckLeave > TIME_LEAVE;
	}

	void protectGoal() {
		gyro.setRotationForTarget();
		pow = gyro.getRotation();

		//if (!doesntSeeGoals) {
		robotMustLeave = mustLeave();
		if (!robotMustLeave && !inLeave && !inReturn) {
			Vec2b vecToBall, vecToCenter;
			angToGoal = int16_t(RAD2DEG * atan2(float(y), float(x)));
			ang0_360 = ang + 90;
			while (ang0_360 > 360) ang0_360 -= 360;
			while (ang0_360 < 0) ang0_360 += 360;
			
			if (!locator.distBad(distSoft) && seeBall) vecToBall = myGoalkeeper.getVecToIntersection(ang0_360);
			else if (!seeBall) {
				vecToBall = myGoalkeeper.getVecToPoint();
			} else vecToBall = Vec2b(0, 0);
			
			vecToCenter = myGoalkeeper.getVecToGoalCenter();
			vecToCenter.length *= myGoalkeeper.getCoeffToGoalCenter(vecToBall.length);
		
			goTo = vecToCenter + vecToBall;
		} else if (robotMustLeave && !inLeave && !inReturn) {
			inLeave = true;
			timeInLeaving = time_service::millis();
			currLeaveTime = myGoalkeeper.getCurrentLeaveTime();
		} else if (inLeave) {
			goTo = getVec2bToBallFollow();
			if (time_service::millis() - timeInLeaving > currLeaveTime 
				|| sqrt(float(x * x + y * y)) > 0.27 * DIST_BETWEEN_GOALS || doesntSeeGoals) {
				inLeave = false;
				inReturn = true;
			}
		} else if (inReturn) {
			if (!doesntSeeGoals) goTo = myGoalkeeper.getVecToReturn();
				
			if (myGoalkeeper.changeFromReturn()) {
				inReturn = false;
				inLeave = false;
				timeCheckLeave = time_service::millis();
			}
		}
		
		if (goTo.length > MAX_VEC2B_LEN) goTo.length = MAX_VEC2B_LEN;
		
		if (!doesntSeeGoals && time_service::millis() != t) {
			currentVector.changeTo(goTo);
			t = time_service::millis();
		}
		
		if (doesntSeeGoals) {
			if (!inReturn) currentVector = Vec2b(0.2, 90);
			else if (y > 40) currentVector = Vec2b(0.5, 270);
			//currentVector.set(goTo.length, adduct180_360(180 - goTo.angle));
		}
			
		if (myMode == P_MODE) omni.move(1, currentVector.length, currentVector.angle, pow, gyro.getMaxRotation());
	}
}