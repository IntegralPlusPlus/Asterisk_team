#pragma once
#include <project_config.h>
#include "Pin.h"
#include "I2C.h"

#define DISTANCE 0x07
#define ANG_FAR 0x06
#define ANG_CLOSE 0x04
#define GRADS_IN_SIDES 57
#define DIAMETER 0.90f
//74

/*#define K1 1
#define POW1 1.0f
#define K2 0.175
#define POW2 2.0f*/
#define Ec 2.718281828459

class IRLocator {
	public:
		IRLocator(I2C &irlI2C, uint32_t address);
		int16_t getValue(uint8_t data);
		int16_t getAngle();
		int16_t detourAngle(int16_t angle);
		int32_t getDist();
		bool ballBehind(int16_t angle);
		double convertDist(double dist);
		float angleOffset(float angle, float dist);
		int16_t adduct(int16_t a);
	private:
		I2C _irlI2C;
		uint32_t _address;
		volatile uint32_t _dist;
		volatile int16_t _angle;
};