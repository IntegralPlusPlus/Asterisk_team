#pragma once
#include "libraries.h"

//IMU-calibration time
#define IMU_CALIBRATE_TIME 40000

//Time when robot doesnt see ball 
//=> robot really doenst see ball
#define TIME_NOT_SEEN 450

#define TIME_LEAVE 3300
#define TIME_FINISH_LEAVE 1600
#define TIME_CANT_CHANGE_DIRECTION 700 
#define TIME_BALL_IN_FRONT 7

//Forward speed to ball following
#define USUAL_FOLLOWING_SPEED 0.59 //0.67

//Maximum Vec2b
#define MAX_VEC2B_LEN 0.89
#define MY_GOAL 1
//1 -- blue
//0 -- yellow

namespace Asterisk {
	Vec2b currentVector; //Result vector of robot moving
	Vec2b goToBall, goOUT, goTo;
	OutPair outStatus, outStatusNow;
	
	//TSOP-values without Linear Regression (Denis know)
	volatile float ang, dist; 
	
	volatile bool imuCalibrated;
	volatile int16_t pow;
	volatile float angleIMU, angSoft, distSoft;
	volatile float angRes, distOld, angOld;
	volatile uint64_t t, timeNotSeenBall, timeUpdateQueue, timeCheckLeave, timeInLeaving;
	volatile uint64_t timeCalibrEnd, timeOUT, timeBallFront, timeMotorsWork;
	volatile int16_t x, y, xGkReturn, yGkReturn;
	volatile float angRaw, angRawOld;
	volatile float distRaw, distRaw2, distSoftOld;
	volatile uint8_t myGoal, myRole, myGoalOld, myRoleOld;
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
	volatile bool tssps_[32];
	volatile bool robotInOUT;
	volatile float xYellow, yYellow, xBlue, yBlue; 
	volatile float capacitorADC;
	volatile bool nearOUT = false;
	volatile bool ballGrip, readed;
	volatile int16_t ballVal;
	volatile float globalAng2Ball;
	volatile uint8_t detownDir;
	volatile uint8_t byEnemyGoalGK, timeDetown;
	volatile float xReturn, yReturn;
	volatile float globalAng;
	
	volatile bool NEED_TO_CALIBRATE;

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
	Button goalButton(button3);
	//Button resetIMU(reset_imu_pin);
	
	Pin swMotorPower('D', 2, read_pupd_down);
	Pin swGoalChoose('E', 4, read_pupd_down);
	Pin swRoleChoose('E', 5, read_pupd_down);
	Pin switcher('D', 3, read_pupd_down);
	Button goalSW(swGoalChoose, switcherConfig);
	Button roleSW(swRoleChoose, switcherConfig);

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

	Forward myForward; //Element of forwards movement, all forward-local-funcs
	Goalkeeper myGoalkeeper; //Element of goalkeepers movement, all forward-local-funcs
	
	bool calibrated() {
		return imuCalibrated;
	}	
	
	uint8_t getRole() {
		return myRole;
	}
	
