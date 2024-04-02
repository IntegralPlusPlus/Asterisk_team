#pragma once
#include "libraries.h"

#define IMU_CALIBRATE_TIME 40000
//20000
#define NEED_TO_CALIBRATE 0
#define TIME_NOT_SEEN 450
#define TIME_LEAVE 2850
#define TIME_FINISH_LEAVE 2650
#define TIME_GO_FROM_OUT 0

#define USUAL_FOLLOWING_SPEED 0.5
//0.67
#define MAX_VEC2B_LEN 0.87
//0.87

namespace Asterisk {
	Vec2b currentVector;
	Vec2b goToBall, goOUT, goTo;
	volatile float ang, angRes;
	volatile bool imuCalibrated;
	volatile int16_t pow;
	volatile float angleIMU, angSoft, distSoft;
	volatile float dist, distOld, angOld;
	volatile uint64_t t, timeNotSeenBall, timeUpdateQueue, timeCheckLeave, timeInLeaving;
	volatile uint64_t timeCalibrEnd, timeOUT;
	volatile int16_t x, y;
	volatile float angRaw, angRawOld;
	volatile float distRaw, distRaw2, distSoftOld;
	volatile uint8_t myGoal, myRole;
	volatile int16_t dBl, dYe;
	volatile int16_t angBlue, angYellow;
	volatile float targetRaw, target;
	volatile double speedForward;
	volatile int16_t angleBallGoal, angToGoal;
	volatile bool doesntSeeGoals;
	volatile int16_t ang0_360, angY;
	volatile uint8_t myMode;
	volatile bool seeBall, motorsWork, neverTurnMotors;
	volatile bool robotMustLeave, robotInOutOld;
	volatile bool inLeave, inReturn, goFromOUT, goFromOutTime;
	volatile double kLen, kAng, volt;
	volatile int16_t currLeaveTime;
	volatile uint8_t outStatus, outStatusNow;
	volatile bool tssps_[32];
	volatile bool robotInOUT;
	volatile float xYellow, yYellow, xBlue, yBlue; 
	volatile bool nearOUT = false;
	
	Pin ballSensPin('A', 4, adc);
	Adc ballSensADC(ADC1, 1, 4);
	Dma ballSens(RCC_AHB1Periph_DMA2, ballSensADC, DMA2_Stream0, DMA_Channel_0);

	Pin capacitorPin('C', 0, adc);
	Adc capacitorPinADC(ADC1, 1, 10);
	Dma capacitor(RCC_AHB1Periph_DMA2, capacitorPinADC, DMA2_Stream4, DMA_Channel_0);

	Pin voltageDividerPin('A', 5, adc);
	Adc voltageDividerPinADC(ADC2, 1, 5);
	Dma voltageDividerDMA(RCC_AHB1Periph_DMA2, voltageDividerPinADC, DMA2_Stream2, DMA_Channel_1);
	VoltageDividor voltageDiv(voltageDividerDMA);

	Pin tsop_in1('D', 11, write_pupd_down);
	Pin tsop_in2('D', 10, write_pupd_down);
	Pin tsop_in3('D', 9, write_pupd_down);
	Pin tsop_in4('D', 8, write_pupd_down);

	Pin tsopPin1('C', 2, read);
	Pin tsopPin2('C', 1, read);

	TSOP tsops(tsop_in1, tsop_in2, tsop_in3, tsop_in4, tsopPin1, tsopPin2);

	//Pin tx_openMV('C', 6, usart6);
	//Pin rx_openMV('C', 7, usart6);
	//OpenMV camera(tx_openMV, rx_openMV, 6);
	Pin tx_openMV('B', 10, usart3);
	Pin rx_openMV('B', 11, usart3);
	OpenMV camera(tx_openMV, rx_openMV, 3);

	Pin reset_imu_pin('A', 0, write_pupd_down);
	Pin tx_imu('B', 6, usart1);
	Pin rx_imu('B', 7, usart1);
	gyro_imu gyro(tx_imu, rx_imu, 1);

	Pin kicker('A', 8, write_pupd_down);

	Pin button1('D', 1, read_pupd_down);
	Pin button2('D', 0, read_pupd_down);
	Pin button3('A', 9, read_pupd_down);
	Button butt1(button1, true);
	Button butt2(button2);
	Button butt3(button3);
	Button resetIMU(reset_imu_pin);
	
	Pin swMotorPower('D', 2, read_pupd_down);
	Pin swGoalChoose('E', 4, read_pupd_down);
	Pin swRoleChoose('E', 5, read_pupd_down);
	Pin switcher('D', 3, read_pupd_down);

	Pin gkLED('C', 13, write);
	Pin forwLED('E', 3, write);
	Pin blueGoalLED('E', 2, write);
	Pin yellowGoalLED('E', 6, write);
	Pin led1('A', 12, write);
	Pin led2('A', 11, write);
	Pin led3('A', 10, write);

