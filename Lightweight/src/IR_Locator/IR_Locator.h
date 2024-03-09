#pragma once
#include <project_config.h>
#include "TSOP.h"
#include "Pin.h"
#include "I2C.h"

#define DISTANCE 0x07
#define ANG_FAR 0x06
#define ANG_CLOSE 0x04
#define GRADS_IN_SIDES 57
#define DIAMETER 0.90f
//74

class IRLocator : public TSOP {
	public:
		IRLocator(I2C &irlI2C, uint32_t address);
		int16_t getValue(uint8_t data);
		int16_t getAngle();
		int32_t getDist();
	private:
		I2C _irlI2C;
		uint32_t _address;
		uint32_t _dist;
		int16_t _angle;
};