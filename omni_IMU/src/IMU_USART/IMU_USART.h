#pragma once
#include "project_config.h"
#include "time_service.h"
#include "Pin.h"
#include "usart1.h"
#include "usart2.h"
#include "usart3.h"
#include "usart6.h"

#define RECEIVED2REAL 1.4117647058823529411764705882353f
#define KOEFF_P 14
#define KOEFF_D 0
#define MAXROTATION 2000

class gyro_imu {
	public:
		gyro_imu(Pin &tx, Pin &rx, uint8_t usartNum);
		void initUSART(uint8_t num);
		void read(); 
		int16_t adduct(int16_t ang);
		//void setRotationForTarget();
		int16_t setRotationForTarget();
		void setTarget(int16_t target);
		void setZeroAngle();
		int16_t getCurrentAngle();
		int16_t getRotation();
		int16_t getMaxRotation();
		int16_t getTarget();
	private:
		uint8_t _usartNumber;
		int16_t _angleNow, _target, _zeroAngle, errOld; 
		int16_t _rotation;
};