	Pin motor1in1('B', 9, tim4);
	Pin motor1in2('B', 8, tim4);
	Motor motor1(motor1in1, 4, motor1in2, 3);

	Pin motor2in1('C', 8, tim8);
	Pin motor2in2('C', 9, tim8);
	Motor motor2(motor2in1, 3, motor2in2, 4);

	Pin motor3in1('A', 7, tim14);
	Pin motor3in2('A', 6, tim13);
	Motor motor3(motor3in1, 1, motor3in2, 1);

	Pin motor4in1('B', 1, tim3);
	Pin motor4in2('B', 0, tim3);
	Motor motor4(motor4in1, 4, motor4in2, 3);

	omniplatform omni(motor4, motor1, motor2, motor3);
	BallVec2b ball;

	Forward myForward;
	Goalkeeper myGoalkeeper;
	
	bool calibrated() {
		return imuCalibrated;
	}	
	
	uint8_t getRole() {
		return myRole;
	}

	void update() {
		camera.read();
		tsops.updateTSOPs();
		//for (uint8_t i = 0; i < 32; ++i) tssps_[i] = tsops.tsopValues[i];

		tsops.calculate();
		angRaw = tsops.getAngle();
		distRaw = tsops.getDist();
		
		if (distRaw && timeUpdateQueue != time_service::millis()) {
			ball.push(Vec2b(distRaw, angRaw), time_service::millis());
			timeUpdateQueue = time_service::millis();
		}
		
		//ang = angRaw;
		//dist = distRaw;
		
		gkLED.set(swRoleChoose.readPin());
		forwLED.set(!swRoleChoose.readPin());
		
		//myGoal = !swGoalChoose.readPin();
		blueGoalLED.set(!swGoalChoose.readPin());
		yellowGoalLED.set(swGoalChoose.readPin());
		//myGoalkeeper.setGoal(myGoal);
		//myForward.setGoal(myGoal);
		
		motorsWork = butt1.pressed();//swMotorPower.readPin();
		
		if (!tsops.distBad(distRaw)) {
			ang = ball.getCurrentVec2b().angle;
			dist = ball.getCurrentVec2b().length;
			timeNotSeenBall = time_service::millis();
		}
		
		kAng = ball.getDerivativeAng();
		kLen = ball.getDerivativeDist();
		
		gyro.read();
		angleIMU = -gyro.getCurrentAngle();
		
		camera.calculate(angleIMU, myGoal, myRole);
		xYellow = camera.xYellow;
		yYellow = camera.yYellow;
		xBlue = camera.xBlue;
		yBlue = camera.yBlue;
		
		dBl = camera.getDistBlue();
		dYe = camera.getDistYellow();
		angY = camera._angleYellow;
		volt = voltageDiv.getVoltage();
		if ((myRole == FORWARD_ROLE && !(!dBl && !dYe)) ||
			 (myRole == GOALKEEPER_ROLE && ((myGoal == YELLOW_GOAL && dYe) || (myGoal == BLUE_GOAL && dBl)))) {
			x = camera.getX();
			y = camera.getY();
			doesntSeeGoals = false;
			
			if (myRole == GOALKEEPER_ROLE) myGoalkeeper.setParams(x, y, angleIMU, dBl, dYe);
			else myForward.setParams(x, y, angleIMU, dBl, dYe);
		} else doesntSeeGoals = true;
		
		angYellow = camera.getAngleYellow();
		angBlue = camera.getAngleBlue();
		
		if (!motorsWork && voltageDiv.voltageLow(volt)) neverTurnMotors = true;
		
		if (abs(dist - distOld) < 1.5f || distOld == -1) distSoft = 0.02f * dist + 0.98f * distSoft;
		distOld = dist;
		distSoftOld = distSoft;
		
		if (!imuCalibrated && (time_service::millis() - t > IMU_CALIBRATE_TIME 
													|| !NEED_TO_CALIBRATE)) {
			gyro.setZeroAngle();
			imuCalibrated = true;
			timeCalibrEnd = time_service::millis();
		}
		
		seeBall = time_service::millis() - timeNotSeenBall < TIME_NOT_SEEN;
		
		if (reset_imu_pin.readPin()) {
			omni.disable();
			gyro.setZeroAngle();
		}
		
		led1.set(imuCalibrated);
		led2.set(abs(angleIMU) <= 5);
	}
	
