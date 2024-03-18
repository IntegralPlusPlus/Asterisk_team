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
	volatile bool but1, but2, but3;
	
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

	Pin tx_openMV('C', 6, usart6);
	Pin rx_openMV('C', 7, usart6);
	OpenMV camera(tx_openMV, rx_openMV, 6);

	Pin reset_imu('A', 0, write_pupd_down);
	Pin tx_imu('B', 11, usart3);
	Pin rx_imu('B', 10, usart3);
	gyro_imu gyro(tx_imu, rx_imu, 3);

	Pin kicker('A', 8, write_pupd_down);

	Pin button1('D', 1, read_pupd_down);
	Pin button2('D', 0, read_pupd_down);
	Pin button3('A', 9, read_pupd_down);
	Button butt1(button1);
	Button butt2(button2);
	Button butt3(button3);
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

	Pin motor1in1('B', 9, tim9);
	Pin motor1in2('B', 8, tim9);
	Motor motor1(motor1in1, 2, motor1in2, 1);

	Pin motor2in1('C', 8, tim3);
	Pin motor2in2('C', 9, tim3);
	Motor motor2(motor2in1, 2, motor2in2, 1);

	Pin motor3in1('A', 7, tim12);
	Pin motor3in2('A', 6, tim12);
	Motor motor3(motor3in1, 2, motor3in2, 1);

	Pin motor4in1('B', 1, tim4);
	Pin motor4in2('B', 0, tim4);
	Motor motor4(motor4in1, 4, motor4in2, 3);

	omniplatform omni(motor3, motor4, motor2, motor1);
	BallVec2b ball;

	Forward myForward;
	Goalkeeper myGoalkeeper;
	
	bool calibrated() {
		return imuCalibrated;
	}	
	
	uint8_t getRole() {
		return myRole;
	}

	bool tssps_[32];
	volatile float volt;
	void update() {
		tsops.updateTSOPs();
		for (uint8_t i = 0; i < 32; ++i) tssps_[i] = tsops.tsopValues[i];

		tsops.calculate();
		angRaw = tsops.getAngle();
		distRaw = tsops.getDist();
		volt = voltageDiv.getVoltage();
		but1 = button1.readPin();
		but2 = button2.readPin();
		but3 = button3.readPin();
		
		if (butt1.pressed()) {
			led1.setBit();
		} else {
			led1.resetBit();
		}
		
		if (butt2.pressed()) {
			led2.setBit();
		} else {
			led2.resetBit();
		}
		
		if (butt3.pressed()) {
			led3.setBit();
		} else {
			led3.resetBit();
		}
	}

	void init(uint8_t goal, uint8_t role, uint8_t mode) {
		x = 0;
		
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
}