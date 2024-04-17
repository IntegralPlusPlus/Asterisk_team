#pragma once
#include "project_config.h"
#include "Pin.h"
#include "Dma.h"

#define SEE_BALL 1900
#define COUNT_THRESHOLD 1

class BallSensor {
	public:
		BallSensor(Dma& sens);
		bool ballInGrip();
		int16_t getValue();
	private:
		Dma& _sensor;
		uint8_t count2See;
};