	Vec2b getVec2bToBallFollow() {
		speedForward = USUAL_FOLLOWING_SPEED;
		if (seeBall) {
			float offset = tsops.angleOffset(ang, distSoft, angleIMU);
			if (dist < 6) offset = 0;
			else if (dist > 9.17 && abs(ang) > 25) {
				speedForward *= 0.9;
				//offset *= 0.8;
			}
			
			angRes = ang + offset + 90;
			
			while (angRes > 360) angRes -= 360;
			while (angRes < 0) angRes += 360;
				
			angSoft = gyro.calculateSoft(angSoft, angRes);	
		} else {
			speedForward = 0;
			angSoft = 0;
		}
		
		if (abs(ang) <= 10 && dist > 9.4) { 
			angSoft = myForward.adduct(myForward.getTargetForward() + 90);
			speedForward *= 1.25;
		}
		
		return Vec2b(speedForward, gyro.adduct0_360(angSoft));
	}
	
	void forwardStrategy() {
		if (!doesntSeeGoals) targetRaw = float(myForward.getTargetForward());
		gyro.setTarget(targetRaw);
		
		gyro.setRotationForTarget();
		pow = gyro.getRotation();
		led3.set(abs(float(pow)) <= 85);
		
		goToBall = getVec2bToBallFollow();
		
		if (!doesntSeeGoals) {
			outStatus = myForward.checkOUTs();
			robotInOUT = myForward.robotInOUT();
			
			if (!goFromOUT && myForward.robotInOUT()) {
				goFromOUT = true;
				outStatusNow = outStatus;
			} else if (goFromOUT) {
				goOUT = myForward.setOUTVector(outStatus, currentVector); //outStatusNow
				
				if (goFromOUT && myForward.robotInFreeField()) {
					goFromOUT = false;
					timeOUT = time_service::millis();
				} 
				
				if (outStatus == unknow) {// && outStatus != outStatusNow) {
					outStatusNow = outStatus;
					goFromOUT = false;
					timeOUT = 0;
				} else if (outStatus != outStatusNow) outStatusNow = outStatus;
			} 
		} else myForward.resetCounts();
		
		if (time_service::millis() != t) {
			if (doesntSeeGoals) goTo = myForward.getVecToPoint(0, DIST_BETWEEN_GOALS / 2);
			else if (!robotInOUT) {
				float globalBall = myForward.adduct180(ang - angleIMU);
				if ((myForward.robotNearOUTSides() || myForward.robotNearOUTUpDown())
						&& globalBall >= -90 && globalBall <= 90) {
					float speed = USUAL_FOLLOWING_SPEED;
					if (myForward.robotNearOUTSides()) speed *= 0.86;
					else if (myForward.robotNearOUTUpDown()) speed *= 0.68;
							
					goTo = Vec2b(speed, ang + 90);
				} else if (myForward.nearMyGoal()) { 
					goTo = myForward.vec2bOnGoal(USUAL_FOLLOWING_SPEED, ang);
				} else {
					goTo = goToBall;
				}
			} else if (robotInOUT) {
				if (myForward.inEnemyGoal()) {
					goTo = goToBall + goOUT;
				} else if (myForward.inMyGoal()) {
					if (myForward.ballInBack(ang)) {
						goOUT *= 0.7;
						goTo = goOUT;
					} else {
						goOUT *= 0.1;
						goTo = Vec2b(USUAL_FOLLOWING_SPEED, 90 + ang) + goOUT;
					}
				} else goTo = goOUT;
			}
			
			if (goTo.length > MAX_VEC2B_LEN) goTo.length = MAX_VEC2B_LEN;
			
			currentVector.changeTo(goTo);
			t = time_service::millis();
		}
		
		if (myMode == P_MODE && motorsWork && !neverTurnMotors) 
			omni.move(1, currentVector.length, currentVector.angle, pow, gyro.getMaxRotation());
		else omni.move(1, 0, 0, 0, gyro.getMaxRotation());
	}
	
	void goalkeeperStrategy() {
	
	}
	
	bool mustLeave() {
		//0.073 0.015
		if (time_service::millis() - timeCalibrEnd < 3000 || !seeBall || 
				(seeBall && !(abs(kAng) < 0.75 && abs(kLen) < 0.023))) timeCheckLeave = time_service::millis();
		
		return time_service::millis() - timeCheckLeave > TIME_LEAVE;
	}

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
		motorsWork = false;
		neverTurnMotors = false;
		robotInOUT = false;
	
		currentVector.set(0, 90);
		timeNotSeenBall = time_service::millis();
		timeUpdateQueue = time_service::millis();
		timeCheckLeave = time_service::millis();
		timeCalibrEnd = 0;
		timeOUT = 0;
		
		myGoalkeeper.setGoal(myGoal);
		myGoalkeeper.setMaxLen(MAX_VEC2B_LEN);
		myGoalkeeper.setLeaveTime(TIME_FINISH_LEAVE);
		
		myForward.setGoal(myGoal);
		myForward.setMaxLen(MAX_VEC2B_LEN);
	
		if (myRole == GOALKEEPER_ROLE) gyro.setTarget(0);
		myMode = mode;
	}
}