	//Read values from all sensors of the robot
	void update() {	
		ballVal = ballSens.getValue(); //Value of ball-sensor
		ballGrip = ballSens.ballInGrip(); //If ball in the robots grip
		
		camera.read();
		tsops.updateTSOPs();
		
		tsops.calculate();
		//Raw values of dist and angle to ball
		angRaw = tsops.getAngle(); 
		distRaw = tsops.getDist();
		
		//Linear regression queques (Denis knows)
		if (distRaw && timeUpdateQueue != time_service::millis()) {
			ball.push(Vec2b(distRaw, angRaw), time_service::millis());
			timeUpdateQueue = time_service::millis();
		}
		
		//Robots role depends on the button
		myRole = swRoleChoose.readPin();
		gkLED.set(myRole);
		forwLED.set(!myRole);
		
		//Role was changed by button
		if (myRole != myRoleOld && myRole == GOALKEEPER_ROLE) {
			xReturn = 0;
			yReturn = GOAL_OUT_Y_THRESHOLD;
		}
		
		//Robots goal depends on the button
		myGoal = goalButton.pressed();//MY_GOAL;//!swGoalChoose.readPin();//goalSW.pressed();
		blueGoalLED.set(myGoal);
		yellowGoalLED.set(!myGoal);
		
		//Goal was changed by button
		if (myGoal != myGoalOld) {
			omni.disable();
			gyro.setZeroAngle();
			camera.initCoords();
		}
		
		//Set goal to myGoalkeeper and myForward
		myGoalkeeper.setGoal(myGoal);
		myForward.setGoal(myGoal);
		
		//If motors turned on
		motorsWork = motorsButton.pressed();
		if (!motorsWork) timeMotorsWork = time_service::millis();
		
		if (!tsops.distBad(distRaw)) {
			//Result ang and dist without linear-regression-filter
			ang = angRaw; //ball.getCurrentVec2b().angle;
			dist = distRaw; //ball.getCurrentVec2b().length;
			timeNotSeenBall = time_service::millis();
		}
		
		//Soft values of TSOP-dist
		if (abs(dist - distOld) < 1.5f || distOld == -1) {
			distSoft = 0.02f * dist + 0.98f * distSoft;
		}
		
		distOld = dist;
		distSoftOld = distSoft;
		
		//Derivative of difference of TSOP-angle and TSOP-dist
		kAng = ball.getDerivativeAng();
		kLen = ball.getDerivativeDist();
		
		//Get value from IMU
		gyro.read();
		angleIMU = -gyro.getCurrentAngle();
		
		camera.calculate(angleIMU, myGoal, myRole);
		dBl = camera.getDistBlue(); //Distance to blue goal
		dYe = camera.getDistYellow(); //Distance to yellow goal

		if (dBl || dYe) { //Robot see at least 1 from goals
			x = camera.getX(); //X-coord from camera
			y = camera.getY(); //Y-coord from camera
			doesntSeeGoals = false; //Robot see goal
			
			//Set params to Goalkeeper and Forward elements
			myGoalkeeper.setParams(x, y, angleIMU, dBl, dYe);
			myForward.setParams(x, y, angleIMU, dBl, dYe);
		} else doesntSeeGoals = true; //Robot doesnt see goal
		
		//Angles to yellow and blue goals
		angYellow = camera.getAngleYellow();
		angBlue = camera.getAngleBlue();
		
		//Voltage on robots accumulator
		volt = voltageDiv.getVoltage();
		//Voltage is low in accumulator => turn off motors
		if (!motorsWork && voltageDiv.voltageLow(volt)) neverTurnMotors = true; 
		
		//If IMU calibrated
		if (!imuCalibrated && (time_service::millis() - t > IMU_CALIBRATE_TIME 
													|| !NEED_TO_CALIBRATE)) {
			gyro.setZeroAngle();
			imuCalibrated = true;
			timeCalibrEnd = time_service::millis();
		}
		
		//Robot see ball (globaly)
		seeBall = time_service::millis() - timeNotSeenBall < TIME_NOT_SEEN;
		
		//Rest IMU
		if (reset_imu_pin.readPin()) {
			omni.disable();
			gyro.setZeroAngle();
		}
		
		led1.set(imuCalibrated);
		
		//ADC-value from Kicker
		capacitorADC = kicker.getDMASignal();
		
		//Old role and goal of the robot
		myGoalOld = myGoal;
		myRoleOld = myRole;
		
		/*if (kicker.canKick()) kicker.open();
		else kicker.close();*/
	}
	
	//Get vector to ball following for Forward
	Vec2b getVec2bToBallFollow() {	
		speedForward = USUAL_FOLLOWING_SPEED; //Base robots speed
		
		if (seeBall) {
			//TSOP-regulator
			float offset = tsops.angleOffset(ang, distSoft, angleIMU);
			
			//If robot far from ball => offset = 0 (robot doesnt detown the ball)
			if (tsops.ballFar(dist)) {
				offset = 0;
				speedForward *= 1.06; //1.12
			} //Robot near ball and ball not in front of the robot:
			else if (dist > 8.2 && abs(ang) > 20) {
				speedForward *= 0.85; //0.85
			}
			
			//Res following-angle
			angRes = ang + offset + 90;
			
			//Adduct
			while (angRes > 360) angRes -= 360;
			while (angRes < 0) angRes += 360;
			
			//Softing
			angSoft = gyro.calculateSoft(angSoft, angRes);	
		} else {
			speedForward = 0;
			angSoft = 0;
		}
		
		//If ball in the front of the robot and near => total increase speed to goal
		if (ballGrip || (abs(ang) <= 15 && dist > 9.2)) { 
			angSoft = myForward.adduct(myForward.getTarget2Enemy() + 90);
			speedForward *= 1.5;
		}
		
		return Vec2b(speedForward, myForward.adduct(angSoft));
	}
	
