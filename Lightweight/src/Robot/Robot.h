#pragma once
#include "libraries.h"

#define IMU_CALIBRATE_TIME 43000
#define NEED_TO_CALIBRATE 1

#define TIME_NOT_SEEN 450
#define TIME_LEAVE 3100
#define TIME_FINISH_LEAVE 2000
#define TIME_CANT_CHANGE_DIRECTION 700 

#define USUAL_FOLLOWING_SPEED 0.67
//0.67
#define MAX_VEC2B_LEN 0.88
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
	volatile int16_t x, y, xGkReturn, yGkReturn;
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
	volatile int16_t currLeaveTime, usart6Available;
	volatile uint8_t outStatus, outStatusNow;
	volatile bool tssps_[32];
	volatile bool robotInOUT;
	volatile float xYellow, yYellow, xBlue, yBlue; 
	volatile float capacitorADC;
	volatile bool nearOUT = false;
	volatile bool ballGrip, readed;
	volatile int16_t ballVal;
	volatile float globalAng2Ball;
	volatile uint8_t detourDir;
	volatile uint8_t byEnemyGoalGK, timeDetour;
	volatile float xReturn, yReturn;
	volatile float globalAng;

	Pin voltageDividerPin('A', 5, adc);
	Adc voltageDividerPinADC(ADC2, 1, 5);
	Dma voltageDividerDMA(RCC_AHB1Periph_DMA2, voltageDividerPinADC, DMA2_Stream2, DMA_Channel_1);
	VoltageDividor voltageDiv(voltageDividerDMA);

	Pin ballSensPin('A', 4, adc);
	Adc ballSensADC(ADC1, 1, 4);
	Dma ballSensDMA(RCC_AHB1Periph_DMA2, ballSensADC, DMA2_Stream4, DMA_Channel_0);
	BallSensor ballSens(ballSensDMA);

	Pin tsop_in1('D', 11, write_pupd_down);
	Pin tsop_in2('D', 10, write_pupd_down);
	Pin tsop_in3('D', 9, write_pupd_down);
	Pin tsop_in4('D', 8, write_pupd_down);

	Pin tsopPin1('C', 2, read);
	Pin tsopPin2('C', 1, read);

	TSOP tsops(tsop_in1, tsop_in2, tsop_in3, tsop_in4, tsopPin1, tsopPin2);

	Pin tx_openMV('B', 10, usart3); //b10
	Pin rx_openMV('B', 11, usart3); //b11
	OpenMV camera(tx_openMV, rx_openMV, 3); //3

	Pin reset_imu_pin('A', 0, write_pupd_down);
	Pin tx_imu('B', 6, usart1);
	Pin rx_imu('B', 7, usart1);
	gyro_imu gyro(tx_imu, rx_imu, 1);

	Pin capacitorPin('C', 0, adc);
	Adc capacitorPinADC(ADC3, 3, 10);
	Dma capacitor(RCC_AHB1Periph_DMA2, capacitorPinADC, DMA2_Stream0, DMA_Channel_2, 5, 20);
	Pin kickerPin('A', 8, write);
	Kicker kicker(kickerPin, capacitor);

	Pin button1('D', 1, read_pupd_down);
	Pin button2('D', 0, read_pupd_down);
	Pin button3('A', 9, read_pupd_down);
	Button motorsButton(button1, invertedConfig);
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
		ballVal = ballSens.getValue();
		ballGrip = ballSens.ballInGrip();
		//usart6Available = uart6::available();
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
		
		//myRole = swRoleChoose.readPin();
		gkLED.set(myRole);
		forwLED.set(!myRole);
		
		//myGoal = !swGoalChoose.readPin();
		blueGoalLED.set(myGoal);
		yellowGoalLED.set(!myGoal);
		
		//myGoalkeeper.setGoal(myGoal);
		//myForward.setGoal(myGoal);
		
		motorsWork = motorsButton.pressed();
		
		if (!tsops.distBad(distRaw)) {
			ang = ball.getCurrentVec2b().angle;
			dist = ball.getCurrentVec2b().length;
			timeNotSeenBall = time_service::millis();
		}
		
		//ang = angRaw;
		//dist = distRaw;
		kAng = ball.getDerivativeAng();
		kLen = ball.getDerivativeDist();
		
		gyro.read();
		angleIMU = -gyro.getCurrentAngle();
		
		camera.calculate(angleIMU, myGoal, myRole);
		dBl = camera.getDistBlue();
		dYe = camera.getDistYellow();

		if (dBl || dYe) {
			x = camera.getX();
			y = camera.getY();
			doesntSeeGoals = false;
			
			if (myRole == GOALKEEPER_ROLE) myGoalkeeper.setParams(x, y, angleIMU, dBl, dYe);
			else myForward.setParams(x, y, angleIMU, dBl, dYe);
		} else doesntSeeGoals = true;
		
		angYellow = camera.getAngleYellow();
		angBlue = camera.getAngleBlue();
		
		volt = voltageDiv.getVoltage();
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
		
		capacitorADC = kicker.getDMASignal();
		/*if (kicker.canKick()) kicker.open();
		else kicker.close();*/
		//omni.move(1, 0, 0, 200, gyro.getMaxRotation());
	}
	
	Vec2b getVec2bToBallFollow(uint8_t zone = middleZone) {	
		speedForward = USUAL_FOLLOWING_SPEED;
		
		if (seeBall) {
			float offset = tsops.angleOffset(ang, distSoft, angleIMU);
			
			if (tsops.ballFar(dist)) offset = 0;
			else if (dist > 9.17 && abs(ang) > 25) speedForward *= 0.9;
			
			if (myRole == FORWARD_ROLE) {
				float globalTSOP = myForward.adduct180(ang - angleIMU);
				if (zone == leftZone && offset < 0 && globalTSOP < -90) { 
						//|| (zone == rightZone && offset > 0 && globalTSOP > 90)) {
					detourDir = rightDetour;	
					//offset *= -1;
				} else if (zone == rightZone && offset > 0 && globalTSOP > 90) {
					detourDir = leftDetour;
				}
				
				if ((detourDir == rightDetour && offset < 0)
						|| (detourDir == leftDetour && offset > 0)) {
					offset *= -1;
					timeDetour = time_service::millis(); 
				} else {
					if (zone == middleZone) {
						if (time_service::millis() - timeDetour < TIME_CANT_CHANGE_DIRECTION) {
							if (detourDir == leftDetour && offset > 0) offset *= -1;
							else if (detourDir == rightDetour && offset < 0) offset *= -1;
						} else detourDir = defaultDetour;
					} 
				}
			}
			
			angRes = ang + offset + 90;
			
			while (angRes > 360) angRes -= 360;
			while (angRes < 0) angRes += 360;
				
			angSoft = gyro.calculateSoft(angSoft, angRes);	
		} else {
			speedForward = 0;
			angSoft = 0;
		}
		
		if (ballGrip) { 
			angSoft = myForward.adduct(myForward.getTarget2Enemy() + 90);
			speedForward *= 1.5;
		}
		
		return Vec2b(speedForward, myForward.adduct(angSoft));
	}
	
	void forwardStrategy() {
		if (!doesntSeeGoals) targetRaw = float(myForward.getTarget2Enemy());
		gyro.setTarget(targetRaw);
		
		gyro.setRotationForTarget();
		pow = gyro.getRotation();
		
		led2.set(abs(angleIMU) <= 4);
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
			if (doesntSeeGoals) 
				goTo = myForward.getVecToPoint(0, DIST_BETWEEN_GOALS / 2);
			else if (!robotInOUT) {
				float globalBall = myForward.adduct180(ang - angleIMU);
				uint8_t nearOutStatus = myForward.robotNearOUT();
				
				if ((nearOutStatus != unknow) && globalBall >= -90 && globalBall <= 90) {
					goTo = Vec2b(myForward.setNearSpeed(nearOutStatus, USUAL_FOLLOWING_SPEED),
											 ang + 90);
				} else if (myForward.nearMyGoal()) { 
					if (globalBall < -70 || globalBall > 70) goTo = myForward.vec2bOnGoal(USUAL_FOLLOWING_SPEED, ang);
					else goTo = Vec2b(USUAL_FOLLOWING_SPEED, 90 + ang);
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
						goOUT *= 0.14;
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
	
	bool mustLeave() {
		if (time_service::millis() - timeCalibrEnd < 2000 || myGoalkeeper.ballInBack(ang, tsopRaw) || dist < 4.1
				|| !seeBall ||  (seeBall && !(abs(kAng) < 0.12 && abs(kLen) < 0.013))) timeCheckLeave = time_service::millis(); //0.6 0.018
		
		return time_service::millis() - timeCheckLeave > TIME_LEAVE;
	}
	
	void goalkeeperStrategy() {
		if (!doesntSeeGoals) {
			if (inLeave && byEnemyGoalGK == byEnemyGoal) {
				targetRaw = float(myGoalkeeper.getTarget2Enemy());
			} else targetRaw = float(myGoalkeeper.getTargetGoalkeeper());			
		}
		
		kicker.setKickerStatus(button3.readPin() || ballGrip && inLeave && myGoalkeeper.suitableParams2Kick());
		
		if (!kicker.canKick()) kicker.close();
		else kicker.open();
		//if (kicker.canKick() && inLeave && abs(gyro.getTarget() - angleIMU) <= 6)
		//kicker.open();
		
		gyro.setTarget(targetRaw);

		gyro.setRotationForTarget();
		pow = gyro.getRotation();
		
		led2.set(!doesntSeeGoals);
		led3.set(myGoalkeeper.ballInBack(ang, tsopRaw));
		//led3.set(abs(-gyro.getTarget() - angleIMU) <= 4);
		
		if (!doesntSeeGoals) {
			robotMustLeave = mustLeave();
											
			if (!robotMustLeave && !inLeave && !inReturn) {
				Vec2b vecToBall, vecToCenter;
				angToGoal = int16_t(RAD2DEG * atan2(float(y), float(x)));
				ang0_360 = myGoalkeeper.adduct(ang + 90);
											
				if (!tsops.distBad(distSoft) && seeBall) vecToBall = myGoalkeeper.getVecToIntersection(ang0_360);
				else if (!seeBall) vecToBall = myGoalkeeper.getVecToPoint();
				else vecToBall = Vec2b(0, 0);
											
				//globalAng2Ball = myGoalkeeper.globalAngToBall;
				vecToCenter = myGoalkeeper.getVecToGoalCenter();
				vecToCenter *= myGoalkeeper.getCoeffToGoalCenter(vecToBall.length);
			
				goTo = vecToCenter + vecToBall;
				
				if (myGoalkeeper.dist2GoalLong()) {
					inLeave = false;
					inReturn = true;
				}
			} else if (robotMustLeave && !inLeave && !inReturn) {
				inLeave = true;
				timeInLeaving = time_service::millis();
				currLeaveTime = myGoalkeeper.getCurrentLeaveTime(ang);
				byEnemyGoalGK = myGoalkeeper.setAngleLeaveStatus();
				xReturn = x;
				yReturn = y;
			} else if (inLeave) {
				if (currLeaveTime == TIME_FINISH_LEAVE) { //ball not in sides 
					float speed2Ball;
					if (abs(-targetRaw - angleIMU) <= 8) speed2Ball = 0.91;
					else speed2Ball = 0.72;
					
					if (!ballGrip) {
						angSoft = gyro.calculateSoft(angSoft, 90 + ang, leavingSoft);	
						goTo = Vec2b(speed2Ball, angSoft);
					} else {
						goTo = Vec2b(speed2Ball, myGoalkeeper.adduct(myGoalkeeper.getTarget2Enemy() + 90));
					}
				} else goTo = Vec2b(0.5, 90 + ang);//speed2Ball = 0.4;
				
				if (time_service::millis() - timeInLeaving > currLeaveTime || 
						myGoalkeeper.distance(x, y) > 0.61 * DIST_BETWEEN_GOALS ||
						!myGoalkeeper.checkXLeft(x, myRole) || !myGoalkeeper.checkXRight(x, myRole)) {
					inLeave = false;
					inReturn = true;
				}
						
				if (myGoalkeeper.angleStopLeaving(ang)) {
					inLeave = false;
					inReturn = true;
				}
			} else if (inReturn) {
				globalAng = myGoalkeeper.adduct180(ang - angleIMU);
				
				if (globalAng >= 90 || globalAng <= -90) {
					Vec2b vecDetour, toPoint;
					if (tsops.ballFar(dist)) vecDetour = Vec2b(0, 0);
					else vecDetour = getVec2bToBallFollow();
					toPoint = myGoalkeeper.getVecToPoint(xReturn, yReturn);
					
					if (!myGoalkeeper.checkXLeft(x, myRole)) {
						goTo = vecDetour + myGoalkeeper.getVecToPoint(xReturn, yReturn) + Vec2b(MAX_VEC2B_LEN, 0 + angleIMU);
						//goTo = goTo + Vec2b(MAX_VEC2B_LEN, 0 + angleIMU);
					} else if (!myGoalkeeper.checkXRight(x, myRole)) {
						goTo = vecDetour + myGoalkeeper.getVecToPoint(xReturn, yReturn) + Vec2b(MAX_VEC2B_LEN, 180 + angleIMU);
					} else {
						toPoint *= 0.25;
						goTo = vecDetour + toPoint;
					}
				} else {
					goTo = myGoalkeeper.getVecToPoint(xReturn, yReturn);
				}
				
				if (myGoalkeeper.changeFromReturn()) {
					inReturn = false;
					inLeave = false;
					timeCheckLeave = time_service::millis();
					
					xReturn = 0;
					yReturn = GOAL_OUT_Y_THRESHOLD;
				}
			}
		}
		
		if (!inLeave && !inReturn && goTo.length > MAX_VEC2B_LEN) goTo.length = MAX_VEC2B_LEN;
		else if (inReturn && goTo.length > MAX_VEC2B_LEN - 0.05) goTo.length = MAX_VEC2B_LEN - 0.05;
		
		//goTo = Vec2b(0.6, angSoft);
		
		if (!doesntSeeGoals && time_service::millis() != t) {
			//if (time_service::millis() != t) {
			currentVector.changeTo(goTo);
			t = time_service::millis();
		}
		
		if (doesntSeeGoals) {
			if (!inReturn && !inLeave) currentVector = Vec2b(0.5, angleIMU + 90);
			else currentVector = Vec2b(0.5, myGoalkeeper.adduct(270 + angleIMU));
		}
			
		if (myMode == P_MODE && motorsWork && !neverTurnMotors) 
			omni.move(1, currentVector.length, currentVector.angle, pow, gyro.getMaxRotation());
		else omni.move(1, 0, 0, 0, gyro.getMaxRotation());
	}
	
	void init(uint8_t goal, uint8_t role, uint8_t mode = P_MODE) {
		myGoal = goal;
		myRole = role;
		myMode = mode;
		time_service::init();
		time_service::startTime();
		time_service::delay(100);
		
		t = time_service::millis();
		timeDetour = time_service::millis();
	
		currLeaveTime = TIME_FINISH_LEAVE;
		target = 0;
		angSoft = 0;
		distSoft = 0;
		distOld = -1;
		angOld = -1;
		outStatus = unknow;
		xReturn = 0;
		yReturn = GOAL_OUT_Y_THRESHOLD;
		
		imuCalibrated = false;
		seeBall = true;
		doesntSeeGoals = false;
		inLeave = false;
		inReturn = false;
		robotInOutOld = false;
		goFromOUT = false;
		goFromOutTime = false;
		motorsWork = false;
		neverTurnMotors = false;
		robotInOUT = false;
		ballGrip = false;
	
		currentVector.set(0.1, 90);
		timeNotSeenBall = time_service::millis();
		timeUpdateQueue = time_service::millis();
		timeCheckLeave = time_service::millis();
		timeCalibrEnd = 0;
		timeOUT = 0;
		detourDir = defaultDetour;
		
		myGoalkeeper.setGoal(myGoal);
		myGoalkeeper.setMaxLen(MAX_VEC2B_LEN);
		myGoalkeeper.setLeaveTime(TIME_FINISH_LEAVE);
		
		myForward.setGoal(myGoal);
		myForward.setMaxLen(MAX_VEC2B_LEN);
	}
}