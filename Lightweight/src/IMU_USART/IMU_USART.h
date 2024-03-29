#pragma once
#include "project_config.h"
#include "time_service.h"
#include "Pin.h"
#include "usart1.h"
#include "usart2.h"
#include "usart3.h"
#include "usart6.h"

#define RECEIVED2REAL 1.4117647058823529411764705882353f
#define KOEFF_P 19
#define KOEFF_D 480
#define MAXROTATION 2048
#define K_SOFT 0.013f
#define TARGET_K_FOFT 0.0022f

class gyro_imu {
	public:
		gyro_imu(Pin &tx, Pin &rx, uint8_t usartNum);
		void initUSART(uint8_t num);
		void read(); 
		void setRotationForTarget();
		void setTarget(float targetRaw);
		void setZeroAngle();
		float calculateSoft(float soft, float now);
		float adduct0_360(float angle);
		int16_t getCurrentAngle();
		int16_t getRotation();
		int16_t getMaxRotation();
		int16_t getTarget();
		int16_t adduct(int16_t ang);
	private:
		uint8_t _usartNumber;
		int16_t _angleNow, _zeroAngle, errOld; 
		int16_t _rotation;
		float _target;
};