	/*bool mayKick2Ball() {
		if (!ballGrip) {
			timeBallFront = time_service::millis();
		}
		
		return true;//time_service::millis() - timeBallFront > TIME_BALL_IN_FRONT;
	}*/
	
	//Strategy of Forward
	void forwardStrategy() {
		//Robot see goals
		if (!doesntSeeGoals) {
			//Target to enemy goal with small offset
			//There is an offset due to angle inaccuracy
			if (myGoal == YELLOW_GOAL) {
				targetRaw = float(myForward.getTarget2Enemy() + 10); //+ 10
			} else {
				targetRaw = float(myForward.getTarget2Enemy() + 7); //+ 15
			}
		}
		
		//Set gyro angle
		gyro.setTarget(targetRaw);
		gyro.setRotationForTarget();
		pow = gyro.getRotation();
		
		//May robot kick
		//Button3 pressed or ball in grip and robot directed on enemy goals and robot near the enemy goal
		kicker.setKickerStatus(button3.readPin() || 
													 ballGrip && myForward.suitableParams2Kick() && myForward.distance(x, y, 0, DIST_BETWEEN_GOALS) < DIST_BETWEEN_GOALS * 0.51);// && motorsWork && !neverTurnMotors);
		
		if (!kicker.canKick()) kicker.close();
		else kicker.open();
		
		led2.set(!doesntSeeGoals);
		
		//Robot directed on enemy goal
		led3.set(abs(-angleIMU - gyro.getTarget()) <= 3);
		
		//Vector ball-following (without outs and other params)
		goToBall = getVec2bToBallFollow();
		
		//Angle to ball taking into account the rotation of the robot 
		float globalBall = myForward.adduct180(ang - angleIMU);
		
		if (!doesntSeeGoals) { //See goals
			outStatus = myForward.checkOUTs(); //Status from which out robot mast go
			robotInOUT = myForward.robotInOUT(); //Is robot now in out
			
			if (!goFromOUT && myForward.robotInOUT()) { //Robot may go from out
				goFromOUT = true;
				outStatusNow = outStatus;
			} else if (goFromOUT) { //Process calculation out-vector
				//Vector go from out
				goOUT = myForward.setResOUTVector(outStatus, currentVector);
				
				//Robot in free field so he mast stop go from out
				if (goFromOUT && myForward.robotInFreeField()) {
					goFromOUT = false;
					timeOUT = time_service::millis();
				} 
				
				//outStatus shows that robot doesnt mast go from out 
				if (outStatus.isDefault()) {// && outStatus != outStatusNow) {
					outStatusNow = outStatus;
					goFromOUT = false;
					timeOUT = 0;
				} else if (outStatus != outStatusNow) { //outStatus changed
					outStatusNow = outStatus;
				}
			} 
		} else myForward.resetCounts();
		
		//Robot may change current vector
		if (time_service::millis() != t) {
			if (doesntSeeGoals) { //Robot doesnt see goals
				//Vector to field center
				goTo = myForward.getVecToPoint(0, DIST_BETWEEN_GOALS / 2);
			} else if (!robotInOUT) { //Robot not in out
				//Robot near out
				uint8_t nearOutStatus = myForward.robotNearOUT();
				float angGoal = RAD2DEG * atan2(float(DIST_BETWEEN_GOALS - y), float(x));
				
				//Robot nears out and ball in front of ball
				if ((nearOutStatus != unknow) && globalBall >= -90 && globalBall <= 90) {			
					//Vector from robot to ball
					goTo = Vec2b(USUAL_FOLLOWING_SPEED, ang + 90);
				} else if (myForward.nearMyGoal() 
					&& !myForward.ballInBack(ang, tsopRaw)) { //Robot nears own goal and ball is not at back of robot
					//Vector ball-following
					goTo = goToBall;//Vec2b(USUAL_FOLLOWING_SPEED, 90 + ang);
				} else {
					//Vector ball-following
					goTo = goToBall;
					
					//Decrease speed near enemy-goal-out
					if (myForward.distance(x, y, 0, DIST_BETWEEN_GOALS) < 1.05 * RADIUS_GOAL_OUT_LEFT) {
						goTo *= 0.95; //0.85
					}
				}
			} else if (robotInOUT) { //Robot in out
				if (myForward.inEnemyGoal()) { //Robot in enemy out
					//Ball in the front of the robot
					if (globalBall < 30 && globalBall > -30) { 
						goTo = goOUT;
					} else { //Ball in sides of the robot
						goToBall *= 0.63;
						//Summ of ball-following * coefficients and vector-from-out
						goTo = goToBall + goOUT;  
					}
				} else if (myForward.inMyGoal()) { //Robot in own out
					if (myForward.ballInBack(ang, tsopRaw)) { //Ball in robots back
						//Result-vector = vector-from-out + projecion of vector-ball-following to out-direct-line of the goal
						goTo = goOUT + myForward.vec2bOnGoal(USUAL_FOLLOWING_SPEED, ang);
					} else if (myForward.myGoalLine(x, y)) { //In out-direct-line
						//Vector from robot to ball
						goTo = Vec2b(USUAL_FOLLOWING_SPEED, 90 + ang);
					} else {
						//Ball in the out
						if (!myForward.ballInOUT(globalBall)) {
							//reduce going from out thanks to
							//adding a vector to the ball to all summ
							goTo = Vec2b(USUAL_FOLLOWING_SPEED, 90 + ang) + goOUT;
						} else {
							//vector-from-out
							goTo = goOUT;
						}
					}
				} else goTo = goOUT; //vector-from-out
			}
			
			//Check maximum value of goTo (result vector)
			if (goTo.length > MAX_VEC2B_LEN) goTo.length = MAX_VEC2B_LEN;
			
			//Smooth change in the robots motion vector
			currentVector.changeTo(goTo);
			
			t = time_service::millis();
		}
		
		//Feed the vector to the motors
		if (myMode == P_MODE && motorsWork && !neverTurnMotors) {
			omni.move(1, currentVector.length, currentVector.angle, pow, gyro.getMaxRotation());
		} else { 
			omni.move(1, 0, 0, 0, gyro.getMaxRotation());
		}
	}
	
	//May goalkeeper leave from the goal
	bool mustLeave() {
		int16_t angGoal = RAD2DEG * atan2(float(y), float(x));
	
		//Many conditions
		//Im fucked-tired of writing everything down here
		if (time_service::millis() - timeMotorsWork < 2000 || time_service::millis() - timeCalibrEnd < 2000 
				|| myGoalkeeper.ballInBack(ang, tsopRaw) || dist < 3.7f// || dist > 9.f
				|| angGoal < ANGLE_LOW_DOESNT_LEAVE || angGoal > ANGLE_HIGH_DOESNT_LEAVE
				|| !seeBall ||  (seeBall && !(abs(kAng) < 0.12 && abs(kLen) < 0.013))) timeCheckLeave = time_service::millis(); //0.6 0.018
		
		return time_service::millis() - timeCheckLeave > TIME_LEAVE;
	}
	
	//Goalkeeper strategy
	void goalkeeperStrategy() {
		if (!doesntSeeGoals) { //See goals
			//Goalkeepers direction on enemy or own goal
			if (inLeave && byEnemyGoalGK == byEnemyGoal) {
				targetRaw = float(myGoalkeeper.getTarget2Enemy());
			} else {
				targetRaw = float(myGoalkeeper.getTargetGoalkeeper());			
			}
		}
		
		//May kick if button pressed or 
		//Ball in grip and in leaving and goalkeeper near the enemy goal  
		kicker.setKickerStatus(button3.readPin() || ballSens.ballLongTimeInGrip() && inLeave && myGoalkeeper.suitableParams2Kick());
		
		if (!kicker.canKick()) kicker.close();
		else kicker.open();
		
		//Gyro target
		gyro.setTarget(targetRaw);

		gyro.setRotationForTarget();
		pow = gyro.getRotation();
		
		led2.set(!doesntSeeGoals);
		led3.set(abs(-angleIMU - gyro.getTarget()) <= 3);//myGoalkeeper.ballInBack(ang, tsopRaw));
		
		if (!doesntSeeGoals) { //Robot see goals
			//if robot mast leave from goals
			robotMustLeave = mustLeave();
											
			//Robot doesnt mast leave and not in leaving
			if (!robotMustLeave && !inLeave && !inReturn) {
				Vec2b vecToBall; //vector to defending goal
				Vec2b	vecToCenter; //vector to pressing to own goal
				
				//Angle to own goal
				angToGoal = int16_t(RAD2DEG * atan2(float(y), float(x)));
				ang0_360 = myGoalkeeper.adduct(ang + 90);
											
				if (!tsops.distBad(distSoft) && seeBall) { //Dist to ball is normal 
					vecToBall = myGoalkeeper.getVecToIntersection(ang0_360);
				} else if (!seeBall) { //Doesnt see ball
					vecToBall = myGoalkeeper.getVecToPoint(); //Vector to goal-line-out center
				} else vecToBall = Vec2b(0, 0);
				
				//Vector to pressing to own out
				vecToCenter = myGoalkeeper.getVecToGoalCenter();
				
				//Pressing to out + Defend
				goTo = vecToCenter + vecToBall;
				
				//Robot is far from the goal
				if (myGoalkeeper.dist2GoalLong() 
					&& angToGoal < 120 && angToGoal > 60) {
					inLeave = false;
					inReturn = true;
				}
			} //Robot must leave but leaving-status not defined
			else if (robotMustLeave && !inLeave && !inReturn) {
				inLeave = true;
				timeInLeaving = time_service::millis();
				
				//Define leaveing time
				currLeaveTime = myGoalkeeper.getCurrentLeaveTime(ang);
				byEnemyGoalGK = myGoalkeeper.setAngleLeaveStatus();
				xReturn = 0;
				yReturn = 0;
			} else if (inLeave) { //Robot is leaving now
				if (currLeaveTime == TIME_FINISH_LEAVE) { //Ball not in sides 
					float speed2Ball;
					speed2Ball = 0.7;
					
					if (!ballGrip) { //Ball not in grip
						//Direction to ball
						angSoft = gyro.calculateSoft(angSoft, 90 + ang, leavingSoft);	
						goTo = Vec2b(speed2Ball, angSoft);
					} else {
						//Direction to enemy goal
						goTo = Vec2b(speed2Ball, myGoalkeeper.adduct(myGoalkeeper.getTarget2Enemy() + 90));
					}
				} else {
					//Direction to ball with reduced speed
					goTo = Vec2b(0.5, 90 + ang);
				}
				
				//Transition from leave to return
				if (!(time_service::millis() - timeInLeaving > currLeaveTime && myGoalkeeper.distance(x, y) < 0.35 * DIST_BETWEEN_GOALS 
						&& (myGoalkeeper.checkXLeft(x, myRole) || myGoalkeeper.checkXRight(x, myRole)))) {				
					if (time_service::millis() - timeInLeaving > currLeaveTime || 
							myGoalkeeper.distance(x, y) > 0.58 * DIST_BETWEEN_GOALS ||
							!myGoalkeeper.checkXLeft(x, myRole) || !myGoalkeeper.checkXRight(x, myRole)) {
						inLeave = false;
						inReturn = true;
					}
				}
						
				if (myGoalkeeper.angleStopLeaving(ang)) {
					inLeave = false;
					inReturn = true;
				}
			} else if (inReturn) { //Robot is returning now
				//Angle to ball taking into account robots rotation
				globalAng = myGoalkeeper.adduct180(ang - angleIMU);
				
				//Ball in back of the robot
				if (globalAng >= 90 || globalAng <= -90) {
					Vec2b vecDetown; //Vector to detour ball
					Vec2b toPoint; //Vector to (0, 0)
					
					//Mustnt detour at ball
					if (tsops.ballFar(dist)) {
						vecDetown = Vec2b(0, 0);
					} else { //Detouring
						vecDetown = getVec2bToBallFollow();
					}
					
					//Calculation vector to (0, 0)
					toPoint = myGoalkeeper.getVecToPoint(0, 0);
					
					//Check left and right threshold
					if (!myGoalkeeper.checkXLeft(x, myRole)) {
						//As usual + out-vector
						goTo = vecDetown + myGoalkeeper.getVecToPoint(0, 0) + Vec2b(MAX_VEC2B_LEN, angleIMU);
					} else if (!myGoalkeeper.checkXRight(x, myRole)) {
						//As usual + out-vector
						goTo = vecDetown + myGoalkeeper.getVecToPoint(0, 0) + Vec2b(MAX_VEC2B_LEN, 180 + angleIMU);
					} else {
						//Summ of detour and vector to (0, 0)
						goTo = vecDetown + toPoint;
					}
				} else { //Ball in the front of the robot
					//to (0, 0)
					goTo = myGoalkeeper.getVecToPoint(0, 0);
					
					//Check left and right thresholds
					if (!myGoalkeeper.checkXLeft(x, myRole)) goTo = goTo + Vec2b(MAX_VEC2B_LEN, angleIMU);
					else if (!myGoalkeeper.checkXRight(x, myRole)) goTo = goTo + Vec2b(MAX_VEC2B_LEN, 180 + angleIMU);
				}
				
				//Change status from return to
				if (myGoalkeeper.changeFromReturn()) {
					inReturn = false;
					inLeave = false;
					timeCheckLeave = time_service::millis();
					
					xReturn = 0;
					yReturn = 0;//GOAL_OUT_Y_THRESHOLD;
				}
			}
		}
		
		//Check maximum vector
		if (!inLeave && !inReturn && goTo.length > MAX_VEC2B_LEN) goTo.length = MAX_VEC2B_LEN;
		else if (inReturn && goTo.length > MAX_VEC2B_LEN - 0.05) goTo.length = MAX_VEC2B_LEN - 0.05;
		
		if (!doesntSeeGoals && time_service::millis() != t) {
			//if (time_service::millis() != t) {
			currentVector.changeTo(goTo); //Smooth change current vector
			t = time_service::millis();
		}
		
		//Doesnt see goals
		if (doesntSeeGoals) {
			if (!inReturn && !inLeave) { //Not in leaving and returning
				//Robot in last moment that he saw goals was near CRITICAL_DOWN_Y
				if (x < CRITICAL_DOWN_Y) {
					currentVector = Vec2b(0.5, angleIMU + 90); //Go forward
				} else currentVector = Vec2b(0, 0); //Stop
			} else { //In leaving or returning
				//Go back
				currentVector = Vec2b(0.5, myGoalkeeper.adduct(270 + angleIMU));
			}
		}
		
		//Feed the vector to the motors
		if (myMode == P_MODE && motorsWork && !neverTurnMotors) 
			omni.move(1, currentVector.length, currentVector.angle, pow, gyro.getMaxRotation());
		else omni.move(1, 0, 0, 0, gyro.getMaxRotation());
	}
	
	//Init all values
	void init(uint8_t goal, uint8_t role, uint8_t mode = P_MODE) {
		myGoal = goal;
		myRole = role;
		myMode = mode;
		time_service::init();
		time_service::startTime();
		time_service::delay(100);
		
		t = time_service::millis();
		timeDetown = time_service::millis();
		timeBallFront = time_service::millis();
	
		currLeaveTime = TIME_FINISH_LEAVE;
		target = 0;
		angSoft = 0;
		distSoft = 0;
		distOld = -1;
		angOld = -1;
		xReturn = 0;
		yReturn = GOAL_OUT_Y_THRESHOLD;
		volt = 0;
		
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
	
		currentVector = Vec2b(0.1, 90);
		timeNotSeenBall = time_service::millis();
		timeUpdateQueue = time_service::millis();
		timeCheckLeave = time_service::millis();
		timeMotorsWork = time_service::millis();
		timeCalibrEnd = 0;
		timeOUT = 0;
		
		myGoalkeeper.setGoal(myGoal);
		myGoalkeeper.setMaxLen(MAX_VEC2B_LEN);
		myGoalkeeper.setLeaveTime(TIME_FINISH_LEAVE);
		
		myForward.setGoal(myGoal);
		myForward.setMaxLen(MAX_VEC2B_LEN);
		myGoalOld = myGoal;
		myRoleOld = myRole;
	}
}